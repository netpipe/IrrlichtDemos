// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_loadingscreen.h"
#include "gui_menu_rivals_score.h"
#include "gui_helper.h"
#include "../gui.h"
#include "../game.h"
#include "../championship.h"
#include "../main.h"
#include "../string_table.h"
#include "../rivalsmode.h"
#include "../profiles.h"
#include "../logging.h"

using namespace irr;
using namespace gui;

GuiMenuLoadingScreen::GuiMenuLoadingScreen(const Config& config)
    : GuiDialog(config)
    , mLoadingMode(MLM_DEFAULT)
    , mTrackName(0)
    , mLaps(0)
    , mOpponents(0)
    , mTime(0)
    , mStaticLoading(0)
    , mPlayerInfo(0)
    , mButtonStart(0)
    , mImageDefault(0)
    , mImageTutorial(0)
    , mStaticTutorialOne1(0)
    , mStaticTutorialOne2(0)
    , mStaticTutorialOne2b(0)
    , mStaticTutorialOne3(0)
    , mStaticTutorialTwo1(0)
    , mStaticTutorialTwo2(0)
    , mStaticTutorialTwo3(0)
{
}

GuiMenuLoadingScreen::~GuiMenuLoadingScreen()
{
    RemoveFunctors();
}

bool GuiMenuLoadingScreen::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_start")), new EventFunctor<GuiMenuLoadingScreen>(this, &GuiMenuLoadingScreen::OnButtonStart) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuLoadingScreen::Load");

        mTrackName = static_cast<IGUIStaticText*>(GetElementByName(root, "track", errorMsg));
        mLaps = static_cast<IGUIStaticText*>(GetElementByName(root, "laps", errorMsg));
        mOpponents = static_cast<IGUIStaticText*>(GetElementByName(root, "opponents", errorMsg));
        mTime = static_cast<IGUIStaticText*>(GetElementByName(root, "time", errorMsg));
        mStaticLoading = static_cast<IGUIStaticText*>(GetElementByName(root, "loading", errorMsg));
        mPlayerInfo = static_cast<IGUIStaticText*>(GetElementByName(root, "player_info", errorMsg));
        mButtonStart = static_cast<IGUIButton*>(GetElementByName(root, "id_start", errorMsg));

        mImageDefault = static_cast<IGUIImage*>(GetElementByName(root, "img1", errorMsg));
        mImageTutorial = static_cast<IGUIImage*>(GetElementByName(root, "img_tutorial", errorMsg));

        mStaticTutorialOne1 = static_cast<IGUIStaticText*>(GetElementByName(root, "tut01_text01", errorMsg));
        mStaticTutorialOne2 = static_cast<IGUIStaticText*>(GetElementByName(root, "tut01_text02", errorMsg));
        mStaticTutorialOne2b = static_cast<IGUIStaticText*>(GetElementByName(root, "tut01_text02B", errorMsg));
        mStaticTutorialOne3 = static_cast<IGUIStaticText*>(GetElementByName(root, "tut01_text03", errorMsg));
        mStaticTutorialTwo1 = static_cast<IGUIStaticText*>(GetElementByName(root, "tut02_text01", errorMsg));
        mStaticTutorialTwo2 = static_cast<IGUIStaticText*>(GetElementByName(root, "tut02_text02", errorMsg));
        mStaticTutorialTwo3 = static_cast<IGUIStaticText*>(GetElementByName(root, "tut02_text03", errorMsg));
    }

    return ok;
}

void GuiMenuLoadingScreen::RemoveFunctors()
{
    if ( !IsLoaded() )
    {
        return;
    }
    RemoveGuiEventFunctor( GetIdForName(std::string("id_start")) );
}

void GuiMenuLoadingScreen::ShowLoadingMode()
{
    if (    !mStaticTutorialOne1
        ||  !mStaticTutorialOne2
        ||  !mStaticTutorialOne2b
        ||  !mStaticTutorialOne3
        ||  !mStaticTutorialTwo1
        ||  !mStaticTutorialTwo2
        ||  !mStaticTutorialTwo3
        ||  !mImageDefault
        ||  !mImageTutorial
        )
    {
        return;
    }

    switch ( mLoadingMode )
    {
        case MLM_DEFAULT:
            mImageDefault->setVisible(true);
            mImageTutorial->setVisible(false);
            mStaticTutorialOne1->setVisible(false);
            mStaticTutorialOne2->setVisible(false);
            mStaticTutorialOne2b->setVisible(false);
            mStaticTutorialOne3->setVisible(false);
            mStaticTutorialTwo1->setVisible(false);
            mStaticTutorialTwo2->setVisible(false);
            mStaticTutorialTwo3->setVisible(false);
        break;
        case MLM_TUTORIAL1:
            mImageDefault->setVisible(false);
            mImageTutorial->setVisible(true);
            mStaticTutorialOne1->setVisible(true);
            mStaticTutorialOne2->setVisible(true);
            mStaticTutorialOne2b->setVisible(true);
            mStaticTutorialOne3->setVisible(true);
            mStaticTutorialTwo1->setVisible(false);
            mStaticTutorialTwo2->setVisible(false);
            mStaticTutorialTwo3->setVisible(false);
        break;
        case MLM_TUTORIAL2:
            mImageDefault->setVisible(false);
            mImageTutorial->setVisible(true);
            mStaticTutorialOne1->setVisible(false);
            mStaticTutorialOne2->setVisible(false);
            mStaticTutorialOne2b->setVisible(false);
            mStaticTutorialOne3->setVisible(false);
            mStaticTutorialTwo1->setVisible(true);
            mStaticTutorialTwo2->setVisible(true);
            mStaticTutorialTwo3->setVisible(true);
        break;
    }
}

