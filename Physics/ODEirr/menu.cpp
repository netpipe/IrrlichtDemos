// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "menu.h"

void MainMenu::Create (IGUIEnvironment *guienv, GameSettings *settings, int open_window) {
	sets = settings;
	env = guienv;
	// Window sizes
	int dx = 560;
	int dy = 400;
	int x = settings->GetScreenWidth () / 2 - dx / 2;
	int y = settings->GetScreenHeight () / 2 - dy / 2;
	/*
	bg = env->addWindow (core::rect<s32> (0, 0,
				settings->GetScreenWidth (),
				settings->GetScreenHeight ()), false, L"Remove this header");
	bg->getCloseButton ()->setVisible (false);
	*/
	opts = env->addWindow (core::rect<s32> (x, y, x + dx, y + dy), false, L"Options", bg);
	opts->getCloseButton ()->setVisible (false);
	ShowOptions (false);
	tabc = env->addTabControl (core::rect<s32> (10, 30, 550, 360), opts,
			true, false);
	// Main buttons
	btn[3] = env->addButton (core::rect<s32> (30, dy - 40, 100, dy - 20), bg, EvOptions,
			L"Options");
	btn[4] = env->addButton (core::rect<s32> (30, dy, 100, dy + 20), bg, EvQuit,
			L"Quit");

	// Options window
	btn[0] = env->addButton (core::rect<s32> (380, 370, 430, 390), opts, EvOk,
			L"OK");
	btn[1] = env->addButton (core::rect<s32> (440, 370, 490, 390), opts, EvCancel,
			L"Cancel");
	btn[2] = env->addButton (core::rect<s32> (500, 370, 550, 390), opts, EvApply,
			L"Apply");

	IGUISkin *skin = env->getSkin ();
	IGUIFont *font = env->getFont ("../../media/fonthaettenschweiler.bmp");
	skin->setFont (font);
	skin->setColor (EGDC_3D_FACE, SColor (255, 205, 205, 225));
	EnableApply (false);
	EnableCancel (false);
	// fps
	fps_text[0] = env->addStaticText (L"FPS", core::rect<s32> (
				settings->GetScreenWidth () - 40,
				settings->GetScreenHeight () - 30,
				settings->GetScreenWidth () - 10,
				settings->GetScreenHeight () - 10),
			false, false, 0);
	fps_text[1] = env->addStaticText (L"FPS", core::rect<s32> (
				settings->GetScreenWidth () - 41,
				settings->GetScreenHeight () - 31,
				settings->GetScreenWidth () - 10,
				settings->GetScreenHeight () - 10),
			false, false, 0);
	fps_text[1]->setOverrideColor (SColor (255, 100, 50, 50));

	switch (open_window) {
		// 0 for Options
		case 0: ShowOptions (true);
	}

	CreateDevTab ();
	CreatePhysTab ();
	Cancel ();
}

void MainMenu::ShowGUI (bool state) {
	ShowBG (state);
	if (state) {
	}
	else {
		ShowOptions (false);
	}
}

void MainMenu::ShowBG (bool state) {
	btn[3]->setVisible (state);
	btn[4]->setVisible (state);
}

void MainMenu::CreateDevTab () {
	IGUITab *cur_tab = tabc->addTab (L"Video", EvTab1);
	env->addStaticText (L"Video driver:", core::rect<s32> (10, 10, 150, 20), false, false, cur_tab);
	vd = env->addComboBox (core::rect<s32> (10, 30, 150, 50),
			cur_tab, EvVD);
	vd->addItem (L"OpenGL");
	vd->addItem (L"Software");
	env->addStaticText (L"Screen resolution:", core::rect<s32> (10, 70, 150, 80), false, false, cur_tab);
	res = env->addComboBox (core::rect<s32> (10, 90, 150, 110),
			cur_tab, EvRes);
	res->addItem (L"640x480");
	res->addItem (L"800x600");
	res->addItem (L"1024x768");

	fs = env->addCheckBox (false, core::rect<s32> (10, 130, 100, 150), cur_tab,
			EvFullScr, L"Fullscreen");
	env->addStaticText (L"Note: Software renderer is a bit strange... =)", core::rect<s32> (200, 10, 500, 60), false, true, cur_tab);
	//Cancel ();
}

