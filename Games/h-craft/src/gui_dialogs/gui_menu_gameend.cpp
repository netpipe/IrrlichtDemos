// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_gameend.h"
#include "gui_hud.h"
#include "gui_helper.h"
#include "../gui.h"
#include "../main.h"
#include "../game.h"
#include "../highscores.h"
#include "../string_table.h"
#include <sstream>

using namespace irr;
using namespace gui;

GuiMenuGameEnd::GuiMenuGameEnd(const Config& config)
    : GuiDialog(config)
    , mStaticResultText(0)
    , mStaticResult(0)
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
    , mStaticTimeraceGap(0)
    , mStaticPassed(0)
    , mStaticGhostRecord(0)
    , mStaticGhostModeRecord(0)
    , mStaticGhostGapInfo(0)
    , mStaticGhostGap(0)
    , mImageGhostRecord(0)
    , mImageGhostGap(0)
    //, mButtonWatchReplay(0)
//    , mButtonSaveReplay(0)
{
}

GuiMenuGameEnd::~GuiMenuGameEnd()
{
    RemoveFunctors();
}

bool GuiMenuGameEnd::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_hdrestart")), new EventFunctor<GuiMenuGameEnd>(this, &GuiMenuGameEnd::OnButtonRestart) );
//        AddGuiEventFunctor( GetIdForName(std::string("id_hdwatchreplay")), new EventFunctor<GuiMenuGameEnd>(this, &GuiMenuGameEnd::OnButtonWatchReplay) );
//        AddGuiEventFunctor( GetIdForName(std::string("id_hdsavereplay")), new EventFunctor<GuiMenuGameEnd>(this, &GuiMenuGameEnd::OnButtonSaveReplay) );
        AddGuiEventFunctor( GetIdForName(std::string("id_hdquit")), new EventFunctor<GuiMenuGameEnd>(this, &GuiMenuGameEnd::OnButtonQuit) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuGameEnd::Load");
        mStaticResultText = static_cast<IGUIStaticText*>(GetElementByName(root, "resultstatic", errorMsg));
        mStaticResult = static_cast<IGUIStaticText*>(GetElementByName(root, "result", errorMsg));
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
        mStaticTimeraceGap = static_cast<IGUIStaticText*>(GetElementByName(root, "timeraceGap", errorMsg));
        mStaticPassed = static_cast<IGUIStaticText*>(GetElementByName(root, "timerace_result", errorMsg));
        mStaticGhostRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "ghost_record", errorMsg));
        mStaticGhostModeRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "ghost_ModeRecord", errorMsg));
        mStaticGhostGapInfo = static_cast<IGUIStaticText*>(GetElementByName(root, "ghost_gapinfo", errorMsg));
        mStaticGhostGap = static_cast<IGUIStaticText*>(GetElementByName(root, "ghost_gap", errorMsg));
        mImageGhostRecord = static_cast<IGUIImage*>(GetElementByName(root, "bg_ghostrecord", errorMsg));
        mImageGhostGap = static_cast<IGUIImage*>(GetElementByName(root, "bg_ghostgap", errorMsg));

//        mButtonWatchReplay = static_cast<IGUIButton*>(GetElementByName(root, "id_hdwatchreplay", errorMsg));
//        mButtonSaveReplay = static_cast<IGUIButton*>(GetElementByName(root, "id_hdsavereplay", errorMsg));
    }
    return ok;
}

