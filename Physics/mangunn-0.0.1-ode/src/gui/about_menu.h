// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GUI_ABOUT_MENU_H
#define OSRPG_GUI_ABOUT_MENU_H

/// \ingroup GUI
///@{

/// About menu

#include <irrlicht.h>
#include "../registry.h"

class AboutMenu {
	public:
		/// Constructor
		AboutMenu (Registry *reg);
		void Show (bool state);
	
	private:
		irr::gui::IGUIWindow *wn;
};

///@}

#endif // OSRPG_GUI_ABOUT_MENU_H

