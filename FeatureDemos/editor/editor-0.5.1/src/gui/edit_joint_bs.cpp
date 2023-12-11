// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "edit_joint_bs.h"
#include "../misc/events.h"
#include "../misc/help.h"
using namespace irr;
using namespace gui;
using namespace core;
using namespace io;

EditJointBS::EditJointBS (Registry *reg, MapScene *ms) {
	edit = true;
	EditJointBS::reg = reg;
	EditJointBS::ms = ms;
	const int dx = 400;
	const int dy = 350;
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
	cx = env->addCheckBox (false, rect<s32> (10, 130, 130, 150), wn, -1,
			L"Constrain X");
	cy = env->addCheckBox (false, rect<s32> (140, 130, 270, 150), wn, -1,
			L"Constrain Y");
	cz = env->addCheckBox (false, rect<s32> (280, 130, dx - 10, 150), wn, -1,
			L"Constrain Z");
	low_text = env->addStaticText (L"Low:", rect<s32> (10, 160, 50, 180),
			false, false, wn);
	low_x = env->addEditBox (L"", rect<s32> (60, 160, 170, 180), true, wn);
	low_y = env->addEditBox (L"", rect<s32> (170, 160, 280, 180), true, wn);
	low_z = env->addEditBox (L"", rect<s32> (280, 160, 390, 180), true, wn);
	high_text = env->addStaticText (L"High:", rect<s32> (10, 190, 50, 210),
			false, false, wn);
	high_x = env->addEditBox (L"", rect<s32> (60, 190, 170, 210), true, wn);
	high_y = env->addEditBox (L"", rect<s32> (170, 190, 280, 210), true, wn);
	high_z = env->addEditBox (L"", rect<s32> (280, 190, 390, 210), true, wn);
	anchor_text = env->addStaticText (L"Anchor:", rect<s32> (10, 220, 100,
				240), false, false, wn);
	x_minus_btn = env->addButton (rect<s32> (10, 240, 30, 260), wn,
			EV_EDIT_JOINT_BALL_SOCKET_X_MINUS, L"-");
	ax = env->addEditBox (L"", rect<s32> (30, 240, 120, 260), true, wn);
	x_plus_btn = env->addButton (rect<s32> (120, 240, 140, 260), wn,
			EV_EDIT_JOINT_BALL_SOCKET_X_PLUS, L"+");
	y_minus_btn = env->addButton (rect<s32> (10, 260, 30, 280), wn,
			EV_EDIT_JOINT_BALL_SOCKET_Y_MINUS, L"-");
	ay = env->addEditBox (L"", rect<s32> (30, 260, 120, 280), true, wn);
	y_plus_btn = env->addButton (rect<s32> (120, 260, 140, 280), wn,
			EV_EDIT_JOINT_BALL_SOCKET_Y_PLUS, L"+");
	z_minus_btn = env->addButton (rect<s32> (10, 280, 30, 300), wn,
			EV_EDIT_JOINT_BALL_SOCKET_Z_MINUS, L"-");
	az = env->addEditBox (L"", rect<s32> (30, 280, 120, 300), true, wn);
	z_plus_btn = env->addButton (rect<s32> (120, 280, 140, 300), wn,
			EV_EDIT_JOINT_BALL_SOCKET_Z_PLUS, L"+");

	ok_btn = env->addButton (rect<s32> (dx - 180, dy - 30, dx - 130,
				dy - 10), wn, EV_EDIT_JOINT_BALL_SOCKET_OK, L"OK");
	cancel_btn = env->addButton (rect<s32> (dx - 120, dy - 30, dx - 70,
				dy - 10), wn, EV_EDIT_JOINT_BALL_SOCKET_CANCEL, L"Cancel");
	apply_btn = env->addButton (rect<s32> (dx - 60, dy - 30, dx - 10,
				dy - 10), wn, EV_EDIT_JOINT_BALL_SOCKET_APPLY, L"Apply");
	Show (false);
}

EditJointBS::~EditJointBS () {

}

