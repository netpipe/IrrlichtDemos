// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_rivals_score.h"
#include "gui_menu_rivals.h"
#include "gui_menu_loadingscreen.h"
#include "gui_helper.h"
#include "gui_dlg_okcancel.h"
#include "../main.h"
#include "../game.h"
#include "../gui.h"
#include "../profiles.h"
#include "../rivalsmode.h"
#include "../string_table.h"

using namespace irr;
using namespace gui;

GuiMenuRivalsScore::GuiMenuRivalsScore(const Config& config)
    : GuiDialog(config)
    , mButtonStart(0)
    , mButtonExit(0)
    , mStaticWinner(0)
    , mStaticBestTimeSum(0)
    , mStaticRaceCounter(0)
    , mStaticRaceCounterLabel(0)
    , mStaticCurrentPlayerName(0)
    , mStaticCurrentPlayerNameLabel(0)
    , mStaticCurrentTrackName(0)
    , mStaticCurrentTrackNameLabel(0)
{
    SetSuppressSceneRendering(true);
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        mStaticName[i] = NULL;
        mStaticTime[i] = NULL;
        mStaticTimeSum[i] = NULL;
        mStaticScore[i] = NULL;
    }
}

GuiMenuRivalsScore::~GuiMenuRivalsScore()
{
    RemoveFunctors();
}

bool GuiMenuRivalsScore::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_start")), new EventFunctor<GuiMenuRivalsScore>(this, &GuiMenuRivalsScore::OnButtonStart) );
        AddGuiEventFunctor( GetIdForName(std::string("id_exit")), new EventFunctor<GuiMenuRivalsScore>(this, &GuiMenuRivalsScore::OnButtonExit) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;
        std::string errorMsg("GuiMenuRivalsScore::Load");

        mButtonStart = static_cast<IGUIButton*>(GetElementByName(root, "id_start", errorMsg));
        mButtonExit = static_cast<IGUIButton*>(GetElementByName(root, "id_exit", errorMsg));

        for ( int i=0; i < MAX_RIVALS; ++i )
        {
            char numStr[8];
            sprintf( numStr, "%d", i+1 );

            std::string strName("player");
            strName += numStr;
            mStaticName[i] = static_cast<IGUIStaticText*>(GetElementByName(root, strName.c_str(), errorMsg));
            std::string strTime("time_p");
            strTime += numStr;
            mStaticTime[i] = static_cast<IGUIStaticText*>(GetElementByName(root, strTime.c_str(), errorMsg));
            std::string strTimeSum("timesum_p");
            strTimeSum += numStr;
            mStaticTimeSum[i] = static_cast<IGUIStaticText*>(GetElementByName(root, strTimeSum.c_str(), errorMsg));
            std::string strScore("score_p");
            strScore += numStr;
            mStaticScore[i] = static_cast<IGUIStaticText*>(GetElementByName(root, strScore.c_str(), errorMsg));
        }

        mStaticWinner = static_cast<IGUIStaticText*>(GetElementByName(root, "rivals_winner", errorMsg));
        mStaticBestTimeSum = static_cast<IGUIStaticText*>(GetElementByName(root, "rivals_besttime", errorMsg));
        mStaticRaceCounter = static_cast<IGUIStaticText*>(GetElementByName(root, "racecounter", errorMsg));
        mStaticRaceCounterLabel = static_cast<IGUIStaticText*>(GetElementByName(root, "id_currentrace", errorMsg));
        mStaticCurrentPlayerName = static_cast<IGUIStaticText*>(GetElementByName(root, "playername", errorMsg));
        mStaticCurrentPlayerNameLabel = static_cast<IGUIStaticText*>(GetElementByName(root, "id_currentplayer", errorMsg));
        mStaticCurrentTrackName = static_cast<IGUIStaticText*>(GetElementByName(root, "trackname", errorMsg));
        mStaticCurrentTrackNameLabel = static_cast<IGUIStaticText*>(GetElementByName(root, "id_currenttrack", errorMsg));

        SetAlternateDefaultFocus( mButtonExit );
    }
    return ok;
}

