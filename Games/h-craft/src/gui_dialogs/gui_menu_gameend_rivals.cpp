// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_gameend_rivals.h"
#include "gui_helper.h"
#include "../gui.h"
#include "../main.h"
#include "../rivalsmode.h"
#include "../profiles.h"
#include "../game.h"
#include "../string_table.h"
#include "../highscores.h"

using namespace irr;
using namespace gui;

GuiMenuGameEndRivals::GuiMenuGameEndRivals(const Config& config)
: GuiDialog(config)
, mStaticTimeCurrentLap(0)
, mStaticBestLap(0)
, mStaticTrack(0)
, mStaticTrackRecord(0)
, mStaticTimeTrackRecord(0)
, mStaticLapRecord(0)
, mStaticLapCount(0)
, mStaticPosition(0)
, mStaticTrackName(0)
, mStaticSessionPlayerName(0)
, mStaticSessionModeRecord(0)
, mStaticSessionGap(0)
{
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        mStaticTime[i] = NULL;
        mStaticResult[i] = NULL;
    }
}

GuiMenuGameEndRivals::~GuiMenuGameEndRivals()
{
    RemoveFunctors();
}

bool GuiMenuGameEndRivals::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_hdcontinue")), new EventFunctor<GuiMenuGameEndRivals>(this, &GuiMenuGameEndRivals::OnButtonContinue) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;
        std::string errorMsg("GuiMenuGameEndRivals::Load");

        mStaticTimeCurrentLap = static_cast<IGUIStaticText*>(GetElementByName(root, "timeCurLap", errorMsg));
        mStaticBestLap = static_cast<IGUIStaticText*>(GetElementByName(root, "timeBestLap", errorMsg));
        mStaticTrack = static_cast<IGUIStaticText*>(GetElementByName(root, "timeTrack", errorMsg));
        mStaticTrackRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "trackrecord", errorMsg));
        mStaticTimeTrackRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "timeTrackRecord", errorMsg));
        mStaticLapRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "timeLapRecord", errorMsg));
        mStaticLapCount = static_cast<IGUIStaticText*>(GetElementByName(root, "lapcount", errorMsg));
        mStaticPosition = static_cast<IGUIStaticText*>(GetElementByName(root, "position", errorMsg));
        mStaticTrackName = static_cast<IGUIStaticText*>(GetElementByName(root, "id_trackname", errorMsg));
        mStaticSessionPlayerName = static_cast<IGUIStaticText*>(GetElementByName(root, "playername", errorMsg));
        mStaticSessionModeRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "session_ModeRecord", errorMsg));
        mStaticSessionGap = static_cast<IGUIStaticText*>(GetElementByName(root, "session_gap", errorMsg));

        for ( int i=0; i < MAX_RIVALS; ++i )
        {
            char numStr[8];
            sprintf( numStr, "%d", i+1 );

            std::string strTime("time_p");
            strTime += numStr;
            mStaticTime[i] = static_cast<IGUIStaticText*>(GetElementByName(root, strTime.c_str(), errorMsg));

            std::string strResult("result_p");
            strResult += numStr;
            mStaticResult[i] = static_cast<IGUIStaticText*>(GetElementByName(root, strResult.c_str(), errorMsg));
        }
    }
    return ok;
}

