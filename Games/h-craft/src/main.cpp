// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "main.h"
#include "config.h"
#include "app_tester.h"
#include "app_restrictions.h"
#include "advert.h"
#include "billing.h"
#include "level.h"
#include "gui_dialogs/gui_controller_display.h"
#include "gui_dialogs/gui_intro.h"
#include "gui_dialogs/gui_game.h"
#include "gui_dialogs/gui_hud.h"
#include "gui_dialogs/gui_menu_main.h"
#include "gui_dialogs/gui_menu_newprofile.h"
#include "gui.h"
#include "sound_openal.h"
#include "input_device_manager.h"
#include "controller.h"
#include "node_manager.h"
#include "profiles.h"
#include "game.h"
#include "player.h"
#include "editor.h"
#include "physics.h"
#include "music_manager.h"
#include "random.h"
#include "string_table.h"
#include "gui_dialog.h"
#include "font_manager.h"
#include "championship.h"
#include "highscores.h"
#include "rivalsmode.h"
#include "logging.h"
#include "tinyxml/tinyxml.h"

#ifdef _IRR_ANDROID_PLATFORM_
#include <android_native_app_glue.h>
#include <android/log.h>
#include "advert_admob.h"
#include "billing_googleplay.h"
#endif

#if defined(_IRR_COMPILE_WITH_X11_DEVICE_) && !defined(_IRR_ANDROID_PLATFORM_)
#include <X11/Xlib.h>
#endif

using namespace irr;

App APP;

App::App()
: mIrrlichtManager(0)
, mInputDeviceManager(0)
, mController(0)
, mNodeManager(0)
, mGui(0)
, mConfig(0)
, mLevelManager(0)
, mGame(0)
, mEditor(0)
, mPhysics(0)
, mSound(0)
, mMusicManager(0)
, mRandomGenerator(0)
, mStringTable(0)
, mFontManager(0)
, mProfileManager(0)
, mChampionship(0)
, mHighscoreManager(0)
, mRivalsMode(0)
, mAppTester(0)
, mAndroidApp(0)
, mLastAdTime(0)
, mAdvert(0)
, mInAppBilling(0)
{
    mIsRunning = false;
    mMode = MODE_GUI;
#ifdef HC1_ENABLE_EDITOR
    mEditGui = true;
#else
    mEditGui = false;
#endif
    mDrawAiTrack = false;
}

App::~App()
{
    // cleanup will be mostly done in App::Quit
    mIsRunning = false;
}

void App::StopApp()
{
	#if defined(_IRR_ANDROID_PLATFORM_)
	ANativeActivity_finish(mAndroidApp->activity);
	#else
    mIsRunning = false;
    #endif
}

