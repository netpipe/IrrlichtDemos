// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "game.h"
#include "physics.h"
#include "timer.h"
#include "main.h"
#include "config.h"
#include "input_device_manager.h"
#include "music_manager.h"
#include "level.h"
#include "controller.h"
#include "sound.h"
#include "player.h"
#include "camera.h"
#include "profiles.h"
#include "highscores.h"
#include "string_table.h"
#include "gui.h"
#include "logging.h"
#include "rivalsmode.h"
#include "helper_str.h"
#include "streaming.h"
#include "gui_dialogs/gui_game.h"
#include "gui_dialogs/gui_hud.h"
#include "gui_dialogs/gui_touch.h"
#include "gui_dialogs/gui_helper.h"
#include "gui_dialogs/gui_menu_pause.h"
#include "gui_dialogs/gui_menu_gameend.h"
#include "gui_dialogs/gui_menu_gameend_champ.h"
#include "gui_dialogs/gui_menu_gameend_rivals.h"
#include "recorder.h"
#include "random.h"
#include "nn.h"
#include "tinyxml/tinyxml.h"

#include <sstream>

using namespace irr;

s32 Game::TIME_UNUSED = 3599999;    // 59:59:99
int GhostRecordSettings::VERSION = 3;

GhostRecordSettings::GhostRecordSettings()
: mVersion(VERSION)
{
}

void GhostRecordSettings::Write(MemArchive * serializeArchive_)
{
    if  (!serializeArchive_ )
        return;

    serializeArchive_->Clear();   // it's the only struct in that archive

    serializeArchive_->Write(mVersion);
    std::string dummyName( ExtStr::ToUtf8(mProfileName) );

    serializeArchive_->WriteString(dummyName.c_str());
    std::string dummnyLevel( ExtStr::ToUtf8(mLevelName) );
    serializeArchive_->WriteString(dummnyLevel.c_str());
    serializeArchive_->Write(mGhostType);
    serializeArchive_->Write(mTime);
    serializeArchive_->Write(mChecksum);
}

void GhostRecordSettings::Read( MemArchive * serializeArchive_)
{
    if  (!serializeArchive_ || !serializeArchive_->GetSize() )
        return;

    serializeArchive_->SetReadPos(0);   // it's the only struct in that archive

    serializeArchive_->Read(mVersion);

    if ( mVersion < 3 )
        return; // no longer supported

    char * str = 0;
    if ( serializeArchive_->ReadString(&str) > 1 )
    {
        std::string dummyName( str );
        mProfileName = ExtStr::FromUtf8(dummyName);
        delete[] str;
        str = 0;
    }
    if ( serializeArchive_->ReadString(&str) > 1 )
    {
        std::string dummyLevel( str );
        mLevelName = ExtStr::FromUtf8(dummyLevel);
        delete[] str;
        str = 0;
    }
    serializeArchive_->Read(mGhostType);
    serializeArchive_->Read(mTime);
    serializeArchive_->Read(mChecksum);
}

int GhostRecordSettings::CalcChecksum()
{
	// Man - how I hate it to get back to years old code and all I find is a stupid comment like that *sigh*
	// I guess I just used 2 random numbers.
    return ((mGhostType + mTime) * 3771) | 4711; // this cryptographic formula will definitly revolutionize the world!!!
}

SteeringSettings::SteeringSettings()
{
	mSteeringStyle = 1;
    mRotationSpeed = 4.f;
    mScaleMinSpeed = 0.f;
    mScaleRotationBySpeed = 0.f;
    mForceFactor = 10000.f;
    mForceSideFactor = 0.f;
    mBrakeFactor = 0.f;
    mScaleRolling = 1.f;
    mScaleRollingBack = 1.f;
    mMaxRolling = 45.f;
    mScalePitchFlying = 0.f;
    mScaleAlignment = 1.f;
    mRelocateNoColl = 3000;
    mRelocateFreeze = 0;
    mResumeFreeze = 1000;
}

void SteeringSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

	settings_->QueryIntAttribute("steering_style", &mSteeringStyle);
    settings_->QueryFloatAttribute("rotation_speed", &mRotationSpeed);
    settings_->QueryFloatAttribute("scale_min_speed", &mScaleMinSpeed);
    settings_->QueryFloatAttribute("scale_rotation_speed", &mScaleRotationBySpeed);
    settings_->QueryFloatAttribute("force_factor", &mForceFactor);
    settings_->QueryFloatAttribute("brake_factor", &mBrakeFactor);
    settings_->QueryFloatAttribute("side_factor", &mForceSideFactor);
    settings_->QueryFloatAttribute("scale_rolling", &mScaleRolling);
    settings_->QueryFloatAttribute("scale_rolling_back", &mScaleRollingBack);
    settings_->QueryFloatAttribute("max_rolling", &mMaxRolling);
    settings_->QueryFloatAttribute("scale_pitch_flying", &mScalePitchFlying);
    settings_->QueryFloatAttribute("scale_alignment", &mScaleAlignment);
    settings_->QueryIntAttribute("relocate_no_coll", &mRelocateNoColl);
    settings_->QueryIntAttribute("relocate_freeze", &mRelocateFreeze);
    settings_->QueryIntAttribute("resume_freeze", &mResumeFreeze);
}

void SteeringSettings::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

	settings_->SetAttribute("steering_style", mSteeringStyle);
    settings_->SetDoubleAttribute("rotation_speed", mRotationSpeed );
    settings_->SetDoubleAttribute("scale_min_speed", mScaleMinSpeed );
    settings_->SetDoubleAttribute("scale_rotation_speed", mScaleRotationBySpeed);
    settings_->SetDoubleAttribute("force_factor", mForceFactor);
    settings_->SetDoubleAttribute("brake_factor", mBrakeFactor);
    settings_->SetDoubleAttribute("side_factor", mForceSideFactor);
    settings_->SetDoubleAttribute("scale_rolling", mScaleRolling);
    settings_->SetDoubleAttribute("scale_rolling_back", mScaleRollingBack);
    settings_->SetDoubleAttribute("max_rolling", mMaxRolling);
    settings_->SetDoubleAttribute("scale_pitch_flying", mScalePitchFlying);
    settings_->SetDoubleAttribute("scale_alignment", mScaleAlignment);
    settings_->SetAttribute("relocate_no_coll", mRelocateNoColl);
    settings_->SetAttribute("relocate_freeze", mRelocateFreeze);
    settings_->SetAttribute("resume_freeze", mResumeFreeze);
}

// --------------------------------------------------------
GameSettings::GameSettings()
{
    mGameType = GT_UNKOWN;
    mNrOfBots = 0;
    mNrOfLaps = -1;
    mTargetTime = 0;
    mBotDifficulty = BD_CHAMPIONSHIP;
    mGhostType = GHT_NONE;
}

// --------------------------------------------------------
Game::Game()
{
    mTrackScores = NULL;
    mLapScores = NULL;
    mIsStartPrepared = false;
    mFinishedByPlaying = false;
    mMode = GM_PAUSED;
    mResumeMode = GM_RUNNING;
    mGameTimer = NULL;
    mGameFinishedTimer = NULL;
    mGameStartTime = 0;
    mResumeGameTime = 0;
    mActiveCameraIndex = 0;
    mLocalPlayerIndex = 0;
    mLocalPlayerStartTrack = 0;
    mLocalPlayerPreviousStartTrack = -1;
    mRaceStartTime = 0;
    mAutosaveRecording = false;
    mAutoloadRecording = false;
    mAiTraining = false;
    mHasGhost = false;

    mCameras.push_back( new Camera() );
    mCameras.push_back( new Camera() );
    mCameras.push_back( new Camera() );
    mCameras.push_back( new Camera() );
    mCameras.push_back( new Camera() );
    mCameras.push_back( new Camera() );
    mCameras.push_back( new Camera() );
    mCameras.push_back( new Camera() );

    mSoundRefCountdown = -1;

    mRecorder = new Recorder();
    mHasRecordBestLap = false;
    mRecordBestLap = new Record();
    mRecordLocalPlayerLap = new Record();
    mHasRecordBestTrack = false;
    mRecordLocalPlayer = new Record();
    mRecordBestTrack = new Record();
    mRecordExportGhost = new Record();
    mRecordSession = new Record();
}

Game::~Game()
{
    if ( mRecordLocalPlayer )
        mRecordLocalPlayer->drop();
    if ( mRecordBestTrack )
        mRecordBestTrack->drop();
    if ( mRecordBestLap )
        mRecordBestLap->drop();
    if ( mRecordLocalPlayerLap )
        mRecordLocalPlayerLap->drop();
    if ( mRecordExportGhost )
        mRecordExportGhost->drop();
    if ( mRecordSession )
        mRecordSession->drop();
    delete mRecorder;
    for ( unsigned int i=0; i< mCameras.size(); ++i )
        delete mCameras[i];
    for ( unsigned int i=0; i< mPlayers.size(); ++i )
        delete mPlayers[i];
    delete mGameTimer;
    delete mGameFinishedTimer;
}

