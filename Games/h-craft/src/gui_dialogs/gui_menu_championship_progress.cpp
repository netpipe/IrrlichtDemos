// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_championship_progress.h"
#include "gui_menu_main.h"
#include "gui_menu_loadingscreen.h"
#include "../gui.h"
#include "../championship.h"
#include "../profiles.h"
#include "../main.h"
#include "../level.h"
#include "../game.h"
#include "../string_table.h"
#include "../app_restrictions.h"
#include <assert.h>

using namespace irr;
using namespace gui;

namespace
{
	const video::SColor COLOR_SCORE0(255, 130, 130, 130);
	const video::SColor COLOR_SCORE1(255, 130, 130, 130);
	const video::SColor COLOR_SCORE2(255, 130, 130, 130);
	const video::SColor COLOR_SCORE3(255, 255, 255, 255);
	const video::SColor COLOR_SELECTED(255, 73, 211, 246);
};

GuiMenuChampionshipProgress::GuiMenuChampionshipProgress(const Config& config)
    : GuiDialog(config)
    , NUM_QUARTERS(12) // 3 seasons with 4 quarters
    , NUM_RACES(NUM_QUARTERS*3)
    , mButtonContinue(0)
    , mButtonQuit(0)
    , mTotalPoints(0)
    , mActiveProfile(0)
    , mButtonSeason1(0)
    , mButtonSeason2(0)
    , mButtonSeason3(0)
    , mSeason1(0)
    , mSeason2(0)
    , mSeason3(0)
    , mRetryRace(-1)
    , mNextRace(-1)
    , mActiveSeason(3)
{
    SetSuppressSceneRendering(true);
}

GuiMenuChampionshipProgress::~GuiMenuChampionshipProgress()
{
    RemoveFunctors();
}