bool App::Init(int argc, char *argv[], void * systemData)
{
#ifdef _IRR_ANDROID_PLATFORM_
	mAndroidApp = (android_app*)systemData;
#endif

    //TiXmlBase::SetCondenseWhiteSpace(false);

    LogPriority prioFile = LP_INFO;
    LogPriority prioStderr = LP_NONE;
    LogPriority prioStdout = LP_DEBUG; // LP_WARN;
    LogPriority prioAndroid = LP_DEBUG;
    LogPriority prioStream = LP_INFO;	// only matters when streaming log to a gui-dialog

#if defined(HOVER_RELEASE)
	prioStream = LP_NONE;
	prioStdout = LP_WARN;
#endif

    std::string testFile;

    // parse params
    if ( argc > 1 )
    {
        for ( int i=1; i < argc; ++i )
        {
			const char * arg = argv[i];
            if ( 0 == strncmp( arg, "--help", strlen("--help") ) )
            {
                std::cerr << "The following parameters are valid:" << std::endl
                    << "-logfile=LEVEL    where LEVEL is a number from 0 (verbose logfile) to 5 (dumb logfilee)" << std::endl
                    << "-logout=LEVEL     where LEVEL is a number from 0 (verbose stdout) to 5 (dumb stdout)" << std::endl
                    << "-logerr=LEVEL     where LEVEL is a number from 0 (verbose stderr) to 5 (dumb stderr)" << std::endl
                    << "-ghostname=NAME   where NAME will override the usual profilename used when exporting ghosts" << std::endl
                    << "-test=NAME        where NAME must a file in the tests folder containing the tests to run" << std::endl
                    ;

                return false;
            }
            else if ( 0 == strncmp( arg, "-logfile=", strlen("-logfile=") ) )
            {
                prioFile = (LogPriority)atoi( &(arg[strlen("-logfile=")]) );
            }
            else if ( 0 == strncmp( arg, "-logout=", strlen("-logout=") ) )
            {
                prioStdout = (LogPriority)atoi( &(arg[strlen("-logout=")]) );
            }
            else if ( 0 == strncmp( arg, "-logerr=", strlen("-logerr=") ) )
            {
                prioStderr = (LogPriority)atoi( &(arg[strlen("-logerr=")]) );
            }
            else if ( 0 == strncmp( arg, "-ghostname=", strlen("-ghostname=") ) )
            {
                mOverrideExportName = std::string(&(arg[strlen("-ghostname=")]));
            }
            else if ( 0 == strncmp( arg, "-test=", strlen("-test=") ) )
            {
                testFile = std::string(&(arg[strlen("-test=")]));
            }
        }
    }

    LOG.SetStdoutPriority(prioStderr);
    LOG.SetStderrPriority(prioStdout);
    LOG.SetAndroidPriority(prioAndroid);
    LOG.SetStreamPriority(prioStream);
    LOG.LogLn(LP_INFO_ESSENTIAL, "Log set-up.");

    mRandomGenerator = new RandomGenerator();
    mIrrlichtManager = new IrrlichtManager();
    mConfig = new Config(systemData);
    mGui = new Gui(*mConfig);
    mInputDeviceManager = new InputDeviceManager(*mConfig);
    mController = new Controller();
    mLevelManager = new LevelManager();
    mNodeManager = new NodeManager();
    mGame = new Game();
#ifdef HC1_ENABLE_EDITOR
    mEditor = new Editor();
#endif
    mPhysics = new Physics();
#ifdef HC1_ENABLE_SOUND
    mMusicManager = new MusicManager();
#endif
    mStringTable = new StringTable();
    mFontManager = new FontManager();
    mProfileManager = new ProfileManager();
    mChampionship = new Championship();
    mHighscoreManager = new HighscoreManager();
    mRivalsMode = new RivalsMode();
    mAppTester = new AppTester();
#if defined(HOVER_ADS)
#ifdef _IRR_ANDROID_PLATFORM_
	if (mAndroidApp)
	{
		mAdvert = new AdvertAdmob(*mAndroidApp);
		mInAppBilling = new BillingGooglePlay(*mAndroidApp);
	}
#else
	mAdvert = new IAdvert();
	mInAppBilling = new IBilling();
#endif
#endif

    mConfig->Init(argv[0]);

#ifndef _IRR_ANDROID_PLATFORM_
    if ( !LOG.OpenFile(prioFile, mConfig->MakeFilenameBase("logging.txt").c_str(), "wt") )
    {
        if ( !LOG.OpenFile(prioFile, "logging.txt", "wt") )
        {
            fprintf(stderr, "Error: could not open logging file\n");
        }
    }
#endif
    LOG.LogLn(LP_INFO_ESSENTIAL, "compiled date ", __DATE__);
    LOG.LogLn(LP_INFO_ESSENTIAL, "compiled time ", __TIME__);
    LOG.LogLn(LP_DEBUG, "log priority file: ", (int)prioFile);
    LOG.LogLn(LP_DEBUG, "log priority stderr: ", (int)prioStderr);
    LOG.LogLn(LP_DEBUG, "log priority stdout: ", (int)prioStdout);
    LOG.LogLn(LP_DEBUG, "log priority stream: ", (int)prioStream);

#ifdef _IRR_ANDROID_PLATFORM_
	// we need the Irrlicht filesystem already for loading the config on Android
	irr::IrrlichtDevice* irrDevice = mIrrlichtManager->CreateIrrlichtDeviceAndroid(*mConfig, mAndroidApp);
	if ( irrDevice && !mConfig->Load(irrDevice->getFileSystem()) )
#else
    if ( !mConfig->Load() )
#endif
    {
        LOG.Warn(L"mConfig->Load failed\n");
        return false;
    }

    if ( LOG.GetFilePriority() == LP_DEBUG )
    {
        std::string strConfig;
        mConfig->WriteToString(strConfig);
        LOG.Debug(strConfig);
        LOG.Debug("\n");
    }

#ifndef _IRR_ANDROID_PLATFORM_
	irr::IrrlichtDevice* irrDevice = mIrrlichtManager->CreateIrrlichtDevicePC(*mConfig);
#endif
    if ( !mIrrlichtManager->Init(*mConfig) )
    {
        LOG.Error("error: could not initialize Irrlicht\n");
        return false;
    }

    LOG.Info(L"mGui->Init\n");
    mGui->Init(irrDevice->getGUIEnvironment(), mIrrlichtManager->GetVideoDriver(), irrDevice->getFileSystem(), GetFontManager());
    mGui->PrintDriverFeatures();

#ifdef _IRR_ANDROID_PLATFORM_
	mGui->SetEditGuiVisible(false);
#endif

    // We want to show the intro as soon as possible (as it's the first thing visible on the screen)
    LOG.Debug(L"CreateIntro\n");
    mGui->LoadIntro();

    mGui->SetActiveDialog(mGui->GetGuiIntro());
    ForceUpdates(true, false);
#ifdef __GNUWIN32__
    ForceUpdates(true, false);  // reduce chance of taskbar is in the foreground in XP and make sure intro is displayed in 98
#endif // ifdef __GNUWIN32__
	u32 timeIntroStart = GetIrrlichtManager()->GetIrrlichtTimer()->getRealTime();
	mLastAdTime = timeIntroStart;

	mStringTable->SetIrrFs(mIrrlichtManager->GetIrrlichtDevice()->getFileSystem());
    if (! mStringTable->Load(mConfig->GetStringTableName().c_str()) )
    {
        LOG.Warn(L"mStringTable->Load failed\n");
    }

#if defined(_IRR_COMPILE_WITH_X11_DEVICE_) && !defined(_IRR_ANDROID_PLATFORM_)
	// This should prevent screensaver from starting (on Windows Irrlicht does that)
	// TODO: not yet tested if it works.
	XSetScreenSaver((Display*)(mIrrlichtManager->GetVideoDriver()->getExposedVideoData().OpenGLLinux.X11Display), 0, 0, DefaultBlanking, DefaultExposures);
#endif

#ifdef HC1_ENABLE_SOUND
    if ( !mConfig->GetDisableSound() )
    {
        LOG.Debug(L"Create sound\n");
#ifdef HC1_ENABLE_OPENAL
        mSound = new SoundOpenAL();


	#ifdef _IRR_ANDROID_PLATFORM_
        mSound->SetIrrFs(mIrrlichtManager->GetIrrlichtDevice()->getFileSystem());
	#endif
#endif
		if ( mSound )
			mSound->Init();
    }
#endif

	// Request connection to billing and ad-server early as those requests take some time
#if defined(HOVER_ADS)
	if ( mInAppBilling )
	{
		mInAppBilling->init(mConfig->GetBillingProviderSettings());
		mInAppBilling->requestServerConnnection();
	}

	if ( mAdvert )
	{
		mAdvert->init(mConfig->GetAdProviderSettings());
		mAdvert->request(EAT_FULLSCREEN);
	}
#endif


    LOG.Debug(L"LoadHovers\n");
    LoadHovers();

    LOG.Debug(L"Load hover\n");
	HC1_PROFILE( s32 profHover = getProfiler().add(L"Hover model", L"STARTUP"); )
	HC1_PROFILE(getProfiler().start(profHover);)
	std::string hoverName(APP.GetConfig()->MakeFilenameModels("Hover01_Body.obj"));
    scene::ISceneNode* node = APP.GetIrrlichtManager()->LoadModel(*mConfig, hoverName.c_str(), NULL);
    HC1_PROFILE(getProfiler().stop(profHover);)
    if ( node )
    {
        GetConfig()->SetHoverRadius(node->getBoundingBox().getExtent().X * 0.5f);
        node->setVisible(false);
        node->remove();
    }
    else
    {
        LOG.Warn(L"WARNING: i need Hover01_Body.obj\n");
    }

    LOG.Debug(L"mInputDeviceManager->CreateAllDevices\n");
    mInputDeviceManager->CreateAllDevices(mIrrlichtManager->GetIrrlichtDevice()->getCursorControl());
    LOG.Debug(L"mInputDeviceManager->ReadFromXml\n");
    mInputDeviceManager->ReadFromXml(mConfig->GetInputSettings());
    LOG.Debug(L"mController->ReadFromXml\n");
    mController->ReadFromXml(mConfig->GetInputSettings());

    LOG.Debug(L"mLevelManager->LoadSettings\n");
    mLevelManager->LoadSettings();
    LOG.Debug(L"mChampionship->LoadSettings\n");
    mChampionship->LoadSettings();
    LOG.Debug(L"mProfileManager->LoadProfiles\n");
    mProfileManager->LoadProfiles();
    LOG.Debug(L"mNodeManager->LoadSettings\n");
    mNodeManager->LoadSettings();

    LOG.Debug(L"mGame->Init\n");
    mGame->Init();

    LOG.Debug(L"Load Highscores\n");
    mHighscoreManager->LoadOrCreate();

#ifdef HC1_ENABLE_EDITOR
	if ( mEditor )
	{
		LOG.Debug(L"mEditor->Init\n");
		mEditor->Init();
	}
#endif
	if ( mMusicManager )
	{
		LOG.Debug(L"mMusicManager->Init\n");
		mMusicManager->Init(mSound);
	}

    LOG.Debug(L"mGui->LoadMenuDialogs\n");
    mGui->LoadMenuDialogs();
    LOG.Debug(L"mGui->LoadMenuDialogs ok\n");

    // We want the intro to be visible at least for minimum time
    const u32 minTimeIntro = 2100;	// try&error value
	u32 timeIntroShown = GetIrrlichtManager()->GetIrrlichtTimer()->getRealTime()-timeIntroStart;
	if (timeIntroShown < minTimeIntro)
	{
		GetIrrlichtManager()->GetIrrlichtDevice()->sleep(minTimeIntro-timeIntroShown);
	}

    if ( mProfileManager->GetSelectedProfile() )
    {
        mGui->SetActiveDialog(mGui->GetGuiMenuMain());
    }
    else
    {
        mGui->GetGuiMenuNewProfile()->InfoFirstProfile();
        mGui->SetActiveDialog(mGui->GetGuiMenuNewProfile());
    }

    HC1_PROFILE(
		core::stringw profileResult;
		getProfiler().printAll(profileResult);
		LOG.Debug(profileResult.c_str());
	)

    //testFile = "one_level.xml";
    //testFile = "levels.xml";
    //testFile = "long_level.xml";
    if ( !testFile.empty() )
    {
		mAppTester->Init(this);
		testFile = mConfig->MakeFilenameTests(testFile);
		mAppTester->Load(testFile.c_str(), mIrrlichtManager->GetIrrlichtDevice()->getFileSystem());
    }

    return true;
}

