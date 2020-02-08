// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_timeattack.h"
#include "gui_menu_main.h"
#include "gui_menu_loadingscreen.h"
#include "gui_helper.h"
#include "gui_dlg_okcancel.h"
#include "../gui.h"
#include "../main.h"
#include "../highscores.h"
#include "../profiles.h"
#include "../string_table.h"
#include "../logging.h"
#include "../recorder.h"
#include "../config.h"
#include "../irrlicht_manager.h"
#include "../logging.h"
#include <assert.h>

using namespace irr;
using namespace gui;

GuiMenuTimeAttack::GuiMenuTimeAttack(const Config& config)
    : GuiDialog(config)
    , mActiveProfile(0)
    , mSliderTracks(0)
    , mSliderDifficulty(0)
    , mSliderRaceMode(0)
    , mSliderGhost(0)
    , mStaticHighscore(0)
    , mStaticScoreGhost(0)
    , mStaticTextTargetTime(0)
    , mStaticTimeTargetTime(0)
    , mButtonExportGhost(0)
    , mButtonStart(0)
    , mButtonBack(0)
{
    SetSuppressSceneRendering(true);
}

GuiMenuTimeAttack::~GuiMenuTimeAttack()
{
    RemoveFunctors();
}

bool GuiMenuTimeAttack::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        GetAllGhostRecordsFromDisk();

        AddGuiEventFunctor( GetIdForName(std::string("id_ghostexport")), new EventFunctor<GuiMenuTimeAttack>(this, &GuiMenuTimeAttack::OnButtonExportGhost) );
        AddGuiEventFunctor( GetIdForName(std::string("id_start")), new EventFunctor<GuiMenuTimeAttack>(this, &GuiMenuTimeAttack::OnButtonStart) );
        AddGuiEventFunctor( GetIdForName(std::string("id_back")), new EventFunctor<GuiMenuTimeAttack>(this, &GuiMenuTimeAttack::OnButtonBack) );
        AddGuiEventFunctor( GetIdForName(std::string("track")), new EventFunctor<GuiMenuTimeAttack>(this, &GuiMenuTimeAttack::OnSliderTracks) );
        AddGuiEventFunctor( GetIdForName(std::string("difficulty")), new EventFunctor<GuiMenuTimeAttack>(this, &GuiMenuTimeAttack::OnSliderDifficulty) );
        AddGuiEventFunctor( GetIdForName(std::string("racemode")), new EventFunctor<GuiMenuTimeAttack>(this, &GuiMenuTimeAttack::OnSliderRaceMode) );
        AddGuiEventFunctor( GetIdForName(std::string("ghost")), new EventFunctor<GuiMenuTimeAttack>(this, &GuiMenuTimeAttack::OnSliderGhost) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuTimeAttack::Load");

        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));

        mSliderDifficulty = static_cast<IGUITextSlider*>(GetElementByName(root, "difficulty", errorMsg));
        if ( !mSliderDifficulty )
            return false;
        mSliderDifficulty->clearTexts();
        mSliderDifficulty->addText( APP.GetStringTable()->Get("id_dif_all").c_str() );
        mSliderDifficulty->addText( APP.GetStringTable()->Get("id_dif_easy").c_str() );
        mSliderDifficulty->addText( APP.GetStringTable()->Get("id_dif_medium").c_str() );
        mSliderDifficulty->addText( APP.GetStringTable()->Get("id_dif_hard").c_str() );

        mSliderTracks = static_cast<IGUITextSlider*>(GetElementByName(root, "track", errorMsg));
        if ( !mSliderTracks )
            return false;
        GuiHelper::FillTextSliderWithTrackNames(mSliderTracks, LD_ALL);

        mSliderRaceMode = static_cast<IGUITextSlider*>(GetElementByName(root, "racemode", errorMsg));
        if ( !mSliderRaceMode )
            return false;
        mSliderRaceMode->clearTexts();
        mSliderRaceMode->addText( APP.GetStringTable()->Get("id_mode_hotlap").c_str() );
        mSliderRaceMode->addText( APP.GetStringTable()->Get("id_mode_timerace").c_str() );

        mSliderGhost = static_cast<IGUITextSlider*>(GetElementByName(root, "ghost", errorMsg));
        if ( !mSliderGhost )
            return false;
        mSliderGhost->clearTexts();
        mSliderGhost->addText( APP.GetStringTable()->Get("id_on").c_str() );
        mSliderGhost->addText( APP.GetStringTable()->Get("id_off").c_str() );

        mStaticHighscore = static_cast<IGUIStaticText*>(GetElementByName(root, "highscore", errorMsg));
        mStaticScoreGhost = static_cast<IGUIStaticText*>(GetElementByName(root, "ghost_highscore", errorMsg));
        mStaticTextTargetTime = static_cast<IGUIStaticText*>(GetElementByName(root, "id_targettime", errorMsg));
        mStaticTimeTargetTime = static_cast<IGUIStaticText*>(GetElementByName(root, "tr_targettime", errorMsg));

        mButtonExportGhost = static_cast<IGUIButton*>(GetElementByName(root, "id_ghostexport", errorMsg));
        mButtonStart = static_cast<IGUIButton*>(GetElementByName(root, "id_start", errorMsg));
        mButtonBack = static_cast<IGUIButton*>(GetElementByName(root, "id_back", errorMsg));
        SetAlternateDefaultFocus( mButtonBack );
    }
    return ok;
}