void MainMenu::CreatePhysTab () {
	IGUITab *cur_tab = tabc->addTab (L"Physics", EvTab2);
	env->addStaticText (L"Note: The changes of physics parameters will take effect only after you restart the application", core::rect<s32> (200, 10, 500, 50), false, true, cur_tab);
	env->addStaticText (L"Note2: After entering value in edit box, always press Enter", core::rect<s32> (200, 60, 500, 100), false, true, cur_tab);
	env->addStaticText (L"Amount of boxes in the wall:", core::rect<s32> (10, 10, 160, 30), false, false, cur_tab);
	boxes_amount = env->addEditBox (L"3", core::rect<s32> (10, 30, 45, 50),
			true, cur_tab, EvBoxesAmount);
	env->addStaticText (L" * 10", core::rect<s32> (48, 30, 70, 50), false, false, cur_tab);
	env->addStaticText (L"Box mass:", core::rect<s32> (10, 60, 100, 80), false, false, cur_tab);
	box_mass = env->addEditBox (L"5", core::rect<s32> (10, 80, 45, 100),
			true, cur_tab, EvBoxMass);
	env->addStaticText (L"Box size:", core::rect<s32> (10, 110, 100, 130), false, false, cur_tab);
	box_size = env->addEditBox (L"5", core::rect<s32> (10, 130, 45, 150),
			true, cur_tab, EvBoxSize);
	env->addStaticText (L"Step:", core::rect<s32> (10, 160, 100, 180), false, false, cur_tab);
	step = env->addEditBox (L"0.04", core::rect<s32> (10, 180, 45, 200),
			true, cur_tab, EvStep);
	env->addStaticText (L"Amount of iterations:", core::rect<s32> (10, 210, 120, 230), false, false, cur_tab);
	iters = env->addEditBox (L"5", core::rect<s32> (10, 230, 45, 250),
			true, cur_tab, EvIters);
	env->addStaticText (L"Gravity:", core::rect<s32> (200, 210, 300, 230), false, false, cur_tab);
	gravity = env->addEditBox (L"-2.5", core::rect<s32> (200, 230, 235, 250),
			true, cur_tab, EvGravity);
	env->addStaticText (L"Capsule mass:", core::rect<s32> (200, 110, 300, 130), false, false, cur_tab);
	capsule = env->addEditBox (L"10", core::rect<s32> (200, 130, 235, 150),
			true, cur_tab, EvCapsule);
}

void MainMenu::ShowOptions (bool state) {
	// TODO: How to set focus to that window?
	// That must not be in the background
	opts->setVisible (state);
}

int MainMenu::Apply () {
	// Here we save data to settings
	int k;
	if (res->getSelected () != settings->GetScreenResolution () ||
			vd->getSelected () != settings->GetDriverTypeIdx () ||
			fs->isChecked () != settings->IsFullscreen ())
		k = 0;
	else
		k = 1;
	settings->SetScreenResolution (res->getSelected ());
	settings->SetDriverType (vd->getSelected ());
	settings->SetFullscreen (fs->isChecked ());

	int imp;
	float fmp;
	// Boxes amount
	core::stringc s;
	s = boxes_amount->getText ();
	imp = (s32)atoi (s.c_str ());
	settings->SetBoxesAmount (imp * 10);
	// Box mass
	s = box_mass->getText ();
	fmp = (f32)atof (s.c_str ());
	settings->SetBoxMass (fmp);
	// Box size
	s = box_size->getText ();
	fmp = (f32)atof (s.c_str ());
	settings->SetBoxSize (fmp);
	// step
	s = step->getText ();
	fmp = (f32)atof (s.c_str ());
	settings->SetStep (fmp);
	// Gravity
	s = gravity->getText ();
	fmp = (f32)atof (s.c_str ());
	settings->SetGravity (fmp);
	// Iters
	s = iters->getText ();
	imp = (s32)atoi (s.c_str ());
	settings->SetIters (imp);
	// Capsule
	s = capsule->getText ();
	fmp = (f32)atof (s.c_str ());
	settings->SetCapsuleMass (fmp);

	EnableApply (false);
	EnableCancel (false);
	return k;
}

void MainMenu::Cancel () {
	// Here we call data from settings
	res->setSelected (settings->GetScreenResolution ());
	fs->setChecked (settings->IsFullscreen ());
	vd->setSelected (settings->GetDriverTypeIdx ());
	core::stringc s;
	// Boxes amount
	s = core::stringc (settings->GetBoxesAmount () / 10);
	boxes_amount->setText (core::stringw (s.c_str ()).c_str ());
	// Box mass
	s = core::stringc (settings->GetBoxMass ());
	box_mass->setText (core::stringw (s.c_str ()).c_str ());
	// Box size
	s = core::stringc (settings->GetBoxSize ());
	box_size->setText (core::stringw (s.c_str ()).c_str ());
	// Step
	s = core::stringc (settings->GetStep ());
	step->setText (core::stringw (s.c_str ()).c_str ());
	// Gravity
	s = core::stringc (settings->GetGravity ());
	gravity->setText (core::stringw (s.c_str ()).c_str ());
	// Iters
	s = core::stringc (settings->GetIters ());
	iters->setText (core::stringw (s.c_str ()).c_str ());
	// Capsule
	s = core::stringc (settings->GetCapsuleMass ());
	capsule->setText (core::stringw (s.c_str ()).c_str ());

	EnableApply (false);
	EnableCancel (false);
}

int MainMenu::Ok () {
	if (btn[1]->isEnabled ()) {
		if (Apply () == 0)
			return 0;
		else
			return 1;
	}
	else {
		ShowOptions (false);
		return -1;
	}
}

void MainMenu::EnableApply (bool state) {
	btn[1]->setEnabled (state);
}

void MainMenu::EnableCancel (bool state) {
	btn[2]->setEnabled (state);
}

void MainMenu::UpdateFPS (int fps) {
	core::stringw s = (core::stringw) fps;
	fps_text[0]->setText (s.c_str ());
	fps_text[1]->setText (s.c_str ());
}