bool Game::Init()
{
    mGameTimer = new Timer(APP.GetIrrlichtManager()->GetIrrlichtTimer());
    mGameFinishedTimer = new Timer(APP.GetIrrlichtManager()->GetIrrlichtTimer());
    mGameTimer->Stop();
    mGameFinishedTimer->Stop();

    if ( APP.GetSound() )
    {
        APP.GetSound()->CacheSample( APP.GetConfig()->MakeFilenameSound(std::string("hc_menu_confirm01.wav")).c_str() );
    }

	HC1_PROFILE( s32 profPlayers = getProfiler().add(L"players(models)", L"STARTUP"); )
	HC1_PROFILE(getProfiler().start(profPlayers);)
    AddPlayer(L"PLAYER 1", "hover_player");
    AddPlayer(L"PLAYER 2", "hover_player");
    AddPlayer(L"PLAYER 3", "hover_player");
    AddPlayer(L"PLAYER 4", "hover_player");
    HC1_PROFILE(getProfiler().stop(profPlayers);)

    ReloadSettingsCamera();
    ReloadSettingsSteering();
    ReloadSettingsPhysics();

    APP.GetPhysics()->AddTickFunctor(0, new PhysicsTickFunctor<Game>(this, &Game::OnPhysicsTick));

    PROFILE_ADD_GROUP(PGROUP_GAME, "game group");
    PROFILE_ADD(200, PGROUP_GAME, "pre-physics");
    PROFILE_ADD(201, PGROUP_GAME, "update physics");
    PROFILE_ADD(202, PGROUP_GAME, "post physics");
    PROFILE_ADD(203, PGROUP_GAME, "local player");
    PROFILE_ADD(204, PGROUP_GAME, "training");
    PROFILE_ADD(205, PGROUP_GAME, "record");
    PROFILE_ADD(206, PGROUP_GAME, "pp 2");
    PROFILE_ADD(207, PGROUP_GAME, "pp 3");
    PROFILE_ADD(208, PGROUP_GAME, "pp 4");
    PROFILE_ADD(209, PGROUP_GAME, "updatestart");

    return true;
}

void Game::AddPlayer(const wchar_t * name_, const char * model_)
{
    Player * player = new Player(*this);
    player->mSettings.mName = name_;
    player->mSettings.mModelFileName = model_;
    player->Init();
    mPlayers.push_back(player);
}

void Game::ResetRecord(Record* record_)
{
    if ( !record_ )
        return;
    record_->Clear();
}

void Game::PrepareStart()
{
    // a lot of cruft in here. In patch 1.2 i made StartSession and a lot of stuff in here
    // would probably be better in there.


    int levelIndex = APP.GetLevelManager()->GetCurrentLevelIndex();
    if ( levelIndex < 0 )
    {
        LOG.Warn("Warning: no level loaded. Can't start game.\n");
        return;
    }

    const LevelSettings &level = APP.GetLevelManager()->GetLevel(levelIndex);

    // reset players
    std::vector<int> oldRecordingIds;
    for ( unsigned int p=0; p< mPlayers.size(); ++p )
    {
        oldRecordingIds.push_back(mPlayers[p]->GetRecordingId());
        mPlayers[p]->SetType(PT_UNKNOWN);
        mPlayers[p]->SetRecordPlayId(-1);
        mPlayers[p]->SetRecordingId(-1);
    }

    APP.GetPhysics()->ClearPhysicsObjects();
    mRecorder->RemoveAllPlayingObjects();
    mRecorder->RemoveAllRecordingObjects();

    // load ghost for best lap (always because it won't cost much and it can just be saved later on)
    ResetRecord(mRecordBestLap);
    std::string filenameLapRecord( APP.GetConfig()->MakeLapRecordName(level.mId, APP.GetStringTable()) );
    GhostRecordSettings dummyRecordSettings;
    mHasRecordBestLap = mRecordBestLap->Load(filenameLapRecord);
    dummyRecordSettings.Read(mRecordBestLap->GetSerializeArchive());

    // load ghost for best track when needed
    mHasRecordBestTrack = false;
    if ( GHT_TRACK == mSettings.mGhostType )
    {
        ResetRecord(mRecordBestTrack);
        std::string filenameTrackRecord( APP.GetConfig()->MakeTrackRecordName(level.mId, APP.GetStringTable()) );
        GhostRecordSettings dummyRecordSettings;
        mHasRecordBestTrack = mRecordBestTrack->Load(filenameTrackRecord);
        dummyRecordSettings.Read(mRecordBestTrack->GetSerializeArchive());
    }
    else if (   GHT_EXPORTED_LAP == mSettings.mGhostType
            ||  GHT_EPORTED_TRACK == mSettings.mGhostType
            )
    {
        ResetRecord(mRecordExportGhost);
        std::string filenameRecord( APP.GetConfig()->MakeFilenameGhosts(mSettings.mExportGhostRecord) );
        mHasRecordBestTrack = mRecordExportGhost->Load(filenameRecord);
        mExportGhostRecordSettings.Read(mRecordExportGhost->GetSerializeArchive());
    }

    // set the local player
    int players = 0;
    if ( !mAiTraining )
    {
        if ( mSettings.mGameType == GT_HOTLAP )
        {
            mLocalPlayerStartTrack = MAX_SPAWNS-1;
        }
        else
        {
            mLocalPlayerStartTrack = mSettings.mNrOfBots%MAX_PLAYERS;
        }

        mPlayers[players]->SetType(PT_LOCAL);
        mPlayers[players]->SetTrackStart(mLocalPlayerStartTrack);
#if defined(NEURAL_AI)
        mPlayers[players]->SetAiTrainingRecord(mRecordBestLap);
#endif

        // set player hover
        int hoverIndex = 0;
        PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
        if ( profile )
        {
            hoverIndex = profile->mHover;
            if ( !profile->IsHoverUnlocked( hoverIndex ) )
                hoverIndex = 0;
        }
        const Hover &hover = APP.GetHover(hoverIndex);
        mPlayers[players]->ChangeModel( hover.mModel );

        mLocalPlayerIndex = players;
        ++players;
    }

    // add ai's
    bool hasAi = false;
    if ( mAiTraining )
    {
        mSettings.mNrOfBots = 50;
    }

    for ( unsigned int i=0; i < (unsigned int)mSettings.mNrOfBots; ++i )
    {
        if ( (unsigned int)players >= mPlayers.size() )
            AddPlayer(L"PLAYER X", "hc_shadow");

        hasAi = true;
        mPlayers[players]->SetType(PT_AI);
        mPlayers[players]->SetAiBotSettings(level.mBotSettings[(i+(int)mSettings.mBotDifficulty*3) % MAX_BOT_SETTINGS]);
        mPlayers[players]->SetTrackStart(i%MAX_PLAYERS);
#if defined(NEURAL_AI)
        mPlayers[players]->SetAiTrainingRecord(mRecordBestLap);
#endif
        ++players;
    }

    // set the ghosts
    if ( !hasAi )
    {
        mHasGhost = false;

        // lap-record ghost
        if ( !mHasGhost
            && ((mSettings.mGhostType == GHT_LAP && mRecordBestLap)
                ||mSettings.mGhostType == GHT_EXPORTED_LAP ) )
        {
            mHasGhost = true;
            mPlayers[players]->SetType(PT_GHOST_LAP);
            mPlayers[players]->SetTrackStart(0);    // doesn't matter, won't start immediatly
            ++players;
        }

        // track record ghost
        if ( !mHasGhost && mHasRecordBestTrack && (mSettings.mGhostType == GHT_TRACK || mSettings.mGhostType == GHT_EPORTED_TRACK))
        {
            mHasGhost = true;
            mPlayers[players]->SetType(PT_GHOST_TRACK);
            mPlayers[players]->SetTrackStart(MAX_SPAWNS-1);
            ++players;
        }

        // session record ghost
        if ( !mHasGhost && HasSessionGhost() )
        {
            mHasGhost = true;
            mPlayers[players]->SetType(PT_GHOST_TRACK);
            mPlayers[players]->SetTrackStart(MAX_SPAWNS-1);
            ++players;
        }

        // record tracks from any start-position (for testing)
//        if ( !mHasGhost && mAutoloadRecording )
//        {
//            // ghost for each trackstart where a saved game is available
//            for ( int g=0; g < 4; ++g )
//            {
//                std::ostringstream filename;
//                filename << APP.GetConfig()->GetPathRecordings() << "L" << level.mId << "P" << g;
//                int ghostRecordId = mRecorder->AddRecord();
//                if ( mRecorder->GetRecord(ghostRecordId)->Load(filename.str()) )
//                {
//                    mHasGhost = true;
//                    mPlayers[players]->SetType(PT_GHOST_TRACK);
//                    mPlayers[players]->SetRecordPlayId(ghostRecordId);
//                    mPlayers[players]->SetTrackStart(g);
//                    mRecorder->AddPlayingObject(mPlayers[players], ghostRecordId);
//                    ++players;
//                }
//                else
//                {
//                    mRecorder->RemoveRecord(ghostRecordId);
//                }
//            }
//         }
    }
    mRecorder->RemoveAllUnusedRecords();
    UpdateHoverAmbience(level);

    for ( unsigned int p=0; p< mPlayers.size(); ++p )
    {
        mPlayers[p]->InfoPrepareForRace();
    }
    ReloadSettingsPhysics(); // doing that here allows to change & test those settings faster

    ResetHud();

    // set camera to start position
    scene::ICameraSceneNode* gameCam = APP.GetIrrlichtManager()->GetGameCamera();
    scene::ISceneNode * hover = mPlayers[mLocalPlayerIndex]->GetHoverNode();
    PhysicsObject* physicsObj = APP.GetPhysics()->GetPhysicsObject(mPlayers[mLocalPlayerIndex]->GetPhysicsId());
    mCameras[mActiveCameraIndex]->UpdateGameIntroCam( 1000, hover->getAbsoluteTransformation(), gameCam, physicsObj, TIME_START_COUNTDOWN );

    mIsStartPrepared = true;
}

