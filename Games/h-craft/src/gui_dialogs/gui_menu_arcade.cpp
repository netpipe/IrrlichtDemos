// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_arcade.h"
#include "gui_menu_main.h"
#include "gui_menu_loadingscreen.h"
#include "gui_helper.h"
#include "../main.h"
#include "../gui.h"
#include "../profiles.h"
#include "../string_table.h"
#include "../game.h"

using namespace irr;
using namespace gui;

GuiMenuArcade::GuiMenuArcade(const Config& config)
    : GuiDialog(config)
    , mActiveProfile(0)
    , mSliderTracks(0)
    , mSliderLevelDifficulty(0)
    , mSliderBotDifficulty(0)
    , mButtonStart(0)
    , mButtonBack(0)
{
    SetSuppressSceneRendering(true);
}

GuiMenuArcade::~GuiMenuArcade()
{
    RemoveFunctors();
}

bool GuiMenuArcade::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_start")), new EventFunctor<GuiMenuArcade>(this, &GuiMenuArcade::OnButtonStart) );
        AddGuiEventFunctor( GetIdForName(std::string("id_back")), new EventFunctor<GuiMenuArcade>(this, &GuiMenuArcade::OnButtonBack) );
        AddGuiEventFunctor( GetIdForName(std::string("difficulty")), new EventFunctor<GuiMenuArcade>(this, &GuiMenuArcade::OnSliderLevelDifficulty) );
        AddGuiEventFunctor( GetIdForName(std::string("track")), new EventFunctor<GuiMenuArcade>(this, &GuiMenuArcade::OnSliderTracks) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuArcade::Load");

        mButtonStart = static_cast<IGUIButton*>(GetElementByName(root, "id_start", errorMsg));
        mButtonBack = static_cast<IGUIButton*>(GetElementByName(root, "id_back", errorMsg));
        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));
        SetAlternateDefaultFocus( mButtonBack );

        mSliderLevelDifficulty = static_cast<IGUITextSlider*>(GetElementByName(root, "difficulty", errorMsg));
        if ( !mSliderLevelDifficulty )
            return false;
        mSliderLevelDifficulty->clearTexts();
        mSliderLevelDifficulty->addText( APP.GetStringTable()->Get("id_dif_all").c_str() );
        mSliderLevelDifficulty->addText( APP.GetStringTable()->Get("id_dif_easy").c_str() );
        mSliderLevelDifficulty->addText( APP.GetStringTable()->Get("id_dif_medium").c_str() );
        mSliderLevelDifficulty->addText( APP.GetStringTable()->Get("id_dif_hard").c_str() );

        mSliderBotDifficulty = static_cast<IGUITextSlider*>(GetElementByName(root, "arc_difficulty", errorMsg));
        if ( !mSliderBotDifficulty )
            return false;
        mSliderBotDifficulty->clearTexts();
        mSliderBotDifficulty->addText( APP.GetStringTable()->Get("id_dif_easy").c_str() );
        mSliderBotDifficulty->addText( APP.GetStringTable()->Get("id_dif_medium").c_str() );
        mSliderBotDifficulty->addText( APP.GetStringTable()->Get("id_dif_hard").c_str() );

        IGUITextSlider* slider = static_cast<IGUITextSlider*>(GetElementByName(root, "laps", errorMsg));
        if ( !slider )
            return false;
        slider->clearTexts();
        slider->addText( L"1" );
        slider->addText( L"3" );
        slider->addText( L"5" );
        slider->addText( L"7" );
        slider->addText( L"9" );
        slider->addText( L"15" );
        slider->setCurrentTextId(1);

        slider = static_cast<IGUITextSlider*>(GetElementByName(root, "opponents", errorMsg));
        if ( !slider )
            return false;
        slider->clearTexts();
        slider->addText( L"1" );
        slider->addText( L"2" );
        slider->addText( L"3" );
        slider->setCurrentTextId(2);

        mSliderTracks = static_cast<IGUITextSlider*>(GetElementByName(root, "track", errorMsg));
        if ( !mSliderTracks )
            return false;
        GuiHelper::FillTextSliderWithTrackNames(mSliderTracks, LD_ALL);
    }
    return ok;
}

void GuiMenuArcade::Show()
{
    if ( mSliderTracks && !mSliderTracks->isVisible() )  // a call to show before dialog is visible
    {
        s32 oldNumTexts = mSliderTracks->getNumTexts();
        s32 oldId = mSliderTracks->getCurrentTextId();
        GuiHelper::FillTextSliderWithTrackNames(mSliderTracks, GetLevelDifficulty());
        if ( mSliderTracks->getNumTexts() != oldNumTexts )
            mSliderTracks->setCurrentTextId(0);
        else
            mSliderTracks->setCurrentTextId(oldId);
    }

    GuiDialog::Show();

    int idxLevel = -1;
    if ( mSliderTracks && mSliderTracks->getCurrentText() && APP.GetLevelManager() )
        idxLevel = APP.GetLevelManager()->GetIndexForName( mSliderTracks->getCurrentText() );

    if ( mButtonStart )
    {
        if ( idxLevel < 0 )
        {
            mButtonStart->setEnabled(false);
        }
        else
        {
            mButtonStart->setEnabled(true);
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
}

void GuiMenuArcade::RemoveFunctors()
{
    if ( !IsLoaded() )
    {
        return;
    }
    RemoveGuiEventFunctor( GetIdForName(std::string("id_start")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_back")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("difficulty")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("track")) );
}

bool GuiMenuArcade::OnButtonStart(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != EGET_BUTTON_CLICKED )
        return false;

    IGUIElement * root = GetDialogParent();
    if ( !root )
        return false;
    IGUITextSlider* sliderOpponents = static_cast<IGUITextSlider*>(GetElementByName(root, "opponents"));
    IGUITextSlider* sliderLaps = static_cast<IGUITextSlider*>(GetElementByName(root, "laps"));
    if ( !mSliderTracks || !mSliderBotDifficulty || !sliderOpponents || !sliderLaps )
        return false;

    APP.GetGui()->PlayOverrideClickSound(GS_CLICK_OK);

    GameSettings gameSettings;
    gameSettings.mGameType = GT_ARCADE;
    gameSettings.mLevelName = mSliderTracks->getCurrentText();
    gameSettings.mNrOfBots = atoi( core::stringc(sliderOpponents->getCurrentText()).c_str() );
    gameSettings.mNrOfLaps = atoi( core::stringc(sliderLaps->getCurrentText()).c_str() );
    gameSettings.mTargetTime = 0;
    gameSettings.mBotDifficulty = (BOT_DIFFICULTY)(mSliderBotDifficulty->getCurrentTextId()+1);
    APP.GetGame()->SetSettings(gameSettings);

    APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuLoadingScreen() );

    return false;
}

bool GuiMenuArcade::OnButtonBack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuMain() );
    }

    return false;
}

bool GuiMenuArcade::OnSliderTracks(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
    {
        Show();
    }
    return false;
}

bool GuiMenuArcade::OnSliderLevelDifficulty(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
    {
        GuiHelper::FillTextSliderWithTrackNames(mSliderTracks, GetLevelDifficulty());
        Show();
    }
    return false;
}

LevelDifficulty GuiMenuArcade::GetLevelDifficulty() const
{
    if ( !mSliderLevelDifficulty )
    {
        return LD_ALL;
    }
    switch ( mSliderLevelDifficulty->getCurrentTextId() )
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
