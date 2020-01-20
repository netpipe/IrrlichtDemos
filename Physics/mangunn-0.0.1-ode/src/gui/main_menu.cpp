// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "main_menu.h"
#include "../misc/help.h"
#include "../misc/events.h"
#include <iostream>
using namespace irr;
using namespace gui;
using namespace video;
using namespace core;
using namespace scene;
using namespace std;

MainMenu::MainMenu (Registry *rg, MapScene *m) {
	ms = m;
	reg = rg;
	Settings *settings = reg->settings;
	IGUIEnvironment *env = reg->env;
	last_fps = -1;
	// Appearance
	IGUISkin *skin = env->createSkin (EGST_WINDOWS_METALLIC);
	env->setSkin (skin);
	stringc smp = settings->GetDataDir () + L"textures/fontlucida.png";
	IGUIFont *font = env->getFont (smp.c_str ());
	if (font)
		skin->setFont (font);
	else
		Report ("Couldn't load font. Using defaults", "MainMenu::MainMenu");
	skin->setColor (EGDC_3D_FACE, SColor (255, 205, 205, 255));
	// Title
	smp = settings->GetDataDir () + L"textures/title.png";
	title = env->addImage (reg->drv->getTexture (smp.c_str ()),
			core::position2d<s32> (settings->GetScreenWidth () / 2 - 256,
				settings->GetScreenHeight () / 2 - 256));
	// FPS text
	fps_text[0] = env->addStaticText (L"FPS", core::rect<s32> (
				settings->GetScreenWidth () - 40,
				settings->GetScreenHeight () - 30,
				settings->GetScreenWidth () - 10,
				settings->GetScreenHeight () - 10),
			false, false, 0);
	// Used for shadow-like FPS text effect
	fps_text[1] = env->addStaticText (L"FPS", core::rect<s32> (
				settings->GetScreenWidth () - 41,
				settings->GetScreenHeight () - 31,
				settings->GetScreenWidth () - 10,
				settings->GetScreenHeight () - 10),
			false, false, 0);
	fps_text[1]->setOverrideColor (SColor (255, 200, 100, 100));
	// Buttons' X coordinates
	const int BCS = settings->GetScreenWidth () / 4;
	// Options button
	options_btn = env->addButton (core::rect<s32> (
				BCS - 40, settings->GetScreenHeight () - 30,
				BCS + 40, settings->GetScreenHeight () - 10),
			0, EV_MENU_OPTIONS, L"Options");
	about_btn = env->addButton (core::rect<s32> (
				BCS * 2 - 40, settings->GetScreenHeight () - 30,
				BCS * 2 + 40, settings->GetScreenHeight () - 10),
			0, EV_MENU_ABOUT, L"About");
	quit_btn = env->addButton (core::rect<s32> (
				BCS * 3 - 40, settings->GetScreenHeight () - 30,
				BCS * 3 + 40, settings->GetScreenHeight () - 10),
			0, EV_MENU_QUIT, L"Quit");
	reg->gui_shown = false;
	Toggle ();

	om = new OptionsMenu (reg);

	// TODO: Remember all open windows
	switch (reg->openwin) {
		case 0:
			om->Show ();
			break;
		default:
			break;
	}
	reg->openwin = -1;
#ifdef USE_PLAYER_RAY
	ray_text = env->addStaticText (L"ray", rect<s32> (10,
				settings->GetScreenHeight () - 30, 100,
				settings->GetScreenHeight () - 10), false, false, 0);
#endif
	// Show main menu
	//Show ();
}

MainMenu::~MainMenu () {
	delete om;
}

void MainMenu::DrawAddons () {
	// GUI is on, darken background
	if (reg->gui_shown) {
		reg->drv->draw2DRectangle (SColor (150, 0, 0, 0),
				reg->drv->getViewPort ());
	}
	int fps = reg->drv->getFPS ();
	if (fps != last_fps) {
		last_fps = fps;
		fps_text[0]->setText (core::stringw (fps).c_str ());
		fps_text[1]->setText (core::stringw (fps).c_str ());
#ifdef USE_PLAYER_RAY
		ray_text->setText (core::stringw (ms->GetRayLength ()).c_str ());
#endif
	}
}