void Game::ResetResult()
{
    mResult.mGameWasFinished = false;
    mResult.mLocalPlayerPlacing = 0;
    mResult.mLocalPlayerTrackTimeMs = TIME_UNUSED;
    mResult.mLocalPlayerLastLapTimeMs = TIME_UNUSED;
    mResult.mLocalPlayerBestLapTimeMs  = TIME_UNUSED;
    mResult.mLocalPlayerNumLaps = 0;
    mResult.mChampionshipScore = 0;
}

void Game::StartSession()
{
    mResult.mLocalPlayerBestSessionLapTimeMs = TIME_UNUSED;
    PrepareStart();
}

void Game::Start()
{
    if ( !mIsStartPrepared )  // because PrepareStart can also be called independent
    {
        assert(!"Game::PrepareStart not called before Game::Start()");
        PrepareStart();
    }
    mIsStartPrepared = false;
    mFinishedByPlaying = false;
    mMode = GM_START;

    ResetResult();
    mLocalPlayerPreviousStartTrack = mLocalPlayerStartTrack;
    ResetRecord(mRecordLocalPlayer);

	IInputDevice * touchDevice = APP.GetInputDeviceManager()->GetDevice(IDP_TOUCH);
	if ( touchDevice )
		touchDevice->Reset();

    APP.GetController()->Reset();

    std::string name(APP.GetConfig()->MakeFilenameSound(std::string("hc_countdown01.wav")));
    if ( APP.GetSound() )
        mSoundRefCountdown = APP.GetSound()->PlaySample(name.c_str(), 1.f, false);

    mGameFinishedTimer->Stop();
    mGameTimer->Start();
    mGameStartTime = mGameTimer->GetTime();

    APP.GetIrrlichtManager()->ForceIrrlichtUpdate();

    for ( unsigned int p=0; p<mPlayers.size(); ++p)
    {
        mPlayers[p]->InfoStartCountDown(mGameStartTime);
    }

    SetActiveCameraIndex(mActiveCameraIndex);
}

void Game::ResetHud()
{
    GuiHud * hud = APP.GetGui()->GetGuiHud();
    if ( !hud )
        return;

    hud->SetFps(0);
    hud->SetTimeLap(0);
    hud->SetTimeBestLap(TIME_UNUSED);
    hud->SetTimeTrack(0);
    hud->SetSpeed(0.f);
    hud->SetLapCount(0, mSettings.mNrOfLaps);
    hud->SetTimeGhostGap(TIME_UNUSED);
    if (   GHT_EXPORTED_LAP == mSettings.mGhostType
        ||  GHT_EPORTED_TRACK == mSettings.mGhostType )
    {
        hud->SetTimeGhostRecord(mExportGhostRecordSettings.mTime);
    }
    else
    {
        hud->SetTimeGhostRecord(TIME_UNUSED);
    }

    if ( GT_RIVALS == mSettings.mGameType )
    {
        hud->SetTimeSessionRecord(mSessionTrackRecord);
        hud->SetTimeSessionGap(TIME_UNUSED);

        RivalsPlayer* currentPlayer = APP.GetRivalsMode()->GetPlayer(APP.GetRivalsMode()->GetCurrentPlayer());
        if ( currentPlayer )
        {
            PlayerProfile* currentProfile = APP.GetProfileManager()->GetProfile(currentPlayer->mProfileId);
            hud->SetSessionPlayerName(currentProfile->mName.c_str());
        }
        else
        {
            hud->SetSessionPlayerName(L"");
        }
    }

    if ( mSettings.mTargetTime == 0 )
        hud->SetPosition(0, mSettings.mNrOfBots+1);
    else
        hud->SetReverseTime(-mSettings.mTargetTime);
    hud->SetSpecialText1(L"");
    hud->SetSpecialText2(L"");
    hud->SetGap(L"");
    hud->SetCountdown(HUD_COUNT_NONE);

    for ( int i=0; i<3; ++i )
    {
        if ( i < mSettings.mNrOfBots )
        {
            hud->SetPosMarkerVisible(i, true);
            hud->SetPosMarker(i, 0.f);
        }
        else
        {
            hud->SetPosMarkerVisible(i, false);
        }
    }
    if ( mHasGhost )
    {
        int i=0;
        for ( unsigned int p=0; p< mPlayers.size(); ++p )
        {
            if (    mPlayers[p]->GetType() == PT_GHOST_LAP
                ||  mPlayers[p]->GetType() == PT_GHOST_TRACK )
            {
                hud->SetPosMarkerVisible(i, true);
                hud->SetPosMarker(i, 0.f);
                ++i;
            }
        }
    }

    int lapRecord = TIME_UNUSED;
    if ( GetCurrentLapHighscores() )
    {
        lapRecord = GetCurrentLapHighscores()->GetScore(0).mTime;
        if ( GHT_LAP == mSettings.mGhostType )
        {
            hud->SetTimeGhostRecord(lapRecord);
        }
    }
    hud->SetTimeLapRecord(lapRecord);

    int trackRecord = TIME_UNUSED;
    if ( GetCurrentTrackHighscores() )
    {
        trackRecord = GetCurrentTrackHighscores()->GetScore(0).mTime;
        if ( GHT_TRACK == mSettings.mGhostType )
        {
            hud->SetTimeGhostRecord(trackRecord);
        }
    }
    hud->SetTimeTrackRecord(trackRecord);
}

void Game::UpdateStart(GuiHud & guiHud)
{
    PROFILE_START(209);
    mGameTimer->Tick();
    u32 time = mGameTimer->GetTime();
    f32 timeTickSec = mGameTimer->GetLastTickInSeconds();
    guiHud.Update(time);
    APP.GetPhysics()->Update(0.1f, false);

    // camera approaching player
    scene::ICameraSceneNode* gameCam = APP.GetIrrlichtManager()->GetGameCamera();
    scene::ISceneNode * hover = mPlayers[mLocalPlayerIndex]->GetHoverNode();
    PhysicsObject* physicsObj = APP.GetPhysics()->GetPhysicsObject(mPlayers[mLocalPlayerIndex]->GetPhysicsId());
    u32 timeRest = TIME_START_COUNTDOWN - (time - mGameStartTime);
    mCameras[mActiveCameraIndex]->UpdateGameIntroCam( timeTickSec, hover->getAbsoluteTransformation(), gameCam, physicsObj, timeRest );

    if (time - mGameStartTime > TIME_START_COUNTDOWN)
    {
        guiHud.SetCountdown(HUD_COUNT_GO);
        mMode = GM_RUNNING;
        mRaceStartTime = mGameTimer->GetTime();
        APP.GetPhysics()->ResetTicksSinceGameStart();

        ResetRecord(mRecordLocalPlayerLap);

        // show track ghost(s)
        if ( mHasRecordBestTrack || mSettings.mGhostType == GHT_EPORTED_TRACK || HasSessionGhost() )
        {
            for ( unsigned int g=0; g < mPlayers.size(); ++g)
            {
                if ( mPlayers[g]->GetType() == PT_GHOST_TRACK )
                {
                    mPlayers[g]->GetHoverNode()->setVisible(true);
                }
            }
        }

        for ( unsigned int p=0; p<mPlayers.size(); ++p)
        {
            mPlayers[p]->InfoStartRace(time);
        }
    }
    else
    {
        if ( time - mGameStartTime < 1000 )
            guiHud.SetCountdown(HUD_COUNT_3);
        else if ( time - mGameStartTime < 2000 )
            guiHud.SetCountdown(HUD_COUNT_2);
        else
            guiHud.SetCountdown(HUD_COUNT_1);
    }
    PROFILE_STOP(209);
}