void GuiMenuGameEnd::Show()
{
    GuiDialog::Show();

//    if ( mButtonWatchReplay && mButtonSaveReplay)    // TODO: disabled until replay is implemented
//    {
//        mButtonWatchReplay->setEnabled(false);
//        mButtonSaveReplay->setEnabled(false);
//    }

    if ( mStaticResultText
        && mStaticResult
        && mStaticTrackName
        && mStaticTimeCurrentLap
        && mStaticBestLap
        && mStaticTrack
        && mStaticLapCount
        && mStaticPosition
        && mStaticTimeraceGap
        && mStaticPassed
        )
    {
        const GameResult &gameResult = APP.GetGame()->GetResult();
        const GameSettings& gameSettings = APP.GetGame()->GetSettings();

        if ( gameSettings.mGameType == GT_HOTLAP )
        {
            mStaticResultText->setText( APP.GetStringTable()->Get("id_hdresultstatic02").c_str() );
            mStaticResult->setText( GuiHelper::MakeTimeString(gameResult.mLocalPlayerBestSessionLapTimeMs).c_str() );
            mStaticTimeraceGap->setVisible(false);
            mStaticPassed->setVisible(false);

			core::stringw editText(APP.GetStringTable()->Get("id_hdposition"));
			editText += L' ';
			editText += core::stringw(gameResult.mLocalPlayerPlacing);
			editText += L" / ";
			editText += core::stringw(gameSettings.mNrOfBots+1);
            mStaticPosition->setText(editText.c_str());
        }
        else
        {
            mStaticResultText->setText( APP.GetStringTable()->Get("id_hdresultstatic").c_str() );

            if ( gameSettings.mGameType == GT_ARCADE )
            {
                mStaticTimeraceGap->setVisible(false);
                mStaticPassed->setVisible(false);
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
            }
            else if ( gameSettings.mGameType == GT_TIMERACE )
            {
                s32 gapTime = gameResult.mLocalPlayerTrackTimeMs - gameSettings.mTargetTime;
                std::wstring hudGapTime( APP.GetStringTable()->Get("id_gap").c_str() );
                hudGapTime += GuiHelper::MakeTimeString(gapTime, true);
                mStaticPosition->setText( hudGapTime.c_str() );

                s32 timeResult = Game::TIME_UNUSED;
                if ( gameResult.mGameWasFinished )
                {
                    timeResult = gameResult.mLocalPlayerTrackTimeMs;

                    std::wstring strGapTime;
                    strGapTime += L"( ";
                    strGapTime += APP.GetStringTable()->Get("id_gap").c_str();
                    strGapTime += GuiHelper::MakeTimeString(gapTime, true);
                    strGapTime += L" )";
                    mStaticTimeraceGap->setVisible(true);
                    mStaticTimeraceGap->setText( strGapTime.c_str() );
                    mStaticPassed->setVisible(true);
                    if ( gapTime <= 0  )
                    {
                        mStaticPassed->setText( APP.GetStringTable()->Get("id_passed").c_str() );
                    }
                    else
                    {
                        mStaticPassed->setText( APP.GetStringTable()->Get("id_failed").c_str() );
                    }
                }
                else
                {
                    mStaticTimeraceGap->setVisible(false);
                    mStaticPassed->setVisible(false);
                }
                mStaticResult->setText( GuiHelper::MakeTimeString(timeResult).c_str() );
            }
        }

        mStaticTrackName->setText( gameSettings.mLevelName.c_str() );

		core::stringw editText(APP.GetStringTable()->Get("id_hdlapcount"));
		editText += L' ';
        if ( gameSettings.mNrOfLaps > 0 )
        {
            int numLaps = gameResult.mLocalPlayerNumLaps;
            if ( numLaps > gameSettings.mNrOfLaps)
                numLaps = gameSettings.mNrOfLaps;

			editText += core::stringw(numLaps);
			editText += L" / ";
			editText += core::stringw(gameSettings.mNrOfLaps);
        }
        else
        {
			editText += core::stringw(gameResult.mLocalPlayerNumLaps);
			editText += L" / -";
        }
        mStaticLapCount->setText(editText.c_str());

        mStaticTimeCurrentLap->setText(GuiHelper::MakeTimeString(gameResult.mLocalPlayerLastLapTimeMs).c_str());
        mStaticBestLap->setText(GuiHelper::MakeTimeString(gameResult.mLocalPlayerBestLapTimeMs).c_str());
        mStaticTrack->setText(GuiHelper::MakeTimeString(gameResult.mLocalPlayerTrackTimeMs).c_str());
    }

    int lapRecord = Game::TIME_UNUSED;
    if ( mStaticLapRecord )
    {
        const Highscores* lapHighscores = APP.GetGame()->GetCurrentLapHighscores();
        if ( lapHighscores )
        {
            lapRecord = lapHighscores->GetScore(0).mTime;
            mStaticLapRecord->setText(GuiHelper::MakeTimeString(lapRecord).c_str());
        }
    }

    int trackRecord = Game::TIME_UNUSED;
    if (   mStaticTimeTrackRecord
        && mStaticTrackRecord
        && mImageTrackRecord
        )
    {
        const Highscores* trackHighscores = APP.GetGame()->GetCurrentTrackHighscores();
        if ( trackHighscores )
        {
            trackRecord = trackHighscores->GetScore(0).mTime;
            mStaticTrackRecord->setVisible(true);
            mStaticTimeTrackRecord->setVisible(true);
            mImageTrackRecord->setVisible(true);
            mStaticTimeTrackRecord->setText(GuiHelper::MakeTimeString(trackRecord).c_str());
        }
        else
        {
            mStaticTrackRecord->setVisible(false);
            mStaticTimeTrackRecord->setVisible(false);
            mImageTrackRecord->setVisible(false);
        }
    }

    if (    mStaticGhostRecord
        &&  mStaticGhostModeRecord
        &&  mStaticGhostGapInfo
        &&  mStaticGhostGap
        &&  mStaticGhostGap
        &&  mImageGhostRecord
        &&  mImageGhostGap )
    {
        bool showGhostRecord = GHT_NONE != APP.GetGame()->GetSettings().mGhostType;

        mStaticGhostRecord->setVisible( showGhostRecord );
        mStaticGhostModeRecord->setVisible( showGhostRecord );
        mStaticGhostGapInfo->setVisible( showGhostRecord );
        mStaticGhostGap->setVisible( showGhostRecord );
        mImageGhostRecord->setVisible( showGhostRecord );
        mImageGhostGap->setVisible( showGhostRecord );

        if ( showGhostRecord )
        {
            const GameResult &gameResult = APP.GetGame()->GetResult();
            const GhostRecordSettings &ghostRecordSettings = APP.GetGame()->GetExportGhostRecordSettings();

            // SetTimeGhostRecord
            int recordTime = ghostRecordSettings.mTime;
            if ( GHT_LAP == APP.GetGame()->GetSettings().mGhostType )
            {
                recordTime = lapRecord;
            }
            else if ( GHT_TRACK == APP.GetGame()->GetSettings().mGhostType )
            {
                recordTime = trackRecord;
            }
            std::wstring strTimeRecord( GuiHelper::MakeTimeString(recordTime, false) );
            strTimeRecord += L" (";
            if (    GHT_EXPORTED_LAP == APP.GetGame()->GetSettings().mGhostType
                ||  GHT_LAP == APP.GetGame()->GetSettings().mGhostType )
            {
                strTimeRecord += APP.GetStringTable()->Get("id_score_lap").c_str();
            }
            else if (   GHT_EPORTED_TRACK== APP.GetGame()->GetSettings().mGhostType
                    ||  GHT_TRACK == APP.GetGame()->GetSettings().mGhostType )
            {
                strTimeRecord += APP.GetStringTable()->Get("id_score_track").c_str();
            }
            else
            {
                return;
            }
            strTimeRecord += L")";
            mStaticGhostModeRecord->setText( strTimeRecord.c_str() );

            // SetTimeGhostGap
            s32 ghostGapTime = Game::TIME_UNUSED;
            if (    GHT_EPORTED_TRACK== APP.GetGame()->GetSettings().mGhostType
                ||  GHT_TRACK == APP.GetGame()->GetSettings().mGhostType )
            {
                if (    gameResult.mLocalPlayerTrackTimeMs != Game::TIME_UNUSED
                    &&  recordTime != Game::TIME_UNUSED )
                {
                    ghostGapTime = gameResult.mLocalPlayerTrackTimeMs - recordTime;
                }
            }
            else if (   GHT_EXPORTED_LAP == APP.GetGame()->GetSettings().mGhostType
                    ||  GHT_LAP == APP.GetGame()->GetSettings().mGhostType )
            {
                if (    gameResult.mLocalPlayerBestLapTimeMs != Game::TIME_UNUSED
                    &&  recordTime != Game::TIME_UNUSED )
                {
                    ghostGapTime = gameResult.mLocalPlayerBestLapTimeMs - recordTime;
                }
            }
            std::wstring strTimeGap( GuiHelper::MakeTimeString(ghostGapTime, true) );
            mStaticGhostGap->setText( strTimeGap.c_str() );
        }
    }
}

void GuiMenuGameEnd::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;

    RemoveGuiEventFunctor( GetIdForName(std::string("id_hdrestart")) );
//    RemoveGuiEventFunctor( GetIdForName(std::string("id_hdwatchreplay")) );
//    RemoveGuiEventFunctor( GetIdForName(std::string("id_hdsavereplay")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_hdquit")) );
}

bool GuiMenuGameEnd::OnButtonRestart(const irr::SEvent &event_)
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

bool GuiMenuGameEnd::OnButtonWatchReplay(const irr::SEvent &event_)
{
    return false;
}

bool GuiMenuGameEnd::OnButtonSaveReplay(const irr::SEvent &event_)
{
    return false;
}

bool GuiMenuGameEnd::OnButtonQuit(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
		APP.AdvertismentCheck();
        GuiHelper::SetGameModeMenu(APP.GetGame()->GetSettings().mGameType);
    }

    return false;
}
