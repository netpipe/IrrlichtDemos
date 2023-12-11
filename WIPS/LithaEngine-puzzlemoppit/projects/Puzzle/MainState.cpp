
#include "MainState.h"
#include "Level.h"
#include "Editor.h"
#include "get_lines.h"
#include "Positioner.h"
#include "FinalScenePlayerProxy.h"
#include "StartScreen.h"
#include "level_stats.h"

#ifdef TRIAL
// set when trial has finished...
// so a different web page is shown when exiting.
extern bool trialFinished;
#endif

extern ISound *bgAmbientSound;
extern ISound *bgMusic;

extern core::vector3df sunDirection;

// this only used for final scene
#include "Map.h"

#ifdef TRIAL
#define LEVEL_LIST_FILE "../projects/Puzzle/trial_levels/levels.list"
#define LEVEL_BASE_PATH "../projects/Puzzle/trial_levels/levels/"
#else
#define LEVEL_LIST_FILE "../projects/Puzzle/levels/levels.list"
#define LEVEL_BASE_PATH "../projects/Puzzle/levels/levels/"
#endif

#define GAME_SAVE_FILENAME "puzzlegame.save"

#define TEXT_COL			video::SColor(150, 255,255,255)
#define TEXT_COL_MOUSEOVER	video::SColor(100, 200,200,200)

// gets the full path to a level file, relative to the executable's directory
core::stringc level_path_rel_exe(core::stringc levelFile)
{
	core::stringc result = core::stringc(LEVEL_BASE_PATH) + levelFile;
	
	/*
	
	// THIS DISABLED SINCE IT MESSES UP SAVING.
	// (game progress gets lost...)
	
	// if doesn't exist in normal level location, we also check the original starting directory...
	// and also just try it as an absolute path (might have been passed as a parameter)
	if (!os::path::exists(result))
	{
		NOTE << "Level did not exist in normal location: " << result;
		
		core::stringc otherResult = os::path::concat(GetEngine()->GetStartingDirectory(), levelFile);
		
		if (os::path::exists(levelFile))
		{
			NOTE << "Level appears to be an absolute path, so will load: " << levelFile;
			result = levelFile;
		}
		else if (os::path::exists(otherResult))
		{
			NOTE << "Level was found in starting location: " << otherResult;
			result = otherResult;
		}
	}
	*/
	
	return result;
}

// Get the full path to the save file.
io::path get_full_save_path()
{
	io::path ret = os::path::concat( GetEngine()->GetLocalSettingsDir(), GAME_SAVE_FILENAME );
	NOTE << "save path: " << ret;
	return ret;
}

io::path get_full_save_path_furthest()
{
	io::path ret = os::path::concat( GetEngine()->GetLocalSettingsDir(), io::path(GAME_SAVE_FILENAME)
			+ io::path(".furthest") );
	NOTE << "furthest save path: " << ret;
	return ret;
}

void delete_game_saves()
{
	if (!os::path::ensure_delete(get_full_save_path().c_str()))
		WARN << "Could not delete save file to restart game.";
	
	if (!os::path::ensure_delete(get_full_save_path_furthest().c_str()))
		WARN << "Could not delete furthest save record.";
	
	
	// and delete score files.
	
	io::path scoreDir = getsavescoredir();
	
	NOTE << "Deleting score files: " << scoreDir;
	
	std::vector<io::path> scoreFiles = os::listfiles(scoreDir);
	
	for (u32 i = 0; i < scoreFiles.size(); i ++)
		os::path::ensure_delete( os::path::concat(scoreDir,scoreFiles[i]) );
}

std::vector<core::stringc> find_levels()
{
	std::vector<core::stringc> levelFileNames;
	
	std::vector<core::stringc> lines = get_lines(LEVEL_LIST_FILE);
	
	for (u32 i = 0; i < lines.size(); i ++)
	{
		if (lines[i] == "###END###")
		{
			// This causes parsing to stop
			// This is useful when I want to test the game ending.
			// I can just insert this in the level list to cause the game
			// to end at that point.
			break;
		}
		
		if (lines[i].size())
		{
			levelFileNames.push_back(lines[i]);
			NOTE << "Identified level: " << lines[i];
		}
	}
	
	return levelFileNames;
}

// Uses the game save (the last completed level), then finds the next level in the list after that,
// which is the level to continue playing at.
// If there is no next level (e.g. end of trial version), will just return the last completed level.
core::stringc find_next_level(bool fromFurthest)
{
	core::stringc nextLevel;
	
	std::vector<core::stringc> levelFileNames = find_levels();
	
	//FILE *fp = fopen(get_full_save_path().c_str(), "rb");
	
	FILE *fp;
	
	if (!fromFurthest)
		fp = fopen(get_full_save_path().c_str(), "rb");
	else
		fp = fopen(get_full_save_path_furthest().c_str(), "rb");
	
	if (fp)
	{
		char line[256];
		
		if (fgets(line, 256, fp))
		{
			if (strlen(line) > 0)
			{
				// Game was changed to save the last level *completed*, not the current level.
				// Therefore, the new level to continue at is the _next_ level from
				// the one saved.
				
				// So find the next level in the list...
				
				// find it in the level list
				u32 i = 0;
				for (; i < levelFileNames.size(); i ++)
				{
					if (levelFileNames[i] == line)
						break;
				}
				
				// not found?
				if (i >= levelFileNames.size())
				{
					WARN << "Saved level not found (" << i << ") [" << line << "]";
				}
				else
				{
					// found old level... but is there a next level?
					if (i+1 < levelFileNames.size())
					{
						NOTE << "Found a valid next level...";
						NOTE << "(have completed level " << line << " so will continue on next level, "
								<< levelFileNames[i+1] << ")";
						
						nextLevel = levelFileNames[i+1];
					}
					else
					{
						// last level is over! Quit the game I guess?
						// should not get here in full version since last level is an exception from saving...
						NOTE << "Cannot continue game, have completed the last level: " << i << ", " << line;
						
						// problem: when finishing trial, alittlebridge will be saved.
						// so in that case we restart that level already completed...
						// saving does not occur until a level is completed, so this is ok.
						
						// Actually, this is an acceptable fallback for the full version too.
						// So we'll do this whatever happens.
						
						NOTE << "Will replay previously completed level: " << line;
						
						nextLevel = line;
					}
				}
			}
		}
		
		fclose(fp);
	}
	
	return nextLevel;
}