void Game::UpdateResume()
{
    // camera approaching player
    f32 timeTickSec = mGameTimer->GetLastTickInSeconds();
    scene::ICameraSceneNode* gameCam = APP.GetIrrlichtManager()->GetGameCamera();
    scene::ISceneNode * hover = mPlayers[mLocalPlayerIndex]->GetHoverNode();
    PhysicsObject* physicsObj = APP.GetPhysics()->GetPhysicsObject(mPlayers[mLocalPlayerIndex]->GetPhysicsId());
    int timeRest = mResumeGameTime - APP.GetIrrlichtManager()->GetIrrlichtTimer()->getTime();
    if ( timeRest > 0 )
    {
        mCameras[mActiveCameraIndex]->UpdateGameIntroCam( timeTickSec, hover->getAbsoluteTransformation(), gameCam, physicsObj, (u32)timeRest );
    }

    if ( APP.GetIrrlichtManager()->GetIrrlichtTimer()->getTime() > mResumeGameTime )
    {
        mMode = mResumeMode;
        for ( unsigned int p=0; p < mPlayers.size(); ++p )
        {
            if ( mPlayers[p]->IsActiveType() )
            {
                mPlayers[p]->InfoGameResumes();
            }
        }
        mGameTimer->Start();
    }
}

void Game::UpdateFinished()
{
    mGameFinishedTimer->Tick();
    f32 timeTickSec = mGameFinishedTimer->GetLastTickInSeconds();
    u32 time = mGameFinishedTimer->GetTime();

    for ( unsigned int p=0; p<mPlayers.size(); ++p)
    {
        mPlayers[p]->PrePhysicsUpdate(time);
    }
    APP.GetPhysics()->Update(timeTickSec, false);
    for ( unsigned int p=0; p<mPlayers.size(); ++p)
    {
        if ( mPlayers[p]->GetType() == PT_UNKNOWN )
            continue;

        mPlayers[p]->PostPhysicsUpdate(time);

        CheckCollisions(mPlayers[p], time);
    }

    PhysicsObject* physicsObj = APP.GetPhysics()->GetPhysicsObject(mPlayers[mLocalPlayerIndex]->GetPhysicsId());
    scene::ICameraSceneNode* gameCam = APP.GetIrrlichtManager()->GetGuiCamera();
    scene::ISceneNode * hover = mPlayers[mLocalPlayerIndex]->GetHoverNode();
//    mCameras[mActiveCameraIndex]->UpdateGameCam(timeTickSec, hover->getAbsoluteTransformation(), gameCam, physicsObj, 0);
    mCameras[mActiveCameraIndex]->UpdateGameOutroCam(timeTickSec, hover->getAbsoluteTransformation(), gameCam, physicsObj, time-mGameFinishedTimer->GetLastStartTime());
}

void Game::OnCrossedFinishLine(Player * player_, u32 time_)
{
    if (    player_->GetCurrentRound() == 0
        ||  APP.GetLevelManager()->GetTrackRelocatesBetween(APP.GetLevelManager()->GetNrOfTrackMarkers()-1, player_->GetLastMarkerReached()) < 3 )
    {
        player_->InfoRoundFinished(time_);

        int lapRecord = TIME_UNUSED;
        if ( GetCurrentLapHighscores() )
        {
            lapRecord = GetCurrentLapHighscores()->GetScore(0).mTime;
        }

        if ( PT_LOCAL == player_->GetType() && !mAiTraining )
        {
            PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
            bool playPingSample = false;

             // check for a new lap record
            if ( player_->GetCurrentRound() > 1 )
            {
                int curLapTime = player_->GetLastRoundTime();

                // show gap to (old) record
                if ( GT_HOTLAP == mSettings.mGameType && lapRecord != (int)TIME_UNUSED)
                {
                    s32 gapTime = curLapTime - lapRecord;
                    //std::wstring hudGapTime( APP.GetStringTable()->Get("id_gap").c_str() );
                    std::wstring hudGapTime( GuiHelper::MakeTimeString(gapTime, true) );
                    APP.GetGui()->GetGuiHud()->SetGap(hudGapTime.c_str() , time_ + 5000 );
                }

                if ( curLapTime < lapRecord )
                {
                    // new record
                    lapRecord = curLapTime;
                    *mRecordBestLap = *mRecordLocalPlayerLap;
                    GhostRecordSettings lapGhostRecordSettings;
                    if ( profile )
                    {
                        lapGhostRecordSettings.mProfileName = profile->mName;
                    }
                    lapGhostRecordSettings.mLevelName = mSettings.mLevelName;
                    lapGhostRecordSettings.mGhostType = (int)mSettings.mGhostType;
                    lapGhostRecordSettings.mTime = lapRecord;
                    lapGhostRecordSettings.mChecksum = lapGhostRecordSettings.CalcChecksum();
                    lapGhostRecordSettings.Write(mRecordBestLap->GetSerializeArchive());
                    mHasRecordBestLap = true;

                    playPingSample = true;
                    APP.GetGui()->GetGuiHud()->SetTimeLapRecord(lapRecord);
                    APP.GetGui()->GetGuiHud()->SetSpecialText1(APP.GetStringTable()->Get("id_hdinsert01").c_str(), time_ + 5000);
                }

                // check for lap highscore
                if ( mLapScores )
                {
                    if ( mLapScores->CheckPosition(curLapTime) >= 0 )
                    {
                        HScoreEntry score;
                        score.mTime = curLapTime;
                        if ( profile )
                        {
                            score.mProfileName = profile->mName;
                        }
                        mLapScores->AddEntry(score);
                    }
                }

                // check for race finished
                if (    mSettings.mNrOfLaps > 0
                    &&  (int)player_->GetCurrentRound() > mSettings.mNrOfLaps )
                {
                    Finish(true);
                    return;
                }
                else if (   mSettings.mNrOfLaps > 0
                        &&  (int)player_->GetCurrentRound() == mSettings.mNrOfLaps )
                {
                    // last round
                    playPingSample = true;
                    APP.GetGui()->GetGuiHud()->SetSpecialText2(APP.GetStringTable()->Get("id_hdfinallap").c_str(), time_ + 5000);
                }
            }

            if ( playPingSample && APP.GetSound() )
            {
                APP.GetSound()->PlaySample( APP.GetConfig()->MakeFilenameSound(std::string("hc_menu_confirm01.wav")).c_str(), 1.f, false );
            }

            // new round, new luck
            ResetRecord(mRecordLocalPlayerLap);

            // show lap ghost(s)
            if ( mHasRecordBestLap || mSettings.mGhostType == GHT_EXPORTED_LAP)
            {
                for ( unsigned int g=0; g < mPlayers.size(); ++g)
                {
                    if ( mPlayers[g]->GetType() == PT_GHOST_LAP )
                    {
                        mPlayers[g]->GetHoverNode()->setVisible(true);
                        APP.GetGui()->GetGuiHud()->SetPosMarkerVisible(0, true);
                        APP.GetGui()->GetGuiHud()->SetPosMarker(0, 0.f);
                        mHasGhost = true;
                        PlayRecords();
                    }
                }
            }
        }
    }
}

void Game::UpdateHud(Player * player_, PhysicsObject* physicsObj_, u32 time_)
{
    GuiHud * hud = APP.GetGui()->GetGuiHud();
    if ( !hud )
        return;

    bool freezeTime = false;

    if ( 0 == player_->GetCurrentRound() ||
        (   time_ - player_->GetRoundStartTime() < 5000
        &&  player_->GetCurrentRound() != 1 )
        )
    {
        freezeTime = true;
    }

    // update HUD
    if ( !freezeTime )
    {
        hud->SetTimeLap(time_ - player_->GetRoundStartTime());
    }
    else
    {
        hud->SetTimeLap( player_->GetLastRoundTime() );
    }

    if ( !APP.GetEditGui() )
    {
        hud->SetSpeed(physicsObj_->GetSpeedScaled());

        hud->SetLapCount(player_->GetCurrentRound(), mSettings.mNrOfLaps);

        if ( mSettings.mTargetTime == 0 )
            hud->SetPosition(player_->GetPlacing(), mSettings.mNrOfBots+1);
        else
        {
            s32 trackTime = time_-mRaceStartTime;
            hud->SetReverseTime(trackTime - mSettings.mTargetTime);
        }

        if ( time_ - mRaceStartTime > 1000 )
        {
            hud->SetCountdown(HUD_COUNT_NONE);
        }

        for ( int i=0; i < mSettings.mNrOfBots && i < 3; ++i )
        {
            s32 timeDiff = 0;
            float trackDist = GetRelativeTrackDistanceToPlayer(mLocalPlayerIndex, i+1, timeDiff );
            hud->SetPosMarker(i, trackDist);
        }
        if ( mHasGhost )
        {
            int i=0;
            for ( unsigned int p=0; p< mPlayers.size(); ++p )
            {
                if (    mPlayers[p]->GetType() == PT_GHOST_LAP
                    ||  mPlayers[p]->GetType() == PT_GHOST_TRACK )
                {
                    s32 timeDiff = 0;
                    float trackDist = GetRelativeTrackDistanceToPlayer(mLocalPlayerIndex, p, timeDiff );
                    hud->SetPosMarker(i, trackDist);
                    if ( 0 != player_->GetCurrentRound() )
                    {
                        hud->SetTimeGhostGap(timeDiff);
                        hud->SetTimeSessionGap(timeDiff);
                    }
                    ++i;
                }
            }
        }
    }
}

