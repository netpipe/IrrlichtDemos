// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "main_menu.h"
#include "../help.h"
#include "../events.h"
using namespace irr;
using namespace gui;
using namespace video;

MainMenu::MainMenu (IrrlichtDevice *dev, Settings *settings,
		unsigned short open_window) {
	MainMenu::dev = dev;
	MainMenu::settings = settings;
	IGUIEnvironment *env = dev->getGUIEnvironment ();
	shown = false;
	last_fps = -1;
	// Appearance
	IGUISkin *skin = env->getSkin ();
	IGUIFont *font = env->getFont ("data/textures/fonthaettenschweiler.png");
	if (font)
		skin->setFont (font);
	else
		Report ("Couldn't load font", "MainMenu::MainMenu");
	skin->setColor (EGDC_3D_FACE, SColor (255, 205, 205, 255));
	// Title
	title = env->addImage (
			dev->getVideoDriver ()->getTexture ("data/textures/title.png"),
			core::position2d<s32> (settings->GetScreenWidth () / 2 - 256,
				settings->GetScreenHeight () / 2 - 64));
	// FPS text
	fps_text[0] = env->addStaticText (L"FPS", core::rect<s32> (
				settings->GetScreenWidth () - 40,
				settings->GetScreenHeight () - 30,
				settings->GetScreenWidth () - 10,
				settings->GetScreenHeight () - 10),
			false, false, 0);
	// Used for shadow
	fps_text[1] = env->addStaticText (L"FPS", core::rect<s32> (
				settings->GetScreenWidth () - 41,
				settings->GetScreenHeight () - 31,
				settings->GetScreenWidth () - 10,
				settings->GetScreenHeight () - 10),
			false, false, 0);
	fps_text[1]->setOverrideColor (SColor (255, 200, 100, 100));
	// Buttons amount (minus one?)
	// Awful calculation
	const int BA = 2;
	// Buttons' X coordinates
	const int BCS = settings->GetScreenWidth () / 2 / BA;
	// Options button
	options_btn = env->addButton (core::rect<s32> (
				BCS - 40, settings->GetScreenHeight () - 30,
				BCS + 40, settings->GetScreenHeight () - 10),
			0, EV_OPTIONS, L"Options");
	about_btn = env->addButton (core::rect<s32> (
				BCS * 2 - 40, settings->GetScreenHeight () - 30,
				BCS * 2 + 40, settings->GetScreenHeight () - 10),
			0, EV_ABOUT, L"About");
	quit_btn = env->addButton (core::rect<s32> (
				BCS * 3 - 40, settings->GetScreenHeight () - 30,
				BCS * 3 + 40, settings->GetScreenHeight () - 10),
			0, EV_QUIT, L"Quit");
	// Create Options menu window
	opts = new OptionsMenu (env, settings);
	// Create About menu window
	about = new AboutMenu (env, settings);
	// TODO: Remember all open windows
	// If we restarted renderer, check if we need to open some window
	switch (open_window) {
		case 1:
			opts->Show (true);
			break;
		default:
			break;
	}
	// Show main menu
	Show (true);
	// Gameplay stuff
	// Blue score
	blue_score[1] = env->addStaticText (L"Score: 0",core::rect<s32> (
				11, 11, 101, 31), false, false, 0);
	blue_score[0] = env->addStaticText (L"Score: 0",core::rect<s32> (
				10, 10, 100, 30), false, false, 0);
	blue_score[1]->setOverrideColor (SColor (255, 255, 255, 255));
	blue_score[0]->setOverrideColor (SColor (255, 50, 50, 255));
	// Red score
	red_score[0] = env->addStaticText (L"Score: 0",core::rect<s32> (
				settings->GetScreenWidth () - 60, 10,
				settings->GetScreenWidth () - 10, 30), false, false, 0);
	red_score[1] = env->addStaticText (L"Score: 0",core::rect<s32> (
				settings->GetScreenWidth () - 61, 9,
				settings->GetScreenWidth () - 11, 29), false, false, 0);
	red_score[1]->setOverrideColor (SColor (255, 255, 150, 150));
}

MainMenu::~MainMenu () {
	delete opts;
	delete about;
}

void MainMenu::DrawAddons (bool red) {
	IVideoDriver *drv = dev->getVideoDriver ();
	if (shown) {
		// Darkening background
		drv->draw2DRectangle (SColor (150, 0, 0, 0),
				dev->getVideoDriver ()->getViewPort ());
	}
	else {
		SColor cl;
		if (red)
			cl = SColor (255, 255, 0, 0);
		else
			cl = SColor (255, 0, 0, 255);
		// Draw aim cross
		drv->draw2DLine (core::position2d<s32> (
					settings->GetScreenWidth () / 2 - 9,
					settings->GetScreenHeight () / 2),
				core::position2d<s32> (settings->GetScreenWidth () / 2 - 2,
					settings->GetScreenHeight () / 2), cl);
		drv->draw2DLine (core::position2d<s32> (
					settings->GetScreenWidth () / 2 + 3,
					settings->GetScreenHeight () / 2),
				core::position2d<s32> (settings->GetScreenWidth () / 2 + 10,
					settings->GetScreenHeight () / 2), cl);
		drv->draw2DLine (core::position2d<s32> (
					settings->GetScreenWidth () / 2,
					settings->GetScreenHeight () / 2 + 9),
				core::position2d<s32> (settings->GetScreenWidth () / 2,
					settings->GetScreenHeight () / 2 + 3), cl);
		drv->draw2DLine (core::position2d<s32> (
					settings->GetScreenWidth () / 2,
					settings->GetScreenHeight () / 2 - 10),
				core::position2d<s32> (
					settings->GetScreenWidth () / 2,
					settings->GetScreenHeight () / 2 - 3), cl);
	}
}

OptionsMenu* MainMenu::GetOptionsMenu () {
	return opts;
}

AboutMenu* MainMenu::GetAboutMenu () {
	return about;
}

void MainMenu::SetBlueScore (unsigned short score) {
	core::stringw s;
	s = L"Score: ";
	s += score;
	blue_score[0]->setText (s.c_str ());
	blue_score[1]->setText (s.c_str ());
}

void MainMenu::SetRedScore (unsigned short score) {
	core::stringw s;
	s = L"Score: ";
	s += score;
	red_score[0]->setText (s.c_str ());
	red_score[1]->setText (s.c_str ());
}

void MainMenu::ShowFPS (int fps) {
	if (fps != last_fps) {
		last_fps = fps;
		fps_text[0]->setText (core::stringw (fps).c_str ());
		fps_text[1]->setText (core::stringw (fps).c_str ());
	}
}

void MainMenu::Show (bool state) {
	quit_btn->setVisible (state);
	about_btn->setVisible (state);
	options_btn->setVisible (state);
	title->setVisible (state);
	// We use this for darkening background when switching to GUI
	shown = state;
	// If we hide GUI, we hide all windows
	if (shown == false) {
		opts->Show (false);
		about->Show (false);
	}
}

