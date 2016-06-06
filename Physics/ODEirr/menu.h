// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Menu (when will we switch to CEGUI?)
#ifndef MENU_H
#define MENU_H

#include "settings.h"
#include "misc/enums.h"

class MainMenu {
	public:
		/// Create main menu

		/// \param guienv GUI environment (irr::gui::IGUIEnvironment)
		/// \param settings Global settings pointer
		/// \param open_window This window will be opened after renderer
		/// restart (this is necessary in case you pressed "Apply" button)
		void Create (IGUIEnvironment *guienv, GameSettings *settings,
				int open_window);
		/// Apply changes
		int Apply ();
		/// Cancel changes
		void Cancel ();
		/// Apply changes and hide open windows
		int Ok ();
		/// Enable/disable "Apply" button

		/// \param state Enable/disable state
		///
		void EnableApply (bool state);
		/// Enable/disable "Cancel" button

		/// \param state Enable/disable state
		///
		void EnableCancel (bool state);
		/// Show/hide "Options" window

		/// \param state Show/hide state
		///
		void ShowOptions (bool state);
		/// Update FPS

		/// \param fps FPS to show
		///
		void UpdateFPS (int fps);
		/// Show/hide GUI

		/// \param state Show/hide state
		///
		void ShowGUI (bool state);

	private:
		void CreateDevTab ();
		void CreatePhysTab ();
		void ShowBG (bool state);
		IGUIEnvironment *env;
		IGUITabControl* tabc;
		IGUIWindow *opts,
				   *bg;
		IGUIButton* btn[5];
		IGUIComboBox *res,
			*vd;
		IGUICheckBox *fs;
		GameSettings *sets;
		IGUIStaticText *fps_text[2];
		IGUIEditBox *boxes_amount,
					*box_mass,
					*box_size,
					*step,
					*iters,
					*gravity,
					*capsule;
};

#endif // MENU_H

