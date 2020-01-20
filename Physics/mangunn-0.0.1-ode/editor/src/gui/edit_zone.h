// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_GUI_EDIT_ZONE_H
#define OSRPG_EDITOR_GUI_EDIT_ZONE_H

#include "../map/scene.h"

class EditZone {
	public:
		EditZone (Registry *reg, MapScene *ms);
		~EditZone ();
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
			*plus_x_rot,
			*plus_y_rot,
			*plus_z_rot,
			*minus_x_pos,
			*minus_y_pos,
			*minus_z_pos,
			*minus_x_rot,
			*minus_y_rot,
			*minus_z_rot;
		irr::gui::IGUIListBox *list;
		irr::gui::IGUIEditBox *name,
			*x_pos,
			*y_pos,
			*z_pos,
			*x_rot,
			*y_rot,
			*z_rot,
			*script_name;
		irr::gui::IGUIStaticText *name_text,
			*position_text,
			*rotation_text,
			*list_text,
			*script_name_text;
		irr::core::stringc model_dir;
		MapScene *ms;
};

#endif // OSRPG_EDITOR_GUI_EDIT_ZONE_H