gui::IGUIStaticText *add_static_text(const wchar_t *str)
{
	gui::IGUIEnvironment *guienv = GetEngine()->GetIrrlichtDevice()->getGUIEnvironment();
	
	gui::IGUIFont *font = guienv->getFont("font2.xml");
	
	core::dimension2du dim = font->getDimension(str);
	
	gui::IGUIStaticText *textElement = guienv->addStaticText(str, core::recti(0,0,dim.Width,dim.Height), false,false);
	textElement->setOverrideFont( guienv->getFont("font2.xml") );
	textElement->setOverrideColor( TEXT_COL );
	return textElement;
}

gui::IGUIStaticText *add_static_text2(const wchar_t *str)
{
	gui::IGUIEnvironment *guienv = GetEngine()->GetIrrlichtDevice()->getGUIEnvironment();
	
	gui::IGUIFont *font = guienv->getFont("fontlarge2.xml");
	
	core::dimension2du dim = font->getDimension(str);
	
	gui::IGUIStaticText *textElement = guienv->addStaticText(str, core::recti(0,0,dim.Width,dim.Height), false,false);
	textElement->setOverrideFont( guienv->getFont("fontlarge2.xml") );
	textElement->setOverrideColor( TEXT_COL );
	return textElement;
}

MainState::MainState(MainState **mainStatePtrLoc)
{
	NOTE << "MainState constructing...";
	
	this->mainStatePtrLoc = mainStatePtrLoc;
	
	startLevelTime = -1.0;
	
	engine = GetEngine();
	world = engine->GetWorld();
	renderSystem = engine->GetRenderSystem();
	
	device = engine->GetIrrlichtDevice();
	
	level = NULL;
	editor = NULL;
	
	engine->GetLogicUpdater().AddUpdatable(this, false);
	
	nextLevel = false;
	isPaused = false;
	inFinalScene = false;
	
	// Read level names
	levelFileNames = find_levels();
	
	// New: read some descriptive level titles.
	levelTitles = file::loadsettings("../projects/Puzzle/level_names.ini");
	
	// general events
	engine->RegisterEventInterest(this, "ButtonDown");
	engine->RegisterEventInterest(this, "AxisMoved");
	engine->RegisterEventInterest(this, "RestartLevel");
	
	// sound
	menuSound = engine->GetSoundSystem()->CreateSound2D();
	menuSound->SetVolume(0.5);
	
	// Nullify final scene stuff
	finalSceneSea = NULL;
	
	// pause menu
	video::IVideoDriver *driver = device->getVideoDriver();
	u32 halfScreenHeight = driver->getScreenSize().Height / 2;
	pauseMenuPositioner = new RowPositioner(device->getVideoDriver(), halfScreenHeight, 50);
	levelSelectMenuPositioner = new RowPositioner(device->getVideoDriver(), halfScreenHeight+100, 25);
	levelConfirmMenuPositioner = new RowPositioner(device->getVideoDriver(), halfScreenHeight+150, 25);
	
	gameEnded = false;
	
	ShowInitTexts();
}

MainState::~MainState()
{
	NOTE << "MainState destructing...";
	
	if (finalSceneSea)
		finalSceneSea->drop();
	
	menuSound->drop();
	delete pauseMenuPositioner;
	delete levelSelectMenuPositioner;
	delete levelConfirmMenuPositioner;
	
	engine->UnregisterAllEventInterest(this);
	
	engine->GetLogicUpdater().RemoveUpdatable(this);
	
	NOTE << "MainState destructed!";
}

void MainState::ShowInitTexts()
{
	video::IVideoDriver *driver = device->getVideoDriver();
	
	// not set until StartLevel
	initFirstLevelTime = -1.0;
	
	// Game name and company name splash text
	// This is shown every time the game starts.
	
	s32 screenHeight = driver->getScreenSize().Height;
	
	// DISABLED THIS SINCE WE ADDED THE START SCREEN.
	// Start screen gives any info like this.
	/*
	{
		gui::IGUIStaticText *text = add_static_text2(L"Puzzle Moppet");
		core::rect<s32> rect = text->getRelativePosition();
		rect += core::vector2di( 25, screenHeight - 110 );
		text->setRelativePosition(rect);
		introTexts.push_back(text);
	}
	
	{
		gui::IGUIStaticText *text = add_static_text(L"by Garnet Games");
		core::rect<s32> rect = text->getRelativePosition();
		rect += core::vector2di( 25, screenHeight - 70 );
		text->setRelativePosition(rect);
		introTexts.push_back(text);
	}
	
	{
		gui::IGUIStaticText *text = add_static_text(L"www.garnetgames.com");
		core::rect<s32> rect = text->getRelativePosition();
		rect += core::vector2di( 25, screenHeight - 40 );
		text->setRelativePosition(rect);
		introTexts.push_back(text);
	}
	*/
}

void MainState::RemoveInitTexts()
{
	for (u32 i = 0; i < introTexts.size(); i ++)
		introTexts[i]->remove();
		
	introTexts.clear();
}

void MainState::RemoveStartLevelTexts()
{
	for (u32 i = 0; i < startLevelTexts.size(); i ++)
		startLevelTexts[i]->remove();
		
	startLevelTexts.clear();
}