void App::SetEditGui(bool enable_)
{
    if ( enable_ != mEditGui )
    {
        EndOldMode();
        mEditGui = enable_;
        SetNewMode(mMode);

        if ( enable_ )
        {
            mGui->SetEditGuiVisible(true);
        }
        else
        {
            mGui->SetEditGuiVisible(false);
        }
    }
}

void App::LoadHovers()
{
    mHovers.clear();
    TiXmlElement * eleHovers = mConfig->GetHoverSettings();
    if ( eleHovers )
    {
        const TiXmlNode* nodeHover = eleHovers->IterateChildren("hover", NULL);
        while ( nodeHover )
        {
            Hover hover;
            hover.ReadFromXml(nodeHover->ToElement());
            mHovers.push_back(hover);

            nodeHover = eleHovers->IterateChildren("hover", nodeHover);
        }
    }
}

void App::EndOldMode()
{
    // Paranoid pointer checking because for test purposes all those pointers are sometimes set to 0
    // and it won't hurt here

    switch ( mMode )
    {
        case MODE_GUI:
        break;
        case MODE_FREEFLY:
        break;
        case MODE_GAME:
        {
#ifdef HC1_ENABLE_EDITOR
            if ( mEditGui && mGui )
            {
                mGui->GetGuiGame()->HideGameGui();
                mGui->GetControllerDisplay()->setVisible(false);
            }
#endif
        }
        break;
        case MODE_EDITOR:
        {
#ifdef HC1_ENABLE_EDITOR
            if ( mEditGui && mLevelManager && mEditor )
            {
                mLevelManager->ShowEditData(false);
                mEditor->Stop();
            }
#endif
        }
        break;
        case MODE_GUI_DLG_TEST:
        {
#ifdef HC1_ENABLE_EDITOR
            if ( mGui )
            {
                mGui->GetTestDialog()->Hide();
            }
#endif
        }
        break;
    }
}

