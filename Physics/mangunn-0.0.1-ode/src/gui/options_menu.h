// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_GUI_OPTIONS_MENU_H
#define OSRPG_GAME_GUI_OPTIONS_MENU_H

/// \ingroup GUI
///@{

/// Options menu

#include "../registry.h"

class OptionsMenu {
	public:
		/// Constructor

		/// \param env GUI environment
		/// \param settings Settings
		OptionsMenu (Registry *rg);
		/// Destructor
		~OptionsMenu ();
		/// Apply changes
		void OnApply ();
		/// Cancel changes
		void OnCancel ();
		bool OnEvent (const irr::SEvent &e);
		/// Enable apply buttons

		/// \param state Enable state
		/// \param video_params_changed Were video parameters changed?
		void EnableApplyBtns (bool state, bool video_params_changed = false);
		/// Ok
		void OnOk ();
		/// Show / hide Options menu

		/// \param state Show state
		///
		void Show (bool state = true);

	private:
		void LoadSettings ();
		void SaveSettings ();

		bool vp_changed;
		Registry *reg;
		irr::gui::IGUIWindow *wnd;
		irr::gui::IGUIButton *ok_btn,
			*cancel_btn,
			*apply_btn;
		irr::gui::IGUIComboBox *res;
		irr::gui::IGUICheckBox *fs;
		irr::gui::IGUIEditBox *map;
};

///@}

#endif // OSRPG_GAME_GUI_OPTIONS_MENU_H

