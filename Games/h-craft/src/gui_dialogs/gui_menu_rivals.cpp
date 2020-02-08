// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_rivals.h"
#include "gui_menu_main.h"
#include "gui_menu_rivals_score.h"
#include "gui_helper.h"
#include "../main.h"
#include "../gui.h"
#include "../string_table.h"
#include "../game.h"
#include "../profiles.h"
#include "../rivalsmode.h"
#include "../app_restrictions.h"

using namespace irr;
using namespace gui;

GuiMenuRivals::GuiMenuRivals(const Config& config)
    : GuiDialog(config)
    , mButtonStart(0)
    , mButtonBack(0)
    , mSliderTrackDifficulty(0)
    , mSliderNumRaces(0)
    , mSliderTracks(0)
{
    SetSuppressSceneRendering(true);
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        mSliderPlayer[i] = NULL;
    }
}

GuiMenuRivals::~GuiMenuRivals()
{
    RemoveFunctors();
}

bool GuiMenuRivals::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_start")), new EventFunctor<GuiMenuRivals>(this, &GuiMenuRivals::OnButtonStart) );
        AddGuiEventFunctor( GetIdForName(std::string("id_mainmenu")), new EventFunctor<GuiMenuRivals>(this, &GuiMenuRivals::OnButtonMain) );
        AddGuiEventFunctor( GetIdForName(std::string("difficulty")), new EventFunctor<GuiMenuRivals>(this, &GuiMenuRivals::OnSliderLevelDifficulty) );
        AddGuiEventFunctor( GetIdForName(std::string("slider_num_races")), new EventFunctor<GuiMenuRivals>(this, &GuiMenuRivals::OnSliderNumRaces) );
        AddGuiEventFunctor( GetIdForName(std::string("player1")), new EventFunctor<GuiMenuRivals>(this, &GuiMenuRivals::OnSliderPlayer) );
        AddGuiEventFunctor( GetIdForName(std::string("player2")), new EventFunctor<GuiMenuRivals>(this, &GuiMenuRivals::OnSliderPlayer) );
        AddGuiEventFunctor( GetIdForName(std::string("player3")), new EventFunctor<GuiMenuRivals>(this, &GuiMenuRivals::OnSliderPlayer) );
        AddGuiEventFunctor( GetIdForName(std::string("player4")), new EventFunctor<GuiMenuRivals>(this, &GuiMenuRivals::OnSliderPlayer) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;
        std::string errorMsg("GuiMenuRivals::Load");

        mButtonStart = static_cast<IGUIButton*>(GetElementByName(root, "id_start", errorMsg));
        mButtonBack = static_cast<IGUIButton*>(GetElementByName(root, "id_mainmenu", errorMsg));
        if ( !mButtonStart )
            return false;
        mButtonStart->setEnabled(false);
        SetAlternateDefaultFocus( mButtonBack );

        mSliderTrackDifficulty = static_cast<IGUITextSlider*>(GetElementByName(root, "difficulty", errorMsg));
        if ( !mSliderTrackDifficulty )
            return false;
        mSliderTrackDifficulty->clearTexts();
        mSliderTrackDifficulty->addText( APP.GetStringTable()->Get("id_dif_all").c_str() );
        mSliderTrackDifficulty->addText( APP.GetStringTable()->Get("id_dif_easy").c_str() );
        mSliderTrackDifficulty->addText( APP.GetStringTable()->Get("id_dif_medium").c_str() );
        mSliderTrackDifficulty->addText( APP.GetStringTable()->Get("id_dif_hard").c_str() );

        mSliderNumRaces = static_cast<IGUITextSlider*>(GetElementByName(root, "slider_num_races", errorMsg));
        if ( !mSliderNumRaces )
            return false;
        mSliderNumRaces->clearTexts();
        mSliderNumRaces->addText( APP.GetStringTable()->Get("id_races_single").c_str() );

        if ( IS_DEMO_VERSION )
        {
            mSliderNumRaces->setCurrentTextId( NRS_SINGLE );
        }
        else
        {
            mSliderNumRaces->addText( APP.GetStringTable()->Get("id_races_three").c_str() );
            mSliderNumRaces->addText( APP.GetStringTable()->Get("id_races_five").c_str() );
            mSliderNumRaces->addText( APP.GetStringTable()->Get("id_races_seven").c_str() );
            mSliderNumRaces->setCurrentTextId( NRS_THREE );
        }

        mSliderTracks = static_cast<IGUITextSlider*>(GetElementByName(root, "track", errorMsg));
        if ( !mSliderTracks )
            return false;
        GuiHelper::FillTextSliderWithTrackNames(mSliderTracks, LD_ALL);
        if ( IS_DEMO_VERSION )
        {
            mSliderTracks->setEnabled(true);
        }
        else
        {
            mSliderTracks->setEnabled(false);
        }

        mSliderPlayer[0] = static_cast<IGUITextSlider*>(GetElementByName(root, "player1", errorMsg));
        mSliderPlayer[1] = static_cast<IGUITextSlider*>(GetElementByName(root, "player2", errorMsg));
        mSliderPlayer[2] = static_cast<IGUITextSlider*>(GetElementByName(root, "player3", errorMsg));
        mSliderPlayer[3] = static_cast<IGUITextSlider*>(GetElementByName(root, "player4", errorMsg));
        for ( int i=0; i < MAX_RIVALS; ++i )
        {
            if ( mSliderPlayer[i] )
            {
                mSliderPlayer[i]->clearTexts();
                mSliderPlayer[i]->addText( APP.GetStringTable()->Get("id_showactiveprofile").c_str() );
            }
        }
    }
    return ok;
}