bool MainMenu::OnEvent (const irr::SEvent &e) {
	// One-time key press
	if (e.EventType == EET_KEY_INPUT_EVENT &&
			e.KeyInput.PressedDown == false)
		switch (e.KeyInput.Key) {
			case KEY_ESCAPE:
				Toggle ();
				return true;
				break;
			default:
				break;
		}
	// GUI events
	if (e.EventType == EET_GUI_EVENT) {
		s32 id = e.GUIEvent.Caller->getID ();
		switch (e.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EV_MENU_QUIT:
						reg->quit = true;
						return true;
						break;
					case EV_MENU_OPTIONS:
						om->Show ();
						return true;
						break;
						/*
					case EV_OPTIONS_APPLY: {
						unsigned short ai = mm->GetOptionsMenu ()->Apply ();
						if (ai) {
							open_window = ai;
							restart = true;
						}
				    }
					break;
					case EV_OPTIONS_CANCEL:
						mm->GetOptionsMenu ()->Cancel ();
						break;
					case EV_OPTIONS_OK:
						if (mm->GetOptionsMenu ()->Ok ())
							restart = true;
						break;
						*/
					case EV_MENU_ABOUT:
						//mm->GetAboutMenu ()->Show (true);
						return true;
						break;
						/*
					case EV_ABOUT_OK:
						mm->GetAboutMenu ()->Show (false);
						break;
						*/
				}
				break;
				/*
			case EGET_COMBO_BOX_CHANGED:
				switch (id) {
					case EV_OPTIONS_RES:
					case EV_OPTIONS_VD:
					case EV_OPTIONS_BPP:
						mm->GetOptionsMenu ()->EnableApplyBtns (true, true);
						break;
				}
				return true;
				break;
			case EGET_CHECKBOX_CHANGED:
				switch (id) {
					case EV_OPTIONS_FS:
						mm->GetOptionsMenu ()->EnableApplyBtns (true, true);
						break;
				}
				return true;
				break;
				*/
			default:
				break;
		}
	}
	/*
	// Mouse events
	if (!gui_shown && ev.EventType == EET_MOUSE_INPUT_EVENT) {
		switch (ev.MouseInput.Event) {
			case EMIE_RMOUSE_PRESSED_DOWN:
				if (holding) {
					holding = false;
					player->ReleaseBody ();
					pulling = false;
				}
				else
					pulling = true;
				return true;
				break;
			case EMIE_RMOUSE_LEFT_UP:
				pulling = false;
				return true;
				break;
			case EMIE_LMOUSE_LEFT_UP:
				if (holding) {
					holding = false;
					player->ShootBody ();
					pulling = false;
				}
				if (editing) {
					n = smgr->getSceneCollisionManager ()->getSceneNodeFromCameraBB (cam);
					if (n && n->getID () != -1) {
						mm->SetSelectedNode (n);
					}
				}
				return true;
				break;
			default:
				return false;
		}
	}
	*/
	if (om && om->OnEvent (e))
		return true;
	return false;
}

void MainMenu::Show (bool state) {
	quit_btn->setVisible (state);
	about_btn->setVisible (state);
	options_btn->setVisible (state);
	title->setVisible (state);
	reg->gui_shown = state;
	// If we hide GUI, we hide all windows
	if (reg->gui_shown == false) {
		om->Show (false);
		//about->Show (false);
	}
}

void MainMenu::Toggle () {
	Show ((reg->gui_shown = !reg->gui_shown));
	reg->dev->getCursorControl ()->setVisible (reg->gui_shown);
	reg->cam->setInputReceiverEnabled (!reg->gui_shown);
	// If we switched from GUI, then position mouse to center,
	// so that its movements while GUI was on don't affect our
	// game movements
	if (!reg->gui_shown)
		reg->dev->getCursorControl ()->setPosition (
				reg->settings->GetScreenWidth () / 2,
				reg->settings->GetScreenHeight () / 2);
}

