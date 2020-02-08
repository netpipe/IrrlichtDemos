// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_tutorial3.h"
#include "gui_menu_championship_progress.h"
#include "../gui.h"
#include "../main.h"

using namespace irr;
using namespace gui;

GuiMenuTutorial3::GuiMenuTutorial3(const Config& config)
: GuiDialog(config)
{
    SetSuppressSceneRendering(true);
}

GuiMenuTutorial3::~GuiMenuTutorial3()
{
    RemoveFunctors();
}

bool GuiMenuTutorial3::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_continue")), new EventFunctor<GuiMenuTutorial3>(this, &GuiMenuTutorial3::OnButtonContinue) );
    }
    return ok;
}

void GuiMenuTutorial3::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;
    RemoveGuiEventFunctor( GetIdForName(std::string("id_continue")) );
}

bool GuiMenuTutorial3::OnButtonContinue(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuChampionshipProgress() );
    }
    return false;
}
