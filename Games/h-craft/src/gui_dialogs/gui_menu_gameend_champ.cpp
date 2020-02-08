// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_gameend_champ.h"
#include "gui_hud.h"
#include "gui_helper.h"
#include "../gui.h"
#include "../main.h"
#include "../game.h"
#include "../highscores.h"
#include "../string_table.h"
#include "../profiles.h"
#include "../compile_config.h"

using namespace irr;
using namespace gui;

GuiMenuGameEndChampionship::GuiMenuGameEndChampionship(const Config& config)
    : GuiDialog(config)
    , mStaticResult(0)
    , mStaticPoints(0)
    , mStaticTrackName(0)
    , mStaticTimeCurrentLap(0)
    , mStaticBestLap(0)
    , mStaticTrack(0)
    , mImageTrackRecord(0)
    , mStaticTrackRecord(0)
    , mStaticTimeTrackRecord(0)
    , mStaticLapRecord(0)
    , mStaticLapCount(0)
    , mStaticPosition(0)
    , mStaticGapTime(0)
//    , mButtonWatchReplay(0)
//    , mButtonSaveReplay(0)
{
}

GuiMenuGameEndChampionship::~GuiMenuGameEndChampionship()
{
    RemoveFunctors();
}

bool GuiMenuGameEndChampionship::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_hdcontinue")), new EventFunctor<GuiMenuGameEndChampionship>(this, &GuiMenuGameEndChampionship::OnButtonContinue) );
        AddGuiEventFunctor( GetIdForName(std::string("id_hdrestart")), new EventFunctor<GuiMenuGameEndChampionship>(this, &GuiMenuGameEndChampionship::OnButtonRestart) );
//        AddGuiEventFunctor( GetIdForName(std::string("id_hdwatchreplay")), new EventFunctor<GuiMenuGameEndChampionship>(this, &GuiMenuGameEndChampionship::OnButtonWatchReplay) );
//        AddGuiEventFunctor( GetIdForName(std::string("id_hdsavereplay")), new EventFunctor<GuiMenuGameEndChampionship>(this, &GuiMenuGameEndChampionship::OnButtonSaveReplay) );
//        AddGuiEventFunctor( GetIdForName(std::string("id_hdquit")), new EventFunctor<GuiMenuGameEndChampionship>(this, &GuiMenuGameEndChampionship::OnButtonQuit) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuGameEndChampionship::Load");
        mStaticResult = static_cast<IGUIStaticText*>(GetElementByName(root, "result", errorMsg));
        mStaticPoints = static_cast<IGUIStaticText*>(GetElementByName(root, "resultpoints", errorMsg));
        mStaticTrackName = static_cast<IGUIStaticText*>(GetElementByName(root, "id_trackname", errorMsg));
        mStaticTimeCurrentLap = static_cast<IGUIStaticText*>(GetElementByName(root, "timeCurLap", errorMsg));
        mStaticBestLap = static_cast<IGUIStaticText*>(GetElementByName(root, "timeBestLap", errorMsg));
        mStaticTrack = static_cast<IGUIStaticText*>(GetElementByName(root, "timeTrack", errorMsg));
        mImageTrackRecord = static_cast<IGUIImage*>(GetElementByName(root, "bg_trackrecord", errorMsg));
        mStaticTrackRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "trackrecord", errorMsg));
        mStaticTimeTrackRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "timeTrackRecord", errorMsg));
        mStaticLapRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "timeLapRecord", errorMsg));
        mStaticLapCount = static_cast<IGUIStaticText*>(GetElementByName(root, "lapcount", errorMsg));
        mStaticPosition = static_cast<IGUIStaticText*>(GetElementByName(root, "position", errorMsg));
        mStaticGapTime = static_cast<IGUIStaticText*>(GetElementByName(root, "gap", errorMsg));

//        mButtonWatchReplay = static_cast<IGUIButton*>(GetElementByName(root, "id_hdwatchreplay", errorMsg));
//        mButtonSaveReplay = static_cast<IGUIButton*>(GetElementByName(root, "id_hdsavereplay", errorMsg));
    }
    return ok;
}