void GuiMenuTimeAttack::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;

    RemoveGuiEventFunctor( GetIdForName(std::string("id_ghostexport")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_start")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_back")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("track")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("difficulty")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("racemode")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("ghost")) );
}


void GuiMenuTimeAttack::Show()
{
    if ( mSliderTracks && !mSliderTracks->isVisible() )  // a call to show before dialog is visible
    {
        s32 oldNumTexts = mSliderTracks->getNumTexts();
        s32 oldId = mSliderTracks->getCurrentTextId();
        GuiHelper::FillTextSliderWithTrackNames(mSliderTracks, GetDifficulty());
        if ( mSliderTracks->getNumTexts() != oldNumTexts )
            mSliderTracks->setCurrentTextId(0);
        else
            mSliderTracks->setCurrentTextId(oldId);
    }

    GuiDialog::Show();

    if ( !APP.GetLevelManager() )
        return;

    int idxLevel = -1;
    if ( mSliderTracks && mSliderTracks->getCurrentText() && APP.GetLevelManager() )
        idxLevel = APP.GetLevelManager()->GetIndexForName( mSliderTracks->getCurrentText() );

    if ( mButtonStart )
    {
        if ( idxLevel < 0 )
        {
            mButtonStart->setEnabled(false);
            mStaticHighscore->setVisible(false);
            mStaticTimeTargetTime->setVisible(false);
        }
        else
        {
            mButtonStart->setEnabled(true);
            mStaticHighscore->setVisible(true);
            mStaticTimeTargetTime->setVisible(true);
        }
    }

    if ( mStaticTextTargetTime && mStaticTimeTargetTime )
    {
        const LevelSettings& levelSettings = APP.GetLevelManager()->GetLevel(idxLevel);
        if ( GT_TIMERACE == GetGameType() )
        {
            std::wstring wstr( GuiHelper::MakeTimeString((s32)(levelSettings.mTargetTime*1000.f)) );
            mStaticTextTargetTime->setVisible(true);
            mStaticTimeTargetTime->setText( wstr.c_str() );
            mStaticTimeTargetTime->setVisible(true);
        }
        else
        {
            mStaticTextTargetTime->setVisible(false);
            mStaticTimeTargetTime->setVisible(false);
        }
    }

    if ( mActiveProfile )
    {
        PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
        if ( profile )
        {
            mActiveProfile->setText( profile->mName.c_str() );
        }
        else
        {
            mActiveProfile->setText( APP.GetStringTable()->Get("id_showactiveprofile").c_str() );
        }
    }

    if ( mStaticHighscore )
    {
        bool canExportGhost = false;

        Highscores* scoresTimerace = APP.GetHighscoreManager()->GetHighscores(EST_TIMERACE, idxLevel);
        Highscores* scoresLap = APP.GetHighscoreManager()->GetHighscores(EST_BESTLAP, idxLevel);
        if ( scoresTimerace && scoresLap )
        {
            if ( GT_HOTLAP == GetGameType() )
            {
                const HScoreEntry& scoreLap = scoresLap->GetScore(0);
                std::wstring scoreStr( scoreLap.mProfileName );
                scoreStr += L" ";
                scoreStr += APP.GetStringTable()->Get("id_score_lap").c_str();
                scoreStr += L" ";
                scoreStr += GuiHelper::MakeTimeString(scoreLap.mTime);
                mStaticHighscore->setText( scoreStr.c_str() );

                if ( scoreLap.IsUserScore() )
                    canExportGhost = true;
            }
            else if ( GT_TIMERACE == GetGameType() )
            {
                const HScoreEntry& scoreTrack = scoresTimerace->GetScore(0);
                std::wstring scoreStr( scoreTrack.mProfileName );
                scoreStr += L" ";
                scoreStr += APP.GetStringTable()->Get("id_score_track").c_str();
                scoreStr += L" ";
                scoreStr += GuiHelper::MakeTimeString(scoreTrack.mTime);
                mStaticHighscore->setText( scoreStr.c_str() );

                if ( scoreTrack.IsUserScore() )
                    canExportGhost = true;

                const LevelSettings& levelSettings = APP.GetLevelManager()->GetLevel(idxLevel);
                if ( scoreTrack.mTime <= levelSettings.mTargetTime*1000 && mStaticTextTargetTime )
                {
                    std::wstring wstr( mStaticTimeTargetTime->getText() );
                    wstr += L' ';
                    wstr += APP.GetStringTable()->Get("id_passed").c_str();
                    mStaticTimeTargetTime->setText( wstr.c_str() );
                }
            }
        }
        if ( mButtonExportGhost )
        {
            mButtonExportGhost->setEnabled( canExportGhost );
        }
    }

    if ( mSliderGhost )
    {
        const LevelSettings& levelSettings = APP.GetLevelManager()->GetLevel(idxLevel);
        s32 oldTextId = mSliderGhost->getCurrentTextId();
        mSliderGhost->clearTexts();
        mSliderGhost->addText( APP.GetStringTable()->Get("id_on").c_str() );
        mSliderGhost->addText( APP.GetStringTable()->Get("id_off").c_str() );
        if ( GT_HOTLAP == GetGameType() )
        {
            for ( unsigned int i=0; i < mGhostLapRecords.size(); ++i )
            {
                if  ( levelSettings.mName == mGhostLapRecords[i].mLevelName )
                {
                    mSliderGhost->addText( mGhostLapRecords[i].mProfileName.c_str() );
                }
            }
        }
        else if ( GT_TIMERACE == GetGameType() )
        {
            for ( unsigned int i=0; i < mGhostTrackRecords.size(); ++i )
            {
                if  ( levelSettings.mName == mGhostTrackRecords[i].mLevelName )
                {
                    mSliderGhost->addText( mGhostTrackRecords[i].mProfileName.c_str() );
                }
            }
        }

        if ( oldTextId < mSliderGhost->getNumTexts() )
        {
            mSliderGhost->setCurrentTextId( oldTextId );
        }
        else
        {
            mSliderGhost->setCurrentTextId( mSliderGhost->getNumTexts() - 1 );
        }
    }

    if ( mStaticScoreGhost && mSliderGhost )
    {
        GhostRecordSettings recordSettings;
        if ( GetSelectedGhostRecordSettings(recordSettings) )
        {
            std::wstring scoreStr( recordSettings.mProfileName );
            scoreStr += L" ";
            if ( recordSettings.mGhostType == GHT_EXPORTED_LAP )
            {
                scoreStr += APP.GetStringTable()->Get("id_score_lap").c_str();
            }
            else if ( recordSettings.mGhostType == GHT_EPORTED_TRACK )
            {
                scoreStr += APP.GetStringTable()->Get("id_score_track").c_str();
            }
            scoreStr += L" ";
            scoreStr += GuiHelper::MakeTimeString( recordSettings.mTime );

            mStaticScoreGhost->setVisible(true);
            mStaticScoreGhost->setText( scoreStr.c_str() );
        }
        else
        {
            mStaticScoreGhost->setVisible(false);
        }
    }

    if ( GetConfig().GetUseTouchInput() != ETI_NO_TOUCH && mButtonExportGhost ) // no ghost exports so far on Android
	{
		mButtonExportGhost->setEnabled(false);
		mButtonExportGhost->setVisible(false);
	}
}