void EditJointBS::OnApply () {
	short x_low,
		  y_low,
		  z_low,
		  x_high,
		  y_high,
		  z_high;
	float fmp;
	char buf[64],
		 name1[32],
		 name2[32];
	int imp;
	// Names
	strcpy (name1, stringc (mesh1_name->getText ()).c_str ());
	strcpy (name2, stringc (mesh2_name->getText ()).c_str ());
	// Constraints
	bool constrain_x = cx->isChecked ();
	bool constrain_y = cy->isChecked ();
	bool constrain_z = cz->isChecked ();
	if (constrain_x) {
		strcpy (buf, stringc (low_x->getText ()).c_str ());
		if (sscanf (buf, "%i", &imp))
			x_low = imp;
		strcpy (buf, stringc (high_x->getText ()).c_str ());
		if (sscanf (buf, "%i", &imp))
			x_high = imp;
	}
	if (constrain_y) {
		strcpy (buf, stringc (low_y->getText ()).c_str ());
		if (sscanf (buf, "%i", &imp))
			y_low = imp;
		strcpy (buf, stringc (high_y->getText ()).c_str ());
		if (sscanf (buf, "%i", &imp))
			y_high = imp;
	}
	if (constrain_z) {
		strcpy (buf, stringc (low_z->getText ()).c_str ());
		if (sscanf (buf, "%i", &imp))
			z_low = imp;
		strcpy (buf, stringc (high_z->getText ()).c_str ());
		if (sscanf (buf, "%i", &imp))
			z_high = imp;
	}
	// Position
	vector3df pos (0, 0, 0);
	strcpy (buf, stringc (ax->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		pos.X = fmp;
	strcpy (buf, stringc (ay->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		pos.Y = fmp;
	strcpy (buf, stringc (az->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		pos.Z = fmp;
	if (edit)
		ms->EditSelectedJointBallSocket (name1, name2, pos, constrain_x,
				constrain_y, constrain_z, x_low, x_high, y_low, y_high,
				z_low, z_high);
	else
		if (!ms->AddJointBallSocket (name1, name2, pos, constrain_x,
					constrain_y, constrain_z, x_low, x_high, y_low, y_high,
					z_low, z_high))
			Report ("Couldn't add ball-socket joint", "EditJointBS::OnApply ()");
}

void EditJointBS::OnCancel () {
	Show (false);
}

bool EditJointBS::OnEvent (const irr::SEvent &e) {
	if (e.EventType == EET_GUI_EVENT) {
		s32 id = e.GUIEvent.Caller->getID ();
		switch (e.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EV_EDIT_JOINT_BALL_SOCKET_APPLY:
						OnApply ();
						return true;
						break;
					case EV_EDIT_JOINT_BALL_SOCKET_CANCEL:
						OnCancel ();
						return true;
						break;
					case EV_EDIT_JOINT_BALL_SOCKET_OK:
						OnOk ();
						return true;
						break;
					case EV_EDIT_JOINT_BALL_SOCKET_X_MINUS:
						Anchor (-1);
						return true;
						break;
					case EV_EDIT_JOINT_BALL_SOCKET_Y_MINUS:
						Anchor (-2);
						return true;
						break;
					case EV_EDIT_JOINT_BALL_SOCKET_Z_MINUS:
						Anchor (-3);
						return true;
						break;
					case EV_EDIT_JOINT_BALL_SOCKET_X_PLUS:
						Anchor (1);
						return true;
						break;
					case EV_EDIT_JOINT_BALL_SOCKET_Y_PLUS:
						Anchor (2);
						return true;
						break;
					case EV_EDIT_JOINT_BALL_SOCKET_Z_PLUS:
						Anchor (3);
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

void EditJointBS::OnOk () {
	OnApply ();
	Show (false);
}

void EditJointBS::Show (bool state, bool edit) {
	EditJointBS::edit = edit;
	wn->setVisible (state);
	// Refresh
	if (state) {
		if (edit) {
			wn->setText (L"Edit ball-socket joint");
			OMFJointBallSocket jb = ms->GetSelectedJointBallSocketCopy ();
			mesh1_name->setText (stringw (jb.mesh1_name).c_str ());
			mesh2_name->setText (stringw (jb.mesh2_name).c_str ());
			cx->setChecked (jb.constrain_x);
			cy->setChecked (jb.constrain_y);
			cz->setChecked (jb.constrain_z);
			if (jb.constrain_x) {
				low_x->setText (stringw (jb.x_low).c_str ());
				high_x->setText (stringw (jb.x_high).c_str ());
			}
			else {
				low_x->setText (L"");
				high_x->setText (L"");
			}
			if (jb.constrain_y) {
				low_y->setText (stringw (jb.y_low).c_str ());
				high_y->setText (stringw (jb.y_high).c_str ());
			}
			else {
				low_y->setText (L"");
				high_y->setText (L"");
			}
			if (jb.constrain_z) {
				low_z->setText (stringw (jb.z_low).c_str ());
				high_z->setText (stringw (jb.z_high).c_str ());
			}
			else {
				low_z->setText (L"");
				high_z->setText (L"");
			}
			ax->setText (stringw (jb.anchor.a).c_str ());
			ay->setText (stringw (jb.anchor.b).c_str ());
			az->setText (stringw (jb.anchor.c).c_str ());
		}
		else {
			wn->setText (L"Add ball-socket joint");
			mesh1_name->setText (L"");
			mesh2_name->setText (L"");
			cx->setChecked (false);
			cy->setChecked (false);
			cz->setChecked (false);
			low_x->setText (L"");
			low_y->setText (L"");
			low_z->setText (L"");
			high_x->setText (L"");
			high_y->setText (L"");
			high_z->setText (L"");
			ax->setText (stringw (reg->cam->getPosition ().X).c_str ());
			ay->setText (stringw (reg->cam->getPosition ().Y).c_str ());
			az->setText (stringw (reg->cam->getPosition ().Z).c_str ());
		}
	}
}

void EditJointBS::Anchor (char action) {
	float scale = 5;
	OMFJointBallSocket b = ms->GetSelectedJointBallSocketCopy ();
	switch (action) {
		case -1:
			b.anchor.a -= scale;
			break;
		case -2:
			b.anchor.b -= scale;
			break;
		case -3:
			b.anchor.c -= scale;
			break;
		case 1:
			b.anchor.a += scale;
			break;
		case 2:
			b.anchor.b += scale;
			break;
		case 3:
			b.anchor.c += scale;
			break;
		default:
			break;
	}
	ms->EditSelectedJointBallSocket (b);
	Show ();
	OnApply ();
}