void MainState::ShowEndTexts()
{
	video::IVideoDriver *driver = device->getVideoDriver();
	initFirstLevelTime = engine->GetEngineTime();
	
	s32 screenHeight = driver->getScreenSize().Height;
	s32 screenWidth = driver->getScreenSize().Width;
	
#ifdef TRIAL
	{
		gui::IGUIStaticText *text = add_static_text(L"Sorry,");
		core::rect<s32> rect = text->getRelativePosition();
		rect += core::vector2di( screenWidth/2 - rect.getWidth()/2, screenHeight/2 - 80 );
		text->setRelativePosition(rect);
		introTexts.push_back(text);
	}
	{
		gui::IGUIStaticText *text = add_static_text2(L"End of Trial Version");
		core::rect<s32> rect = text->getRelativePosition();
		rect += core::vector2di( screenWidth/2 - rect.getWidth()/2, screenHeight/2 - 20 );
		text->setRelativePosition(rect);
		introTexts.push_back(text);
	}
	{
		gui::IGUIStaticText *text = add_static_text(L"Go to www.GarnetGames.com to get the full game!");
		core::rect<s32> rect = text->getRelativePosition();
		rect += core::vector2di( screenWidth/2 - rect.getWidth()/2, screenHeight/2 + 40 );
		text->setRelativePosition(rect);
		introTexts.push_back(text);
	}
	
	// so main.cpp knows and will show a different web page.
	trialFinished = true;
	
#else
	{
		gui::IGUIStaticText *text = add_static_text2(L"The End");
		core::rect<s32> rect = text->getRelativePosition();
		rect += core::vector2di( screenWidth/2 - rect.getWidth()/2, screenHeight/2 );
		text->setRelativePosition(rect);
		introTexts.push_back(text);
	}
#endif
}

core::stringc MainState::GetCurrentLevelName()
{
	ASSERT(level);
	
	// get base name of current level
	return engine->GetIrrlichtDevice()->getFileSystem()->getFileBasename(level->GetFileName());
}

void MainState::RemoveLevelAndEditor()
{
	if (editor)
	{
		NOTE << "Removing editor.";
		world->GetUpdater().RemoveUpdatable(editor);
		editor = NULL;
	}
	
	if (level)
	{
		NOTE << "Removing level.";
		world->GetUpdater().RemoveUpdatable(level);
		level = NULL;
	}
}

/*void MainState::PreviewLevel(core::stringc levelFileName)
{
	RemoveLevelAndEditor();
	
	NOTE("Creating new level preview.");
	
	level = new Level( this, level_path_rel_exe(levelFileName) );
	engine->GetLogicUpdater().AddUpdatable(level);
	level->drop();
	
	// Remove input from level.
	world->SetInputProfile(NULL);
}*/