void GuiMenuGameEndChampionship::Show()
{
    GuiDialog::Show();

//    if ( mButtonWatchReplay && mButtonSaveReplay)    // TODO: disabled until replay is implemented
//    {
//        mButtonWatchReplay->setEnabled(false);
//        mButtonSaveReplay->setEnabled(false);
//    }

    if ( mStaticResult
        && mStaticPoints
        && mStaticTrackName
        && mStaticTimeCurrentLap
        && mStaticBestLap
        && mStaticTrack
        && mStaticLapCount
        && mStaticPosition
        && mStaticGapTime)
    {
        const GameResult &gameResult = APP.GetGame()->GetResult();
        const GameSettings& gameSettings = APP.GetGame()->GetSettings();

        if ( 0 == APP.GetGame()->GetSettings().mTargetTime )
        {
            if ( gameResult.mGameWasFinished )
            {
                switch ( gameResult.mLocalPlayerPlacing )
                {
                    case 1:
                        mStaticResult->setText( APP.GetStringTable()->Get("id_hdresult1").c_str() );
                    break;
                    case 2:
                        mStaticResult->setText( APP.GetStringTable()->Get("id_hdresult2").c_str() );
                    break;
                    case 3:
                        mStaticResult->setText( APP.GetStringTable()->Get("id_hdresult3").c_str() );
                    break;
                    case 4:
                        mStaticResult->setText( APP.GetStringTable()->Get("id_hdresult4").c_str() );
                    break;
                    default:
                        mStaticResult->setText( L"???" );
                }
            }
            else
            {
                mStaticResult->setText( APP.GetStringTable()->Get("id_hdresult").c_str() );
            }

			core::stringw editText(APP.GetStringTable()->Get("id_hdposition"));
			editText += L' ';
			editText += core::stringw(gameResult.mLocalPlayerPlacing);
			editText += L" / ";
			editText += core::stringw(gameSettings.mNrOfBots+1);
            mStaticPosition->setText(editText.c_str());

            mStaticGapTime->setVisible(false);
        }
        else
        {
            s32 gapTime = gameResult.mLocalPlayerTrackTimeMs - gameSettings.mTargetTime;

            std::wstring hudGapTime( APP.GetStringTable()->Get("id_gap").c_str() );
            hudGapTime += GuiHelper::MakeTimeString(gapTime, true);
            mStaticPosition->setText( hudGapTime.c_str() );

            s32 timeResult = Game::TIME_UNUSED;
            if ( !gameResult.mGameWasFinished )
            {
                mStaticGapTime->setVisible(false);
            }
            else
            {
                timeResult = gameResult.mLocalPlayerTrackTimeMs;

                mStaticGapTime->setVisible(true);
                std::wstring strGapTime;
                strGapTime += L"( ";
                strGapTime += APP.GetStringTable()->Get("id_gap").c_str();
                strGapTime += GuiHelper::MakeTimeString(gapTime, true);
                strGapTime += L" )";

                mStaticGapTime->setText( strGapTime.c_str() );
            }
            mStaticResult->setText( GuiHelper::MakeTimeString(timeResult).c_str() );
        }

		// id_hdresultpoints contains a %d
		core::stringw editText(APP.GetStringTable()->Get("id_hdresultpoints"));
		editText.replace( core::stringw(L"%d"), core::stringw(gameResult.mChampionshipScore));
        mStaticPoints->setText( editText.c_str() );

        mStaticTrackName->setText( gameSettings.mLevelName.c_str() );

        int numLaps = gameResult.mLocalPlayerNumLaps;
        if ( gameSettings.mNrOfLaps > 0 && numLaps > gameSettings.mNrOfLaps)
            numLaps = gameSettings.mNrOfLaps;
		editText = APP.GetStringTable()->Get("id_hdlapcount");
		editText += L' ';
		editText += core::stringw(numLaps);
		editText += L" / ";
		editText += core::stringw(gameSettings.mNrOfLaps);
        mStaticLapCount->setText(editText.c_str());

        mStaticTimeCurrentLap->setText(GuiHelper::MakeTimeString(gameResult.mLocalPlayerLastLapTimeMs).c_str());
        mStaticBestLap->setText(GuiHelper::MakeTimeString(gameResult.mLocalPlayerBestLapTimeMs).c_str());
        mStaticTrack->setText(GuiHelper::MakeTimeString(gameResult.mLocalPlayerTrackTimeMs).c_str());
    }

    if ( mStaticLapRecord )
    {
        const Highscores* lapHighscores = APP.GetGame()->GetCurrentLapHighscores();
        if ( lapHighscores )
        {
            int lapRecord = lapHighscores->GetScore(0).mTime;
            mStaticLapRecord->setText(GuiHelper::MakeTimeString(lapRecord).c_str());
        }
    }

    if (   mStaticTimeTrackRecord
        && mStaticTrackRecord
        && mImageTrackRecord )
    {
        const Highscores* trackHighscores = APP.GetGame()->GetCurrentTrackHighscores();
        if ( trackHighscores )
        {
            int trackRecord = trackHighscores->GetScore(0).mTime;
            mImageTrackRecord->setVisible(true);
            mStaticTrackRecord->setVisible(true);
            mStaticTimeTrackRecord->setVisible(true);
            mStaticTimeTrackRecord->setText(GuiHelper::MakeTimeString(trackRecord).c_str());
        }
        else
        {
            mImageTrackRecord->setVisible(false);
            mStaticTrackRecord->setVisible(false);
            mStaticTimeTrackRecord->setVisible(false);
        }
    }
}

void GuiMenuGameEndChampionship::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;

    RemoveGuiEventFunctor( GetIdForName(std::string("id_hdcontinue")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_hdrestart")) );
//    RemoveGuiEventFunctor( GetIdForName(std::string("id_hdwatchreplay")) );
//    RemoveGuiEventFunctor( GetIdForName(std::string("id_hdsavereplay")) );
//    RemoveGuiEventFunctor( GetIdForName(std::string("id_hdquit")) );
}

bool GuiMenuGameEndChampionship::OnButtonContinue(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.AdvertismentCheck();
        GuiHelper::SetGameModeMenu(APP.GetGame()->GetSettings().mGameType);
    }

    return false;
}

bool GuiMenuGameEndChampionship::OnButtonRestart(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiHud() );
        APP.GetGame()->PrepareStart();
        APP.SetMode(MODE_GAME);
        APP.GetGame()->Start();
    }

    return false;
}

bool GuiMenuGameEndChampionship::OnButtonWatchReplay(const irr::SEvent &event_)
{
    // no longer used
    return false;
}

bool GuiMenuGameEndChampionship::OnButtonSaveReplay(const irr::SEvent &event_)
{
    // no longer used
    return false;
}

bool GuiMenuGameEndChampionship::OnButtonQuit(const irr::SEvent &event_)
{
    // no longer used
//    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
//    {
//        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
//        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuMain() );
//    }

    return false;
}