bool GuiMenuTimeAttack::IsExchangableGhostRecord( const GhostRecordSettings &r1_, const GhostRecordSettings &r2_ )
{
    return      r1_.mGhostType == r2_.mGhostType
            &&  r1_.mProfileName == r2_.mProfileName
            &&  r1_.mLevelName == r2_.mLevelName;
}

bool GuiMenuTimeAttack::OnExportAcknowledged(const irr::SEvent &event_)
{
    if ( !mSliderTracks )
        return false;

    int levelIndex = APP.GetLevelManager()->GetIndexForName( mSliderTracks->getCurrentText() );
    if ( levelIndex < 0 )
        return false;

    const LevelSettings& levelSettings = APP.GetLevelManager()->GetLevel(levelIndex);
    Record recordDummy;
    GhostRecordSettings recordSettings;

    // copy the right file into the ghosts folder
    if ( GetGameType() == GT_HOTLAP )
    {
        std::string filenameOldRecord( APP.GetConfig()->MakeLapRecordName(levelSettings.mId, APP.GetStringTable()) );
        if ( !recordDummy.Load(filenameOldRecord) )
            return false;

        recordSettings.Read(recordDummy.GetSerializeArchive());
        recordSettings.mGhostType = GHT_EXPORTED_LAP;
        // this was added so we can re-export ghosts with another name
        std::string overrideName( APP.GetOverrideExportName() );
        if ( overrideName.size() )
        {
            recordSettings.mProfileName = std::wstring( overrideName.begin(), overrideName.end() );
        }
        recordSettings.Write(recordDummy.GetSerializeArchive());

        std::string filenameNewRecord( APP.GetConfig()->GhostRecordSettingsToFilename(recordSettings) );
        recordDummy.Save(filenameNewRecord);

        bool isExchangable = false;
        for ( unsigned int i=0; i < mGhostLapRecords.size(); ++i )
        {
            if ( IsExchangableGhostRecord( recordSettings, mGhostLapRecords[i] ) )
            {
                isExchangable = true;
                break;
            }
        }
        if ( !isExchangable )
        {
            mGhostLapRecords.push_back(recordSettings);
            Show();
        }
    }
    else if ( GetGameType() == GT_TIMERACE )
    {
        std::string filenameOldRecord( APP.GetConfig()->MakeTrackRecordName(levelSettings.mId, APP.GetStringTable()) );
        if ( !recordDummy.Load(filenameOldRecord) )
            return false;

        recordSettings.Read(recordDummy.GetSerializeArchive());
        recordSettings.mGhostType = GHT_EPORTED_TRACK;
        // this was added so we can re-export ghosts with another name
        std::string overrideName( APP.GetOverrideExportName() );
        if ( overrideName.size() )
        {
            recordSettings.mProfileName = std::wstring( overrideName.begin(), overrideName.end() );
        }
        recordSettings.Write(recordDummy.GetSerializeArchive());

        std::string filenameNewRecord( APP.GetConfig()->GhostRecordSettingsToFilename(recordSettings) );
        recordDummy.Save(filenameNewRecord);

        bool isExchangable = false;
        for ( unsigned int i=0; i < mGhostTrackRecords.size(); ++i )
        {
            if ( IsExchangableGhostRecord( recordSettings, mGhostTrackRecords[i] ) )
            {
                isExchangable = true;
                break;
            }
        }
        if ( !isExchangable )
        {
            mGhostTrackRecords.push_back(recordSettings);
            Show();
        }
    }

    return false;
}