void MainState::StartLevel(core::stringc levelFileName, bool startEditor, std::deque<UndoState> *undoHistory)
{
	startLevelTime = engine->GetEngineTime();
	
#ifdef TRIAL
	shownBetweenLevelTrialScreen = false;
#endif
	
	RemoveLevelAndEditor();
	
	NOTE << "Loading level... (" << levelFileName << ")";
	
	ASSERT(levelFileName.size());
	
	
	// IS IN FINAL SCENE?
	if (levelFileName == "finalscene1.lev"
			|| levelFileName == "finalscene2.lev"
			// Don't want this stuff when editing the level
			&& !startEditor)
	{
		inFinalScene = true;
	}
	
	RemoveInitTexts();
	RemoveStartLevelTexts();
	
	
	// SAVE CURRENT GAME PROGRESS
	// provided not in intro or second part of final scene...
	// (want the last save point to be where the player has control on the beach)
	
	// GAME SAVING HAS BEEN MOVE TO NextLevel.
	// So that saving across trial -> full version works properly.
	
	/*
	if (levelFileName != "intro.lev" && !inFinalScene)//levelFileName != "finalscene2.lev")
	{
		FILE *fp = fopen(get_full_save_path().c_str(), "wb");
		
		if (fp)
		{
			if (fputs(levelFileName.c_str(), fp) == EOF)
				WARN << "Could not save game (" << levelFileName << ")";
			else
				NOTE << "Saved game successfully (" << levelFileName << ")";
			
			fclose(fp);
		}
	}
	*/
	
	
	level = new Level(this, level_path_rel_exe(levelFileName), undoHistory);
	world->GetUpdater().AddUpdatable(level);
	level->Start();
	level->drop();
	
	
	if (startEditor)
	{
		editor = new Editor(level);
		world->GetUpdater().AddUpdatable(editor);
		editor->drop();
	}
	
	if (initFirstLevelTime < 0.0)
		initFirstLevelTime = engine->GetEngineTime();
	
	
	// New thing.
	// Let's see if there is a level name!
	if (levelTitles.count(levelFileName)
			&& !undoHistory)
	{
		video::IVideoDriver *driver = device->getVideoDriver();
		
		s32 screenWidth = driver->getScreenSize().Width;
		s32 screenHeight = driver->getScreenSize().Height;
		
		core::stringc levelTitle = levelTitles[levelFileName].To<core::stringc>();
		core::stringw levelTitleW = levelTitle.c_str();
		
		gui::IGUIStaticText *text = add_static_text2( levelTitleW.c_str() );
		core::rect<s32> rect = text->getRelativePosition();
		rect += core::vector2di( screenWidth/2-rect.getWidth()/2, 50 );
		text->setRelativePosition(rect);
		startLevelTexts.push_back(text);
	}
	
	
	
	// FINAL SCENE STUFF
	// This is the non-player controlled final scene as the character walks off into the sunset...
	
	if (inFinalScene)
	{
		// Different ambience
		
		if (bgAmbientSound)
			bgAmbientSound->Stop();
		
		// Stop any music.
		
		if (bgMusic)
			bgMusic->Stop();
		
		if (!finalSceneSea)
		{
			finalSceneSea = engine->GetSoundSystem()->CreateSound3D();
			finalSceneSea->SetIsLooped(true);
			finalSceneSea->SetVolume(15.0);
			finalSceneSea->SetPosition(core::vector3df(-50,0,0));
			finalSceneSea->Play("../projects/Puzzle/media/sfx/sea.ogg");
		}
		
		// Different sky box
		core::stringc skyDir = "skies/Set 26/final_";
		core::stringc skyExt = "png";
		
		world->SetSkyBox(
				skyDir+"6."+skyExt,
				skyDir+"5."+skyExt,
				skyDir+"1."+skyExt,
				skyDir+"3."+skyExt,
				skyDir+"2."+skyExt,
				skyDir+"4."+skyExt
				);
		
		// NOTE: All transformables added here are removed when Level destructs by 
		// the call to RemoveAllTransformables(). That's why no references are kept by MainState.
		
		
		// different sun position (for sky box brightness)
		sunDirection = core::vector3df(0,1,0);
		
		// actually, we'll disable the shader completely...
		world->SetSkyBoxShader(NULL);
		
		
		// Create the sandy beach
		
		IPhysics *physics = world->GetPhysics();
		
		IBody *groundBody = physics->AddStaticBody();
		
		IMesh *groundMesh = world->AddMesh("land.irrmesh");
		level->ApplyLandShaders(groundMesh);
		groundBody->AddChild(groundMesh);
		
		// new. grass layer on cliff.
		// NEED TO ONLY ADD THIS IF SHADERS ARE AVAILABLE.
		// (since we need a shader to render it. Blender will not let me paint vertex alpha,
		// so I will use vertex colours and a special shader to use colours as transparency values)
		if (renderSystem->ShadersAreAvailable())
		{
			//IMesh *cliffGrassMesh = world->AddMesh("cliffgrass.irrmesh");
			//level->ApplyCliffGrassShaders(cliffGrassMesh);
			//groundBody->AddChild(cliffGrassMesh);
		}
		
		IMesh *cliffMesh = world->AddMesh("cliff.irrmesh");
		level->ApplyLandShaders(cliffMesh);
		groundBody->AddChild(cliffMesh);
		
		/*
		//for (u32 i = 0; i < groundMesh->GetMaterialCount(); i ++)
		u32 i = 1;
		{
			video::SMaterial &material = groundMesh->GetMaterial(i);
			material.MaterialType = video::EMT_LIGHTMAP;
			material.setTexture(1, device->getVideoDriver()->getTexture("cliff_lightmap.jpg"));
			
			core::matrix4 mat;
			mat.setTextureScale(0.25,0.25);
			material.setTextureMatrix(1, mat);
		}
		*/
		
		ICollisionGeometry *groundGeom = physics->CreateMeshCollisionGeometry(groundMesh);
		groundBody->AddCollisionGeometry(groundGeom);
		groundGeom->drop();
		
		ICollisionGeometry *cliffGeom = physics->CreateMeshCollisionGeometry(cliffMesh);
		groundBody->AddCollisionGeometry(cliffGeom);
		cliffGeom->drop();
		
		// Don't want regular falling as it interferes with physics terrain collision
		level->SetLogicEnabled(false);
		
		
		// A an invisible barrier to stop player walking out to sea
		IBody *barrierBody = physics->AddStaticBody();
		ICollisionGeometry *barrierGeom = physics->CreateBoxCollisionGeometry(core::vector3df(0.5,3.0,50.0));
		barrierBody->AddCollisionGeometry(barrierGeom);
		barrierGeom->drop();
		barrierBody->SetPosition(core::vector3df(-12,0,0));
		
		
		// Following stuff is a rather hackish way to prevent the camera going below sea level.
		// This was (perhaps foolishly...) deemed to be quicker than the "proper" solution, which
		// would have been to simply allow the camera to go below sea + set some kind of blue
		// underwater effect on the screen.
		
		
		// A collision geometry for the sea.
		// This is only used to prevent the camera intersecting the sea, other objects like the player
		// should NOT collide with it.
		IBody *seaBody = physics->AddStaticBody();
		ICollisionGeometry *seaGeom = physics->CreateBoxCollisionGeometry(core::vector3df(50.0,0.5,50.0));
		seaGeom->SetCollisionLayer(1);
		seaBody->AddCollisionGeometry(seaGeom);
		seaGeom->drop();
		seaBody->SetPosition(core::vector3df(0,-0.5,0));
		
		// Disable collisions between the sea's geometry and all objects in the default layer.
		// So the only thing that will collide with the sea is the camera which is in layer 2.
		physics->SetLayerCollisions(0,1, false);
		
		
		// Use a simple third person collider rather than the special one.
		// (the special one can't cope with trimesh geometry)
		// But we have one change: we add a child to the player and watch the child.
		// The child is the special transformable FinalScenePlayerProxy which will
		// not go lower than a certain height (i.e. does not go below the sea)
		
		FinalScenePlayerProxy *proxy = new FinalScenePlayerProxy();
		world->AddTransformable(proxy);
		level->GetPlayer()->AddChild(proxy);
		
		ICameraCollider *collider = level->GetCamera()->CreateThirdPersonCameraCollider();
		collider->SetCollisionLayer(2);
		level->GetCamera()->Follow(proxy);
		level->GetCamera()->SetCollider(collider);
		//level->GetCamera()->SetZoomLimits(1.5, 8.0);
		// still want to exclude player's collision geometries
		collider->ExcludeGeometry( level->GetPlayer()->GetBody()->GetCollisionGeometries() );
		// exclude the invisible barrier from collisions
		collider->ExcludeGeometry(barrierBody->GetCollisionGeometries());
		collider->drop();
		
		
		// Create a regular block as decoration
		core::vector3di woodBlockPos(4,0,2);
		level->CreateObject(woodBlockPos, EOT_MOVABLE_BLOCK);
		level->GetMap()->GetObject(woodBlockPos)->SetRotation(core::vector3df(22,42,11));
		
		
		// needed as there will be no blocks under player on final scene.
		level->ForceFootsteps(true);
		
		
		if (levelFileName == "finalscene1.lev")
		{
			// Stuff peculiar to first part of final scene
			// (player controlled walk on the beach)
			
			ICharacter *player = level->GetPlayer();
			player->SetRotation( core::vector3df(0, -90, 0) );
		}
		else
		{
			// Stuff for second part of final scene
			// (the walk off into distance)
			
			ICharacter *player = level->GetPlayer();
			
			// set player position to end of the beach
			player->SetPosition( core::vector3df(-4,0.241472,
					walkedLeft ? 20 : -20) );
			
			// set player rotation
			player->SetRotation( core::vector3df(0, walkedLeft ? 0 : 180, 0) );
			
			// disabled user control
			player->SetController(NULL);
			
			// walk off by itself
			player->SetMoveVec( core::vector2df(0.2, walkedLeft ? 1.0 : -1.0) );
			player->SetMedialMotion(1);
			
			// walk slower...
			player->SetMoveSpeed(0.2);
			
			// turn off footstep sound...
			ISoundSource *footStepSoundSource = player->GetFirstChildOfType<ISoundSource>();
			
			if (footStepSoundSource)
				footStepSoundSource->GetSound()->SetVolume(0.0);
			
			// Fixed camera
			world->SetCameraController(NULL);
			ICamera *camera = world->GetCamera();
			camera->SetPosition(core::vector3df(-6,2,walkedLeft ? 15 : -15));
			camera->SetTarget(core::vector3df(-6,1,walkedLeft ? 20 : -20));
		}
		
		
		
		
		// *********** The sea! **********
		
		
		scene::ISceneManager *smgr = device->getSceneManager();
		scene::IAnimatedMesh *meshWater = smgr->getMesh("sea.b3d");
		scene::IMesh *staticMeshWater = meshWater->getMesh(0);
		
		// clear materials to default
		
		video::SMaterial defaultMaterial;
		
		for (u32 i = 0; i < meshWater->getMeshBufferCount(); i ++)
		{
			meshWater->getMeshBuffer(i)->getMaterial() = defaultMaterial;
		}
		
		for (u32 i = 0; i < staticMeshWater->getMeshBufferCount(); i ++)
		{
			staticMeshWater->getMeshBuffer(i)->getMaterial() = defaultMaterial;
		}
		
		smgr->getMeshManipulator()->setVertexColors(staticMeshWater, video::SColor(255,255,255,255));
		
		{
			const f32 waveHeight = 0.1;
			const f32 waveSpeed = 1000.0;
			const f32 waveLength = 2.5;
			
			smgr->getMeshManipulator()->setVertexColorAlpha(staticMeshWater, 200);
			
			scene::ISceneNode *nodeWater = smgr->addWaterSurfaceSceneNode(staticMeshWater,
					waveHeight, waveSpeed, waveLength);
			
			//nodeWater->setMaterialFlag(video::EMF_LIGHTING, false); 
			nodeWater->setMaterialTexture(0, device->getVideoDriver()->getTexture("watersand.png"));
			nodeWater->setMaterialTexture(1, device->getVideoDriver()->getTexture("water.png"));
			//nodeWater->setMaterialType(video::EMT_REFLECTION_2_LAYER);
			nodeWater->setMaterialType(video::EMT_TRANSPARENT_REFLECTION_2_LAYER);
			
			nodeWater->setPosition(core::vector3df(0,-0.25,0));
			finalSceneNodes.push_back(nodeWater);
		}
		
		
		{
			const f32 waveHeight = 0.1;
			const f32 waveSpeed = 1000.0;
			const f32 waveLength = 2.5;
			
			scene::ISceneNode *nodeWater = smgr->addWaterSurfaceSceneNode(staticMeshWater,
					waveHeight, waveSpeed, waveLength);
			
			nodeWater->setMaterialFlag(video::EMF_LIGHTING, false);
			nodeWater->setMaterialTexture(0, device->getVideoDriver()->getTexture("watersand_add.png"));
			nodeWater->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
			
			nodeWater->setPosition(core::vector3df(0,0,0));
			finalSceneNodes.push_back(nodeWater);
		}
		
		
	}
}

