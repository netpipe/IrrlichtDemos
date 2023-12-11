
#include "Litha.h"
#include "MainState.h"
#include "StartScreen.h"
#include "volume.h"

#ifdef TRIAL
// omg a global!
bool trialFinished = false;
#endif

// oh no another global
ISound *bgAmbientSound = NULL;
ISound *bgMusic = NULL;

// and another
bool globalIsInEditor = false;

// grid based movement enabled?
bool gridBasedMovement = true;
// sky effects?
bool skyEffectsEnabled = true;
// mouse inversion
bool invertMouseX = false;
bool invertMouseY = false;

core::stringc globalLogFurthestLevel;
core::stringc globalLogLastLevel;

core::vector3df sunDirection(0.4,0.08,1);

// For sky box shader.
class SkyBoxShaderCallback : public IShaderCallback
{
	IEngine *engine;
	video::IVideoDriver *driver;
	
	f32 lastBrightness;
	
public:
	SkyBoxShaderCallback(IEngine *engine)
	{
		this->engine = engine;
		this->driver = engine->GetIrrlichtDevice()->getVideoDriver();
		
		lastBrightness = 1.f;
	}
	
	void ShaderOnSetConstants(IShader *shader)
	{
		shader->SetPixelConstant("rippleScroll", engine->GetRenderUpdater().GetVirtualTime() * 0.02);
		
		
		// Are we looking in to the sun?
		core::vector3df camVec = maths::rotation_to_direction( engine->GetWorld()->GetCamera()->GetRotation() );
		
		sunDirection.normalize();
		
		f32 dp = camVec.dotProduct(sunDirection);
		//f32 brightness = 1.f + 0.5 * dp*dp*dp*dp*dp*dp*dp*dp*dp*dp*dp*dp*dp*dp*dp*dp;
		f32 brightness = 1.f + 0.5 * dp*dp*dp*dp*dp*dp*dp*dp*dp*dp;
		
		if (brightness < 1.f || camVec.getDistanceFrom(sunDirection) > 1.f)
			brightness = 1.f;
		
		brightness = core::lerp(lastBrightness, brightness,
				core::clamp(engine->GetRenderUpdater().GetLastDeltaTime(), 0.f, 1.f));
		
		lastBrightness = brightness;
		
		shader->SetPixelConstant("brightness", brightness);
	}
};