bool GuiMenuTimeAttack::OnButtonExportGhost(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != EGET_BUTTON_CLICKED )
        return false;

    // show ok/cancel dialog
    APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
    GuiDlgOkCancel * dlg = static_cast<GuiDlgOkCancel*>(APP.GetGui()->GetGuiDlgOkCancel());
    dlg->SetInfoText( APP.GetStringTable()->Get("id_ghost_export_dlg").c_str() );
    dlg->SetOkFunctor( new EventFunctor<GuiMenuTimeAttack>(this, &GuiMenuTimeAttack::OnExportAcknowledged) );
    dlg->SetCancelFunctor(0);
    APP.GetGui()->SetModalDialog( dlg );
    dlg->SetFocusOkButton();

    return false;
}

bool GuiMenuTimeAttack::OnButtonStart(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != EGET_BUTTON_CLICKED )
        return false;

    if ( !mSliderTracks || !mSliderRaceMode || !mSliderGhost)
        return false;

    int levelIndex = APP.GetLevelManager()->GetIndexForName( mSliderTracks->getCurrentText() );
    if ( levelIndex < 0 )
        return false;

    APP.GetGui()->PlayOverrideClickSound(GS_CLICK_OK);

    const LevelSettings& levelSettings = APP.GetLevelManager()->GetLevel(levelIndex);

    GameSettings gameSettings;

    gameSettings.mTargetTime = 0;
    gameSettings.mNrOfBots = 0;
    gameSettings.mGameType = GetGameType();
    if ( GT_HOTLAP == gameSettings.mGameType )
    {
        gameSettings.mNrOfLaps = 0;
        gameSettings.mGhostType = GetGhostType();
    }
    else if ( GT_TIMERACE == gameSettings.mGameType )
    {
        gameSettings.mTargetTime = (u32)(levelSettings.mTargetTime*1000.f);
        gameSettings.mNrOfLaps = levelSettings.mLaps;
        gameSettings.mGhostType = GetGhostType();
    }
    else    // default, should not happen
    {
        LOG.Info("unknown game type selected\n");
        gameSettings.mNrOfLaps = 0;
        gameSettings.mGhostType = GHT_NONE;
    }

    GhostRecordSettings recordSettings;
    if ( GetSelectedGhostRecordSettings(recordSettings) )
    {
        gameSettings.mExportGhostRecord = APP.GetConfig()->GhostRecordSettingsToFilename(recordSettings, false);
    }

    gameSettings.mLevelName = levelSettings.mName;
    APP.GetGame()->SetSettings(gameSettings);

    APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuLoadingScreen() );

    return false;
}

