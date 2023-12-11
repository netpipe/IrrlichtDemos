
#include "PostProcessingChain.h"
#include "IShader.h"

// for RTT dropping bug (see below)
#include "IEngine.h"
#include "IRenderSystem.h"

inline bool is_POT(s32 n)
{
	return (n) && !(n & (n - 1));
}

PostProcessingChain::PostProcessingChain(video::IVideoDriver *driver, bool renderScreen)
{
	this->driver = driver;
	this->renderScreen = renderScreen;
	
	quadIndices[0] = 0;
	quadIndices[1] = 1;
	quadIndices[2] = 2;
	quadIndices[3] = 2;
	quadIndices[4] = 3;
	quadIndices[5] = 0;
	quadVertices[0].TCoords.set(0.0f, 0.0f); // To invert texture would toggle Vs.
	quadVertices[1].TCoords.set(0.0f, 1.0f);
	quadVertices[2].TCoords.set(1.0f, 1.0f);
	quadVertices[3].TCoords.set(1.0f, 0.0f);
	quadVertices[0].Pos = core::vector3df(-1.0,-1.0,0.0);
	quadVertices[1].Pos = core::vector3df(-1.0,1.0,0.0);
	quadVertices[2].Pos = core::vector3df(1.0,1.0,0.0);
	quadVertices[3].Pos = core::vector3df(1.0,-1.0,0.0);
}

PostProcessingChain::~PostProcessingChain()
{
	// Remove all RTTs and drop all shaders
	for (u32 i = 0; i < effects.size(); i ++)
	{
		driver->removeTexture( effects[i].material.TextureLayer[0].Texture );
		
		if (effects[i].shader)
			effects[i].shader->drop();
	}
}

void PostProcessingChain::AddEffect(video::SMaterial &material, IShader *shader, video::ITexture *rt)
{
	if (!rt)
	{
		WARN << "AddEffect failed.";
		return;
	}
	
	NOTE << "Actual effect RTT size is {" << rt->getSize().Width << "," << rt->getSize().Height << "}.";
	
	if (shader)
	{
		shader->grab();
		shader->ApplyToIrrMaterial(material);
	}
	
	// Create RTT
	material.TextureLayer[0].Texture = rt;
	material.TextureLayer[0].TextureWrap = video::ETC_CLAMP_TO_EDGE;
	
	/*
	material.TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
	material.TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;
	*/
	
	Effect effect =
	{
		material,
		shader
	};
	
	effects.push_back(effect);
}

// Fallback is done as described here, however it is probably actually never necessary as either
// Irrlicht or the graphics driver does a similar fallback transparently.
// If the specific size of render target for an effect is not available, it will fall back as follows:
// - if non power-of-two, fall back to biggest POT dimensions smaller than the specified dimensions
// - then fall back dividing by 2 each time until one of the dimensions reaches a minimum size
// If falling back, will output fallback size to console.
void PostProcessingChain::AddEffect(video::SMaterial material, IShader *shader, core::dimension2du size)
{
	const u32 MINIMUM_POT = 8;
	
	// This kind of hacked in here.
	// Temporary fix for PP chain bug.
	// (when PP chains are created but not used and are deleted, dropping of RTTs causes other textures
	// to be dropped? An Irrlicht bug?).
	if (!GetEngine()->GetRenderSystem()->PostProcessingEnabled())
		return;
	
	NOTE << "AddEffect " << "{" << size.Width << "," << size.Height << "}.";
	
	if (size.Width < MINIMUM_POT || size.Height < MINIMUM_POT)
	{
		WARN << "AddEffect aborted, screen fraction too small.";
		return;
	}
	
	video::ITexture *rt = driver->addRenderTargetTexture(size, "rt", video::ECF_A8R8G8B8);
	
	if (!rt)
	{
		// if not POT
		if ( (!is_POT(size.Width)) || (!is_POT(size.Height)) )
		{
			NOTE << "Not POT, falling back...";
			
			// find greatest POT dimensions that are just smaller
			// I guess this could be optimised??
			
			while (!is_POT(size.Width) && size.Width > MINIMUM_POT)
				size.Width --;
			
			while (!is_POT(size.Height) && size.Height > MINIMUM_POT)
				size.Height --;
		}
		
		// Attempt creation again with POT sizes
		NOTE << "Trying again: {" << size.Width << "," << size.Height << "}.";
		rt = driver->addRenderTargetTexture(size, "rt", video::ECF_A8R8G8B8);
		
		// Still not created?
		if (!rt)
		{
			// Decrease RTT size until success
			while (true)
			{
				size /= 2;
				
				// end if a dimension reaches a minimum size
				if (size.Width < MINIMUM_POT || size.Height < MINIMUM_POT)
					break;
				
				NOTE << "Trying yet again: {" << size.Width << "," << size.Height << "}.";
				
				if (rt = driver->addRenderTargetTexture(size, "rt", video::ECF_A8R8G8B8))
					break;
			}
		}
	}
	
	if (rt)
	{
		NOTE << "Success!";
		AddEffect(material, shader, rt);
	}
	else
	{
		WARN << "Failed to create RTT, post processing will be disabled.";
		GetEngine()->GetRenderSystem()->ForceNoPostProcessing(true);
	}
}

void PostProcessingChain::AddEffect(video::SMaterial material, IShader *shader, u32 screenFraction)
{
	AddEffect(material, shader, driver->getScreenSize() / screenFraction);
}

video::ITexture *PostProcessingChain::GetInputTexture()
{
	if (effects.size())
	{
		// Input to first effect's texture.
		return effects[0].material.TextureLayer[0].Texture;
	}
	else
	{
		return NULL;
	}
}

void PostProcessingChain::Process()
{
	// Render each effect's texture into the next.
	// Last effect is excluded as it renders to screen (and must be called at a different time)
	
	for (u32 i = 0; i < effects.size()-1; i ++)
	{
		// Render to the input texture of the next effect.
		driver->setRenderTarget(effects[i+1].material.TextureLayer[0].Texture, true, false, video::SColor(0,0,0,0));
		driver->setMaterial( effects[i].material );
		driver->drawIndexedTriangleList(quadVertices,4,quadIndices,2);
	}
}

void PostProcessingChain::ApplyToScreen()
{
	// Last effect rendered to screen
	// If screen hasn't been rendered to, we must also clear the frame buffer.
	// (as it may have stuff left over from rendering to textures in it)
	driver->setRenderTarget(video::ERT_FRAME_BUFFER, !renderScreen, false, video::SColor(0,0,0,0));
	
	driver->setMaterial( effects[effects.size()-1].material );
	driver->drawIndexedTriangleList(quadVertices,4,quadIndices,2);
}