/*void MainState::StartFromPreview()
{
	NOTE("StartFromPreview...");
	RestartLevel();
}*/

void MainState::StartFirstLevel()
{
	if (levelFileNames.size())
	{
		core::stringc continueLevel = find_next_level();
		
		if (continueLevel.size())
		{
			NOTE << "Continuing level... " << continueLevel;
			StartLevel(continueLevel);
			return;
		}
		
		// No level to continue from.
		// So start the first level.
		
		NOTE << "Starting first level (" << levelFileNames[0] << ")";
		StartLevel(levelFileNames[0]);
		
		// First level is the falling intro, so we want some special modifications...
		
		world->SetCameraController(NULL);
		level->GetPlayer()->SetController(NULL);
		
		ICamera *camera = world->GetCamera();
		camera->SetPosition(core::vector3df(0,0,6));
		camera->SetTarget(core::vector3df(0,0,0));
		
		//level->GetPlayer()->SetRotation(core::vector3df(0,0,0));
		
		level->GetPlayer()->SetRotation(core::vector3df(-90,0,0));
		level->GetPlayer()->SetAnimations(ANIM_FALL, -1);
		
		// Rotation disabled, I'll probably just have a "flail arms" animation instead...
		/*
		IMotionAnimator *anim = world->CreateRotationAnimator( core::vector3df(0,0,100) );
		level->GetPlayer()->GetMesh()->AddAnimator(anim);
		anim->drop();
		*/
	}
	else
	{
		WARN << "No levels found.";
	}
}

