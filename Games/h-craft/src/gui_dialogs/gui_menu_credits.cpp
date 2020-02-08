// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_credits.h"
#include "gui_menu_licenses.h"
#include "gui_menu_options.h"
#include "../gui.h"
#include "../main.h"
#include "../config.h"

using namespace irr;
using namespace gui;


GuiMenuCredits::GuiMenuCredits(const Config& config)
    : GuiDialog(config)
{
    SetSuppressSceneRendering(true);
}

GuiMenuCredits::~GuiMenuCredits()
{
    RemoveFunctors();
}

bool GuiMenuCredits::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_back")), new EventFunctor<GuiMenuCredits>(this, &GuiMenuCredits::OnButtonBack) );
		if ( GetConfig().GetUseTouchInput() != ETI_NO_TOUCH )
		{
			AddGuiEventFunctor( GetIdForName(std::string("id_licenses")), new EventFunctor<GuiMenuCredits>(this, &GuiMenuCredits::OnButtonLicenses) );
		}
    }
    return ok;
}

void GuiMenuCredits::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;
    RemoveGuiEventFunctor( GetIdForName(std::string("id_back")) );
	if ( GetConfig().GetUseTouchInput() != ETI_NO_TOUCH )
	{
		RemoveGuiEventFunctor( GetIdForName(std::string("id_licenses")) );
	}
}

bool GuiMenuCredits::OnButtonLicenses(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuLicenses() );
    }

    return false;
}

bool GuiMenuCredits::OnButtonBack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuOptions() );
    }

    return false;
}
