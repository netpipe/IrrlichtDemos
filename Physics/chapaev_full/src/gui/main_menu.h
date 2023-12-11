// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GUI_MAIN_MENU_H
#define OSRPG_GUI_MAIN_MENU_H

/// \defgroup GUI
///@{

/// Main menu

#include <irrlicht.h>
#include "../settings.h"
#include "options_menu.h"
#include "about_menu.h"

class MainMenu {
	public:
		/// Constructor

		/// \param dev Irrlicht device
		/// \param settings Settings
		/// \param open_window Which window to open after renderer restart
		MainMenu (irr::IrrlichtDevice *dev, Settings *settings,
				unsigned short open_window = 0);
		/// Destructor
		~MainMenu ();
		/// Draw addons

		/// \param red Is it the red team's turn now?
		///
		void DrawAddons (bool red);
		/// Get About menu

		/// \return About menu
		///
		AboutMenu* GetAboutMenu ();
		/// Get Options menu

		/// \return Options menu
		///
		OptionsMenu* GetOptionsMenu ();
		/// Set the blue team's score

		/// \param score Score
		///
		void SetBlueScore (unsigned short score);
		/// Set the red team's score

		/// \param score Score
		///
		void SetRedScore (unsigned short score);
		/// Show / hide main menu

		/// \param state Show state
		///
		void Show (bool state);
		/// Show FPS
		
		/// fps FPS
		///
		void ShowFPS (int fps);

	private:
		int last_fps;
		bool shown;
		irr::IrrlichtDevice *dev;
		irr::gui::IGUIStaticText *fps_text[2],
			*blue_score[2],
			*red_score[2];
		irr::gui::IGUIButton *quit_btn,
			*options_btn,
			*about_btn;
		irr::gui::IGUIImage *title;
		OptionsMenu *opts;
		AboutMenu *about;
		Settings *settings;
};

///@}

#endif // OSRPG_GUI_MAIN_MENU_H

