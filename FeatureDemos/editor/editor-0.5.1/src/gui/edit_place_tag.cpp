// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "edit_place_tag.h"
#include "../misc/events.h"
using namespace irr;
using namespace gui;
using namespace core;
using namespace io;

EditPlaceTag::EditPlaceTag (Registry *reg, MapScene *ms) {
	EditPlaceTag::reg = reg;
	EditPlaceTag::ms = ms;
	const int dx = 400;
	const int dy = 300;
	const int x = 100;
	const int y = 100;
	IGUIEnvironment *env = reg->env;
	wn = env->addWindow (rect<s32> (x, y, x + dx, y + dy), false,
			L"Edit place tag");
	wn->getCloseButton ()->setVisible (false);
	name_text = env->addStaticText (L"Name:", rect<s32> (10, 30, 100, 50),
			false, false, wn);
	name = env->addEditBox (L"", rect<s32> (10, 50, dx - 10, 70), true, wn);
	position_text = env->addStaticText (L"Position:", rect<s32> (10, 80, 100,
				100), false, false, wn);
	// X position
	minus_x_pos = env->addButton (rect<s32> (10, 100, 30, 120),
			wn, EV_EDIT_PLACE_TAG_ITEM_MINUS_X_POS, L"-");
	x_pos = env->addEditBox (L"", rect<s32> (30, 100, 130, 120),
			true, wn);
	plus_x_pos = env->addButton (rect<s32> (130, 100, 150, 120),
			wn, EV_EDIT_PLACE_TAG_ITEM_PLUS_X_POS, L"+");
	// Y position
	minus_y_pos = env->addButton (rect<s32> (10, 120, 30, 140),
			wn, EV_EDIT_PLACE_TAG_ITEM_MINUS_Y_POS, L"-");
	y_pos = env->addEditBox (L"", rect<s32> (30, 120, 130, 140),
			true, wn);
	plus_y_pos = env->addButton (rect<s32> (130, 120, 150, 140),
			wn, EV_EDIT_PLACE_TAG_ITEM_PLUS_Y_POS, L"+");
	// Z position
	minus_z_pos = env->addButton (rect<s32> (10, 140, 30, 160),
			wn, EV_EDIT_PLACE_TAG_ITEM_MINUS_Z_POS, L"-");
	z_pos = env->addEditBox (L"", rect<s32> (30, 140, 130, 160),
			true, wn);
	plus_z_pos = env->addButton (rect<s32> (130, 140, 150, 160),
			wn, EV_EDIT_PLACE_TAG_ITEM_PLUS_Z_POS, L"+");

	ok_btn = env->addButton (rect<s32> (dx - 180, dy - 30, dx - 130,
				dy - 10), wn, EV_EDIT_PLACE_TAG_ITEM_OK, L"OK");
	cancel_btn = env->addButton (rect<s32> (dx - 120, dy - 30, dx - 70,
				dy - 10), wn, EV_EDIT_PLACE_TAG_ITEM_CANCEL, L"Cancel");
	apply_btn = env->addButton (rect<s32> (dx - 60, dy - 30, dx - 10,
				dy - 10), wn, EV_EDIT_PLACE_TAG_ITEM_APPLY, L"Apply");
	Show (false);
}

EditPlaceTag::~EditPlaceTag () {

}

void EditPlaceTag::OnApply () {
	float fmp;
	char buf[64];
	vector3df pos;
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
	strcpy (buf, stringc (name->getText ()).c_str ());
	ms->EditSelectedPlaceTag (buf, pos);
}

void EditPlaceTag::OnCancel () {
	Show (false);
}

bool EditPlaceTag::OnEvent (const irr::SEvent &e) {
	if (e.EventType == EET_GUI_EVENT) {
		s32 id = e.GUIEvent.Caller->getID ();
		switch (e.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EV_EDIT_PLACE_TAG_ITEM_APPLY:
						OnApply ();
						return true;
						break;
					case EV_EDIT_PLACE_TAG_ITEM_CANCEL:
						OnCancel ();
						return true;
						break;
					case EV_EDIT_PLACE_TAG_ITEM_OK:
						OnOk ();
						return true;
						break;
					case EV_EDIT_PLACE_TAG_ITEM_MINUS_X_POS:
						PosRot (-1);
						return true;
						break;
					case EV_EDIT_PLACE_TAG_ITEM_MINUS_Y_POS:
						PosRot (-2);
						return true;
						break;
					case EV_EDIT_PLACE_TAG_ITEM_MINUS_Z_POS:
						PosRot (-3);
						return true;
						break;
					case EV_EDIT_PLACE_TAG_ITEM_PLUS_X_POS:
						PosRot (1);
						return true;
						break;
					case EV_EDIT_PLACE_TAG_ITEM_PLUS_Y_POS:
						PosRot (2);
						return true;
						break;
					case EV_EDIT_PLACE_TAG_ITEM_PLUS_Z_POS:
						PosRot (3);
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

void EditPlaceTag::OnOk () {
	OnApply ();
	Show (false);
}

void EditPlaceTag::Show (bool state) {
	wn->setVisible (state);
	// Refresh
	if (state) {
		OMFPlaceTag pt = ms->GetSelectedPlaceTagCopy ();
		name->setText (stringw (pt.name).c_str ());
		x_pos->setText (stringw (pt.pos.a).c_str ());
		y_pos->setText (stringw (pt.pos.b).c_str ());
		z_pos->setText (stringw (pt.pos.c).c_str ());
	}
}

void EditPlaceTag::PosRot (char action) {
	float scale = 5;
	OMFPlaceTag pt = ms->GetSelectedPlaceTagCopy ();
	switch (action) {
		case -1:
			pt.pos.a -= scale;
			break;
		case -2:
			pt.pos.b -= scale;
			break;
		case -3:
			pt.pos.c -= scale;
			break;
		case 1:
			pt.pos.a += scale;
			break;
		case 2:
			pt.pos.b += scale;
			break;
		case 3:
			pt.pos.c += scale;
			break;
		default:
			break;
	}
	ms->EditSelectedPlaceTag (pt);
	Show ();
	OnApply ();
}