int main(int argc, const char **argv)
{
	// Command line parameters
	
	bool editor = false;
	core::stringc levelFileName;
	
// Mac passes a -psn (process serial number) argument when running from a .app directory,
// which messes this up. So we just ignore command line arguments on mac for now.
// Ideally though, we'd just strip that out and ignore it.
#ifndef __APPLE__
	if (argc == 3)
	{
		// Edit?
		if (core::stringc(argv[1]) == "-e")
		{
			editor = true;
			globalIsInEditor = true;
		}
		
		levelFileName = argv[2];
	}
	else if (argc == 2)
	{
		levelFileName = argv[1];
	}
#endif
	
	VariantMap settings;
	settings["appName"] = "Puzzle Moppet";
	settings["screenWidth"] = 1024;
	settings["screenHeight"] = 768;
	settings["fullScreen"] = true;
	
	IEngine *engine = CreateEngine(argc, argv, &settings);
	
	
#ifdef TRIAL
	NOTE << "*** THIS IS THE TRIAL VERSION ***";
#endif
	
	
	engine->GetIrrlichtDevice()->getFileSystem()->addFileArchive("../projects/Puzzle/media/");
	
	
	IWorld *world = engine->GetWorld();
	IRenderSystem *renderSystem = engine->GetRenderSystem();
	
	renderSystem->SetShaderLevel(ESL_LOW);
	
	
	// Loading text. Hacked in! Uses add_static_text2 from MainState.cpp
	
	{
		video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
		gui::IGUIEnvironment *guienv = engine->GetIrrlichtDevice()->getGUIEnvironment();
		
		s32 screenHeight = driver->getScreenSize().Height;
		s32 screenWidth = driver->getScreenSize().Width;
		
		gui::IGUIElement *loadingGUI = add_static_text(L"Loading...");
		core::rect<s32> rect = loadingGUI->getRelativePosition();
		rect += core::vector2di( screenWidth/2 - rect.getWidth()/2, screenHeight/2 - rect.getHeight()/2 );
		loadingGUI->setRelativePosition(rect);
		
		// Render the gui once.
		driver->beginScene(true, true, video::SColor(0,0,0,0));
		guienv->drawAll();
		driver->endScene();
		
		// We can remove the element now we've rendered.
		loadingGUI->remove();
	}
	
	
	// Set up post processing.
	if (renderSystem->ShadersAreAvailable()
			&& renderSystem->PostProcessingEnabled())
	{
		// bool flag indicates whether rendering to screen still occurs first before post processing
		IPostProcessingChain *chain = renderSystem->CreatePostProcessingChain(true);
		
		// Materials need:
		// - Lighting = false
		// - ZBuffer = false
		// - ZWriteEnable = false
		
		// For bloom, need:
		// - bright filter
		// - BlurH
		// - BlurV
		// (final one blended additively on to the screen)
		
		
		video::SMaterial material;
		material.Lighting		= false;
		material.ZBuffer		= false;
		material.ZWriteEnable	= false;
		material.AntiAliasing	= video::EAAM_OFF;
		
		IShader *shader;
		
		// Bright filter
		shader = renderSystem->CreateShader("ScreenQuad.vert", "Brightfilter.frag");
		chain->AddEffect(material, shader, 2);
		shader->drop();
		
		// Horizontal blur
		shader = renderSystem->CreateShader("ScreenQuad.vert", "BlurH.frag");
		chain->AddEffect(material, shader, 2);
		shader->drop();
		
		// Vertical blur
		// As this is the last effect it is rendered additively on to the screen.
		shader = renderSystem->CreateShader("ScreenQuad.vert", "BlurV.frag", video::EMT_TRANSPARENT_ADD_COLOR);
		chain->AddEffect(material, shader, 2);
		shader->drop();
		
		renderSystem->SetActivePostProcessingChain(chain);
		chain->drop();
	}
	
	// Some sky
	// Maybe this should be in Level?
	// --> move it only when it is needed...
	
	core::stringc skyDir = "skies/Set 42/";
	core::stringc skyExt = "png";
	
	world->SetSkyBox(
			skyDir+"6."+skyExt,
			skyDir+"5."+skyExt,
			skyDir+"1."+skyExt,
			skyDir+"3."+skyExt,
			skyDir+"2."+skyExt,
			skyDir+"4."+skyExt
			);
	
	// load sky effects enabled flag
	{
		VariantMap creationSettings = engine->GetCreationSettings();
		
		if (creationSettings.count("skyEffects"))
			skyEffectsEnabled = creationSettings["skyEffects"];
		else
			skyEffectsEnabled = true;
	}
	
	if (renderSystem->ShadersAreAvailable() && skyEffectsEnabled)
	{
		IShader *skyShader = renderSystem->CreateShader("SkyBox.vert", "SkyBox.frag", video::EMT_SOLID);
		
		LowLevelShaderRegisterMap rmap;
		rmap.PushSingleRegister("rippleScroll", "brightness", NULL,NULL);
		
		skyShader->SetPixelRegisterMap(rmap);
		
		IShaderCallback *skyShaderCallback = new SkyBoxShaderCallback(engine);
		skyShader->SetCallback( skyShaderCallback );
		skyShaderCallback->drop();
		
		world->SetSkyBoxShader(skyShader);
		skyShader->drop();
		
		// sky box ripple texture !!
		
		video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
		video::ITexture *rippleTexture = driver->getTexture("cloudshadow.png");
		
		for (u8 i = 0; i < 6; i ++)
			world->GetSkyBoxMaterial(i).setTexture(1, rippleTexture);
	}
	
	
	// ********** PRELOAD EVERYTHING!! ***********
	
	NOTE << "Preloading media...";
	
	// Meshes
	{
		scene::ISceneManager *smgr = engine->GetIrrlichtDevice()->getSceneManager();
		smgr->getMesh("ground_single_sideless.b3d");
		smgr->getMesh("ground_single.b3d");
		smgr->getMesh("ground_single_fall.b3d");
		smgr->getMesh("box.b3d");
		smgr->getMesh("icybox.irrmesh");
		smgr->getMesh("icybox_inner.irrmesh");
		smgr->getMesh("balloon.irrmesh");
		smgr->getMesh("magicfan_base.b3d");
		smgr->getMesh("magicfan_blades.b3d");
		smgr->getMesh("magicfan_orb.irrmesh");
		smgr->getMesh("lift.b3d");
		smgr->getMesh("lift_glow.irrmesh");
		smgr->getMesh("nothing.irrmesh");
		smgr->getMesh("teleport.irrmesh");
		smgr->getMesh("player.b3d");
		smgr->getMesh("sea.b3d");
		smgr->getMesh("land.irrmesh");
		smgr->getMesh("cliff.irrmesh");
		smgr->getMesh("titletext.b3d");
		smgr->getMesh("gridarrow.irrmesh");
	}
	
	// Textures (stand alone, others are loaded with meshes)
	{
		video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
		driver->getTexture("brass_spheremap.png");
		driver->getTexture("watersand.png");
		driver->getTexture("water.png");
		driver->getTexture("watersand_add.png");
		driver->getTexture("dust.png");
		driver->getTexture("sparkle.png");
		driver->getTexture("fog.png");
		driver->getTexture("cloudshadow.png");
		driver->getTexture("mud.jpg");
		
		// Final sky box textures
		{
			core::stringc skyDir = "skies/Set 26/final_";
			core::stringc skyExt = "png";
			driver->getTexture(skyDir+"6."+skyExt);
			driver->getTexture(skyDir+"5."+skyExt);
			driver->getTexture(skyDir+"1."+skyExt);
			driver->getTexture(skyDir+"3."+skyExt);
			driver->getTexture(skyDir+"2."+skyExt);
			driver->getTexture(skyDir+"4."+skyExt);
		}
	}
	
	// Fonts
	{
		gui::IGUIEnvironment *guienv = engine->GetIrrlichtDevice()->getGUIEnvironment();
		guienv->getFont("font2.xml");
		guienv->getFont("fontlarge2.xml");
	}
	
	// Sounds
	{
		ISoundSystem *soundSystem = engine->GetSoundSystem();
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/sea.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/beep.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/fallblock.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/buttonflutter_micro.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/hithard.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/liftrun.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/balloonpush.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/slide.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/windy.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/speedcore.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/bell.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/stepballoon.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/step.ogg");
		
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/appear.ogg");
		
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/laugh.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/fair.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/good.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/excellent.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/perfect.ogg");
		soundSystem->PreloadSound("../projects/Puzzle/media/sfx/extraordinary.ogg");
		
		//soundSystem->PreloadSound("../projects/Puzzle/media/music.ogg");
	}
	
	// Shaders!?
	/*
	if (renderSystem->ShadersAreAvailable())
	{
		IShader *shader;
		
		shader = renderSystem->CreateShader("Hemisphere.vert", "Hemisphere.frag", video::EMT_SOLID);
		shader->drop();
		
		shader = renderSystem->CreateShader("HemisphereBalloon.vert", "HemisphereBalloon.frag", video::EMT_SOLID);
		shader->drop();
		
		shader = renderSystem->CreateShader("HemisphereLand.vert", "Hemisphere.frag", video::EMT_SOLID);
		shader->drop();
		
		shader = renderSystem->CreateShader("Hemisphere.vert", "HemisphereWood.frag", video::EMT_SOLID);
		shader->drop();
		
		shader = renderSystem->CreateShader("PlainWithAlpha.vert", "PlainWithAlpha.frag", video::EMT_SOLID);
		shader->drop();
	}
	*/
	
	NOTE << "Finished preloading!";
	
	
	// Default sound volume
	engine->GetSoundSystem()->SetGlobalVolume(5.0);
	
	
	// background sfx
	bgAmbientSound = engine->GetSoundSystem()->CreateSound2D();
	bgAmbientSound->SetIsLooped(true);
	//bgAmbientSound->SetVolume(0.035);
	
	bgMusic = engine->GetSoundSystem()->CreateSound2D();
	bgMusic->SetIsLooped(true);
	
	// Load sfx volume
	set_volumes_from_settings(engine->GetCreationSettings());
	
	bgAmbientSound->Play("../projects/Puzzle/media/sfx/windy.ogg");
	bgMusic->Play("../projects/Puzzle/media/sfx/speedcore.ogg");
	
	// load default movement type
	{
		VariantMap creationSettings = engine->GetCreationSettings();
		
		if (creationSettings.count("gridBasedMovement"))
			gridBasedMovement = creationSettings["gridBasedMovement"];
		else
			gridBasedMovement = true;
		
		// mouse axis inversion?
		
		if (creationSettings.count("invertMouseX"))
			invertMouseX = creationSettings["invertMouseX"];
		else
			invertMouseX = false;
		
		if (creationSettings.count("invertMouseY"))
			invertMouseY = creationSettings["invertMouseY"];
		else
			invertMouseY = false;
	}
	
	
	// Now, instead of going straight into the game we have a start screen.
	// ...unless the game was started by command line, in which case we jump straight in.
	
	MainState *mainState = NULL;
	
	if (levelFileName.size())
	{
		NOTE << "Started with command line parameter, will skip start screen.";
		mainState = new MainState(&mainState);
		mainState->StartLevel(levelFileName, editor);
	}
	else
	{
		NOTE << "Entering start screen...";
		// StartScreen *MAY* create a MainState, returning the pointer to mainState
		// so that we can delete it from here in main.cpp
		// (why do we want to delete it from main? because we always have...)
		StartScreen *startScreen = new StartScreen(&mainState);
		engine->GetLogicUpdater().AddUpdatable(startScreen);
		startScreen->drop();
	}
	
	// Main loop
	engine->Run();
	
	// MainState may not exist if StartScreen was entered but no level was played.
	if (mainState)
		delete mainState;
	
	if (bgAmbientSound)
		bgAmbientSound->drop();
	
	if (bgMusic)
		bgMusic->drop();
	
	bool engineWillRestart = engine->WillRestart();
	
	engine->drop();
	
#ifdef TRIAL
	if (!globalLogLastLevel.size())
		globalLogLastLevel = "unknown";
	
	if (!globalLogFurthestLevel.size())
		globalLogFurthestLevel = "unknown";
	
	if (!engineWillRestart)
	{
		// use maybe function in case user already clicked "buy" button and the buy page is going to open.
		os::maybeopenwebpage( core::stringc("http://www.garnetgames.com/puzzlemoppet/getfullversion/")
				+ "?last=" + globalLogLastLevel + "&furthest=" + globalLogFurthestLevel );
	}
#endif
	
	return 0;
}