void App::SetNewMode(APP_MODES mode_)
{
    // Paranoid pointer checking because for test purposes all those pointers are sometimes set to 0
    // and it won't hurt here

    // set new mode
    switch ( mode_ )
    {
        case MODE_GUI:
        {
            if ( mGui )
            {
                mGui->SetActiveDialog(mGui->GetActiveDialog());
            }
            if ( mIrrlichtManager )
            {
                if ( mEditGui )
                {
                    mIrrlichtManager->SetCameraMaya();
                }
                else
                {
                    mIrrlichtManager->SetCameraGui();
                }
            }
        }
        break;
        case MODE_FREEFLY:
        {
            if ( mIrrlichtManager )
            {
                mIrrlichtManager->SetCameraFPS();
            }
        }
        break;
        case MODE_GAME:
        {
            if ( mGui )
            {
                mGui->SetActiveDialog(mGui->GetGuiHud());
                if ( mEditGui )
                {
#ifdef HC1_ENABLE_EDITOR
                    mGui->GetGuiGame()->CreateGameGui();
#endif
                }
            }

            if ( mGame && mIrrlichtManager )
            {
                if ( mGame->GetAiTraining() )
                {
                    mIrrlichtManager->SetCameraEditor();
                }
                else
                {
                    mIrrlichtManager->SetCameraGame(mConfig->GetUseTouchInput() == ETI_NO_TOUCH);
                }
            }
            // if ( mEditGui )
            //  mGui->GetControllerDisplay()->setVisible(true);
        }
        break;
        case MODE_EDITOR:
        {
#ifdef HC1_ENABLE_EDITOR
            if ( mEditGui && mEditor && mIrrlichtManager && mLevelManager )
            {
                mEditor->Start();
                mIrrlichtManager->SetCameraEditor();
                mLevelManager->ShowEditData(true);
            }
#endif
        }
        break;
        case MODE_GUI_DLG_TEST:
        {
#ifdef HC1_ENABLE_EDITOR
            if ( mGui )
            {
                mGui->GetTestDialog()->Show();
            }
#endif
        }
        break;
    }
    if ( mSound )
    {
        mSound->StopMusic();
    }
    mMode = mode_;
}

