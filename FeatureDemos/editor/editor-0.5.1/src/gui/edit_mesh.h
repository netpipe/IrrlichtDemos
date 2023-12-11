// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_GUI_EDIT_MESH_H
#define OSRPG_EDITOR_GUI_EDIT_MESH_H

#include "../map/scene.h"

class EditMesh {
	public:
		EditMesh (Registry *reg, MapScene *ms);
		~EditMesh ();
		void OnApply ();
		void OnCancel ();
		bool OnEvent (const irr::SEvent &e);
		void OnOk ();
		void Show (bool state = true);

	private:
		void GetSelectedItem (irr::gui::IGUIEditBox *b);
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
			*minus_z_rot,
			*model_btn,
			*collision_btn;
		irr::gui::IGUIListBox *list;
		irr::gui::IGUIEditBox *name,
			*script,
			*model,
			*collision,
			*maggun_effect,
			*mass,
			*x_pos,
			*y_pos,
			*z_pos,
			*x_rot,
			*y_rot,
			*z_rot;
		irr::gui::IGUICheckBox *movable,
			*optimize,
			*collidable;
		irr::gui::IGUIStaticText *name_text,
			*script_text,
			*maggun_effect_text,
			*mass_text,
			*position_text,
			*rotation_text,
			*model_text,
			*collision_text;
		irr::core::stringc model_dir;
		MapScene *ms;
};

#endif // OSRPG_EDITOR_GUI_EDIT_MESH_H

