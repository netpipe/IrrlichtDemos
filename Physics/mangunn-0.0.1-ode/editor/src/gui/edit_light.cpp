// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "edit_light.h"
#include "../misc/events.h"
using namespace irr;
using namespace gui;
using namespace core;
using namespace video;
using namespace io;

EditLight::EditLight (Registry *reg, MapScene *ms) {
	EditLight::reg = reg;
	EditLight::ms = ms;
	const int dx = 400;
	const int dy = 300;
	const int x = 100;
	const int y = 100;
	IGUIEnvironment *env = reg->env;
	wn = env->addWindow (rect<s32> (x, y, x + dx, y + dy), false,
			L"Edit light");
	wn->getCloseButton ()->setVisible (false);
	name_text = env->addStaticText (L"Name:", rect<s32> (10, 30, 100, 50),
			false, false, wn);
	name = env->addEditBox (L"", rect<s32> (10, 50, dx - 10, 70), true, wn);
	radius_text = env->addStaticText (L"Radius:", rect<s32> (10, 80, 100, 100),
			false, false, wn);
	radius = env->addEditBox (L"", rect<s32> (10, 100, dx - 10, 120), true, wn);
	position_text = env->addStaticText (L"Position:", rect<s32> (10,
				130, 150, 150), false, false, wn);
	// X position
	minus_x_pos = env->addButton (rect<s32> (10, 150, 30, 170),
			wn, EV_EDIT_LIGHT_ITEM_MINUS_X_POS, L"-");
	x_pos = env->addEditBox (L"", rect<s32> (30, 150, 130, 170),
			true, wn);
	plus_x_pos = env->addButton (rect<s32> (130, 150, 150, 170),
			wn, EV_EDIT_LIGHT_ITEM_PLUS_X_POS, L"+");
	// Y position
	minus_y_pos = env->addButton (rect<s32> (10, 170, 30, 190),
			wn, EV_EDIT_LIGHT_ITEM_MINUS_Y_POS, L"-");
	y_pos = env->addEditBox (L"", rect<s32> (30, 170, 130, 190),
			true, wn);
	plus_y_pos = env->addButton (rect<s32> (130, 170, 150, 190),
			wn, EV_EDIT_LIGHT_ITEM_PLUS_Y_POS, L"+");
	// Z position
	minus_z_pos = env->addButton (rect<s32> (10, 190, 30, 210),
			wn, EV_EDIT_LIGHT_ITEM_MINUS_Z_POS, L"-");
	z_pos = env->addEditBox (L"", rect<s32> (30, 190, 130, 210),
			true, wn);
	plus_z_pos = env->addButton (rect<s32> (130, 190, 150, 210),
			wn, EV_EDIT_LIGHT_ITEM_PLUS_Z_POS, L"+");
	color_text = env->addStaticText (L"RGBA color:", rect<s32> (10, 220, 100, 240),
			false, false, wn);
	r_col = env->addEditBox (L"", rect<s32> (10, 240, 105, 260), true, wn);
	g_col = env->addEditBox (L"", rect<s32> (105, 240, 200, 260), true, wn);
	b_col = env->addEditBox (L"", rect<s32> (200, 240, 295, 260), true, wn);
	a_col = env->addEditBox (L"", rect<s32> (295, 240, 390, 260), true, wn);

	ok_btn = env->addButton (rect<s32> (dx - 180, dy - 30, dx - 130,
				dy - 10), wn, EV_EDIT_LIGHT_ITEM_OK, L"OK");
	cancel_btn = env->addButton (rect<s32> (dx - 120, dy - 30, dx - 70,
				dy - 10), wn, EV_EDIT_LIGHT_ITEM_CANCEL, L"Cancel");
	apply_btn = env->addButton (rect<s32> (dx - 60, dy - 30, dx - 10,
				dy - 10), wn, EV_EDIT_LIGHT_ITEM_APPLY, L"Apply");
	Show (false);
}

EditLight::~EditLight () {

}

void EditLight::OnApply () {
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
	SColorf col;
	strcpy (buf, stringc (r_col->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		col.r = fmp;
	strcpy (buf, stringc (g_col->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		col.g = fmp;
	strcpy (buf, stringc (b_col->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		col.b = fmp;
	strcpy (buf, stringc (a_col->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		col.a = fmp;
	strcpy (buf, stringc (radius->getText ()).c_str ());
	sscanf (buf, "%f", &fmp);
	strcpy (buf, stringc (name->getText ()).c_str ());
	ms->EditSelectedLight (buf, pos, fmp, col);
}

void EditLight::OnCancel () {
	Show (false);
}

bool EditLight::OnEvent (const irr::SEvent &e) {
	if (e.EventType == EET_GUI_EVENT) {
		s32 id = e.GUIEvent.Caller->getID ();
		switch (e.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EV_EDIT_LIGHT_ITEM_APPLY:
						OnApply ();
						return true;
						break;
					case EV_EDIT_LIGHT_ITEM_CANCEL:
						OnCancel ();
						return true;
						break;
					case EV_EDIT_LIGHT_ITEM_OK:
						OnOk ();
						return true;
						break;
					case EV_EDIT_LIGHT_ITEM_MINUS_X_POS:
						PosRot (-1);
						return true;
						break;
					case EV_EDIT_LIGHT_ITEM_MINUS_Y_POS:
						PosRot (-2);
						return true;
						break;
					case EV_EDIT_LIGHT_ITEM_MINUS_Z_POS:
						PosRot (-3);
						return true;
						break;
					case EV_EDIT_LIGHT_ITEM_PLUS_X_POS:
						PosRot (1);
						return true;
						break;
					case EV_EDIT_LIGHT_ITEM_PLUS_Y_POS:
						PosRot (2);
						return true;
						break;
					case EV_EDIT_LIGHT_ITEM_PLUS_Z_POS:
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

void EditLight::OnOk () {
	OnApply ();
	Show (false);
}

void EditLight::Show (bool state) {
	wn->setVisible (state);
	// Refresh
	if (state) {
		OMFLight l = ms->GetSelectedLightCopy ();
		name->setText (stringw (l.name).c_str ());
		radius->setText (stringw (l.radius).c_str ());
		x_pos->setText (stringw (l.pos.a).c_str ());
		y_pos->setText (stringw (l.pos.b).c_str ());
		z_pos->setText (stringw (l.pos.c).c_str ());
		r_col->setText (stringw (l.color.a).c_str ());
		g_col->setText (stringw (l.color.b).c_str ());
		b_col->setText (stringw (l.color.c).c_str ());
		a_col->setText (stringw (l.color.d).c_str ());
	}
}

void EditLight::PosRot (char action) {
	float scale = 5;
	OMFLight l = ms->GetSelectedLightCopy ();
	switch (action) {
		case -1:
			l.pos.a -= scale;
			break;
		case -2:
			l.pos.b -= scale;
			break;
		case -3:
			l.pos.c -= scale;
			break;
		case 1:
			l.pos.a += scale;
			break;
		case 2:
			l.pos.b += scale;
			break;
		case 3:
			l.pos.c += scale;
			break;
		default:
			break;
	}
	ms->EditSelectedLight (l);
	Show ();
	OnApply ();
}