void App::SetMode(APP_MODES mode_)
{
    EndOldMode();

    SetNewMode(mode_);
}

void App::LoadSettings()
{
    GetConfig()->Load();
    mGame->ReloadSettingsCamera();
    mGame->ReloadSettingsSteering();
    mGame->ReloadSettingsPhysics();
}

void App::SaveSettings()
{
    mInputDeviceManager->WriteToXml(mConfig->GetInputSettings());
    mController->WriteToXml(mConfig->GetInputSettings());
    if ( GetConfig() )
        GetConfig()->Save();
}

void App::HackBadFPS()
{
    // TEST - simulate bad framerate
    static int freezCount = 0;
    if ( ++freezCount == 10 )
    {
        float dummy= 0.f;
        freezCount = 0;
        for ( int wait=0; wait<4000000; wait++)
        {
            dummy += sqrt((float)wait);
        }
    }
}

void App::AdvertismentCheck()
{
#if defined(HOVER_ADS)
	if ( DO_DISPLAY_ADS )
	{
		const unsigned int MIN_TIME_BETWEEN_ADS_MS = 1000*60*5;	// Only every few minutes, don't annoy people.
	    unsigned int timeNow = UINT_MAX;
		if ( GetIrrlichtManager() )
		{
			timeNow = GetIrrlichtManager()->GetIrrlichtTimer()->getRealTime();
		}
		if ( mLastAdTime == 0 )
		{
			mLastAdTime = timeNow;
		}
		if ( timeNow < mLastAdTime+MIN_TIME_BETWEEN_ADS_MS )
			return;

		mLastAdTime = timeNow;

		if ( mAdvert && mAdvert->show(EAT_FULLSCREEN) )
		{
			ForceUpdates(true, false);

			// request next one immediately
			mAdvert->request(EAT_FULLSCREEN);
		}
	}
#endif
}

void App::ForceUpdates(bool graphics_, bool sound_)
{
    if ( graphics_)
    {
        GetIrrlichtManager()->ForceIrrlichtUpdate();
    }
    if ( sound_ )
    {
        UpdateSound();
    }
}