bool GuiMenuTimeAttack::OnButtonBack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuMain() );
    }

    return false;
}

bool GuiMenuTimeAttack::OnSliderTracks(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
    {
        Show();
    }
    return false;
}

bool GuiMenuTimeAttack::OnSliderDifficulty(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
    {
        GuiHelper::FillTextSliderWithTrackNames(mSliderTracks, GetDifficulty());
        Show();
    }
    return false;
}

bool GuiMenuTimeAttack::OnSliderRaceMode(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
    {
        Show();
    }
    return false;
}

bool GuiMenuTimeAttack::OnSliderGhost(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
    {
        Show();
    }
    return false;
}

LevelDifficulty GuiMenuTimeAttack::GetDifficulty() const
{
    if ( !mSliderDifficulty )
    {
        return LD_ALL;
    }
    switch ( mSliderDifficulty->getCurrentTextId() )
    {
        case 0:
            return LD_ALL;
        case 1:
            return LD_EASY;
        case 2:
            return LD_MEDIUM;
        case 3:
            return LD_HARD;
        default:
            break;
    }
    return LD_HARD;
}

GAME_TYPE GuiMenuTimeAttack::GetGameType() const
{
    if ( !mSliderRaceMode )
    {
        return GT_HOTLAP;
    }

    switch ( mSliderRaceMode->getCurrentTextId() )
    {
        case 0:
            return GT_HOTLAP;
        case 1:
            return GT_TIMERACE;
        default:
            break;
    }

    return GT_HOTLAP;
}

GHOST_TYPE GuiMenuTimeAttack::GetGhostType() const
{
    if ( !mSliderGhost  || !mSliderRaceMode )
        return GHT_NONE;

    if ( mSliderGhost->getCurrentTextId() == 1 )
        return GHT_NONE;

    if ( mSliderGhost->getCurrentTextId() == 0 )
    {
        if (GetGameType() == GT_HOTLAP )
            return GHT_LAP;
        else
            return GHT_TRACK;
    }
    else
    {
        if (GetGameType() == GT_HOTLAP )
            return GHT_EXPORTED_LAP;
        else
            return GHT_EPORTED_TRACK;
    }

    // should not reach this
    return GHT_NONE;
}

