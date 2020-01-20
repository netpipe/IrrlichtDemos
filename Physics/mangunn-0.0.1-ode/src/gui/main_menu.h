// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_GUI_MAIN_MENU_H
#define OSRPG_GAME_GUI_MAIN_MENU_H

/// Main menu

#include "options_menu.h"
#include "../map/scene.h"

class MainMenu {
	public:
		MainMenu (Registry *rg, MapScene *m);
		~MainMenu ();
		void DrawAddons ();
		bool OnEvent (const irr::SEvent &e);
		void Show (bool state = true);
		void Toggle ();

	private:
		Registry *reg;
		int last_fps;
		irr::gui::IGUIStaticText *fps_text[2],
			*ray_text;
		irr::gui::IGUIButton *quit_btn,
			*options_btn,
			*about_btn;
		irr::gui::IGUIImage *title;
		OptionsMenu *om;
		MapScene *ms;
};

#endif // OSRPG_GAME_GUI_MAIN_MENU_H