void MainState::NextLevel(bool wait)
{
	if (wait)
	{
		// if wait flag is set, NextLevel will be called next frame.
		nextLevel = true;
	}
	else
	{
		
		// Clear anything left over from the intro
		
		RemoveInitTexts();
		RemoveStartLevelTexts();
		
		// Clear any irrlicht scene nodes.
		// This removes water between the two parts of end game cutscene.
		for (u32 i = 0; i < finalSceneNodes.size(); i ++)
			finalSceneNodes[i]->remove();
		
		finalSceneNodes.clear();
		
		
		// Perhaps we need to wait a frame before processing this call.
		
		ASSERT(level);
		
		// current level file name
		core::stringc currentLevelFileName = GetCurrentLevelName();
		
		
		// SAVE LEVEL THAT WE JUST FINISHED
		// we exclude pyramid_mining, the last level, since we don't want to continue the game in the final scene.
		// (since the menu does not appear in final scene, so wouldn't be able to restart the game)
		if (currentLevelFileName != "pyramid_mining.lev" && currentLevelFileName != "finalscene1.lev"
				&& currentLevelFileName != "finalscene2.lev"
				// Also, let's exclude the first level
				&& currentLevelFileName != "intro.lev")
		{
			FILE *fp = fopen(get_full_save_path().c_str(), "wb");
			
			if (fp)
			{
				if (fputs(currentLevelFileName.c_str(), fp) == EOF)
					WARN << "Could not save game (" << currentLevelFileName << ")";
				else
					NOTE << "Saved game successfully (" << currentLevelFileName << ")";
				
				fclose(fp);
			}
			
			// EDIT. Hacked in. Save furthest reached.
			// Read current contents of file
			
			core::stringc currentFurthest = file::get(get_full_save_path_furthest());
			
			if (currentFurthest.size())
			{
				NOTE << "current furthest level: " << currentFurthest;
				
				// Through levels, find *both*.
				
				s32 furthestPos = -1;
				s32 currentPos = -1;
				
				for (u32 i = 0; i < levelFileNames.size(); i ++)
				{
					if (levelFileNames[i] == currentFurthest)
						furthestPos = i;
					
					if (levelFileNames[i] == currentLevelFileName)
						currentPos = i;
				}
				
				// Update if current pos is new furthest
				if (currentPos > furthestPos)
				{
					NOTE << "Current level is furthest, will write new furthest...";
					
					if (!file::put(get_full_save_path_furthest(), currentLevelFileName))
						WARN << "writing failed";
				}
				else
				{
					NOTE << "(will not update furthest: " << currentPos << "," << furthestPos << ")";
				}
			}
			else
			{
				NOTE << "(no current furthest level set)";
				NOTE << "will write current level as furthest: " << currentLevelFileName;
				
				if (!file::put(get_full_save_path_furthest(), currentLevelFileName))
					WARN << "writing failed";
			}
		}
		
		
		
		// find it in the level list
		u32 i = 0;
		for (; i < levelFileNames.size(); i ++)
		{
			if (levelFileNames[i] == currentLevelFileName)
				break;
		}
		
		// not found?
		if (i >= levelFileNames.size())
		{
			WARN << "Current level not found (" << i << ")";
			engine->Exit();
		}
		
		// found old level... but is there a next level?
		if (i+1 < levelFileNames.size())
		{
			// next level
			StartLevel(levelFileNames[i+1]);
		}
		else
		{
			// last level is over! Quit the game I guess?
			NOTE << "All levels finished!";
			
			// Show some end texts...
			// Actual stuff shown varies depending on whether this is just
			// the end of the trial, or the end of the full game.
			RemoveLevelAndEditor();
			ShowEndTexts();
			world->ClearSkyBox();
			renderSystem->ScreenFade(1.0, 2, true);
			gameEnded = true;
		}
	}
}

void MainState::RestartLevel(std::deque<UndoState> *undoHistory)
{
	// Ignore a call to restart if in intro
	if ( (level && GetCurrentLevelName() == "intro.lev") || inFinalScene)
		return;
	
	NOTE << "Restarting current level...";
	
	if (!level)
		WARN << "No level currently started.";
	
	ASSERT(level);
	
	std::deque<UndoState> undoHistoryCopy;
	// we copy undo state in case the pointer becomes invalid
	// (due to Level getting deleted, and level may have called this)
	// possibly this is not needed. my low level knowledge is not good.
	if (undoHistory)
		undoHistoryCopy = *undoHistory;
	
	// this is already relative to exe
	core::stringc fileName = GetCurrentLevelName();
	
	RemoveLevelAndEditor();
	
	StartLevel( fileName, false, undoHistory ? &undoHistoryCopy : NULL );
	
	
	// Hacked in.
	// Set camera if undoing.
	if (undoHistory && !undoHistoryCopy.empty())
	{
		level->GetCamera()->SetAngles( undoHistoryCopy.back().cameraAngle );
		level->GetCamera()->SetZoom( undoHistoryCopy.back().cameraZoom );
		level->GetPlayer()->SetRotation( core::vector3df(0,undoHistoryCopy.back().playerAngle,0) );
	}
}

// These two methods are called by Level when level is OnPaused/OnResumed.
// Since mainstate is never paused since it is owned by LogicTask.
// (top level tasks are pausable)
// Better solution would be for this sfx to be in level or some other class
// that is owned by World...
void MainState::OnPause()
{
	if (finalSceneSea)
		finalSceneSea->Pause();
}
void MainState::OnResume()
{
	if (finalSceneSea)
		finalSceneSea->Resume();
}