void GuiMenuTimeAttack::GetAllGhostRecordsFromDisk()
{
    if ( !APP.GetIrrlichtManager() )
        return;
    IrrlichtDevice* device = APP.GetIrrlichtManager()->GetIrrlichtDevice();
    if ( !device )
        return;
    io::IFileSystem * fs = device->getFileSystem();
    if ( !fs )
        return;

    io::path oldDir = fs->getWorkingDirectory();

    std::string ghostDir( APP.GetConfig()->GetPathGhosts() );
    if ( !fs->changeWorkingDirectoryTo(ghostDir.c_str()) )
    {
        LOG.Warn("Warning: GetAllGhostRecordsFromDisk: changeWorkingDirectoryTo failed\n");
        return;
    }
    io::IFileList * fileList = fs->createFileList();
    if ( fileList )
    {
        mGhostLapRecords.clear();
        mGhostTrackRecords.clear();

        for ( u32 i = 0; i < fileList->getFileCount(); ++i )
        {
            if ( fileList->isDirectory(i) )
                continue;

            const io::path& fileName = fileList->getFileName(i);

            if ( APP.GetConfig()->IsGhostRecordFilename(fileName.c_str()) )
            {
                Record record;
                if ( !record.Load(fileList->getFullFileName(i).c_str(), true) )
                {
                    LOG.WarnLn("Warning: GetAllGhostRecordsFromDisk: can't load record", fileList->getFullFileName(i).c_str());
                }
                else
                {
                    GhostRecordSettings recordsettings;

                    // we could do additional checkds here, like comparing filename with file content if necessary
                    recordsettings.Read(record.GetSerializeArchive());

                    if ( recordsettings.mGhostType == GHT_EXPORTED_LAP )
                        mGhostLapRecords.push_back( recordsettings );
                    else if ( recordsettings.mGhostType == GHT_EPORTED_TRACK )
                        mGhostTrackRecords.push_back( recordsettings );
                    else
                    {
                        LOG.WarnLn("Warning: GetAllGhostRecordsFromDisk: unknown ghost type", fileList->getFullFileName(i).c_str());
                    }
                }
            }
        }
        fileList->drop();
    }

    if ( !fs->changeWorkingDirectoryTo(oldDir) )
    {
        LOG.Warn("Warning: GetAllGhostRecordsFromDisk: changeWorkingDirectoryTo failed when setting oldDir\n");
    }
}

bool GuiMenuTimeAttack::GetSelectedGhostRecordSettings(GhostRecordSettings &settings_)
{
    if ( !mSliderGhost || !mSliderTracks || !mSliderTracks->getCurrentText() || !APP.GetLevelManager() )
        return false;

    int id = mSliderGhost->getCurrentTextId() - 2;
    if ( id < 0 )
        return false;

    int idxLevel = APP.GetLevelManager()->GetIndexForName( mSliderTracks->getCurrentText() );
    if ( idxLevel < 0 )
        return false;
    const LevelSettings& levelSettings = APP.GetLevelManager()->GetLevel(idxLevel);

    if ( GetGameType() == GT_HOTLAP )
    {
        for ( unsigned int i=0; i < mGhostLapRecords.size(); ++i )
        {
            if  ( levelSettings.mName == mGhostLapRecords[i].mLevelName )
            {
                if ( 0 == id )
                {
                    settings_ = mGhostLapRecords[i];
                    return true;
                }
                else
                {
                    --id;
                }
            }
        }
    }
    else if ( GetGameType() == GT_TIMERACE )
    {
        for ( unsigned int i=0; i < mGhostTrackRecords.size(); ++i )
        {
            if  ( levelSettings.mName == mGhostTrackRecords[i].mLevelName )
            {
                if ( 0 == id )
                {
                    settings_ = mGhostTrackRecords[i];
                    return true;
                }
                else
                {
                    --id;
                }
            }
        }
    }
    return false;
}