void GuiMenuGameEndRivals::Show()
{
    GuiDialog::Show();

    RivalsMode * rivalsMode = APP.GetRivalsMode();
    if ( !rivalsMode )
        return;

    if ( !rivalsMode->GetRaces().size() )
        return;

    if ( rivalsMode->GetCurrentRace() >= (int)rivalsMode->GetRaces().size() )
        return;

    RivalsRace race = rivalsMode->GetRaces()[rivalsMode->GetCurrentRace()];
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        RivalsPlayer* player = rivalsMode->GetPlayer(i);
        bool playerValid = player->mProfileId >= 0 ? true : false;
        if ( mStaticTime[i] )
        {
            mStaticTime[i]->setVisible(playerValid);
            if ( playerValid )
            {
                mStaticTime[i]->setText( GuiHelper::MakeTimeString(race.mTimeResults[i]).c_str() );
            }
        }
        if ( mStaticResult[i] )
        {
            PlayerProfile* profile = APP.GetProfileManager()->GetProfile(player->mProfileId);
            mStaticResult[i]->setVisible(playerValid);
            if ( playerValid && profile )
            {
                // build dat string: "Px Profilename :"
                core::stringw wstr(L"P");
                wstr += core::stringw(i+1);
                wstr += L' ';
                wstr += profile->mName.c_str();
                wstr += L" :";
                mStaticResult[i]->setText( wstr.c_str() );
            }
        }
    }

    if (    mStaticTimeCurrentLap
        &&  mStaticBestLap
        &&  mStaticTrack
        &&  mStaticLapCount
        &&  mStaticPosition )
    {
        const GameResult &gameResult = APP.GetGame()->GetResult();
        const GameSettings& gameSettings = APP.GetGame()->GetSettings();

        mStaticPosition->setVisible(false);
        mStaticTimeCurrentLap->setText(GuiHelper::MakeTimeString(gameResult.mLocalPlayerLastLapTimeMs).c_str());
        mStaticBestLap->setText(GuiHelper::MakeTimeString(gameResult.mLocalPlayerBestLapTimeMs).c_str());
        mStaticTrack->setText(GuiHelper::MakeTimeString(gameResult.mLocalPlayerTrackTimeMs).c_str());

        int numLaps = gameResult.mLocalPlayerNumLaps;
        if ( numLaps > gameSettings.mNrOfLaps)
            numLaps = gameSettings.mNrOfLaps;

        core::stringw editText(APP.GetStringTable()->Get("id_hdlapcount"));
        editText += L' ';
        editText += core::stringw(numLaps);
        editText += L" / ";
        editText += core::stringw(gameSettings.mNrOfLaps);
        mStaticLapCount->setText(editText.c_str());
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
        && mStaticTrackRecord )
    {
        const Highscores* trackHighscores = APP.GetGame()->GetCurrentTrackHighscores();
        if ( trackHighscores )
        {
            int trackRecord = trackHighscores->GetScore(0).mTime;
            mStaticTrackRecord->setVisible(true);
            mStaticTimeTrackRecord->setVisible(true);
            mStaticTimeTrackRecord->setText(GuiHelper::MakeTimeString(trackRecord).c_str());
        }
        else
        {
            mStaticTrackRecord->setVisible(false);
            mStaticTimeTrackRecord->setVisible(false);
        }
    }

    if ( mStaticSessionPlayerName && mStaticSessionModeRecord && mStaticSessionGap)
    {
        std::wstring strTime( GuiHelper::MakeTimeString(APP.GetGame()->GetSessionTrackRecord(), false) );
        strTime += L" (";
        strTime += APP.GetStringTable()->Get("id_score_track").c_str();
        strTime += L")";
        mStaticSessionModeRecord->setText( strTime.c_str() );

        RivalsPlayer* currentPlayer = rivalsMode->GetPlayer(rivalsMode->GetCurrentPlayer());
        if ( currentPlayer )
        {
            PlayerProfile* currentProfile = APP.GetProfileManager()->GetProfile(currentPlayer->mProfileId);
            mStaticSessionPlayerName->setText(currentProfile->mName.c_str());
        }
        else
        {
            mStaticSessionPlayerName->setText(L"");
        }

        if (    APP.GetGame()->GetSessionTrackRecord() == 0
            ||  APP.GetGame()->GetSessionTrackRecord() == Game::TIME_UNUSED )
        {
            mStaticSessionGap->setText( GuiHelper::MakeTimeString(Game::TIME_UNUSED).c_str() );
        }
        else
        {
            const GameResult &gameResult = APP.GetGame()->GetResult();
            s32 time = gameResult.mLocalPlayerTrackTimeMs - APP.GetGame()->GetSessionTrackRecord();
            mStaticSessionGap->setText( GuiHelper::MakeTimeString(time).c_str() );
        }
    }

    if ( mStaticTrackName )
    {
        const LevelSettings& levelSetting = APP.GetLevelManager()->GetLevel(race.mLevelIndex);
        mStaticTrackName->setText(levelSetting.mName.c_str());
    }
}

void GuiMenuGameEndRivals::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;

    RemoveGuiEventFunctor( GetIdForName(std::string("id_hdcontinue")) );
}

bool GuiMenuGameEndRivals::OnButtonContinue(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != EGET_BUTTON_CLICKED )
        return false;

    APP.GetRivalsMode()->InfoContinue();

    GuiHelper::SetGameModeMenu(APP.GetGame()->GetSettings().mGameType);

    return false;
}
