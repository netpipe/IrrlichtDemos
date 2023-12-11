// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_GUI_PROPERTIES_H
#define OSRPG_EDITOR_GUI_PROPERTIES_H

#include "../map/scene.h"

class Properties {
	public:
		Properties (Registry *reg, MapScene *ms);
		~Properties ();
		void OnCancel ();
		bool OnEvent (const irr::SEvent &e);
		void OnOk ();
		void Show (bool state = true);

	private:
		void DisplaySkyDomeOn (bool state = true);
		void SetSkyDomeOn (bool state = true);
		void UseTexture (char i);
		Registry *reg;
		MapScene *ms;
		irr::gui::IGUIWindow *wn;
		irr::gui::IGUIButton *ok_btn,
			*cancel_btn,
			*b1,
			*b2,
			*b3,
			*b4,
			*b5,
			*b6;
		irr::gui::IGUIEditBox *name,
			*script,
			*gr_x,
			*gr_y,
			*gr_z,
			*t1,
			*t2,
			*t3,
			*t4,
			*t5,
			*t6;
		irr::gui::IGUIListBox *list;
		irr::gui::IGUIStaticText *name_text,
			*script_text,
			*gravity_text,
			*list_text;
		irr::gui::IGUICheckBox *skydome;
		irr::core::stringc tex_dir;
};

#endif // OSRPG_EDITOR_GUI_PROPERTIES_H

