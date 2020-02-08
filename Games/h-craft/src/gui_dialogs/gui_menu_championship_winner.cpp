// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_championship_winner.h"
#include "gui_menu_championship_progress.h"
#include "../gui.h"
#include "../main.h"

using namespace irr;
using namespace gui;

GuiMenuChampWinner::GuiMenuChampWinner(const Config& config)
: GuiDialog(config)
{
}

GuiMenuChampWinner::~GuiMenuChampWinner()
{
    RemoveFunctors();
}

bool GuiMenuChampWinner::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_continue")), new EventFunctor<GuiMenuChampWinner>(this, &GuiMenuChampWinner::OnButtonContinue) );
    }
    return ok;
}

void GuiMenuChampWinner::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;
    RemoveGuiEventFunctor( GetIdForName(std::string("id_continue")) );
}

bool GuiMenuChampWinner::OnButtonContinue(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuChampionshipProgress() );
    }
    return false;
}

