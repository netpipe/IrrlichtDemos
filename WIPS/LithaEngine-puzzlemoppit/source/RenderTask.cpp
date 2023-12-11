
#include "RenderTask.h"
#include "IEngine.h"
#include "World.h"
#include "IGraphic.h"
#include "PostProcessingChain.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "Event.h"

RenderTask::RenderTask(World *world)
{
	engine = GetEngine();
	this->world = world;
	
	IrrlichtDevice *device = engine->GetIrrlichtDevice();
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	
	shaderManager = new ShaderManager( engine->GetIrrlichtDevice()->getVideoDriver() );
	
	// No post processing chain set.
	ppChain = NULL;
	postProcessingEnabled = true;
	
	// No screen fade
	fadeGUI = false;
	oldScreenFade = 1.0;
	newScreenFade = 1.0;
	fadeStartTime = 0.0;
	fadeFinishTime = 0.0;
	isFadeFinished = true;
	
	// Current fade...
	currentScreenFade = 1.0;
	
	// Default background colour
	SetBackgroundCol(video::SColor(0,0,0,0));
	
	renderInvisible = false;
}

RenderTask::~RenderTask()
{
	if (ppChain)
		ppChain->drop();
	
	shaderManager->drop();
}

IPostProcessingChain *RenderTask::CreatePostProcessingChain(bool renderScreen)
{
	return new PostProcessingChain(driver, renderScreen);
}
	
void RenderTask::SetActivePostProcessingChain(IPostProcessingChain *chain)
{
	if (!postProcessingEnabled)
		chain = NULL;
	
	SET_REF_COUNTED_POINTER(ppChain, chain)
}

void RenderTask::SetShaderLevel(E_SHADER_LEVEL level)
{
	shaderManager->SetShaderLevel(level);
}

IShader *RenderTask::CreateShader(const c8 *vertexShaderFileName, const c8 *pixelShaderFileName, video::E_MATERIAL_TYPE baseMaterial)
{
	return shaderManager->CreateShader(vertexShaderFileName, pixelShaderFileName, baseMaterial);
}

bool RenderTask::ShadersAreAvailable()
{
	return shaderManager->ShadersAreAvailable();
}

bool RenderTask::PostProcessingEnabled()
{
	return postProcessingEnabled;
}

void RenderTask::ForceNoShaders(bool noShaders)
{
	shaderManager->ForceNoShaders(noShaders);
}

void RenderTask::ForceNoPostProcessing(bool noPostProcessing)
{
	postProcessingEnabled = !noPostProcessing;
}

void RenderTask::ScreenFade(f32 fadeAmount, f32 time, bool fadeGUI)
{
	this->fadeGUI = fadeGUI;
	oldScreenFade = newScreenFade; // set to the end state of the last applied fade, even if it hasn't finished.
	newScreenFade = fadeAmount;
	fadeStartTime = GetVirtualTime();
	fadeFinishTime = fadeStartTime + time;
	
	isFadeFinished = false;
	
	if (time != 0.0)
	{
		Event event("ScreenFadeStarted");
		engine->PostEvent(event);
	}
}

void RenderTask::ScreenFadeFromCurrent(f32 fadeAmount, f32 time, bool fadeGUI)
{
	/*
	we take time to go from oldScreenFade (old end state) to newScreenFade.
	but we might not have been actually at oldScreenFade,
	in which case we go from currentScreenFade to newScreenFade,
	and take less time to do so.
	Less time is time = time * (newScreenFade-currentScreenFade) / (newScreenFade-oldScreenFade)
	*/
	
	this->fadeGUI = fadeGUI;
	oldScreenFade = currentScreenFade; // this time we go from the current fade state
	newScreenFade = fadeAmount;
	fadeStartTime = GetVirtualTime();
	fadeFinishTime = fadeStartTime + time * (newScreenFade-currentScreenFade) / (newScreenFade-oldScreenFade);
	
	isFadeFinished = false;
	
	if (time != 0.0)
	{
		Event event("ScreenFadeStarted");
		engine->PostEvent(event);
	}
}

bool RenderTask::IsFading()
{
	return !isFadeFinished;
}

void RenderTask::RenderInvisible()
{
	renderInvisible = true;
	Update(0.f);
	renderInvisible = false;
}