void Game::CheckCollisions(Player* player_, u32 time_)
{
    if ( !player_ )
        return;

	HC1_PROFILE(CProfileScope ps1(207);)

    player_->GetHoverNode()->updateAbsolutePosition();
    core::line3d<f32> moveLine(player_->GetHoverNode()->getAbsolutePosition(), player_->GetLastPos());

    // check collision with markers
    int indexMarker = 0;
    if ( APP.GetLevelManager()->CheckMarkerCollision(moveLine, indexMarker) )
    {
        if (    indexMarker > player_->GetLastMarkerReached()
            &&  APP.GetLevelManager()->GetTrackRelocatesBetween(indexMarker, player_->GetLastMarkerReached()) < 5 )
        {
            player_->InfoLevelMarkerReached( indexMarker );
        }
    }

    // check finish line collision
    if ( APP.GetLevelManager()->CheckFinishLineCollision(moveLine) )
    {
        OnCrossedFinishLine(player_, time_);
        if ( mMode == GM_FINISHED )
            return;
    }

    if ( !player_->IsActiveType() )
        return;

    // check teleport collision
    PhysicsObject* physicsObj = APP.GetPhysics()->GetPhysicsObject(player_->GetPhysicsId());
    core::vector3df tpTarget;
    core::vector3df tpRotation;
    core::vector3df tpVelocity( physicsObj->GetVelocity() );
    if ( APP.GetLevelManager()->CheckTeleportLineCollision(moveLine, tpTarget, tpRotation, tpVelocity) )
    {
        physicsObj->mSceneNode->setPosition(tpTarget);
        physicsObj->mSceneNode->setRotation(tpRotation);
        physicsObj->mSceneNode->updateAbsolutePosition();
        physicsObj->SetVelocity(tpVelocity);
        physicsObj->ForcePositionUpdate();

        player_->InfoTeleport(time_);
        if ( player_->GetType() == PT_LOCAL )
        {
            scene::ICameraSceneNode* gameCam = APP.GetIrrlichtManager()->GetGameCamera();
            mCameras[mActiveCameraIndex]->ResetGameCam(player_->GetHoverNode()->getAbsoluteTransformation(), gameCam, physicsObj);
        }
    }
    else    // ignore wall checks when a teleport was there
    {
        // check wall (drop off from track) collision
        int indexWall = 0;
        bool droppingOutside = time_ - player_->GetLastTimeTouchedFloor() > 2000 ? true : false;
        if ( player_->GetLastTimeTouchedFloor() == 0 )
            droppingOutside = false;
        if ( droppingOutside || APP.GetLevelManager()->CheckWallCollision(moveLine, indexWall) )
        {
            if ( droppingOutside )
            {
                indexWall = player_->GetLastMarkerReached();
            }
            core::vector3df center;
            core::vector3df rotation;
            GetRelocationPos(indexWall, center, rotation);
            physicsObj->mSceneNode->setPosition(center);
            physicsObj->mSceneNode->setRotation(rotation);
            physicsObj->mSceneNode->updateAbsolutePosition();
            physicsObj->SetVelocity(core::vector3df(0,0,0));
            physicsObj->ForcePositionUpdate();

            player_->InfoDroppedFromTrack(time_);

            if ( player_->GetType() == PT_LOCAL )
            {
                scene::ICameraSceneNode* gameCam = APP.GetIrrlichtManager()->GetGameCamera();
                mCameras[mActiveCameraIndex]->ResetGameCam(player_->GetHoverNode()->getAbsoluteTransformation(), gameCam, physicsObj);
            }
        }
    }
}

void Game::Update()
{
	if ( !APP.GetGui()->GetGuiHud() )
		return;
    GuiHud & guiHud = *APP.GetGui()->GetGuiHud();
    guiHud.SetFps( APP.GetIrrlichtManager()->GetVideoDriver()->getFPS() );

    if ( mMode == GM_START )
    {
        UpdateStart(guiHud);
    }
    else if ( mMode == GM_RESUME_PAUSE )
    {
        UpdateResume();
    }
    else if ( mMode == GM_FINISHED )
    {
        UpdateFinished();
    }
    else if ( mMode == GM_RUNNING)
    {
        mGameTimer->Tick();
    }

	u32 time = mGameTimer->GetTime();
    f32 timeTickSec = mGameTimer->GetLastTickInSeconds();

    GuiTouch * guiTouch = APP.GetGui()->GetGuiTouch();
    if ( guiTouch )
	{
		guiTouch->Update(time, mMode == GM_RUNNING);
	}

    if ( mMode != GM_RUNNING )
        return;

#ifdef HC1_ENABLE_SOUND
    if ( APP.GetSound() && !APP.GetSound()->IsMusicPlaying() && !mAiTraining )
    {
        APP.GetMusicManager()->PlayLevelMusic();
    }
#endif

    guiHud.Update(time);
    guiHud.SetTimeTrack(time-mRaceStartTime);

    PROFILE_START(200);
    for ( unsigned int p=0; p<mPlayers.size(); ++p)
    {
        mPlayers[p]->PrePhysicsUpdate(time);
    }
    PROFILE_STOP(200);

    PROFILE_START(201);
    APP.GetPhysics()->Update(timeTickSec, !GetAiTraining());
    PROFILE_STOP(201);

    PlayRecords();

    PROFILE_START(202);
    for ( unsigned int p=0; p<mPlayers.size(); ++p)
    {
        if ( mPlayers[p]->GetType() == PT_UNKNOWN )
            continue;

        PROFILE_START(206);
        mPlayers[p]->PostPhysicsUpdate(time);
        PROFILE_STOP(206);

        CheckCollisions(mPlayers[p], time);
        if ( mMode == GM_FINISHED )
            return;
        if ( !mPlayers[p]->IsActiveType() )
            continue;

#if defined(NEURAL_AI)
        PROFILE_START(208);
        if ( mAiTraining )
        {
            if ( time - mPlayers[p]->GetRoundStartTime() > 5000
                ||  mPlayers[p]->GetCurrentRound() == 1
                )
            {
                if ( mPlayers[p]->GetBestRoundTime() < guiHud.GetTimeBestLap() )
                {
                    std::string fileNN( APP.GetConfig()->MakeFilenameLevel("easy.nn") );
                    mPlayers[p]->GetNeuralNet()->Save(fileNN.c_str());
                    guiHud.SetTimeBestLap( mPlayers[p]->GetBestRoundTime() );
                }
            }
        }
        PROFILE_STOP(208);
#endif
    }
    PROFILE_STOP(202);

    UpdatePlacings();

    // do some stuff for local gamer controlled player
    PROFILE_START(203);
    if ( mPlayers.size() > mLocalPlayerIndex && mPlayers[mLocalPlayerIndex]->GetType() == PT_LOCAL) // is there a local player?
    {
        PhysicsObject* physicsObj = APP.GetPhysics()->GetPhysicsObject(mPlayers[mLocalPlayerIndex]->GetPhysicsId());
        UpdateHud(mPlayers[mLocalPlayerIndex], physicsObj, time);

        if ( !mAiTraining )
        {
            guiHud.SetTimeBestLap( mPlayers[mLocalPlayerIndex]->GetBestRoundTime() );
        }

        // camera
        scene::ICameraSceneNode* gameCam = APP.GetIrrlichtManager()->GetGameCamera();
        Controller* controller = APP.GetController();
        scene::ISceneNode * hover = mPlayers[mLocalPlayerIndex]->GetHoverNode();

        if ( controller->GetSwitchCamera() == 1)
        {
            SetNextCameraActive();
        }
        int timeInAir = time - mPlayers[mLocalPlayerIndex]->GetLastTimeTouchedFloor();
        mCameras[mActiveCameraIndex]->UpdateGameCam(timeTickSec, hover->getAbsoluteTransformation(), gameCam, physicsObj, timeInAir);

        // pause
        if ( controller->GetPause() )
        {
            controller->Reset(); // to avoid chatter
            Pause();
        }
    }
    PROFILE_STOP(203);

#if defined(NEURAL_AI)
    if ( mAiTraining )
    {
        PROFILE_START(204);
        gui::ICursorControl* cursor = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getCursorControl();
        assert(cursor);
        Camera::UpdateEditCam(timeTickSec, APP.GetIrrlichtManager()->GetEditorCamera(), !cursor->isVisible() );
        TrainAi();
        PROFILE_STOP(204);
    }
#endif
}

void Game::OnPhysicsTick(unsigned int ticksSinceGameStart_)
{
    PROFILE_START(205);

    if ( mMode != GM_RUNNING )
    {
        PROFILE_STOP(205);
        return;
    }

    unsigned int physicsTicks = APP.GetPhysics()->GetTicksSinceGameStart();
    unsigned int physicsTicksThisRound = physicsTicks - mPlayers[mLocalPlayerIndex]->GetRoundStartPhysicsTicks();

    // recording
    mRecorder->UpdateRecording( physicsTicks );

    mRecordLocalPlayerLap->UpdateRecording( physicsTicksThisRound, mPlayers[mLocalPlayerIndex]);
    if ( GT_TIMERACE == mSettings.mGameType || GT_RIVALS == mSettings.mGameType )
    {
        mRecordLocalPlayer->UpdateRecording( physicsTicks, mPlayers[mLocalPlayerIndex]);
    }
    PROFILE_STOP(205);
}

