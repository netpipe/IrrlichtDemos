// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_main.h"
#include "gui_menu_rivals.h"
#include "gui_menu_championship.h"
#include "gui_menu_arcade.h"
#include "gui_menu_timeattack.h"
#include "gui_menu_selecthover.h"
#include "gui_menu_replaytheatre.h"
#include "gui_menu_highscores.h"
#include "gui_menu_options.h"
#include "gui_menu_profiles.h"
#include "gui_menu_nagscreen.h"
#include "../gui.h"
#include "../main.h"
#include "../profiles.h"
#include "../string_table.h"
#include "../config.h"
#include "../app_restrictions.h"
#include "../tinyxml/tinyxml.h"
#include "../logging.h"

using namespace irr;
using namespace gui;

GuiMenuMain::GuiMenuMain(const Config& config)
    : GuiDialog(config)
    , mButtonChampionShip(0)
    , mButtonArcade(0)
    , mButtonTimeattack(0)
    , mButtonSelectHover(0)
    , mButtonQuit(0)
    , mButtonOptions(0)
    , mButtonRivalsMode(0)
    , mActiveProfile(0)
    , mDemoText(0)
    , mDebugText(0)
{
    SetSuppressSceneRendering(true);
}

GuiMenuMain::~GuiMenuMain()
{
    RemoveFunctors();
}

bool GuiMenuMain::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_partymode")), new EventFunctor<GuiMenuMain>(this, &GuiMenuMain::OnButtonRivals) ); // partymode is now called rivalsmode. Can't change it here easily
        AddGuiEventFunctor( GetIdForName(std::string("id_championship")), new EventFunctor<GuiMenuMain>(this, &GuiMenuMain::OnButtonChampionship) );
        AddGuiEventFunctor( GetIdForName(std::string("id_arcade")), new EventFunctor<GuiMenuMain>(this, &GuiMenuMain::OnButtonArcade) );
        AddGuiEventFunctor( GetIdForName(std::string("id_timeattack")), new EventFunctor<GuiMenuMain>(this, &GuiMenuMain::OnButtonTimeAttack) );
        AddGuiEventFunctor( GetIdForName(std::string("id_selecth")), new EventFunctor<GuiMenuMain>(this, &GuiMenuMain::OnButtonSelectHover) );
        //AddGuiEventFunctor( GetIdForName(std::string("id_replay")), new EventFunctor<GuiMenuMain>(this, &GuiMenuMain::OnButtonReplayTheatre) );
        AddGuiEventFunctor( GetIdForName(std::string("id_highscores")), new EventFunctor<GuiMenuMain>(this, &GuiMenuMain::OnButtonHighscores) );
        AddGuiEventFunctor( GetIdForName(std::string("id_options")), new EventFunctor<GuiMenuMain>(this, &GuiMenuMain::OnButtonOptions) );
        AddGuiEventFunctor( GetIdForName(std::string("id_profiles")), new EventFunctor<GuiMenuMain>(this, &GuiMenuMain::OnButtonProfiles) );
        AddGuiEventFunctor( GetIdForName(std::string("id_quit")), new EventFunctor<GuiMenuMain>(this, &GuiMenuMain::OnButtonQuit) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuMain::Load");
        mButtonChampionShip = static_cast<IGUIButton*>(GetElementByName(root, "id_championship", errorMsg));
        mButtonArcade = static_cast<IGUIButton*>(GetElementByName(root, "id_arcade", errorMsg));
        mButtonTimeattack = static_cast<IGUIButton*>(GetElementByName(root, "id_timeattack", errorMsg));
        mButtonSelectHover = static_cast<IGUIButton*>(GetElementByName(root, "id_selecth", errorMsg));
        mButtonQuit = static_cast<IGUIButton*>(GetElementByName(root, "id_quit", errorMsg));
        mButtonOptions = static_cast<IGUIButton*>(GetElementByName(root, "id_options", errorMsg));
        mButtonRivalsMode = static_cast<IGUIButton*>(GetElementByName(root, "id_partymode", errorMsg)); // partymode is now called rivalsmode. Can't change it here easily
        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));
        mDemoText = static_cast<IGUIStaticText*>(GetElementByName(root, "demoversion", errorMsg));
        mDebugText = static_cast<IGUIStaticText*>(GetElementByName(root, "debug", errorMsg));
    }

    return ok;
}

