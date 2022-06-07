// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "edit_zone.h"
#include "../misc/events.h"
using namespace irr;
using namespace gui;
using namespace core;
using namespace io;

EditZone::EditZone (Registry *reg, MapScene *ms) {
	EditZone::reg = reg;
	EditZone::ms = ms;
	model_dir = reg->settings->GetDataDir () + "models/";
	const int dx = 400;
	const int dy = 400;
	const int x = 100;
	const int y = 100;
	IGUIEnvironment *env = reg->env;
	wn = env->addWindow (rect<s32> (x, y, x + dx, y + dy), false,
			L"Edit zone");
	wn->getCloseButton ()->setVisible (false);
	Show (false);
	name_text = env->addStaticText (L"Name:", rect<s32> (10, 30, 100, 50),
			false, false, wn);
	name = env->addEditBox (L"", rect<s32> (10, 50, dx - 10, 70), true, wn);
	list_text = env->addStaticText (L"Model file:", rect<s32> (10, 80, 150,
				100), false, false, wn);
	list = env->addListBox (rect<s32> (10, 100, dx - 10, 200), wn);
	position_text = env->addStaticText (L"Position:", rect<s32> (10,
				dy - 190, 150, dy - 170), false, false, wn);
	// X position
	minus_x_pos = env->addButton (rect<s32> (10, dy - 170, 30, dy - 150),
			wn, EV_EDIT_ZONE_ITEM_MINUS_X_POS, L"-");
	x_pos = env->addEditBox (L"", rect<s32> (30, dy - 170, 130, dy - 150),
			true, wn);
	plus_x_pos = env->addButton (rect<s32> (130, dy - 170, 150, dy - 150),
			wn, EV_EDIT_ZONE_ITEM_PLUS_X_POS, L"+");
	// Y position
	minus_y_pos = env->addButton (rect<s32> (10, dy - 150, 30, dy - 130),
			wn, EV_EDIT_ZONE_ITEM_MINUS_Y_POS, L"-");
	y_pos = env->addEditBox (L"", rect<s32> (30, dy - 150, 130, dy - 130),
			true, wn);
	plus_y_pos = env->addButton (rect<s32> (130, dy - 150, 150, dy - 130),
			wn, EV_EDIT_ZONE_ITEM_PLUS_Y_POS, L"+");
	// Z position
	minus_z_pos = env->addButton (rect<s32> (10, dy - 130, 30, dy - 110),
			wn, EV_EDIT_ZONE_ITEM_MINUS_Z_POS, L"-");
	z_pos = env->addEditBox (L"", rect<s32> (30, dy - 130, 130, dy - 110),
			true, wn);
	plus_z_pos = env->addButton (rect<s32> (130, dy - 130, 150, dy - 110),
			wn, EV_EDIT_ZONE_ITEM_PLUS_Z_POS, L"+");
	rotation_text = env->addStaticText (L"Rotation:", rect<s32> (dx - 150,
				dy - 190, dx - 10, dy - 170), false, false, wn);
	// X rotation
	minus_x_rot = env->addButton (rect<s32> (dx - 150, dy - 170, dx - 130,
				dy - 150), wn, EV_EDIT_ZONE_ITEM_MINUS_X_ROT, L"-");
	x_rot = env->addEditBox (L"", rect<s32> (dx - 130, dy - 170, dx - 30,
				dy - 150), true, wn);
	plus_x_rot = env->addButton (rect<s32> (dx - 30, dy - 170, dx - 10,
				dy - 150), wn, EV_EDIT_ZONE_ITEM_PLUS_X_ROT, L"+");
	// Y rotation
	minus_y_rot = env->addButton (rect<s32> (dx - 150, dy - 150, dx - 130,
				dy - 130), wn, EV_EDIT_ZONE_ITEM_MINUS_Y_ROT, L"-");
	y_rot = env->addEditBox (L"", rect<s32> (dx - 130, dy - 150, dx - 30,
				dy - 130), true, wn);
	plus_y_rot = env->addButton (rect<s32> (dx - 30, dy - 150, dx - 10,
				dy - 130), wn, EV_EDIT_ZONE_ITEM_PLUS_Y_ROT, L"+");
	// Z rotation
	minus_z_rot = env->addButton (rect<s32> (dx - 150, dy - 130, dx - 130,
				dy - 110), wn, EV_EDIT_ZONE_ITEM_MINUS_Z_ROT, L"-");
	z_rot = env->addEditBox (L"", rect<s32> (dx - 130, dy - 130, dx - 30,
				dy - 110), true, wn);
	plus_z_rot = env->addButton (rect<s32> (dx - 30, dy - 130, dx - 10,
				dy - 110), wn, EV_EDIT_ZONE_ITEM_PLUS_Z_ROT, L"+");
	script_name_text = env->addStaticText (L"Script name:", rect<s32> (10,
				dy - 100, 100, dy - 80),
			false, false, wn);
	script_name = env->addEditBox (L"", rect<s32> (10, dy - 80, dx - 10,
				dy - 60), true, wn);

	ok_btn = env->addButton (rect<s32> (dx - 180, dy - 30, dx - 130,
				dy - 10), wn, EV_EDIT_ZONE_ITEM_OK, L"OK");
	cancel_btn = env->addButton (rect<s32> (dx - 120, dy - 30, dx - 70,
				dy - 10), wn, EV_EDIT_ZONE_ITEM_CANCEL, L"Cancel");
	apply_btn = env->addButton (rect<s32> (dx - 60, dy - 30, dx - 10,
				dy - 10), wn, EV_EDIT_ZONE_ITEM_APPLY, L"Apply");
}