void App::UpdateSound()
{
    if ( !mSound )
        return;

    unsigned int timeMs = 0;

    if ( GetIrrlichtManager() )
    {
        timeMs = GetIrrlichtManager()->GetIrrlichtTimer()->getRealTime();
    }

    core::vector3df listenerPos(0.0 ,0.0 ,0.0);
    core::vector3df listenerAt(0.0, 0.0, 1.0);
    core::vector3df listenerUp(0.0, 1.0, 0.0);
    bool hasPlayerMatrix = false;

    if ( GetGame() && GetGame()->GetLocalPlayer() )
    {
        const Player * player = GetGame()->GetLocalPlayer();
        scene::ISceneNode * hoverPlayer = player->GetHoverNode();
        if ( hoverPlayer )
        {
            hasPlayerMatrix = true;
            const core::matrix4 & matHover = hoverPlayer->getAbsoluteTransformation();
            listenerPos = matHover.getTranslation();
            matHover.rotateVect(listenerAt);
            matHover.rotateVect(listenerUp);
        }
    }

    if ( !hasPlayerMatrix && GetIrrlichtManager() && GetIrrlichtManager()->GetActiveCamera())
    {
        listenerPos = GetIrrlichtManager()->GetActiveCamera()->getPosition();
        listenerAt = GetIrrlichtManager()->GetActiveCamera()->getTarget()  - GetIrrlichtManager()->GetActiveCamera()->getPosition();
        listenerUp = GetIrrlichtManager()->GetActiveCamera()->getUpVector();
    }

    mSound->Update(listenerPos, listenerAt, listenerUp, timeMs);
}

void App::Run()
{
    PROFILE_ADD_GROUP(PGROUP_MAIN, "main group");
    PROFILE_ADD(100, PGROUP_MAIN, "main");
    PROFILE_ADD(101, PGROUP_MAIN, "beginScene");
    PROFILE_ADD(102, PGROUP_MAIN, "inputmgr");
    PROFILE_ADD(103, PGROUP_MAIN, "game");
    PROFILE_ADD(104, PGROUP_MAIN, "editor");
    PROFILE_ADD(105, PGROUP_MAIN, "smgr-draw");
    PROFILE_ADD(106, PGROUP_MAIN, "env-draw");
    PROFILE_ADD(107, PGROUP_MAIN, "gui");
    PROFILE_ADD(108, PGROUP_MAIN, "soundupdate");
    PROFILE_ADD(109, PGROUP_MAIN, "endScene");

    mIsRunning = true;

    if ( mAppTester->HasTest() && APP.GetEditGui() )
    {
		APP.SetEditGui(false);
	}

    IrrlichtDevice * irrlichtDevice = mIrrlichtManager->GetIrrlichtDevice();
    video::IVideoDriver* videoDriver =  mIrrlichtManager->GetVideoDriver();
    gui::IGUIEnvironment* env = irrlichtDevice->getGUIEnvironment();
    scene::ISceneManager* sceneManager = mIrrlichtManager->GetSceneManager();
	bool runningAppTest = mAppTester->HasTest();

    LOG.Info(L"start running\n");

	// draw everything
	while(mIsRunning)
	{
	    if ( !irrlichtDevice->run() )
	    {
			LOG.Info(L"stop running\n");
            break;
		}

		// nothing to do in minimized state except waiting until this changes
		if (irrlichtDevice->isWindowMinimized())
		{
			irrlichtDevice->sleep(100);
			continue;
		}

        PROFILE_START(108);
		UpdateSound();
        PROFILE_STOP(108);

		// inactive - just update less often
        if (!irrlichtDevice->isWindowActive() )
		{
			irrlichtDevice->sleep(10);
		}

        // cap framerate
        static u32 oldTime = GetIrrlichtManager()->GetIrrlichtTimer()->getRealTime();
        u32 newTime = GetIrrlichtManager()->GetIrrlichtTimer()->getRealTime();
        if ( newTime-oldTime < (u32)GetConfig()->GetMinTimePerFrameMs() )
        {
            continue;
		}
        oldTime = newTime;

		PROFILE_START(100);

		PROFILE_START(101);
		// begin-scene early do allow drawing debug-output in the rest of updates
		videoDriver->beginScene(true, true, video::SColor(150,50,50,200));
		PROFILE_STOP(101);

		if ( mSound && mMode == MODE_GUI && !mSound->IsMusicPlaying() )
		{
			mMusicManager->PlayMenuMusic();
		}

		PROFILE_START(102);
		mInputDeviceManager->Update();
		mController->Update(*mInputDeviceManager, mConfig->GetUseTouchInput());
		PROFILE_STOP(102);

		PROFILE_START(103);
		mGame->Update();
		PROFILE_STOP(103);

#ifdef HC1_ENABLE_EDITOR
		if ( mEditor )
		{
			PROFILE_START(104);
			mEditor->Update();
			PROFILE_STOP(104);
		}
#endif

		//HackBadFPS();

		// draw everything
		PROFILE_START(105);
		if (    GetMode() != MODE_GUI
			||  !mGui->GetActiveDialog()
			||  !mGui->GetActiveDialog()->GetSuppressSceneRendering()
			)
		{
			sceneManager->drawAll();
		}
		PROFILE_STOP(105);

		PROFILE_START(106);
		env->drawAll();
		PROFILE_STOP(106);

		if ( mDrawAiTrack )
		{
			APP.GetLevelManager()->GetAiTrack().Draw();
		}

		PROFILE_START(109);
		videoDriver->endScene();
		PROFILE_STOP(109);

		PROFILE_START(107);
		mGui->Update(newTime);
		PROFILE_STOP(107);

		PROFILE_STOP(100);

		if ( runningAppTest )
		{
			if ( !mAppTester->Update() )
				break;
		}

		// be nice
		irrlichtDevice->yield();
	}
}