void Game::PlayRecords()
{
    PROFILE_START(205);

    float interpolatedPhysicsTick = APP.GetPhysics()->GetInterpolatedTick();
    float interpolatedPhysicsTickThisRound = interpolatedPhysicsTick - mPlayers[mLocalPlayerIndex]->GetRoundStartPhysicsTicks();

    // playing
    mRecorder->UpdatePlaying( interpolatedPhysicsTick );
    if ( mSettings.mGhostType != GHT_NONE || HasSessionGhost() )
    {
        for ( unsigned int g=0; g < mPlayers.size(); ++g)
        {
            if ( mPlayers[g]->GetType() == PT_GHOST_LAP )
            {
                if ( mSettings.mGhostType == GHT_LAP )
                {
                    mRecordBestLap->UpdatePlaying(interpolatedPhysicsTickThisRound, mPlayers[g]);
                }
                else if ( mSettings.mGhostType  == GHT_EXPORTED_LAP )
                {
                    mRecordExportGhost->UpdatePlaying(interpolatedPhysicsTickThisRound, mPlayers[g]);
                }
            }
            else if ( mPlayers[g]->GetType() == PT_GHOST_TRACK )
            {
                if ( mSettings.mGhostType == GHT_TRACK )
                {
                    mRecordBestTrack->UpdatePlaying(interpolatedPhysicsTick, mPlayers[g]);
                }
                else if ( mSettings.mGhostType == GHT_EPORTED_TRACK )
                {
                    mRecordExportGhost->UpdatePlaying(interpolatedPhysicsTick, mPlayers[g]);
                }
                else if ( HasSessionGhost() )
                {
                    mRecordSession->UpdatePlaying(interpolatedPhysicsTick, mPlayers[g]);
                }
            }
        }
    }

    PROFILE_STOP(205);
}

bool Game::HasSessionGhost() const
{
    return GT_RIVALS == mSettings.mGameType && mSessionTrackRecord != (s32)TIME_UNUSED;
}

void Game::UpdatePlacings()
{
    core::array<SortHelper> sortArray;
    for ( unsigned int p=0; p<mPlayers.size(); ++p)
    {
        if ( !mPlayers[p]->IsActiveType() )
            continue;

        SortHelper helper;
        helper.mPlayerId = p;
        helper.mRating = mPlayers[p]->GetCurrentRound() * 10000
                        + mPlayers[p]->GetCurrentMarkerReached();

        sortArray.push_back(helper);
    }
    sortArray.sort();

    for ( unsigned int i=0; i < sortArray.size(); ++i )
    {
        mPlayers[sortArray[i].mPlayerId]->SetPlacing(i+1);
    }
}

// return a value between -1 and 1
float Game::GetRelativeTrackDistanceToPlayer(int sourcePlayer_, int targetPlayer_, s32 &timeDiff_)
{
	if ( mPlayers[sourcePlayer_]->GetCurrentRound() == 0 )
	{
		return 0.f;
	}

    timeDiff_ = 0;
    const LevelSettings& levelSettings = APP.GetLevelManager()->GetCurrentLevel();

    int maxMarkers = APP.GetLevelManager()->GetAiTrack().GetNumTrackInfos();

    bool hasLapGhost = mPlayers[sourcePlayer_]->GetType() == PT_GHOST_LAP
                    ||  mPlayers[targetPlayer_]->GetType() == PT_GHOST_LAP;

    int p1 = mPlayers[sourcePlayer_]->GetCurrentMarkerReached();
    int p1orig = p1;
    if ( !hasLapGhost)
    {
        p1 +=  maxMarkers * mPlayers[sourcePlayer_]->GetCurrentRound();
    }

    int p2 = mPlayers[targetPlayer_]->GetCurrentMarkerReached();
    int p2orig = p2;
    if ( !hasLapGhost )
    {
        p2 += maxMarkers * mPlayers[targetPlayer_]->GetCurrentRound();
    }

    maxMarkers = (int)(maxMarkers * levelSettings.mHudPosScaling);
    if ( !maxMarkers )
        return 0.f;

    if ( p1 >= p2 )
    {
        if ( p2orig >= 0 )
        {
            timeDiff_ = mPlayers[sourcePlayer_]->GetMarkerReachedTime(p2orig)
                    - mPlayers[targetPlayer_]->GetMarkerReachedTime(p2orig);
        }
    }
    else
    {
        if ( p1orig >= 0 )
        {
            timeDiff_ = mPlayers[sourcePlayer_]->GetMarkerReachedTime(p1orig)
                        - mPlayers[targetPlayer_]->GetMarkerReachedTime(p1orig);
        }
    }

    if ( (p1 - p2) >= maxMarkers )
        return 1.f;
    if ( (p2 - p1) >= maxMarkers )
        return -1.f;

    return (float)(p1-p2) / (float)maxMarkers;
}

#if defined(NEURAL_AI)
std::string Game::GetNextFilenameSaveNN()
{
    static int countFilenames = 0;
    char fileName[100];
    sprintf(fileName, "easy%d.nn", countFilenames );
    ++countFilenames;
    std::string fileNN( APP.GetConfig()->MakeFilenameLevel(fileName) );
    return fileNN;
}

void Game::TrainAi()
{
    Timer* gameTimer = APP.GetGame()->GetGameTimer();

    bool needRestart = false;
    RandomGenerator randGen;
    static u32 mostAiRounds = 0;
    int time = gameTimer->GetTime();
    static int lastTrainingTime = 0;
    if ( lastTrainingTime == 0)
    {
        lastTrainingTime = time;
    }

    //float TEST_TIME = 500.f + mostAiRounds;
//    float MIN_TIME_SINCE_RESET = 5000.f;
    float TEST_TIME = 10000.f;
    if (    time - lastTrainingTime > TEST_TIME )
//        &&  time - lastResetTime > MIN_TIME_SINCE_RESET )
    {
        lastTrainingTime = time;

        // sort nn's by award function
        core::array<SortHelper> parents;
        for ( unsigned int p=0; p<mPlayers.size(); ++p)
        {
            if ( mPlayers[p]->GetType() == PT_AI )
            {
                SortHelper helper;
                helper.mPlayerId = p;
                helper.mRating = mPlayers[p]->CalculateAiTrainingAward(time);
                parents.push_back(helper);

                if ( mPlayers[p]->GetCurrentRound() > mostAiRounds )
                {
                    mostAiRounds = mPlayers[p]->GetCurrentRound();
                }
            }
        }
        parents.sort();

        // regularly saving the currently best nn
        static int saveTime = time;
        if ( time - saveTime > 3600000 )    // once per hour
        {
            saveTime = time;
            std::string fileNN( GetNextFilenameSaveNN() );
            mPlayers[parents[0].mPlayerId]->GetNeuralNet()->Save(fileNN.c_str());
        }

        static int debugTime = time;
//        static int WINS_TO_SAVE = 2;
        static int lastWinner = 0;
//        static int numWins = 0;
//        static int debugTest = 0;
        if ( time - debugTime > 25000 )
        {
            debugTime = time;

            // debug output
            fprintf(stderr, "\nRatings: ");
            for ( unsigned int i=0; i<parents.size(); ++i )
                fprintf(stderr, "(%d)%f %d ", parents[i].mPlayerId, parents[i].mRating, mPlayers[parents[i].mPlayerId]->GetLastRoundTime());
            fprintf(stderr, "\n");

//            // save a good nn
//            if ( parents[0].mPlayerId == lastWinner )
//            {
//                ++numWins;
//                if ( numWins == WINS_TO_SAVE )
//                {
//                    std::string fileNN( GetNextFilenameSaveNN() );
//                    mPlayers[parents[0].mPlayerId]->GetNeuralNet()->Save(fileNN.c_str());
//                }
//            }
//            else
//            {
//                numWins = 0;
//            }
            lastWinner = parents[0].mPlayerId;
        }

        // create next generation of nn's
		needRestart = true;
		for ( unsigned int p=0; p<mPlayers.size(); ++p )
		{
		    int r1 = randGen.LinearSlope(parents.size());
			NeuralNet* net = mPlayers[p]->GetNeuralNet();
            *net = *(mPlayers[r1]->GetTrainingNeuralNet());

			int r2 = randGen.LinearSlope(parents.size());
			NeuralNet* cross = mPlayers[r2]->GetTrainingNeuralNet();

			net->CrossInputWeights(0.5, *cross, false);
			net->CrossHiddenWeights(0.5, *cross, false);

			float mutationRate = 3.f / (float)net->GetNumWeights();
			net->MutateInputWeights(mutationRate, -150.f, 150.f, 3.f);
			net->MutateHiddenWeights(mutationRate, -150.f, 150.f, 3.f);
		}

//        // create next generation of nn's
//        for ( unsigned int p=3; p<parents.size(); ++p )
//        {
//            NeuralNet* net = mPlayers[parents[p].mPlayerId]->GetNeuralNet();
//
////            int p1 = randGen.LinearSlope(parents.size());
//            int p2 = randGen.LinearSlope(parents.size()/2);
////            *net = *(mPlayers[parents[p1].mPlayerId]->GetTrainingNeuralNet());
//            NeuralNet* cross = mPlayers[parents[p2].mPlayerId]->GetTrainingNeuralNet();
//            net->CrossInputWeights(0.003, *cross, false);
//            net->CrossHiddenWeights(0.003, *cross, false);
//
//            net->MutateInputWeights(0.0003f*(p+1), -1000.f, 1000.f, 3.f);
//            net->MutateHiddenWeights(0.0003f*(p+1), -1000.f, 1000.f, 3.f);
//        }
    }

    // set all player to same random start position
    const int RANDOM_RESTART_TIME = 180000;  // seconds*1000
    static int lastResetTime = time;
    if ( needRestart || time - lastResetTime > RANDOM_RESTART_TIME )
    {
        lastResetTime = time;
        core::vector3df rotation;
        core::vector3df center;

        int indexMarker = randGen.GetNumberInRange(0, APP.GetLevelManager()->GetNrOfTrackMarkers()-1);
        fprintf(stderr, "**reset to %d\n", indexMarker);
        GetRelocationPos(indexMarker, center, rotation);

        for ( unsigned int p=0; p<mPlayers.size(); ++p)
        {
            if ( mPlayers[p]->GetType() == PT_AI )
            {
                PhysicsObject* physicsObj = APP.GetPhysics()->GetPhysicsObject(mPlayers[p]->GetPhysicsId());
                physicsObj->mSceneNode->setPosition(center);
                physicsObj->mSceneNode->setRotation(rotation);
                physicsObj->mSceneNode->updateAbsolutePosition();
                mPlayers[p]->InfoTrainingAiTeleport(time);
            }
        }
        return;
    }
}
#endif