void GuiMenuLoadingScreen::Show()
{
    GuiDialog::Show();

    if ( !mTrackName || !mLaps || !mOpponents || !mTime || !mStaticLoading )
        return;

    if ( !APP.GetGame() || !APP.GetIrrlichtManager() )
        return;
    const GameSettings& gameSettings = APP.GetGame()->GetSettings();

    mLoadingMode = MLM_DEFAULT;
    if (  GT_CHAMPIONSHIP == gameSettings.mGameType )
    {
        if ( !APP.GetChampionship() )
            return;
        if ( 0 == APP.GetChampionship()->GetRaceSelected() )
            mLoadingMode = MLM_TUTORIAL1;
        else if ( 1 == APP.GetChampionship()->GetRaceSelected() )
            mLoadingMode = MLM_TUTORIAL2;
    }

    ShowLoadingMode();

    core::stringw trackName( APP.GetStringTable()->Get("id_ldtrack") );
    trackName += L" ";
    trackName += gameSettings.mLevelName.c_str();
    mTrackName->setText( trackName.c_str() );

    core::stringw lapText( APP.GetStringTable()->Get("id_ldlaps"));
    lapText += core::stringw(gameSettings.mNrOfLaps);
    mLaps->setText( lapText.c_str() );

    if ( gameSettings.mNrOfBots > 0 )
    {
        core::stringw botText( APP.GetStringTable()->Get("id_ldopponents") );
        botText += core::stringw(gameSettings.mNrOfBots);
        mOpponents->setVisible(true);
        mOpponents->setText( botText.c_str() );
    }
    else
    {
        mOpponents->setVisible(false);
    }

    if ( (gameSettings.mGameType == GT_CHAMPIONSHIP || gameSettings.mGameType == GT_TIMERACE)
        && gameSettings.mTargetTime )
    {
        std::wstring timeText( APP.GetStringTable()->Get("id_ldtargettime").c_str() );
        timeText += GuiHelper::MakeTimeString(gameSettings.mTargetTime);
        mTime->setVisible(true);
        mTime->setText( timeText.c_str() );
    }
    else
    {
        mTime->setVisible(false);
    }

    if ( GT_RIVALS == gameSettings.mGameType )
    {
        mPlayerInfo->setVisible(true);
        std::wstring text( APP.GetStringTable()->Get("id_ldplayer").c_str() );

        RivalsPlayer* currentPlayer = APP.GetRivalsMode()->GetPlayer(APP.GetRivalsMode()->GetCurrentPlayer());
        if ( currentPlayer )
        {
            PlayerProfile* currentProfile = APP.GetProfileManager()->GetProfile(currentPlayer->mProfileId);
            text += L" ";
            text += currentProfile->mName.c_str();
        }
        mPlayerInfo->setText(text.c_str());
    }
    else
    {
        mPlayerInfo->setVisible(false);
    }

    bool mustLoad = true;
    if ( GT_RIVALS == gameSettings.mGameType )
    {
        mustLoad = false;
        if ( APP.GetRivalsMode()->GetIsNewLevel() )
        {
            APP.GetRivalsMode()->ResetIsNewLevel();
            mustLoad = true;
        }
    }

    if ( mustLoad )
    {
        mButtonStart->setVisible(false);
        mStaticLoading->setVisible(true);
        APP.GetIrrlichtManager()->ForceIrrlichtUpdate();
        APP.GetLevelManager()->LoadLevel(gameSettings.mLevelName);
    }
    else
    {
        mStaticLoading->setVisible(false);
    }

    APP.GetGame()->StartSession();
	APP.GetIrrlichtManager()->GetIrrlichtDevice()->clearSystemMessages();

    mButtonStart->setVisible(true);
    GetEnvironment()->setFocus( mButtonStart );
    mStaticLoading->setVisible(false);
    APP.GetIrrlichtManager()->ForceIrrlichtUpdate();


	HC1_PROFILE(
		u32 profGroupIdx=0;
		if ( getProfiler().findGroupIndex(profGroupIdx, L"START_LVL"))
		{
			core::stringw profileResult;
			getProfiler().printGroup(profileResult, profGroupIdx, true);
			LOG.Debug(profileResult.c_str());
			getProfiler().resetGroup(profGroupIdx);
		}
	)
}

bool GuiMenuLoadingScreen::OnButtonStart(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != EGET_BUTTON_CLICKED )
        return false;

    APP.SetMode(MODE_GAME);
    APP.GetGame()->Start();

    return false;
}
