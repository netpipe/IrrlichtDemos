// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "edit_joint_hinge.h"
#include "../misc/events.h"
#include "../misc/help.h"
using namespace irr;
using namespace gui;
using namespace core;
using namespace io;

EditJointHinge::EditJointHinge (Registry *reg, MapScene *ms) {
	edit = true;
	EditJointHinge::reg = reg;
	EditJointHinge::ms = ms;
	const int dx = 400;
	const int dy = 330;
	const int x = 100;
	const int y = 100;
	IGUIEnvironment *env = reg->env;
	wn = env->addWindow (rect<s32> (x, y, x + dx, y + dy), false);
	wn->getCloseButton ()->setVisible (false);
	name1_text = env->addStaticText (L"Mesh1 name:", rect<s32> (10, 30, 100, 50),
			false, false, wn);
	mesh1_name = env->addEditBox (L"", rect<s32> (10, 50, dx - 10, 70), true, wn);
	name2_text = env->addStaticText (L"Mesh2 name:", rect<s32> (10, 80, 100, 100),
			false, false, wn);
	mesh2_name = env->addEditBox (L"", rect<s32> (10, 100, dx - 10, 120), true, wn);
	constrain = env->addCheckBox (false, rect<s32> (10, 130, 150, 150), wn, -1,
			L"Constrain angle");
	low_text = env->addStaticText (L"Low:", rect<s32> (10, 160, 50, 180),
			false, false, wn);
	angle_low = env->addEditBox (L"", rect<s32> (60, 160, 170, 180), true,
			wn);
	high_text = env->addStaticText (L"High:", rect<s32> (200, 160, 250, 180),
			false, false, wn);
	angle_high = env->addEditBox (L"", rect<s32> (260, 160, 370, 180), true,
			wn);
	anchor_text = env->addStaticText (L"Anchor:", rect<s32> (10, 190, 100,
				210), false, false, wn);
	anchor_x_minus_btn = env->addButton (rect<s32> (10, 210, 30, 230), wn,
			EV_EDIT_JOINT_HINGE_ANCHOR_X_MINUS, L"-");
	anchor_x = env->addEditBox (L"", rect<s32> (30, 210, 120, 230), true, wn);
	anchor_x_plus_btn = env->addButton (rect<s32> (120, 210, 140, 230), wn,
			EV_EDIT_JOINT_HINGE_ANCHOR_X_PLUS, L"+");
	anchor_y_minus_btn = env->addButton (rect<s32> (10, 230, 30, 250), wn,
			EV_EDIT_JOINT_HINGE_ANCHOR_Y_MINUS, L"-");
	anchor_y = env->addEditBox (L"", rect<s32> (30, 230, 120, 250), true, wn);
	anchor_y_plus_btn = env->addButton (rect<s32> (120, 230, 140, 250), wn,
			EV_EDIT_JOINT_HINGE_ANCHOR_Y_PLUS, L"+");
	anchor_z_minus_btn = env->addButton (rect<s32> (10, 250, 30, 270), wn,
			EV_EDIT_JOINT_HINGE_ANCHOR_Z_MINUS, L"-");
	anchor_z = env->addEditBox (L"", rect<s32> (30, 250, 120, 270), true, wn);
	anchor_y_plus_btn = env->addButton (rect<s32> (120, 250, 140, 270), wn,
			EV_EDIT_JOINT_HINGE_ANCHOR_Z_PLUS, L"+");
	axis_text = env->addStaticText (L"Axis:", rect<s32> (210, 190, 300, 210),
			false, false, wn);
	axis_x_minus_btn = env->addButton (rect<s32> (210, 210, 230, 230), wn,
			EV_EDIT_JOINT_HINGE_AXIS_X_MINUS, L"-");
	axis_x = env->addEditBox (L"", rect<s32> (230, 210, 320, 230), true, wn);
	axis_x_plus_btn = env->addButton (rect<s32> (320, 210, 340, 230), wn,
			EV_EDIT_JOINT_HINGE_AXIS_X_PLUS, L"+");
	axis_y_minus_btn = env->addButton (rect<s32> (210, 230, 230, 250), wn,
			EV_EDIT_JOINT_HINGE_AXIS_Y_MINUS, L"-");
	axis_y = env->addEditBox (L"", rect<s32> (230, 230, 320, 250), true, wn);
	axis_y_plus_btn = env->addButton (rect<s32> (320, 230, 340, 250), wn,
			EV_EDIT_JOINT_HINGE_AXIS_Y_PLUS, L"+");
	axis_z_minus_btn = env->addButton (rect<s32> (210, 250, 230, 270), wn,
			EV_EDIT_JOINT_HINGE_AXIS_Z_MINUS, L"-");
	axis_z = env->addEditBox (L"", rect<s32> (230, 250, 320, 270), true, wn);
	axis_z_plus_btn = env->addButton (rect<s32> (320, 250, 340, 270), wn,
			EV_EDIT_JOINT_HINGE_AXIS_Z_PLUS, L"+");

	ok_btn = env->addButton (rect<s32> (dx - 180, dy - 30, dx - 130,
				dy - 10), wn, EV_EDIT_JOINT_HINGE_OK, L"OK");
	cancel_btn = env->addButton (rect<s32> (dx - 120, dy - 30, dx - 70,
				dy - 10), wn, EV_EDIT_JOINT_HINGE_CANCEL, L"Cancel");
	apply_btn = env->addButton (rect<s32> (dx - 60, dy - 30, dx - 10,
				dy - 10), wn, EV_EDIT_JOINT_HINGE_APPLY, L"Apply");
	Show (false);
}

