// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_GUI_EDIT_JOINT_BS_H
#define OSRPG_EDITOR_GUI_EDIT_JOINT_BS_H

#include "../map/scene.h"

class EditJointBS {
	public:
		EditJointBS (Registry *reg, MapScene *ms);
		~EditJointBS ();
		void OnApply ();
		void OnCancel ();
		bool OnEvent (const irr::SEvent &e);
		void OnOk ();
		void Show (bool state = true, bool edit = true);

	private:
		void Anchor (char action);

		Registry *reg;
		irr::gui::IGUIWindow *wn;
		irr::gui::IGUIButton *ok_btn,
			*cancel_btn,
			*apply_btn,
			*x_plus_btn,
			*x_minus_btn,
			*y_plus_btn,
			*y_minus_btn,
			*z_plus_btn,
			*z_minus_btn;
		irr::gui::IGUIEditBox *mesh1_name,
			*mesh2_name,
			*low_x,
			*high_x,
			*low_y,
			*high_y,
			*low_z,
			*high_z,
			*ax,
			*ay,
			*az;
		irr::gui::IGUIStaticText *name1_text,
			*name2_text,
			*low_text,
			*high_text,
			*anchor_text;
		irr::gui::IGUICheckBox *cx,
			*cy,
			*cz;
		MapScene *ms;
		bool edit;
};

#endif // OSRPG_EDITOR_GUI_EDIT_JOINT_BS_H

