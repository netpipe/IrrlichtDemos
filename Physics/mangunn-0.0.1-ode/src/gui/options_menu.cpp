// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "options_menu.h"
#include "../misc/events.h"
#include "../misc/help.h"
using namespace irr;
using namespace gui;
using namespace core;

OptionsMenu::OptionsMenu (Registry *rg) {
	reg = rg;
	Settings *settings = reg->settings;
	IGUIEnvironment *env = reg->env;
	vp_changed = false;
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
			EV_OPTIONS_MENU_OK, L"OK");
	cancel_btn = env->addButton (rect<s32> (440, 370, 490, 390), wnd,
			EV_OPTIONS_MENU_CANCEL, L"Cancel");
	apply_btn = env->addButton (rect<s32> (500, 370, 550, 390), wnd,
			EV_OPTIONS_MENU_APPLY, L"Apply");
	
	IGUITab *tab = 0;
	// Create Video tab
	tab = tabc->addTab (L"Video");
	env->addStaticText (L"Screen resolution:", rect<s32> (10, 10, 150, 30),
			false, false, tab);
	res = env->addComboBox (rect<s32> (10, 30, 150, 50), tab,
			EV_OPTIONS_MENU_RES);
	res->addItem (L"640x480");
	res->addItem (L"800x600");
	res->addItem (L"1024x768");
	fs = env->addCheckBox (false, rect<s32> (10, 60, 150, 80), tab,
			EV_OPTIONS_MENU_FS, L"FullScreen");
	// Create Misc tab
	tab = tabc->addTab (L"Misc");
	env->addStaticText (L"Map:", rect<s32> (10, 10, 150, 30), false, false,
			tab);
	map = env->addEditBox (L"", rect<s32> (10, 30, 530, 50), true, tab,
			EV_OPTIONS_MENU_MAP);
	OnCancel ();
}

OptionsMenu::~OptionsMenu () {

}

void OptionsMenu::OnApply () {
	// Save settings before renderer restart (even if we don't restart :))
	SaveSettings ();
	// Video parameters were changed, query renderer restart
	if (vp_changed) {
		// Leave Options window open after restart
		reg->openwin = 0;
		reg->restart = true;
		return;
	}
	EnableApplyBtns (false);
}

void OptionsMenu::OnCancel () {
	LoadSettings ();
	EnableApplyBtns (false);
}

bool OptionsMenu::OnEvent (const irr::SEvent &e) {
	// GUI events
	if (e.EventType == EET_GUI_EVENT) {
		s32 id = e.GUIEvent.Caller->getID ();
		switch (e.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EV_OPTIONS_MENU_APPLY:
						OnApply ();
						return true;
						break;
					case EV_OPTIONS_MENU_CANCEL:
						OnCancel ();
						return true;
						break;
					case EV_OPTIONS_MENU_OK:
						OnOk ();
						return true;
						break;
					default:
						break;
				}
				break;
			case EGET_COMBO_BOX_CHANGED:
				switch (id) {
					case EV_OPTIONS_MENU_RES:
					case EV_OPTIONS_MENU_FS:
						EnableApplyBtns (true, true);
						return true;
						break;
					default:
						break;
				}
				break;
			case EGET_CHECKBOX_CHANGED:
				switch (id) {
					case EV_OPTIONS_MENU_FS:
						EnableApplyBtns (true, true);
						return true;
						break;
					default:
						break;
				}
				break;
			case EGET_EDITBOX_ENTER:
				switch (id) {
					case EV_OPTIONS_MENU_MAP:
						EnableApplyBtns (true);
						return true;
						break;
					default:
						break;
				}
			default:
				break;
		}
	}
	return false;
}

void OptionsMenu::EnableApplyBtns (bool state, bool video_params_changed) {
	apply_btn->setEnabled (state);
	cancel_btn->setEnabled (state);
	vp_changed = video_params_changed;
}

void OptionsMenu::OnOk () {
	// If we changed smth, apply changes
	if (cancel_btn->isEnabled ()) {
		SaveSettings ();
		if (vp_changed) {
			reg->restart = true;
			return;
		}
	}
	// Hide Options menu if all changes were applied
	Show (false);
}

void OptionsMenu::Show (bool state) {
	wnd->setVisible (state);
}

void OptionsMenu::LoadSettings () {
	Settings *settings = reg->settings;
	// Video tab
	res->setSelected (settings->GetScreenRes ());
	fs->setChecked (settings->IsFullScreen ());
	map->setText (stringw (settings->GetMapFile ().c_str ()).c_str ());
}

void OptionsMenu::SaveSettings () {
	Settings *settings = reg->settings;
	// Video tab
	// This idea came from Stroustrup's C++ book :P
	settings->SetScreenRes (res->getSelected ()).SetFullScreen (fs->isChecked ());
	settings->SetMapFile (stringc (map->getText ()));
}