EditJointHinge::~EditJointHinge () {

}

void EditJointHinge::OnApply () {
	short low_angle,
		  high_angle;
	float fmp;
	char buf[64],
		 name1[32],
		 name2[32];
	int imp;
	// Names
	strcpy (name1, stringc (mesh1_name->getText ()).c_str ());
	strcpy (name2, stringc (mesh2_name->getText ()).c_str ());
	// Constraint
	bool constrn = constrain->isChecked ();
	if (constrn) {
		strcpy (buf, stringc (angle_low->getText ()).c_str ());
		if (sscanf (buf, "%i", &imp)) {
			low_angle = imp;
		}
		strcpy (buf, stringc (angle_high->getText ()).c_str ());
		if (sscanf (buf, "%i", &imp)) {
			high_angle = imp;
		}
	}
	// Anchor
	vector3df pos (0, 0, 0);
	strcpy (buf, stringc (anchor_x->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		pos.X = fmp;
	strcpy (buf, stringc (anchor_y->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		pos.Y = fmp;
	strcpy (buf, stringc (anchor_z->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		pos.Z = fmp;
	// Axis
	vector3df axis (0, 0, 0);
	strcpy (buf, stringc (axis_x->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		axis.X = fmp;
	strcpy (buf, stringc (axis_y->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		axis.Y = fmp;
	strcpy (buf, stringc (axis_z->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		axis.Z = fmp;
	if (edit)
		ms->EditSelectedJointHinge (name1, name2, pos, axis, constrn,
				low_angle, high_angle);
	else
		if (!ms->AddJointHinge (name1, name2, pos, axis, constrn,
					low_angle, high_angle))
			Report ("Couldn't add hinge joint", "EditJointHinge::OnApply ()");
}

void EditJointHinge::OnCancel () {
	Show (false);
}

bool EditJointHinge::OnEvent (const irr::SEvent &e) {
	if (e.EventType == EET_GUI_EVENT) {
		s32 id = e.GUIEvent.Caller->getID ();
		switch (e.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EV_EDIT_JOINT_HINGE_APPLY:
						OnApply ();
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_CANCEL:
						OnCancel ();
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_OK:
						OnOk ();
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_ANCHOR_X_MINUS:
						Anchor (-1);
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_ANCHOR_Y_MINUS:
						Anchor (-2);
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_ANCHOR_Z_MINUS:
						Anchor (-3);
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_ANCHOR_X_PLUS:
						Anchor (1);
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_ANCHOR_Y_PLUS:
						Anchor (2);
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_ANCHOR_Z_PLUS:
						Anchor (3);
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_AXIS_X_MINUS:
						Axis (-1);
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_AXIS_Y_MINUS:
						Axis (-2);
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_AXIS_Z_MINUS:
						Axis (-3);
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_AXIS_X_PLUS:
						Axis (1);
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_AXIS_Y_PLUS:
						Axis (2);
						return true;
						break;
					case EV_EDIT_JOINT_HINGE_AXIS_Z_PLUS:
						Axis (3);
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

void EditJointHinge::OnOk () {
	OnApply ();
	Show (false);
}

void EditJointHinge::Show (bool state, bool edit) {
	EditJointHinge::edit = edit;
	wn->setVisible (state);
	// Refresh
	if (state) {
		if (edit) {
			wn->setText (L"Edit hinge joint");
			OMFJointHinge jh = ms->GetSelectedJointHingeCopy ();
			mesh1_name->setText (stringw (jh.mesh1_name).c_str ());
			mesh2_name->setText (stringw (jh.mesh2_name).c_str ());
			constrain->setChecked (jh.constrain);
			if (jh.constrain) {
				angle_low->setText (stringw (jh.angle_low).c_str ());
				angle_high->setText (stringw (jh.angle_high).c_str ());
			}
			else {
				angle_low->setText (L"");
				angle_high->setText (L"");
			}
			anchor_x->setText (stringw (jh.anchor.a).c_str ());
			anchor_y->setText (stringw (jh.anchor.b).c_str ());
			anchor_z->setText (stringw (jh.anchor.c).c_str ());
			axis_x->setText (stringw (jh.axis.a).c_str ());
			axis_y->setText (stringw (jh.axis.b).c_str ());
			axis_z->setText (stringw (jh.axis.c).c_str ());
		}
		else {
			wn->setText (L"Add hinge joint");
			mesh1_name->setText (L"");
			mesh2_name->setText (L"");
			constrain->setChecked (false);
			angle_low->setText (L"");
			angle_high->setText (L"");
			anchor_x->setText (stringw (reg->cam->getPosition ().X).c_str ());
			anchor_y->setText (stringw (reg->cam->getPosition ().Y).c_str ());
			anchor_z->setText (stringw (reg->cam->getPosition ().Z).c_str ());
			axis_x->setText (stringw (0).c_str ());
			axis_y->setText (stringw (1).c_str ());
			axis_z->setText (stringw (0).c_str ());
		}
	}
}

void EditJointHinge::Anchor (char action) {
	float scale = 5;
	OMFJointHinge h = ms->GetSelectedJointHingeCopy ();
	switch (action) {
		case -1:
			h.anchor.a -= scale;
			break;
		case -2:
			h.anchor.b -= scale;
			break;
		case -3:
			h.anchor.c -= scale;
			break;
		case 1:
			h.anchor.a += scale;
			break;
		case 2:
			h.anchor.b += scale;
			break;
		case 3:
			h.anchor.c += scale;
			break;
		default:
			break;
	}
	ms->EditSelectedJointHinge (h);
	Show ();
	OnApply ();
}

void EditJointHinge::Axis (char action) {
	float scale = 1;
	OMFJointHinge h = ms->GetSelectedJointHingeCopy ();
	switch (action) {
		case -1:
			h.axis.a -= scale;
			break;
		case -2:
			h.axis.b -= scale;
			break;
		case -3:
			h.axis.c -= scale;
			break;
		case 1:
			h.axis.a += scale;
			break;
		case 2:
			h.axis.b += scale;
			break;
		case 3:
			h.axis.c += scale;
			break;
		default:
			break;
	}
	ms->EditSelectedJointHinge (h);
	Show ();
	OnApply ();
}