void RenderTask::Update(f32 dt)
{
	const Set<IGraphic *> &graphics = world->GetAllGraphics();
	
	f32 logicInterpolationAlpha = engine->GetLogicInterpolationAlpha();
	
	// - Through graphics, set transform to interpolated (from last logic transform to current transform)
	// - Rendering (custom Graphic.Render plus Irrlicht drawAll)
	
	for (u32 i = 0; i < graphics.size(); i ++)
	{
		// Interpolate
		
		graphics[i]->ReceiveRenderPosition(graphics[i]->GetInterpolatedAbsolutePosition(logicInterpolationAlpha));
		graphics[i]->ReceiveRenderRotation(graphics[i]->GetInterpolatedAbsoluteRotation(logicInterpolationAlpha));
	}
	
	// Multipass rendering.
	// Need to first count the maximum passes required (find which IGraphic has greatest GetPassCount()).
	// Then smgr->drawAll and IGraphic.Render for each pass, calling SetCurrentPass on *ALL* IGraphics
	// before each drawAll.
	
	// Count number of passes (through all IGraphics, and find the maximum pass count)
	// Default to at least one pass so that non Litha Engine managed objects
	// (other Irrlicht scene nodes) get rendered.
	u16 passCount = 1;
	
	for (u32 i = 0; i < graphics.size(); i ++)
	{
		if (graphics[i]->GetPassCount() > passCount)
			passCount = graphics[i]->GetPassCount();
	}
	
	driver->beginScene(true, true, backgroundCol);
	
	if (ppChain && PostProcessingEnabled())
	{
		// If a post processing chain is set, we must first render to its input texture, and then
		// process it
		if (ppChain->GetInputTexture())
		{
			driver->setRenderTarget(ppChain->GetInputTexture(), true, true, backgroundCol);
			
			Render(passCount);
			
			// Render all steps in the post processing chain.
			ppChain->Process();
		}
		
		// Perhaps render screen. (optional when post processing; post processing may completely replace it)
		// Screen must be cleared and then rendered to *after* Process(), as Process() may modify frame buffer.
		if (ppChain->ShouldRenderScreen())
		{
			driver->setRenderTarget(video::ERT_FRAME_BUFFER, true, true, backgroundCol);
			Render(passCount);
		}
		
		// Apply post processing effects to screen.
		ppChain->ApplyToScreen();
	}
	else
	{
		driver->setRenderTarget(video::ERT_FRAME_BUFFER, true, true, backgroundCol);
		Render(passCount);
	}
	
	// Reset render distination
	driver->setRenderTarget(video::ERT_FRAME_BUFFER, false, false, backgroundCol);
	
	
	// This fixes an Irrlicht bug in 1.7+ where rendering something with a RTT causes
	// GUI rendering to be messed up. (GUI textures vertically flipped?)
	video::SMaterial mat;
	driver->setMaterial(mat);
	
	
	if (!fadeGUI) // Render fade before GUI
		RenderFade();
	
	// GUI is not post-processed.
	// If GUI post processing is desired, see notes in Render method below.
	guienv->drawAll();
	
	if (fadeGUI) // Render fade after GUI
		RenderFade();
	
	// Draw the background colour over the top?
	// This can be useful to init animations and stuff...
	// i.e. is a HACK!!
	if (renderInvisible)
	{
		driver->draw2DRectangle(video::SColor(255, backgroundCol.getRed(),backgroundCol.getGreen(),backgroundCol.getBlue()),
				core::recti(0,0,driver->getCurrentRenderTargetSize().Width,driver->getCurrentRenderTargetSize().Height));
	}
	
	driver->endScene();
	
	// Super
	Task::Update(dt);
}

void RenderTask::Render(u16 passCount)
{
	const Set<IGraphic *> &graphics = world->GetAllGraphics();
	
	// Render each pass
	// (this was nested in some other loop, that's why the iterator starts at j)
	
	for (u16 i = 0; i < passCount; i ++)
	{
		// Prepare each IGraphic for this pass.
		// The most likely use for this is to modify the materials of any wrapped irrlicht scene nodes.
		// This is called for each pass of a graphic irrespective of whether the graphic has the
		// pass set. This is so the graphic can set any scene nodes invisible if they are not to be rendered
		// this pass.
		
		for (u32 j = 0; j < graphics.size(); j ++)
			graphics[j]->SetCurrentPass(i);
		
		// Render the entire world
		smgr->drawAll();
		
		// Then render each IGraphic
		// NOTE: in most cases graphics Render methods will not be overridden and will do nothing,
		// as graphics rendering (of Irrlicht scene nodes) is usually handled by Irrlicht internally.
		// (and rendered in ISceneManager->drawAll() )
		
		for (u32 j = 0; j < graphics.size(); j ++)
		{
			if (graphics[j]->GetPassCount() > i)
				graphics[j]->Render(i);
		}
	}
	
	// To have the GUI post processed, would need to modify Irrlicht.
	// The problem is that GUI dimensions are in pixels.
	// So on a small RTT they take up much more space.
	// and thus when the small bloomed RTT is rendered over the screen,
	// the GUI has been scaled massively.
	
	// Really need an IVideoDriver method that defines a render target that 2D images
	// are drawn relative to.
	// (usually it uses the current render target, I would want to change it so it always uses the screen render
	// target dimensions)
	// render target dimensions are gotten in setRenderStates2DMode.
	
	// Could in COpenGLDriver::setRenderStates2DMode
	// use ScreenSize instead of getCurrentRenderTargetSize().
	//guienv->drawAll();
}

void RenderTask::RenderFade()
{
	// default is fade has finished
	currentScreenFade = newScreenFade;
	
	f32 fadeTimePeriod = fadeFinishTime - fadeStartTime;
	f32 currentTime = GetVirtualTime();
	
	if (fadeTimePeriod > 0.0 && currentTime < fadeFinishTime)
	{
		// fraction that current time is between start and finish fade times.
		f32 alphaTime = (currentTime-fadeStartTime) / fadeTimePeriod;
		
		if (alphaTime <= 0.0)
			currentScreenFade = oldScreenFade;
		else if (alphaTime >= 1.0)
			currentScreenFade = newScreenFade;
		else
		{
			// so now current fade must be set to that fraction between old and new fades.
			currentScreenFade = oldScreenFade + (newScreenFade-oldScreenFade) * alphaTime;
		}
	}
	else
	{
		if (!isFadeFinished)
		{
			isFadeFinished = true;
			
			// Send fade finished event?
			// Only send if fade took some time, not sent for instant fades.
			if (fadeFinishTime > fadeStartTime)
			{
				Event event("ScreenFadeFinished");
				engine->PostEvent(event);
			}
		}
	}
	
	// No fade!
	if (currentScreenFade == 1.0)
		return;
	
	// Draw faded rect
	driver->draw2DRectangle(video::SColor((u32)((1.0-currentScreenFade) * 255),0,0,0),
			core::recti(0,0,driver->getCurrentRenderTargetSize().Width,driver->getCurrentRenderTargetSize().Height));
}




