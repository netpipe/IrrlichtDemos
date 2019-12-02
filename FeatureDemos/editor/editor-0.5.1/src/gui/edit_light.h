// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_GUI_EDIT_LIGHT_H
#define OSRPG_EDITOR_GUI_EDIT_LIGHT_H

#include "../map/scene.h"

class EditLight {
	public:
		EditLight (Registry *reg, MapScene *ms);
		~EditLight ();
		void OnApply ();
		void OnCancel ();
		bool OnEvent (const irr::SEvent &e);
		void OnOk ();
		void Show (bool state = true);

	private:
		void PosRot (char action);

		Registry *reg;
		irr::gui::IGUIWindow *wn;
		irr::gui::IGUIButton *ok_btn,
			*cancel_btn,
			*apply_btn,
			*plus_x_pos,
			*plus_y_pos,
			*plus_z_pos,
			*minus_x_pos,
			*minus_y_pos,
			*minus_z_pos;
		irr::gui::IGUIEditBox *name,
			*radius,
			*x_pos,
			*y_pos,
			*z_pos,
			*r_col,
			*g_col,
			*b_col,
			*a_col;
		irr::gui::IGUIStaticText *name_text,
			*radius_text,
			*color_text,
			*position_text;
		MapScene *ms;
};

#endif // OSRPG_EDITOR_GUI_EDIT_LIGHT_H