void GuiMenuMain::Show()
{
    if ( APP.GetIrrlichtManager() && APP.GetIrrlichtManager()->GetIrrlichtDevice() )
    {
        APP.GetIrrlichtManager()->GetIrrlichtDevice()->clearSystemMessages();
    }
    GuiDialog::Show();

    bool hasProfile = APP.GetProfileManager()->GetSelectedProfile() != NULL ? true : false;

    bool hasTutorial = false;
    bool hasTrack = false;
    if ( !IS_DEMO_VERSION )
    {
        hasTutorial = true;
        hasTrack = true;
        if ( mDemoText )
        {
            mDemoText->setVisible(false);
        }
    }

    TiXmlElement * ele = APP.GetConfig()->GetEtcSettings();
    if ( ele && !hasTutorial)
    {
        int tut=0;
        if ( ele->Attribute("tutorial_done", &tut ) )
            hasTutorial = tut;
    }
    if ( !hasTrack && APP.GetProfileManager() )
    {
        if ( APP.GetProfileManager()->IsLevelUnlocked(18) )
            hasTrack = true;
    }

    if ( mButtonChampionShip )
        mButtonChampionShip->setEnabled(hasProfile);
    if ( mButtonArcade )
        mButtonArcade->setEnabled(hasProfile && hasTutorial && hasTrack);
    if ( mButtonTimeattack )
        mButtonTimeattack->setEnabled(hasProfile&& hasTutorial && hasTrack);
    if ( mButtonSelectHover )
        mButtonSelectHover->setEnabled(hasProfile);
    if ( mButtonOptions )
        mButtonOptions->setEnabled(hasProfile);
    if ( mButtonRivalsMode )
        mButtonRivalsMode->setEnabled(hasProfile && hasTutorial && hasTrack);

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

#if !defined(HOVER_RELEASE)	// just to be certain it doesn't get in
	if ( mDebugText )
	{
//		mDebugText->setText( LOG.GetStream().str().c_str() );
	}
#endif
}

void GuiMenuMain::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;

    RemoveGuiEventFunctor( GetIdForName(std::string("id_partymode")) ); // partymode is now called rivalsmode. Can't change it here easily
    RemoveGuiEventFunctor( GetIdForName(std::string("id_championship")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_arcade")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_timeattack")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_selecth")) );
    //RemoveGuiEventFunctor( GetIdForName(std::string("id_replay")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_highscores")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_options")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_profiles")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_quit")) );
}

bool GuiMenuMain::OnButtonRivals(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuRivals() );
    }

    return false;
}

bool GuiMenuMain::OnButtonChampionship(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuChampionship() );
    }

    return false;
}

bool GuiMenuMain::OnButtonArcade(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuArcade() );
    }

    return false;
}

bool GuiMenuMain::OnButtonTimeAttack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuTimeAttack() );
    }

    return false;
}

bool GuiMenuMain::OnButtonSelectHover(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuSelectHover() );
    }

    return false;
}

bool GuiMenuMain::OnButtonReplayTheatre(const irr::SEvent &event_)
{
//    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
//    {
//        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuReplayTheatre() );
//    }

    return false;
}

bool GuiMenuMain::OnButtonHighscores(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuHighscores() );
    }

    return false;
}

bool GuiMenuMain::OnButtonOptions(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuOptions() );
    }

    return false;
}

bool GuiMenuMain::OnButtonProfiles(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuProfiles() );
    }

    return false;
}

bool GuiMenuMain::OnButtonQuit(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        if (IS_DEMO_VERSION || DO_DISPLAY_ADS)
        {
            APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuNagscreen() );
        }
        else if ( !IS_DEMO_VERSION )
        {
            APP.StopApp();
        }
    }

    return false;
}
