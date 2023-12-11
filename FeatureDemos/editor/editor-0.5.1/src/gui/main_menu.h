// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_GUI_MAIN_MENU_H
#define OSRPG_EDITOR_GUI_MAIN_MENU_H

#include "add_mesh.h"
#include "edit_mesh.h"
#include "edit_light.h"
#include "edit_zone.h"
#include "edit_place_tag.h"
#include "edit_joint_bs.h"
#include "edit_joint_hinge.h"
#include "save_as.h"
#include "open.h"
#include "properties.h"

class MainMenu {
	public:
		MainMenu (Registry *reg);
		~MainMenu ();
		bool EditItem ();
		void MoveCam (char dir);
		bool OnEvent (const irr::SEvent &e);
		void OnSave ();
		void Show (bool state);
		void Toggle ();
		void Update (float elapsed_time);

	private:
		void DrawAxes ();
		bool shown;
		irr::gui::IGUIContextMenu *menu;
		irr::gui::IGUIStaticText *text;
		Registry *reg;
		MapScene *ms;
		AddMesh *add_mesh;
		EditMesh *edit_mesh;
		EditLight *edit_light;
		EditZone *edit_zone;
		EditPlaceTag *edit_place_tag;
		EditJointBS *edit_joint_bs;
		EditJointHinge *edit_joint_hinge;
		Open *open;
		Properties *properties;
		SaveAs *save_as;
		bool cam_is_moving;
		char cam_dir;
		irr::video::SMaterial mat;
		irr::core::matrix4 m;
};

#endif // OSRPG_EDITOR_GUI_MAIN_MENU_H