void MainState::Update(f32 dt)
{
	IUpdatable::Update(dt);
	
	if (nextLevel)
	{
		NextLevel();
		nextLevel = false;
	}
	
	f32 introTextLife = 9.0;
	f32 introTextFadeOffTime = 2.0;
	
	// StartLevel should have been called before any updates...
	// (so this will assert if StartLevel was not called)
	ASSERT(initFirstLevelTime >= 0.0);
	
	if ( (engine->GetEngineTime() > initFirstLevelTime + introTextLife)
			&& !gameEnded // Don't fade off the end of game text.
			)
	{
		// 2 seconds fade off
		f32 alpha = (initFirstLevelTime+introTextLife+introTextFadeOffTime - engine->GetEngineTime())/introTextFadeOffTime;
		
		// Delete text?
		if (engine->GetEngineTime() > initFirstLevelTime + introTextLife + introTextFadeOffTime)
			RemoveInitTexts();
		
		// fade text?
		
		for (u32 i = 0; i < introTexts.size(); i ++)
		{
			introTexts[i]->setOverrideColor( TEXT_COL.getInterpolated(video::SColor(0, 255,255,255), alpha) );
		}
	}
	
	introTextLife = 5.0;
	introTextFadeOffTime = 2.0;
	
	// And fade off start level texts (i.e. level name)
	// using same lifetime variables as intro text
	if ( (engine->GetEngineTime() > startLevelTime + introTextLife)
			&& !gameEnded // Don't fade off the end of game text.
			)
	{
		// 2 seconds fade off
		f32 alpha = (startLevelTime+introTextLife+introTextFadeOffTime - engine->GetEngineTime())/introTextFadeOffTime;
		
		// Delete text?
		if (engine->GetEngineTime() > startLevelTime + introTextLife + introTextFadeOffTime)
			RemoveStartLevelTexts();
		
		// fade text?
		
		for (u32 i = 0; i < startLevelTexts.size(); i ++)
		{
			startLevelTexts[i]->setOverrideColor( TEXT_COL.getInterpolated(video::SColor(0, 255,255,255), alpha) );
		}
	}
	
	
#ifdef TRIAL
	// Show trial message a certain time after level was started.
	
	if (startLevelTime != -1.0 && (engine->GetEngineTime() > startLevelTime + 5.0) )
	{
		if (level && !inFinalScene && !isPaused && !gameEnded
				&& !level->IsEnding()
				&& GetCurrentLevelName() != "intro.lev" )
		{
			
			// count actual level number...
			// hacked in
			// we only want to show this trial message after the player has reached
			// a certain point. (i.e. don't show on the really quick easy levels, let
			// the player relax and get into the game...)
			
			// current level file name
			core::stringc currentLevelFileName = GetCurrentLevelName();
			
			// find it in the level list
			u32 levelNum = 0;
			for (; levelNum < levelFileNames.size(); levelNum ++)
			{
				if (levelFileNames[levelNum] == currentLevelFileName)
					break;
			}
			
			
			/*
			// DISABLED NAG PROMPTS!
			// Maybe I'll enable them again later?
			// But possibly with less frequency. (e.g. only on initial start
			// of a level, not on restarts)
			if (!shownBetweenLevelTrialScreen && levelNum >= 4)
			{
				// WE USE THE PAUSE MENU FOR THIS.
				// THIS STUFF FROM OnEvent BELOW!
				
				
				isPaused = true;
				
				// Pause
				renderSystem->ScreenFade(0.2);
				world->Pause();
				device->getCursorControl()->setVisible(true);
				
				ShowPauseMenu();
				
				
				
				// THEN SHOW A SUB MENU OF PAUSE MENU
				
				pauseMenuPositioner->Reset();
				pauseMenuPositioner->SetTitle( add_static_text(
						L"You're playing the trial version of Puzzle Moppet!") );
				pauseMenuPositioner->Add( add_static_text(L"Continue trial..."), EPM_CONTINUE );
				pauseMenuPositioner->Add( add_static_text(L"Get the full game!"), EPM_TRIAL_BUY );
				pauseMenuPositioner->Apply();
				
				
				
				shownBetweenLevelTrialScreen = true;
			}
			*/
		}
	}
	
#endif
	
	
	// Special logic for when in final scene
	if (level)
	{
		if (inFinalScene && !level->IsEnding())
		{
			if (GetCurrentLevelName() == "finalscene1.lev")
			{
				// LOGIC FOR DETECTING END OF FIRST PART OF END CUTSCENE
				// (where player has control on the beach)
				if (level->GetPlayer()->GetPosition().getLength() > 25.0)
				{
					if (level->GetPlayer()->GetPosition().Z > 0)
						walkedLeft = true;
					else
						walkedLeft = false;
					
					level->StartNextLevelTransition(false);
				}
			}
			else
			{
				// Logic for detecting end of final scene, where character walks off.
				// Then the game exits (after fading out)
				if (level->GetPlayer()->GetPosition().getLength() > 50.0)
				{
					level->StartNextLevelTransition(false);
				}
			}
		}
	}
}