EditZone::~EditZone () {

}

void EditZone::OnApply () {
	float fmp;
	char buf[64];
	vector3df pos,
			  rot;
	// Position
	strcpy (buf, stringc (x_pos->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		pos.X = fmp;
	strcpy (buf, stringc (y_pos->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		pos.Y = fmp;
	strcpy (buf, stringc (z_pos->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		pos.Z = fmp;
	// Rotation
	strcpy (buf, stringc (x_rot->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		rot.X = fmp;
	strcpy (buf, stringc (y_rot->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		rot.Y = fmp;
	strcpy (buf, stringc (z_rot->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		rot.Z = fmp;
	stringc s = "";
	if (list->getSelected () != -1)
		s = stringc (list->getListItem (list->getSelected ()));
	ms->EditSelectedZone (stringc (name->getText ()).c_str (), s.c_str (),
			stringc (script_name->getText ()).c_str (), pos, rot);
}

void EditZone::OnCancel () {
	Show (false);
}

bool EditZone::OnEvent (const irr::SEvent &e) {
	if (e.EventType == EET_GUI_EVENT) {
		s32 id = e.GUIEvent.Caller->getID ();
		switch (e.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EV_EDIT_ZONE_ITEM_APPLY:
						OnApply ();
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_CANCEL:
						OnCancel ();
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_OK:
						OnOk ();
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_MINUS_X_POS:
						PosRot (-1);
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_MINUS_Y_POS:
						PosRot (-2);
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_MINUS_Z_POS:
						PosRot (-3);
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_MINUS_X_ROT:
						PosRot (-4);
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_MINUS_Y_ROT:
						PosRot (-5);
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_MINUS_Z_ROT:
						PosRot (-6);
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_PLUS_X_POS:
						PosRot (1);
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_PLUS_Y_POS:
						PosRot (2);
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_PLUS_Z_POS:
						PosRot (3);
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_PLUS_X_ROT:
						PosRot (4);
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_PLUS_Y_ROT:
						PosRot (5);
						return true;
						break;
					case EV_EDIT_ZONE_ITEM_PLUS_Z_ROT:
						PosRot (6);
						return true;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
    return false;
}

void EditZone::OnOk () {
	OnApply ();
	Show (false);
}

void EditZone::Show (bool state) {
	wn->setVisible (state);
	// Refresh
	if (state) {
		reg->dev->getFileSystem ()->changeWorkingDirectoryTo (
				model_dir.c_str ());
		IFileList *fl = reg->dev->getFileSystem ()->createFileList ();
		list->clear ();
		for (unsigned short i = 0; i < fl->getFileCount (); i++)
			if (!fl->isDirectory (i))
				if (strstr (fl->getFileName (i).c_str(), ".obj") != 0)
					list->addItem (stringw (fl->getFileName (i)).c_str ());
		OMFZone z = ms->GetSelectedZoneCopy ();
		for (unsigned short i = 0; i < list->getItemCount (); i++)
			if (strcmp (z.model_file, stringc (
							list->getListItem (i)).c_str ()) == 0)
				list->setSelected (i);
		name->setText (stringw (z.name).c_str ());
		script_name->setText (stringw (z.script_name).c_str ());
		x_pos->setText (stringw (z.pos.a).c_str ());
		y_pos->setText (stringw (z.pos.b).c_str ());
		z_pos->setText (stringw (z.pos.c).c_str ());
		x_rot->setText (stringw (z.rot.a).c_str ());
		y_rot->setText (stringw (z.rot.b).c_str ());
		z_rot->setText (stringw (z.rot.c).c_str ());
	}
}

void EditZone::PosRot (char action) {
	float scale = 5;
	OMFZone z = ms->GetSelectedZoneCopy ();
	switch (action) {
		case -1:
			z.pos.a -= scale;
			break;
		case -2:
			z.pos.b -= scale;
			break;
		case -3:
			z.pos.c -= scale;
			break;
		case -4:
			z.rot.a -= scale;
			break;
		case -5:
			z.rot.b -= scale;
			break;
		case -6:
			z.rot.c -= scale;
			break;
		case 1:
			z.pos.a += scale;
			break;
		case 2:
			z.pos.b += scale;
			break;
		case 3:
			z.pos.c += scale;
			break;
		case 4:
			z.rot.a += scale;
			break;
		case 5:
			z.rot.b += scale;
			break;
		case 6:
			z.rot.c += scale;
			break;
		default:
			break;
	}
	ms->EditSelectedZone (z);
	Show ();
	OnApply ();
}