bool GuiMenuChampionshipProgress::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("*** GuiMenuChampionshipProgress::Load");

        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));

        assert( APP.GetChampionship()->GetNumRequirements() == NUM_QUARTERS );
        if ( APP.GetChampionship()->GetNumRequirements() != NUM_QUARTERS )
            return false;

        for ( unsigned int q=0; q<NUM_QUARTERS; ++q )
        {
            char numStr[16];
            sprintf( numStr, "%d", q+1 );

            std::string strRequired("required");
            strRequired += numStr;
            mRequiredPoints.push_back( static_cast<IGUIStaticText*>(GetElementByName(root, strRequired, errorMsg)) );

            const CS_Requirement& requirement = APP.GetChampionship()->GetRequirement(q);
            core::stringw reqText( mRequiredPoints[q]->getText() );
            reqText += core::stringw(requirement.mPoints);
            mRequiredPoints[q]->setText( reqText.c_str() );

            std::string strResQuat("res_quat");
            strResQuat += numStr;
            mQuarterResults.push_back( static_cast<IGUIStaticText*>(GetElementByName(root, strResQuat, errorMsg)) );

            std::string strResText("restext");
            strResText += numStr;
            mQuarterResultTexts.push_back( static_cast<IGUIStaticText*>(GetElementByName(root, strResText, errorMsg)) );
        }

        for ( unsigned int r=0; r<NUM_RACES; ++r )
        {
            char numStr[8];
            sprintf( numStr, "%d", r+1 );

            std::string strRaceRes("race_res");
            strRaceRes += numStr;
            mRaceResults.push_back( static_cast<IGUIStaticText*>(GetElementByName(root, strRaceRes, errorMsg)) );

            std::string strRaceName("race_name");
            strRaceName += numStr;
            mRaceNames.push_back( static_cast<IGUIStaticText*>(GetElementByName(root, strRaceName, errorMsg)) );

            std::string strRetry("retry");
            strRetry += numStr;
            mRetryButtons.push_back( static_cast<IGUIButton*>(GetElementByName(root, strRetry, errorMsg)) );
            AddGuiEventFunctor( GetIdForName(strRetry), new EventFunctor<GuiMenuChampionshipProgress>(this, &GuiMenuChampionshipProgress::OnButtonRetry) );
        }

        mSeason1 = static_cast<IGUIStaticText*>(GetElementByName(root, "season1", errorMsg));
        mSeason2 = static_cast<IGUIStaticText*>(GetElementByName(root, "season2", errorMsg));
        mSeason3 = static_cast<IGUIStaticText*>(GetElementByName(root, "season3", errorMsg));
        if ( !mSeason1 || !mSeason2 || !mSeason3 )
		{
			mActiveSeason = 0;	// Backward compatibility to old dialog
		}
		else	// careful - those elements exist, but are not buttons in old dialog! (yeah, one case where rtti would be nice)
		{
			mButtonSeason1 = static_cast<IGUIButton*>(GetElementByName(root, "1ch_title", errorMsg));
			mButtonSeason2 = static_cast<IGUIButton*>(GetElementByName(root, "2ch_title", errorMsg));
			mButtonSeason3 = static_cast<IGUIButton*>(GetElementByName(root, "3ch_title", errorMsg));
		}

        mTotalPoints = static_cast<IGUIStaticText*>(GetElementByName(root, "ch_totalpoints", errorMsg));
        mButtonContinue = static_cast<IGUIButton*>(GetElementByName(root, "id_continue", errorMsg));
        mButtonQuit = static_cast<IGUIButton*>(GetElementByName(root, "id_quit", errorMsg));

        SetAlternateDefaultFocus(mButtonQuit);

        AddGuiEventFunctor( GetIdForName(std::string("id_quit")), new EventFunctor<GuiMenuChampionshipProgress>(this, &GuiMenuChampionshipProgress::OnButtonQuit) );
        AddGuiEventFunctor( GetIdForName(std::string("id_continue")), new EventFunctor<GuiMenuChampionshipProgress>(this, &GuiMenuChampionshipProgress::OnButtonContinue) );

        if ( mButtonSeason1 )
			AddGuiEventFunctor( GetIdForName(std::string("1ch_title")), new EventFunctor<GuiMenuChampionshipProgress>(this, &GuiMenuChampionshipProgress::OnButtonSeason1) );
		if ( mButtonSeason2 )
			AddGuiEventFunctor( GetIdForName(std::string("2ch_title")), new EventFunctor<GuiMenuChampionshipProgress>(this, &GuiMenuChampionshipProgress::OnButtonSeason2) );
		if ( mButtonSeason3 )
			AddGuiEventFunctor( GetIdForName(std::string("3ch_title")), new EventFunctor<GuiMenuChampionshipProgress>(this, &GuiMenuChampionshipProgress::OnButtonSeason3) );
    }
    return ok;
}

int GuiMenuChampionshipProgress::FindNextRace(PlayerProfile* profile) const
{
    if ( profile )
    {
		assert( profile->mChampionship.GetNumRaceResults() == NUM_RACES );
		int nextRace = profile->mChampionship.GetNextRaceNeeded();
		if ( nextRace >= (int)APP.GetChampionship()->GetNumRaces() )
			return -1;

		CS_RaceResult raceResult = profile->mChampionship.GetRaceResult(nextRace);
		if ( raceResult.mIsPlayed )
			return -1;

		return nextRace;
    }

    return -1;
}