void GuiMenuRivals::RemoveFunctors()
{
    if ( !IsLoaded() )
    {
        return;
    }
    RemoveGuiEventFunctor( GetIdForName(std::string("id_start")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_mainmenu")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("difficulty")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("slider_num_races")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("player1")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("player2")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("player3")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("player4")) );
}

void GuiMenuRivals::Show()
{
    if ( mSliderTracks && !mSliderTracks->isVisible() )
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

    CheckCanStart();

    if ( APP.GetProfileManager() )
    {
        for ( int i=0; i < MAX_RIVALS; ++i )
        {
            if ( mSliderPlayer[i] )
            {
                int oldId = mSliderPlayer[i]->getCurrentTextId();
                mSliderPlayer[i]->clearTexts();
                mSliderPlayer[i]->addText( APP.GetStringTable()->Get("id_showactiveprofile").c_str() );

                for ( unsigned int p=0; p < APP.GetProfileManager()->GetNumProfiles(); ++p )
                {
                    std::wstring name( APP.GetProfileManager()->GetProfile(p)->mName );
                    mSliderPlayer[i]->addText( name.c_str() );
                }
                if ( oldId >= 0 && oldId < mSliderPlayer[i]->getNumTexts() )
                {
                    mSliderPlayer[i]->setCurrentTextId(oldId);
                }
            }
        }
    }
}

bool GuiMenuRivals::OnSliderLevelDifficulty(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
    {
        GuiHelper::FillTextSliderWithTrackNames(mSliderTracks, GetLevelDifficulty());
        Show();
    }
    return false;
}

bool GuiMenuRivals::OnSliderNumRaces(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED  || !mSliderNumRaces || !mSliderTracks)
        return false;

    if ( (NumRacesSelected)mSliderNumRaces->getCurrentTextId() == NRS_SINGLE )
    {
        mSliderTracks->setEnabled(true);
    }
    else
    {
        mSliderTracks->setEnabled(false);
    }
    CheckCanStart();

    return false;
}

bool GuiMenuRivals::CheckCanStart()
{
    bool hasPlayer = false;
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        if ( mSliderPlayer[i] && mSliderPlayer[i]->getCurrentTextId() != 0 )
        {
            hasPlayer = true;
            break;
        }
    }

    bool canStart = hasPlayer;

    if ( canStart && APP.GetRivalsMode() )
    {
        if ( !APP.GetRivalsMode()->GetNumRacesAvailable(GetLevelDifficulty()) )
            canStart = false;
    }

    if ( canStart )
    {
        unsigned int numRaces = GetNumRacesSelected();
        if ( !numRaces )
            canStart = false;
        if ( !mSliderTracks )
            canStart = false;
        else if (   1 == numRaces
                &&  APP.GetLevelManager()->GetIndexForName(mSliderTracks->getCurrentText()) < 0 )
        {
            canStart = false;
        }
    }

    if ( mButtonStart )
    {
        mButtonStart->setEnabled(canStart);
    }

    return canStart;
}

bool GuiMenuRivals::OnSliderPlayer(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED || !mButtonStart )
        return false;

    CheckCanStart();

    return false;
}

LevelDifficulty GuiMenuRivals::GetLevelDifficulty() const
{
    if ( !mSliderTrackDifficulty )
    {
        return LD_ALL;
    }
    switch ( mSliderTrackDifficulty->getCurrentTextId() )
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

unsigned int GuiMenuRivals::GetNumRacesSelected()
{
    if ( !mSliderNumRaces )
        return 0;

    switch ( (NumRacesSelected)mSliderNumRaces->getCurrentTextId() )
    {
        case NRS_SINGLE:
            return 1;
        case NRS_THREE:
            return 3;
        case NRS_FIVE:
            return 5;
        case NRS_SEVEN:
            return 7;
    }

    return 0;
}

bool GuiMenuRivals::OnButtonStart(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != EGET_BUTTON_CLICKED )
        return false;

    if ( !CheckCanStart() )
        return false;

    APP.GetGui()->PlayOverrideClickSound(GS_CLICK_OK);

    // create the new rivalsgame
    APP.GetRivalsMode()->ClearAll();
    unsigned int numRaces = GetNumRacesSelected();
    APP.GetRivalsMode()->CreateRaces(numRaces);
    if ( 1 == numRaces )
    {
        if ( !mSliderTracks )
            return false;
        std::vector<RivalsRace>& races = APP.GetRivalsMode()->GetRaces();
        races[0].mLevelIndex = APP.GetLevelManager()->GetIndexForName( mSliderTracks->getCurrentText() );
    }
    else
    {
        if ( !APP.GetRivalsMode()->RandomizeAllRaces(GetLevelDifficulty()) )
            return false;
    }

    // we always fill the first RivalsPlayers (that's easier later on)
    int playerUsed = 0;
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        if ( mSliderPlayer[i] && mSliderPlayer[i]->getCurrentTextId() > 0 )
        {
            RivalsPlayer* player = APP.GetRivalsMode()->GetPlayer(playerUsed);
            player->mProfileId = mSliderPlayer[i]->getCurrentTextId()-1;
            ++playerUsed;
        }
    }
    // disable the unused players
    for ( int i=playerUsed; i < MAX_RIVALS; ++i )
    {
        RivalsPlayer* player = APP.GetRivalsMode()->GetPlayer(i);
        player->mProfileId = -1;
    }

    APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuRivalsScore() );

    return false;
}

bool GuiMenuRivals::OnButtonMain(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuMain() );
    }

    return false;
}