void App::Quit()
{
    LOG.Info(L"clean quit start\n");

    mIsRunning = false;

	if ( mIrrlichtManager )
		mIrrlichtManager->ShutDownEventReceiver();

#if defined(HOVER_ADS)
	delete mAdvert;
	mAdvert = NULL;
	delete mInAppBilling;
	mInAppBilling = NULL;
#endif
    delete mAppTester;
    mAppTester = NULL;
    delete mRivalsMode;
    mRivalsMode = NULL;
    delete mHighscoreManager;
    mHighscoreManager = NULL;
    delete mChampionship;
    mChampionship = NULL;
    delete mProfileManager;
    mProfileManager = NULL;
    delete mFontManager;
    mFontManager = NULL;
    delete mStringTable;
    mStringTable = NULL;
    delete mMusicManager;
    mMusicManager = NULL;
    delete mSound;
    mSound = NULL;
    delete mPhysics;
    mPhysics = NULL;
#ifdef HC1_ENABLE_EDITOR
    delete mEditor;
#endif
    mEditor = NULL;
    delete mGame;
    mGame = NULL;
    delete mNodeManager;
    mNodeManager = NULL;
    delete mLevelManager;
    mLevelManager = NULL;
    delete mController;
    mController = NULL;
    delete mInputDeviceManager;
    mInputDeviceManager = NULL;
    delete mGui;
    mGui = NULL;
    delete mConfig;
    mConfig = NULL;
    delete mRandomGenerator;
    mRandomGenerator = NULL;

	if ( mIrrlichtManager )
		mIrrlichtManager->Quit();
    delete mIrrlichtManager;
    mIrrlichtManager = NULL;

    LOG.Info(L"clean quit successful\n");
}

#ifndef _IRR_ANDROID_PLATFORM_
int main(int argc, char *argv[])
{
    if ( !APP.Init(argc, argv) )
    {
		APP.Quit();
        return 1;
	}

    LOG.Info(L"Init finished\n");

    APP.SetMode(MODE_GUI);

    LOG.Info(L"MODE_GUI enabled\n");

	//APP.GetIrrlichtManager()->LogAllTexturesInformation(LP_DEBUG);

    APP.Run();
    APP.Quit();

	return 0;
}
#endif

#ifdef _IRR_ANDROID_PLATFORM_
void android_main(android_app* app)
{
	// Make sure glue isn't stripped.
	app_dummy();

	__android_log_print(ANDROID_LOG_DEBUG, "Irrlicht", "starting android_main\n");

	char ** argv;
	argv = (char **)malloc(1 * sizeof(char*));
    argv[0] = (char*)malloc(255 * sizeof(char));
    strcpy( argv[0], "h-craft");

	if ( !APP.Init(1, argv, app) )
	{
		APP.Quit();
		return;
	}

	LOG.Info(L"Init finished\n");
	__android_log_print(ANDROID_LOG_INFO, "Irrlicht", "init finished\n");

	APP.SetMode(MODE_GUI);

	LOG.Info(L"MODE_GUI enabled\n");

	//APP.GetIrrlichtManager()->LogAllTexturesInformation(LP_DEBUG);
	APP.Run();
	APP.Quit();
}
#endif