void Game::GetRelocationPos(int wallIndex_, core::vector3df &pos_, core::vector3df &rotation_)
{
    while ( wallIndex_ >= 0 )
    {
        TrackMarker marker = APP.GetLevelManager()->GetTrackMarker(wallIndex_);
        if ( marker.mSettings.mRelocate )
        {
            pos_ = marker.mSettings.mCenter;
            rotation_ = marker.mSettings.mRotation;
            return;
        }
        --wallIndex_;
    }
    pos_ = APP.GetLevelManager()->GetTrackStart(0).mSettings.mCenter;
    rotation_ = APP.GetLevelManager()->GetTrackStart(0).mSettings.mRotation;
}

void Game::Pause()
{
    if ( mMode == GM_PAUSED )
        return;

    APP.GetGui()->DisplayInfoText("pause game", 3000);
    APP.SetMode(MODE_GUI);

    for ( unsigned int p=0; p<mPlayers.size(); ++p)
    {
        mPlayers[p]->InfoGamePaused();
    }
    if ( APP.GetSound() )
        APP.GetSound()->StopSample(mSoundRefCountdown);

    if ( mMode != GM_PAUSED && mMode != GM_RESUME_PAUSE )
    {
        mResumeMode = mMode;
    }
    mMode = GM_PAUSED;
    mGameTimer->Stop();

    // save recording for best lap
    int levelIndex = APP.GetLevelManager()->GetCurrentLevelIndex();
    const LevelSettings &level = APP.GetLevelManager()->GetLevel(levelIndex);
    std::string filenameLapRecord( APP.GetConfig()->MakeLapRecordName(level.mId, APP.GetStringTable()) );
    mRecordBestLap->Save(filenameLapRecord);

    APP.GetHighscoreManager()->Save();
    APP.GetProfileManager()->SaveProfiles();

    APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuPause() );
}

void Game::Finish(bool finishByPlaying_)
{
    if ( mMode == GM_FINISHED )
    {
        return;
    }

    for ( unsigned int p=0; p<mPlayers.size(); ++p)
    {
        mPlayers[p]->InfoGameFinished();
    }
    if ( APP.GetSound() )
        APP.GetSound()->StopSample(mSoundRefCountdown);

    mMode = GM_FINISHED;
    mGameTimer->Stop();
    mGameFinishedTimer->Start();

    // save recording for best lap
    int levelIndex = APP.GetLevelManager()->GetCurrentLevelIndex();
    const LevelSettings &level = APP.GetLevelManager()->GetLevel(levelIndex);
    std::string filenameLapRecord( APP.GetConfig()->MakeLapRecordName(level.mId, APP.GetStringTable()) );
    mRecordBestLap->Save(filenameLapRecord);

//    if ( mAutosaveRecording  )
//    {
//        SaveLastRecording();
//    }
    CalcGameResult(finishByPlaying_);

    if ( finishByPlaying_)
    {
        mFinishedByPlaying = true;
        PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
        if ( profile )
        {
            profile->InfoGameFinished();
        }
        // check for track highscore
        if ( mTrackScores )
        {
            int timeResult = mResult.mLocalPlayerTrackTimeMs;
            if ( mTrackScores->CheckPosition(timeResult) >= 0 )
            {
                HScoreEntry score;
                score.mTime = timeResult;
                if ( profile )
                {
                    score.mProfileName = profile->mName;
                }
                int pos = mTrackScores->AddEntry(score);

                if ( pos == 0 && (GT_TIMERACE == mSettings.mGameType || GT_RIVALS == mSettings.mGameType) )
                {
                    // save recording for new track record
                    int levelIndex = APP.GetLevelManager()->GetCurrentLevelIndex();
                    const LevelSettings &level = APP.GetLevelManager()->GetLevel(levelIndex);
                    std::string filenameTrackRecord( APP.GetConfig()->MakeTrackRecordName(level.mId, APP.GetStringTable()) );
                    GhostRecordSettings trackGhostRecordSettings;
                    if ( profile )
                    {
                        trackGhostRecordSettings.mProfileName = profile->mName;
                    }
                    trackGhostRecordSettings.mLevelName = mSettings.mLevelName;
                    trackGhostRecordSettings.mGhostType = (int)mSettings.mGhostType;
                    trackGhostRecordSettings.mTime = timeResult;
                    trackGhostRecordSettings.mChecksum = trackGhostRecordSettings.CalcChecksum();
                    trackGhostRecordSettings.Write(mRecordLocalPlayer->GetSerializeArchive());
                    mRecordLocalPlayer->Save(filenameTrackRecord);
                    *mRecordBestTrack = *mRecordLocalPlayer;
                }
            }
        }

        // check for session highscore
        if (    GT_RIVALS == mSettings.mGameType
            &&  mResult.mLocalPlayerTrackTimeMs < mSessionTrackRecord )
        {
            mSessionTrackRecord = mResult.mLocalPlayerTrackTimeMs;
            *mRecordSession = *mRecordLocalPlayer;
        }

        if (    GT_CHAMPIONSHIP == GetSettings().mGameType
            &&  APP.GetChampionship()->GetRaceSelected() >= 1 )
        {
            TiXmlElement * ele = APP.GetConfig()->GetEtcSettings();
            if ( ele )
            {
                ele->SetAttribute("tutorial_done", 1 );
                APP.GetConfig()->Save();
            }
        }

        if ( mSettings.mGameType == GT_RIVALS )
        {
            APP.GetRivalsMode()->InfoGameFinished();
        }
    }
    APP.GetHighscoreManager()->Save();
    APP.GetProfileManager()->SaveProfiles();

    APP.SetMode( MODE_GUI );
    if ( mSettings.mGameType == GT_CHAMPIONSHIP )
    {
        APP.GetGui()->SetActiveDialog(APP.GetGui()->GetGuiMenuGameEndChampionship());
    }
    else if ( mSettings.mGameType == GT_RIVALS )
    {
        APP.GetGui()->SetActiveDialog(APP.GetGui()->GetGuiMenuGameEndRivals());
    }
    else
    {
        APP.GetGui()->SetActiveDialog(APP.GetGui()->GetGuiMenuGameEnd());
    }
}

void Game::Resume()
{
    APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiHud() );
    mMode = GM_RESUME_PAUSE;
    mResumeGameTime = APP.GetIrrlichtManager()->GetIrrlichtTimer()->getTime() + mSteeringSettings.mResumeFreeze;

	IInputDevice * touchDevice = APP.GetInputDeviceManager()->GetDevice(IDP_TOUCH);
	if ( touchDevice )
		touchDevice->Reset();

    for ( unsigned int p=0; p < mPlayers.size(); ++p )
    {
        if ( mPlayers[p]->IsActiveType() )
        {
            mPlayers[p]->InfoGameResumePause();
        }
    }

    if ( !mAiTraining )
    {
        SetActiveCameraIndex(mActiveCameraIndex);
    }
}

