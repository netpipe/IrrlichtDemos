// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "edit_menu.h"
#include "../misc/events.h"
using namespace irr;
using namespace gui;
using namespace core;

EditMenu::EditMenu (Registry *reg) {
	EditMenu::reg = reg;
	Settings *settings = reg->settings;
	IGUIEnvironment *env = reg->env;
	const int dx = 300;
	const int dy = 400;
	const int x = settings->GetScreenWidth () - dx;
	const int y = 0;
	wnd = env->addWindow (rect<s32> (x, y, x + dx, y + dy), false,
			L"Edit");
	wnd->getCloseButton ()->setVisible (false);
	Show (false);
	IGUITabControl *tabc = env->addTabControl (
			rect<s32> (10, 30, dx - 10, dy - 10), wnd, true, false);
	IGUITab *tab = 0;
	// Add tab
	tab = tabc->addTab (L"Add");
	// Delete tab
	tab = tabc->addTab (L"Delete");
	// Move tab
	tab = tabc->addTab (L"Move");

}

void EditMenu::Show (bool state) {
	wnd->setVisible (state);
}

