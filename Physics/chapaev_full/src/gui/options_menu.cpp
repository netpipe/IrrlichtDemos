// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "options_menu.h"
#include "../events.h"
#include "../help.h"
using namespace irr;
using namespace gui;
using namespace core;

OptionsMenu::OptionsMenu (IGUIEnvironment *env, Settings *settings) {
	vp_changed = false;
	OptionsMenu::settings = settings;
	const int dx = 560;
	const int dy = 400;
	const int x = settings->GetScreenWidth () / 2 - dx / 2;
	const int y = settings->GetScreenHeight () / 2 - dy / 2;
	// Create Options menu window at the center of a screen
	wnd = env->addWindow (rect<s32> (x, y, x + dx, y + dy), false,
			L"Options");
	// Hide close button
	wnd->getCloseButton ()->setVisible (false);
	// Hide Options menu itselft
	Show (false);
	// Create tab control
	IGUITabControl *tabc = env->addTabControl (rect<s32> (10, 30, 550, 360),
			wnd, true, false);
	// And essential buttons
	ok_btn = env->addButton (rect<s32> (380, 370, 430, 390), wnd,
			EV_OPTIONS_OK, L"OK");
	cancel_btn = env->addButton (rect<s32> (440, 370, 490, 390), wnd,
			EV_OPTIONS_CANCEL, L"Cancel");
	apply_btn = env->addButton (rect<s32> (500, 370, 550, 390), wnd,
			EV_OPTIONS_APPLY, L"Apply");
	
	IGUITab *tab = 0;
	// Create Video tab
	tab = tabc->addTab (L"Video");
	env->addStaticText (L"Video driver:", rect<s32> (10, 10, 150, 20), false,
			false, tab);
	vd = env->addComboBox (rect<s32> (10, 30, 150, 50), tab, EV_OPTIONS_VD);
	vd->addItem (L"OpenGL");
	vd->addItem (L"Software");
	env->addStaticText (L"Screen resolution:", rect<s32> (10, 70, 150, 80),
			false, false, tab);
	res = env->addComboBox (rect<s32> (10, 90, 150, 110), tab,
			EV_OPTIONS_RES);
	res->addItem (L"640x480");
	res->addItem (L"800x600");
	res->addItem (L"1024x768");
	env->addStaticText (L"Bits per pixel:", rect<s32> (10, 130, 150, 150),
			false, false, tab);
	bpp = env->addComboBox (rect<s32> (10, 150, 150, 170), tab,
			EV_OPTIONS_BPP);
	bpp->addItem (L"16");
	bpp->addItem (L"32");
	fs = env->addCheckBox (false, rect<s32> (10, 190, 150, 210), tab,
			EV_OPTIONS_FS, L"FullScreen");
	// Create Physics tab
	tab = tabc->addTab (L"Physics");
	env->addStaticText (L"Physics parameters change will take effect only "
			"after you restart the application", rect<s32> (200, 10, 500, 50),
			false, true, tab);
	env->addStaticText (L"Always press Enter whenever you change value in "
			"edit boxes", rect<s32> (200, 60, 500, 100), false, true, tab);
	env->addStaticText (L"Step:", rect<s32> (10, 10, 100, 30), false, false,
			tab);
	step = env->addEditBox (L"0.05", rect<s32> (10, 30, 100, 50), true, tab,
			EV_OPTIONS_STEP);
	env->addStaticText (L"Iterations:", rect<s32> (10, 70, 100, 90), false,
			false, tab);
	iters = env->addEditBox (L"10", rect<s32> (10, 90, 100, 110), true, tab,
			EV_OPTIONS_ITERS);
	env->addStaticText (L"Gravity:", rect<s32> (10, 130, 100, 150), false,
			false, tab);
	gravity = env->addEditBox (L"-9.81", rect<s32> (10, 150, 100, 170), true,
			tab, EV_OPTIONS_GRAVITY);

	Cancel ();
}

OptionsMenu::~OptionsMenu () {

}

unsigned short OptionsMenu::Apply () {
	// Save settings before renderer restart (even if we don't restart :))
	SaveSettings ();
	// Video parameters were changed, query renderer restart
	if (vp_changed)
		// Leave Options window open after restart
		return 1;
	// We don't do vp_changed = false, because all menus will be recreated
	// and, consequently, re-initialized
	EnableApplyBtns (false);
	return 0;
}

void OptionsMenu::Cancel () {
	LoadSettings ();
	EnableApplyBtns (false);
}

void OptionsMenu::EnableApplyBtns (bool state, bool video_params_changed) {
	apply_btn->setEnabled (state);
	cancel_btn->setEnabled (state);
	vp_changed = video_params_changed;
}

bool OptionsMenu::Ok () {
	// If we changed smth, apply changes
	if (cancel_btn->isEnabled ())
		return Apply ();
	// Hide Options menu if all changes were applied
	Show (false);
	return false;
}

void OptionsMenu::Show (bool state) {
	wnd->setVisible (state);
}

void OptionsMenu::LoadSettings () {
	// Video tab
	vd->setSelected (settings->GetDriverIndex ());
	res->setSelected (settings->GetScreenRes ());
	bpp->setSelected (settings->GetBPPIndex ());
	fs->setChecked (settings->IsFullScreen ());
	// Physics tab
	step->setText (stringw (settings->GetStep ()).c_str ());
	iters->setText (stringw (settings->GetIters ()).c_str ());
	gravity->setText (stringw (settings->GetGravity ()).c_str ());
}

void OptionsMenu::SaveSettings () {
	// Video tab
	// This idea came from Stroustrup's C++ book :P
	settings->SetDriverIndex (vd->getSelected ()).SetScreenRes (res->getSelected ()).SetBPPIndex (bpp->getSelected ()).SetFullScreen (fs->isChecked ());
	/*
	settings->SetDriverIndex (vd->getSelected ());
	settings->SetScreenRes (res->getSelected ());
	settings->SetBPPIndex (bpp->getSelected ());
	settings->SetFullScreen (fs->isChecked ());
	*/
	// Physics tab
	char buf[64];
	float fmp;
	// Save step
	strcpy (buf, stringc (step->getText ()).c_str ());
	// If we don't have dumb text in edit box, but meaningful value, save it
	if (sscanf (buf, "%f", &fmp))
		settings->SetStep (fmp);
	int imp;
	// Save iters
	strcpy (buf, stringc (iters->getText ()).c_str ());
	if (sscanf (buf, "%i", &imp))
		settings->SetIters (imp);
	// Save gravity
	strcpy (buf, stringc (gravity->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		settings->SetGravity (fmp);
}