void Game::SetActiveCameraIndex(size_t index_)
{
    mActiveCameraIndex = index_;

    PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
    if ( profile )
    {
        profile->mActiveCameraIndex = mActiveCameraIndex;
    }

    if (    mMode != GM_RUNNING
        &&  mMode != GM_RESUME_PAUSE
        &&  mMode != GM_START
        )
    {
        return;
    }

    scene::ICameraSceneNode* gameCam = APP.GetIrrlichtManager()->GetGameCamera();

    if ( mCameras[mActiveCameraIndex] )
    {
		const CameraSettings& camSettings = mCameras[mActiveCameraIndex]->GetSettings();
        std::wstring wName( camSettings.mName );
        u32 targetTime = mGameTimer->GetTime() + 3000;
        APP.GetGui()->GetGuiHud()->SetCameraName( wName.c_str(), targetTime );

		scene::ISceneNode * hover = mPlayers[mLocalPlayerIndex]->GetHoverNode();
		if ( hover )
		{
			int physicsId = mPlayers[mLocalPlayerIndex]->GetPhysicsId();
			mCameras[mActiveCameraIndex]->ResetGameCam(hover->getAbsoluteTransformation(), gameCam, APP.GetPhysics()->GetPhysicsObject(physicsId));
			if ( camSettings.mIsFirstPerson )
			{
				hover->setVisible(false);
			}
			else
			{
				hover->setVisible(true);
			}
		}
    }
    else
    {
        APP.GetGui()->GetGuiHud()->SetCameraName(L"");
    }
}

void Game::SetNextCameraActive()
{
    int index = (mActiveCameraIndex+1)%GetNumCameras();
    SetActiveCameraIndex(index);
}

void Game::SetControllerSettings(const TiXmlElement * settings_)
{
    mSteeringSettings.ReadFromXml(settings_);
}

void Game::ReloadSettingsCamera()
{
    TiXmlElement * camerasNode = APP.GetConfig()->GetCamerasSettings();
    TiXmlNode* nodeCamera = camerasNode ? camerasNode->IterateChildren(NULL) : NULL;
    for ( unsigned int i=0; i< mCameras.size(); ++i )
    {
        if ( camerasNode && nodeCamera )
        {
            mCameras[i]->ReloadSettings(nodeCamera->ToElement());
            nodeCamera = camerasNode->IterateChildren(nodeCamera);
        }
    }
}

void Game::ReloadSettingsSteering()
{
    mSteeringSettings.ReadFromXml(APP.GetConfig()->GetSteeringSettings());
}

void Game::ReloadSettingsPhysics()
{
    APP.GetPhysics()->GetSettings().ReadFromXml(APP.GetConfig()->GetPhysicsSettings());
    for ( unsigned int p=0; p< mPlayers.size(); ++p )
    {
        PhysicsObject* hoverPhysics = APP.GetPhysics()->GetPhysicsObject(mPlayers[p]->GetPhysicsId());
        if ( hoverPhysics )
        {
            hoverPhysics->GetSettings().ReadFromXml(APP.GetConfig()->GetPhysicsObjectSettings("hover"));
        }
    }
}

void Game::ReloadSettingsSound()
{
    if ( APP.GetSound() )
        APP.GetSound()->GetSettings().ReadFromXml(APP.GetConfig()->GetSoundSettings());
}

void Game::SaveLastRecording()
{
    if ( mPlayers[mLocalPlayerIndex]->GetRecordingId() >= 0 && mLocalPlayerPreviousStartTrack >= 0)
    {
        const LevelSettings & levelSettings = APP.GetLevelManager()->GetCurrentLevel();
        std::ostringstream filename;
        filename << APP.GetConfig()->GetPathRecordings() << "L" << levelSettings.mId << "P" << mLocalPlayerPreviousStartTrack;

        int id = mPlayers[mLocalPlayerIndex]->GetRecordingId();
        Record * record = mRecorder->GetRecord(id);
        if ( record )
            record->Save(filename.str());
        //mRecorder->PrintRecordSize(mPlayers[mLocalPlayerIndex]->GetRecordingId());
    }
}

void Game::SetAutosaveRecording(bool enable_)
{
    mAutosaveRecording = enable_;
}

bool Game::GetAutosaveRecording()
{
    return mAutosaveRecording;
}

void Game::SetAutoloadRecording(bool enable_)
{
    mAutoloadRecording = enable_;
}

bool Game::GetAutoloadRecording()
{
    return mAutoloadRecording;
}

void Game::SetAiTraining(bool enable_)
{
    if ( enable_ && !mAiTraining )
    {
        for ( unsigned int p=0; p< mPlayers.size(); ++p )
        {
            mPlayers[p]->ChangeModel("hc_shadow");
        }
    }
    else if ( !enable_ && mAiTraining )
    {
        for ( unsigned int p=0; p< mPlayers.size() && p < 4; ++p )
        {
            mPlayers[p]->ChangeModel("hover_player");
        }
    }
    mAiTraining = enable_;
}

void Game::UpdateHoverAmbience(const LevelSettings& settings_)
{
    for ( unsigned int i=0; i<mPlayers.size(); ++i )
    {
        scene::ISceneNode* node = mPlayers[i]->GetHoverNode();
        APP.GetIrrlichtManager()->SetNodeAndChildsAmbientMaterial(node, settings_.mAmbHoverRed, settings_.mAmbHoverGreen, settings_.mAmbHoverBlue, -1);
    }
}

void Game::CalcGameResult(bool finishByPlaying_)
{
    mResult.mGameWasFinished = finishByPlaying_;
    mResult.mLocalPlayerTrackTimeMs = mGameTimer->GetTime()-mRaceStartTime;
    mResult.mLocalPlayerNumLaps = mPlayers[mLocalPlayerIndex]->GetCurrentRound();
    if ( mResult.mLocalPlayerNumLaps > 0 )
    {
        mResult.mLocalPlayerLastLapTimeMs = mPlayers[mLocalPlayerIndex]->GetLastRoundTime();
        mResult.mLocalPlayerBestLapTimeMs = mPlayers[mLocalPlayerIndex]->GetBestRoundTime();
        if ( mResult.mLocalPlayerBestLapTimeMs < mResult.mLocalPlayerBestSessionLapTimeMs )
            mResult.mLocalPlayerBestSessionLapTimeMs = mResult.mLocalPlayerBestLapTimeMs;
    }

    mResult.mLocalPlayerPlacing = mPlayers[mLocalPlayerIndex]->GetPlacing();
    if ( finishByPlaying_ )
    {
        PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
        if ( GT_CHAMPIONSHIP == mSettings.mGameType )
        {
            // mLocalPlayerPlacing and mLocalPlayerTrackTimeMs must be set here already!
            profile->mChampionship.CalcRaceScore( APP.GetChampionship()->GetRaceSelected(), mResult);
        }
    }
}

void Game::SetSettings(const GameSettings &settings_)
{
    mSettings = settings_;

    int levelIndex = APP.GetLevelManager()->GetIndexForName( mSettings.mLevelName );
    mLapScores = APP.GetHighscoreManager()->GetHighscores(EST_BESTLAP, levelIndex);
    if ( mSettings.mGameType == GT_CHAMPIONSHIP )
    {
        int champIndex = APP.GetChampionship()->GetRaceSelected();
        mTrackScores =  APP.GetHighscoreManager()->GetHighscores(EST_CHAMPIONSHIP, champIndex);
    }
    else if ( mSettings.mGameType == GT_TIMERACE || GT_RIVALS == mSettings.mGameType )
    {
        mTrackScores = APP.GetHighscoreManager()->GetHighscores(EST_TIMERACE, levelIndex);
    }
    else
    {
        mTrackScores = NULL;
    }
}

bool Game::OnEvent(const SEvent &event)
{
    // Some special handling for edit-cam in training mode
    if ( mAiTraining )
    {
        switch ( event.EventType )
        {
            case EET_MOUSE_INPUT_EVENT:
            {
                switch ( event.MouseInput.Event )
                {
                    case EMIE_RMOUSE_PRESSED_DOWN:
                    {
                        gui::ICursorControl* cursor = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getCursorControl();
                        assert(cursor);
                        if ( cursor->isVisible() )
                        {
                            mOldCursorPos = cursor->getPosition();
                            cursor->setVisible(false);
                        }
                    }
                    break;
                    case EMIE_RMOUSE_LEFT_UP:
                    {
                        gui::ICursorControl* cursor = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getCursorControl();
                        assert(cursor);
                        if ( !cursor->isVisible() )
                        {
                            cursor->setPosition(mOldCursorPos.X, mOldCursorPos.Y);
                            cursor->setVisible(true);
                        }
                    }
                    break;
                    default:
                    break;
                }
                break;
            }
            default:
            break;
        }
    }

    return false;
}

void Game::ResetSessionRecord()
{
    mSessionTrackRecord = TIME_UNUSED;
    ResetRecord(mRecordSession);
}
