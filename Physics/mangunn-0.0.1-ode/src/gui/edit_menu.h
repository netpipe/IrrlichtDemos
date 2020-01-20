// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GUI_EDIT_MENU_H
#define OSRPG_GUI_EDIT_MENU_H

#include <irrlicht.h>
#include "../registry.h"

class EditMenu {
	public:
		EditMenu (Registry *reg);
		void Show (bool state);

	private:
		Registry *reg;
		irr::gui::IGUIWindow *wnd;
};

#endif // OSRPG_GUI_EDIT_MENU_H