void GuiMenuChampionshipProgress::Show()
{
    GuiDialog::Show();

    if ( mRaceResults.empty() || mRaceNames.empty() || mRetryButtons.empty() )
		return;

    PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
    SetNextRace(FindNextRace(profile));
    SetRetryRace(-1);

    if (!profile )
        return;

    if (    mRaceResults.size() == NUM_RACES	// just checking if already got the races at this point
        &&  mRaceNames.size() == NUM_RACES
        &&  mRetryButtons.size() == NUM_RACES )
    {
        for ( unsigned int r=0; r<NUM_RACES; ++r )
        {
            CS_RaceResult raceResult = profile->mChampionship.GetRaceResult(r);

            bool enableRace = raceResult.mIsOpen;
            bool enableRetry = raceResult.mIsPlayed && raceResult.mIsOpen;
            mRaceResults[r]->setEnabled( enableRace );
            mRaceNames[r]->setEnabled( enableRace );

            if (enableRace )
		    {
		    	if ( mNextRace == (int)r )
				{
					mRaceResults[r]->setOverrideColor( COLOR_SELECTED );
				}
				else
				{
					switch ( raceResult.mScore )
					{
						case 0: mRaceResults[r]->setOverrideColor( COLOR_SCORE0 ); break;
						case 1: mRaceResults[r]->setOverrideColor( COLOR_SCORE1 ); break;
						case 2: mRaceResults[r]->setOverrideColor( COLOR_SCORE2 ); break;
						case 3: mRaceResults[r]->setOverrideColor( COLOR_SCORE3 ); break;
					}
				}
		    }
		    else
		    {
		        mRaceResults[r]->enableOverrideColor(false);
		    }

            if ( IS_DEMO_VERSION )
            {
                if ( r >= 3 && r != 18 )
                {
                    mRaceNames[r]->setText( APP.GetStringTable()->Get("id_demolocked").c_str() );
                }
            }

            mRetryButtons[r]->setEnabled( enableRetry );
            mRetryButtons[r]->setVisible( enableRetry );
            if ( profile )
            {
                if ( !IS_DEMO_VERSION || r < 3 )
                {
                    mRetryButtons[r]->setEnabled( profile->mChampionship.IsQuarterPlayedComplete(r / 3) );
                    mRetryButtons[r]->setVisible( profile->mChampionship.IsQuarterPlayedComplete(r / 3) );
                }
            }

			core::stringw resultTxt(raceResult.mScore);
			resultTxt += L"/3";
            mRaceResults[r]->setText( resultTxt.c_str() );
        }
    }

    if ( mRequiredPoints.size() == NUM_QUARTERS
        && mQuarterResults.size() == NUM_QUARTERS
        && mQuarterResultTexts.size() == NUM_QUARTERS )
    {
        for ( unsigned int q=0; q<NUM_QUARTERS; ++q )
        {
			core::stringw resultTxt(profile->mChampionship.GetScoreSumForQuarter(q));
			resultTxt += L'/';
			resultTxt += core::stringw(profile->mChampionship.GetMaxScoreForQuarter(q));
            mQuarterResults[q]->setText( resultTxt.c_str() );
            mQuarterResults[q]->setEnabled( profile->mChampionship.IsQuarterOpen(q) );
            mQuarterResultTexts[q]->setEnabled( profile->mChampionship.IsQuarterOpen(q) );

            /*	TODO: The "level required" part should be removed, but currently that's one string and it's filled on loading
            together with the quarter information, so can't kick it out.
            if ( profile->mChampionship.IsQuarterOpen(q) )
                mRequiredPoints[q]->setVisible(false);
            else
                mRequiredPoints[q]->setVisible(true);
               */
        }
    }

    if ( mTotalPoints )
    {
        core::stringw wTotalScore( APP.GetStringTable()->Get("id_ch_totalpoints") );
        wTotalScore += core::stringw(profile ? profile->mChampionship.GetTotalScore() : L'0');
        wTotalScore += L'/';
        wTotalScore += core::stringw(APP.GetChampionship()->GetMaxScore());

        mTotalPoints->setText( wTotalScore.c_str() );
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

    ShowActiveSeason();
}

void GuiMenuChampionshipProgress::ShowActiveSeason()
{
	if ( mButtonSeason1 )
	{
		mButtonSeason1->setPressed(mActiveSeason == 1 || mActiveSeason == 0);
//		mButtonSeason1->setOverrideColor( COLOR_SELECTED ); // TODO: Irrlicht does not have yet for buttons
	}
	if ( mButtonSeason2 )
	{
		mButtonSeason2->setPressed(mActiveSeason == 2 || mActiveSeason == 0);
	}
	if ( mButtonSeason3 )
	{
		mButtonSeason3->setPressed(mActiveSeason == 3 || mActiveSeason == 0);
	}
	if ( mSeason1 )
		mSeason1->setVisible(mActiveSeason == 1 || mActiveSeason == 0);
	if ( mSeason2 )
		mSeason2->setVisible(mActiveSeason == 2 || mActiveSeason == 0);
	if ( mSeason3 )
		mSeason3->setVisible(mActiveSeason == 3 || mActiveSeason == 0);
}

void GuiMenuChampionshipProgress::SetNextRace(int index )
{
	if  (mNextRace >= 0 )
	{
        mRaceNames[mNextRace]->enableOverrideColor(false);
	}

	mNextRace = index;

    if ( mButtonContinue )
    {
        mButtonContinue->setEnabled(mNextRace >= 0);
    }

    if ( mNextRace >= 0 )
    {
		mRaceNames[mNextRace]->setOverrideColor( COLOR_SELECTED );

		if ( mNextRace >= 24 )
			mActiveSeason = 3;
		else if ( mNextRace >= 12 )
			mActiveSeason = 2;
		else
			mActiveSeason = 1;
	}
}

void GuiMenuChampionshipProgress::SetRetryRace( int selected_ )
{
    if ( mRetryRace >= 0 )
    {
        mRaceResults[mRetryRace]->enableOverrideColor(false);
        mRaceNames[mRetryRace]->enableOverrideColor(false);
    }

    mRetryRace = selected_;

	if ( mRetryRace >= 0 )
	{
		mRaceResults[mRetryRace]->setOverrideColor( COLOR_SELECTED );
		mRaceNames[mRetryRace]->setOverrideColor( COLOR_SELECTED );
	}
}

void GuiMenuChampionshipProgress::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;

    for ( unsigned int r = 0; r < mRetryButtons.size(); ++r )
    {
        char numStr[8];
        sprintf( numStr, "%d", r+1 );
        std::string strRetry("retry");
        strRetry += numStr;
        RemoveGuiEventFunctor( GetIdForName(strRetry) );
    }

    RemoveGuiEventFunctor( GetIdForName(std::string("id_quit")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_continue")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("1ch_title")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("2ch_title")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("3ch_title")) );
}

bool GuiMenuChampionshipProgress::OnButtonRetry(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        for ( unsigned int i=0; i<mRetryButtons.size(); ++i)
        {
            if ( event_.GUIEvent.Caller == mRetryButtons[i] )
            {
                APP.GetGui()->PlayOverrideClickSound(GS_CLICK_OK);
                APP.GetChampionship()->SetRaceSelected(i);
                SetRetryRace(i);
				StartSelected();
                return false;
            }
        }
    }

    return false;
}

bool GuiMenuChampionshipProgress::OnButtonContinue(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        if ( mNextRace < 0 )
            return false;

        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_OK);

        APP.GetChampionship()->SetRaceSelected( mNextRace );

        StartSelected();
    }

    return false;
}

