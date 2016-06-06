// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GUI_OPTIONS_MENU_H
#define OSRPG_GUI_OPTIONS_MENU_H

/// \ingroup GUI
///@{

/// Options menu

#include <irrlicht.h>
#include "../settings.h"

class OptionsMenu {
	public:
		/// Constructor

		/// \param env GUI environment
		/// \param settings Settings
		OptionsMenu (irr::gui::IGUIEnvironment *env, Settings *settings);
		/// Destructor
		~OptionsMenu ();
		/// Apply changes

		/// \return If we changed video parameters, then return which
		/// window to leave open after renderer restart, and restart renderer
		unsigned short Apply ();
		/// Cancel changes
		void Cancel ();
		/// Enable apply buttons

		/// \param state Enable state
		/// \param video_params_changed Were video parameters changed?
		void EnableApplyBtns (bool state, bool video_params_changed = false);
		/// Ok
		
		/// \return Should we restart renderer or not?
		///
		bool Ok ();
		/// Show / hide Options menu

		/// \param state Show state
		///
		void Show (bool state);

	private:
		void LoadSettings ();
		void SaveSettings ();

		bool vp_changed;
		Settings *settings;
		irr::gui::IGUIWindow *wnd;
		irr::gui::IGUIButton *ok_btn,
			*cancel_btn,
			*apply_btn;
		irr::gui::IGUIComboBox *res,
			*vd,
			*bpp;
		irr::gui::IGUICheckBox *fs;
		irr::gui::IGUIEditBox *step,
			*gravity,
			*iters;
};

///@}

#endif // OSRPG_GUI_OPTIONS_MENU_H

