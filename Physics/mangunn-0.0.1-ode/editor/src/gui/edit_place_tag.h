// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_GUI_EDIT_PLACE_TAG_H
#define OSRPG_EDITOR_GUI_EDIT_PLACE_TAG_H

#include "../map/scene.h"

class EditPlaceTag {
	public:
		EditPlaceTag (Registry *reg, MapScene *ms);
		~EditPlaceTag ();
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
			*x_pos,
			*y_pos,
			*z_pos;
		irr::gui::IGUIStaticText *name_text,
			*position_text;
		MapScene *ms;
};

#endif // OSRPG_EDITOR_GUI_EDIT_PLACE_TAG_H

