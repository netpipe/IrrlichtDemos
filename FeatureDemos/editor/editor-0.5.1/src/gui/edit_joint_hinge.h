// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_GUI_EDIT_JOINT_HINGE_H
#define OSRPG_EDITOR_GUI_EDIT_JOINT_HINGE_H

#include "../map/scene.h"

class EditJointHinge {
	public:
		EditJointHinge (Registry *reg, MapScene *ms);
		~EditJointHinge ();
		void OnApply ();
		void OnCancel ();
		bool OnEvent (const irr::SEvent &e);
		void OnOk ();
		void Show (bool state = true, bool edit = true);

	private:
		void Anchor (char action);
		void Axis (char action);

		Registry *reg;
		irr::gui::IGUIWindow *wn;
		irr::gui::IGUIButton *ok_btn,
			*cancel_btn,
			*apply_btn,
			*anchor_x_plus_btn,
			*anchor_x_minus_btn,
			*anchor_y_plus_btn,
			*anchor_y_minus_btn,
			*anchor_z_plus_btn,
			*anchor_z_minus_btn,
			*axis_x_plus_btn,
			*axis_x_minus_btn,
			*axis_y_plus_btn,
			*axis_y_minus_btn,
			*axis_z_plus_btn,
			*axis_z_minus_btn;
		irr::gui::IGUIEditBox *mesh1_name,
			*mesh2_name,
			*angle_low,
			*angle_high,
			*anchor_x,
			*anchor_y,
			*anchor_z,
			*axis_x,
			*axis_y,
			*axis_z;
		irr::gui::IGUIStaticText *name1_text,
			*name2_text,
			*low_text,
			*high_text,
			*anchor_text,
			*axis_text;
		irr::gui::IGUICheckBox *constrain;
		MapScene *ms;
		bool edit;
};

#endif // OSRPG_EDITOR_GUI_EDIT_JOINT_HINGE_H