void MainState::OnEvent(const Event &event)
{
	if (gameEnded)
	{
		// Exit on any button press.
		if (event.IsType("ButtonDown"))
		{
#ifdef TRIAL
			engine->Exit("http://www.garnetgames.com/puzzlemoppet/trialover/");
#else
			engine->Exit();
#endif
		}
		
		return;
	}
	
	// No events if in "intro" level
	// (ignore all input, no pause menu or restarting this level)
	if (level && GetCurrentLevelName() == "intro.lev")
	{
		// EXCEPT:
		// If any button is pressed, we make the character fall immediately,
		// (assuming he has not already appeared)
		
		if (event.IsType("ButtonDown"))
		{
			core::vector3df pos = level->GetPlayer()->GetPosition();
			
			if (pos.Y > 7.f)
			{
				NOTE << "Skipping intro, player at Y: " << pos.Y;
				
				// problem, is in a MapObjectMove, due to gravity,
				// which will immediately reset the position...
				// so, HACK!
				// We access the mapObjectMove list and finish the move...
				level->ClearPlayerMapObjectMove();
				
				pos.Y = 7.f;
				level->GetPlayer()->SetPosition(pos);
			}
		}
		
		return;
	}
	
	// If in final scene, the only thing we check for is ESC key
	if (inFinalScene)
	{
		if (event.IsType("ButtonDown") &&
				// this line copy and pasted from just below
				event["button"] == KEY_ESCAPE && level && !renderSystem->IsFading() && !level->IsEnding()
				)
		{
			//engine->Exit();
			
			// changed.
			// we don't exit, just jump to the next level.
			if (level)
			{
				if (GetCurrentLevelName() == "finalscene1.lev")
				{
					walkedLeft = true;
					level->StartNextLevelTransition(false);
				}
				else
				{
					level->StartNextLevelTransition(false);
				}
			}
		}
		return;
	}
	
	if (event.IsType("RestartLevel") && !level->IsEnding())
	{
		NOTE << "Restarting level due to RestartLevel event.";
		RestartLevel();
		return;
	}
	
	// We should only be receiving ButtonDown events for now.
	
	if (event.IsType("ButtonDown"))
	{
		if (event["button"] == KEY_ESCAPE && level && !renderSystem->IsFading() && !level->IsEnding())
		{
			if (isPaused)
			{
				isPaused = false;
				
				// Un pause
				renderSystem->ScreenFade(1.0);
				world->Resume();
				device->getCursorControl()->setVisible(false);
				
				HidePauseMenu();
			}
			else
			{
				isPaused = true;
				
				// Pause
				renderSystem->ScreenFade(0.2);
				world->Pause();
				device->getCursorControl()->setVisible(true);
				
				ShowPauseMenu();
			}
		}
		else if (event["button"] == KEY_LBUTTON)
		{
			switch (pauseMenuPositioner->GetMouseOverId())
			{
			case EPM_EXIT:
				//engine->Exit();
				{
					// Continue
					HidePauseMenu();
					
					// resume... (especially, bgAmbient sfx paused in Level)
					world->Resume();
					
					// ??
					RemoveLevelAndEditor();
					
					// clear main.cpp pointer
					// this is all stupidly hacked in.
					*mainStatePtrLoc = NULL;
					
					StartScreen *startScreen = new StartScreen(mainStatePtrLoc);
					engine->GetLogicUpdater().AddUpdatable(startScreen);
					startScreen->drop();
					
					// remove this
					// we don't use RemoveUpdatable as MainState wasn't grabbed
					// by AddUpdatable... for some reason.
					drop();
					
					return;
				}
				break;
			case EPM_NEW_GAME:
				{
					pauseMenuPositioner->Reset();
					pauseMenuPositioner->SetTitle( add_static_text(L"This will completely clear your game progress. Are you sure?") );
					pauseMenuPositioner->Add( add_static_text(L"Cancel"), EPM_NEW_GAME_NO );
					pauseMenuPositioner->Add( add_static_text(L"Start New Game"), EPM_NEW_GAME_YES );
					pauseMenuPositioner->Apply();
				}
				break;
			case EPM_CHANGE_LEVEL:
				NOTE << "Change level!?";
				break;
			case EPM_SKIP_LEVEL:
				{
					pauseMenuPositioner->Reset();
					pauseMenuPositioner->SetTitle( add_static_text(L"Are you sure? You can replay this level from the main menu at any time.") );
					pauseMenuPositioner->Add( add_static_text(L"Cancel"), EPM_SKIP_LEVEL_NO );
					pauseMenuPositioner->Add( add_static_text(L"Skip Level"), EPM_SKIP_LEVEL_YES );
					pauseMenuPositioner->Apply();
				}
				break;
			case EPM_RESTART_LEVEL:
				{
					Event event("ButtonDown");
					event["button"] = KEY_ESCAPE;
					event.Send(this);
					RestartLevel();
				}
				break;
			case EPM_CONTINUE:
				{
					// Override the mouse button state.
					// This is needed otherwise the GUI click in the menu causes zoom to occur when resuming the level.
					engine->SetButtonState(KEY_LBUTTON, false);
					
					// Continue
					Event event("ButtonDown");
					event["button"] = KEY_ESCAPE;
					event.Send(this);
				}
				break;

#ifdef TRIAL
			// TRIAL thing
			case EPM_TRIAL_BUY:
				engine->Exit("http://www.garnetgames.com/puzzlemoppet/buy/");
				break;
#endif
			
			// New Game sub menu
			case EPM_NEW_GAME_YES:
				NOTE << "NEW GAME STARTED!";
				{
					Event event("ButtonDown");
					event["button"] = KEY_ESCAPE;
					event.Send(this);
					
					// Remove the save file
					delete_game_saves();
					
					ShowInitTexts();
					StartFirstLevel();
				}
				return;
			case EPM_NEW_GAME_NO:
				ShowPauseMenu();
				return;
			
			
			// Skip Level sub menu
			case EPM_SKIP_LEVEL_YES:
				NOTE << "User decided to skip level...";
				{
					Event event("ButtonDown");
					event["button"] = KEY_ESCAPE;
					event.Send(this);
					
					NextLevel();
				}
				return;
			case EPM_SKIP_LEVEL_NO:
				ShowPauseMenu();
				return;
			};
		}
	}
	else if (event.IsType("AxisMoved"))
	{
		if (event["axis"] == AXIS_MOUSE_X || event["axis"] == AXIS_MOUSE_Y)
		{
			gui::IGUIElement *mouseOverElement = pauseMenuPositioner->GetMouseOverElement();
			
			// Update all colours
			
			const std::vector<gui::IGUIElement *> &elements = pauseMenuPositioner->GetElements();
			
			for (u32 i = 0; i < elements.size(); i ++)
			{
				gui::IGUIStaticText *textElement = (gui::IGUIStaticText *)elements[i];
				
				if (elements[i] == mouseOverElement)
				{
					if (textElement->getOverrideColor() != TEXT_COL_MOUSEOVER)
						menuSound->Play("../projects/Puzzle/media/sfx/beep.ogg");
					
					textElement->setOverrideColor( TEXT_COL_MOUSEOVER );
				}
				else
				{
					textElement->setOverrideColor( TEXT_COL );
				}
			}
		}
	}
}

void MainState::ShowPauseMenu()
{
	// Clears intro texts if we show pause menu
	RemoveInitTexts();
	RemoveStartLevelTexts();
	
	
	
	pauseMenuPositioner->Reset();
	//pauseMenuPositioner->Add( add_static_text(L"Exit"), EPM_EXIT );
	pauseMenuPositioner->Add( add_static_text(L"Quit to Menu"), EPM_EXIT );
	//pauseMenuPositioner->Add( add_static_text(L"Change Level"), EPM_CHANGE_LEVEL );
	//pauseMenuPositioner->Add( add_static_text(L"New Game"), EPM_NEW_GAME );
	pauseMenuPositioner->Add( add_static_text(L"Skip Level"), EPM_SKIP_LEVEL);
	pauseMenuPositioner->Add( add_static_text(L"Restart Level"), EPM_RESTART_LEVEL );
	pauseMenuPositioner->Add( add_static_text(L"Continue"), EPM_CONTINUE );
	pauseMenuPositioner->Apply();
	
	/*
	levelSelectMenuPositioner->Reset();
	levelSelectMenuPositioner->Add( add_static_text(L"<"), -1 );
	levelSelectMenuPositioner->Add( add_static_text(L"1 / 32"), -1 );
	levelSelectMenuPositioner->Add( add_static_text(L">"), -1 );
	levelSelectMenuPositioner->Apply();
	
	levelConfirmMenuPositioner->Reset();
	levelConfirmMenuPositioner->Add( add_static_text(L"Cancel"), -1 );
	levelConfirmMenuPositioner->Add( add_static_text(L"OK"), -1 );
	levelConfirmMenuPositioner->Apply();
	*/
}

void MainState::HidePauseMenu()
{
	pauseMenuPositioner->Reset();
}