bool GuiMenuChampionshipProgress::OnButtonSeason1(const irr::SEvent &event_)
{
	if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
	{
		mActiveSeason = 1;
		ShowActiveSeason();
	}
	return false;
}

bool GuiMenuChampionshipProgress::OnButtonSeason2(const irr::SEvent &event_)
{
	if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
	{
		mActiveSeason = 2;
		ShowActiveSeason();
	}
	return false;
}

bool GuiMenuChampionshipProgress::OnButtonSeason3(const irr::SEvent &event_)
{
	if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
	{
		mActiveSeason = 3;
		ShowActiveSeason();
	}
	return false;
}

bool GuiMenuChampionshipProgress::OnButtonQuit(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuMain() );
    }

    return false;
}

void GuiMenuChampionshipProgress::StartSelected()
{
    const CS_Race &race = APP.GetChampionship()->GetRace( APP.GetChampionship()->GetRaceSelected() );
    const LevelSettings& levelSettings = APP.GetLevelManager()->GetLevel( APP.GetLevelManager()->GetIndexForId(race.mLevelId) );

    GameSettings gameSettings;
    gameSettings.mGameType = GT_CHAMPIONSHIP;
    gameSettings.mLevelName = levelSettings.mName;
    if ( race.mType == CSRT_QUALIFICATION )
    {
        gameSettings.mNrOfBots = 0;
        gameSettings.mTargetTime = (u32)(race.mTime1Seconds * 1000);
    }
    else if ( race.mType == CSRD_RACE )
    {
        gameSettings.mNrOfBots = 3;
        gameSettings.mTargetTime = 0;
    }
    else
    {
        gameSettings.mNrOfBots = 0;
    }
    gameSettings.mNrOfLaps = race.mLaps;
    APP.GetGame()->SetSettings(gameSettings);

    APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuLoadingScreen() );
}