void GuiMenuRivalsScore::RemoveFunctors()
{
    if ( !IsLoaded() )
    {
        return;
    }
    RemoveGuiEventFunctor( GetIdForName(std::string("id_start")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_exit")) );
}

void GuiMenuRivalsScore::Show()
{
    GuiDialog::Show();

    RivalsMode * rivalsMode = APP.GetRivalsMode();
    if ( !rivalsMode )
        return;

    if ( !rivalsMode->GetRaces().size() )
        return;

    RivalsPlayer* currentPlayer = rivalsMode->GetPlayer(rivalsMode->GetCurrentPlayer());
    if ( !rivalsMode->GetIsTournamentFinished() )
    {
        if ( currentPlayer )
        {
            APP.GetProfileManager()->SelectProfile(currentPlayer->mProfileId);
        }
    }

    RivalsRace race = rivalsMode->GetRaces()[rivalsMode->GetCurrentRace()];
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        RivalsPlayer* player = rivalsMode->GetPlayer(i);
        bool playerValid = player->mProfileId >= 0 ? true : false;
        mStaticName[i]->setVisible(playerValid);
        mStaticTime[i]->setVisible(playerValid);
        mStaticTimeSum[i]->setVisible(playerValid);
        mStaticScore[i]->setVisible(playerValid);
        if ( playerValid )
        {
            PlayerProfile* profile = APP.GetProfileManager()->GetProfile(player->mProfileId);

            mStaticName[i]->setText(profile->mName.c_str());
            mStaticTime[i]->setText( GuiHelper::MakeTimeString(race.mTimeResults[i]).c_str() );
            mStaticTimeSum[i]->setText( GuiHelper::MakeTimeString(player->mSumTime).c_str() );

			core::stringw scoreTxt(player->mSumScore);
            mStaticScore[i]->setText(scoreTxt.c_str());
        }
    }

    if ( rivalsMode->GetIsTournamentFinished() )
    {
        mButtonStart->setEnabled(false);

        RivalsPlayer* winner = rivalsMode->GetWinnerPlayer();
        RivalsPlayer* bestTime = rivalsMode->GetBestTimeSumPlayer();
        PlayerProfile* profile = NULL;

        if ( winner )
        {
            mStaticWinner->setVisible(true);
            profile = APP.GetProfileManager()->GetProfile(winner->mProfileId);
            core::stringw strWinner( profile->mName.c_str() );
            strWinner += L' ';
            strWinner += APP.GetStringTable()->Get("id_win_tournament");
            mStaticWinner->setText(strWinner.c_str());
        }
        else
        {
            mStaticWinner->setVisible(false);
        }
        if ( bestTime )
        {
            mStaticBestTimeSum->setVisible(true);
            profile = APP.GetProfileManager()->GetProfile(bestTime->mProfileId);
            core::stringw strTime(profile->mName.c_str());
            strTime += L' ';
            strTime += APP.GetStringTable()->Get("id_best_time");
            mStaticBestTimeSum->setText(strTime.c_str());
        }
        else
        {
            mStaticBestTimeSum->setVisible(false);
        }
    }
    else
    {
        mButtonStart->setEnabled(true);
        mStaticWinner->setVisible(false);
        mStaticBestTimeSum->setVisible(false);
    }

    if ( !rivalsMode->GetIsTournamentFinished() )
    {
        mStaticRaceCounter->setVisible(true);
        mStaticRaceCounterLabel->setVisible(true);
        mStaticCurrentPlayerName->setVisible(true);
        mStaticCurrentPlayerNameLabel->setVisible(true);
        mStaticCurrentTrackName->setVisible(true);
        mStaticCurrentTrackNameLabel->setVisible(true);

		core::stringw racesTxt(rivalsMode->GetCurrentRace()+1);
		racesTxt += L"/";
		racesTxt += core::stringw((s32)rivalsMode->GetRaces().size());
        mStaticRaceCounter->setText(racesTxt.c_str());

        if ( currentPlayer )
        {
            PlayerProfile* currentProfile = APP.GetProfileManager()->GetProfile(currentPlayer->mProfileId);
            mStaticCurrentPlayerName->setText(currentProfile->mName.c_str());
        }

        const LevelSettings& levelSetting = APP.GetLevelManager()->GetLevel(race.mLevelIndex);
        mStaticCurrentTrackName->setText(levelSetting.mName.c_str());
    }
    else
    {
        mStaticRaceCounter->setVisible(false);
        mStaticRaceCounterLabel->setVisible(false);
        mStaticCurrentPlayerName->setVisible(false);
        mStaticCurrentPlayerNameLabel->setVisible(false);
        mStaticCurrentTrackName->setVisible(false);
        mStaticCurrentTrackNameLabel->setVisible(false);
    }
}

bool GuiMenuRivalsScore::OnButtonStart(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != EGET_BUTTON_CLICKED )
        return false;

    // next race settings
    GameSettings gameSettings;
    APP.GetRivalsMode()->GetGameSettingsForRace( APP.GetRivalsMode()->GetCurrentRace(), gameSettings);
    APP.GetGame()->SetSettings(gameSettings);

    APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuLoadingScreen() );

    return false;
}

bool GuiMenuRivalsScore::OnExitAcknowledged(const irr::SEvent &event_)
{
    APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
    APP.AdvertismentCheck();
    APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuRivals() );

    return false;
}

bool GuiMenuRivalsScore::OnExitCanceled(const irr::SEvent &event_)
{
    SetActiveFocus();

    return false;
}

bool GuiMenuRivalsScore::OnButtonExit(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != EGET_BUTTON_CLICKED )
        return false;

    if ( APP.GetRivalsMode()->GetIsTournamentFinished() )
    {
        return OnExitAcknowledged(event_);
    }
    else
    {
        GuiDlgOkCancel * dlg = static_cast<GuiDlgOkCancel*>(APP.GetGui()->GetGuiDlgOkCancel());
        dlg->SetInfoText( APP.GetStringTable()->Get("id_end_challenge").c_str() );
        dlg->SetOkFunctor( new EventFunctor<GuiMenuRivalsScore>(this, &GuiMenuRivalsScore::OnExitAcknowledged) );
        dlg->SetCancelFunctor(new EventFunctor<GuiMenuRivalsScore>(this, &GuiMenuRivalsScore::OnExitCanceled) );
        APP.GetGui()->SetModalDialog( dlg );
    }

    return false;
}
