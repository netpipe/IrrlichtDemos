// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "core.h"
#include <iostream>
#include "events.h"
using namespace std;
using namespace irr;
using namespace gui;
using namespace scene;
using namespace video;

#define DEBUG
#define USE_TIMESTEP

CoreGame::CoreGame () {
	// Load settings
	settings = new Settings ("data/settings");
	quit = false;
	first_run = true;
	open_window = 0;
	// Create physics world
	phys_world.Create (settings->GetGravity ());
	// Cam default pos and target for each team
	cam_pos = core::vector3df (-0.3, 10.7, -17);
	cam_target = core::vector3df (-0.3, 10.1, -16.3);
	cam_pos2 = core::vector3df (-0.3, 10.7, 17);
	cam_target2 = core::vector3df (-0.3, 10.1, 16.3);
	// Amount of boxes
	amount = 16;
	// First turn is red's
	red_play = true;
	// We need this in NearCallback to define which team's box we hit
	blue_data = new char ();
	red_data = new char ();
	// Previous teams' scores
	prev_bs = 0;
	prev_rs = 0;
	shooted = false;
	game_over = false;
}

CoreGame::~CoreGame () {
	phys_world.Destroy ();
	delete settings;
}

void CoreGame::NearCallback (void *data, dGeomID o1, dGeomID o2) {
	CoreGame *pthis = (CoreGame*) data;
	if (pthis)
		pthis->NearCallback (o1, o2);
}

void CoreGame::NearCallback (dGeomID o1, dGeomID o2) {
	dBodyID b1 = dGeomGetBody (o1);
	dBodyID b2 = dGeomGetBody (o2);
	// Check if the sphere can hit a box
	// first: o1 - sphere, o2 - box
	// second: o2 - sphere, o1 - box
	// Actually, we can check only the first (or the second) case,
	// but I don't know if it's legal
	if ((dGeomGetClass (o1) == dSphereClass && dGeomGetClass (o2) ==
				dBoxClass) ||
			(dGeomGetClass (o1) == dBoxClass && dGeomGetClass (o2) ==
			 dSphereClass)) {
		// If the blue play and hit red box with a sphere, then
		// don't check for collision
		// We check for addresses equality to define if we deal with a red
		// or a blue box
		if ((!red_play && (int*)red_data == (int*)dBodyGetData (b2)) ||
				(!red_play && (int*)red_data == (int*)dBodyGetData (b1)))
			return;
		// And vice versa
		if ((red_play && (int*)blue_data == (int*)dBodyGetData (b2)) ||
				(red_play && (int*)blue_data == (int*)dBodyGetData (b1)))
			return;
	}

	// Avarage
	if (b1 && b2 && dAreConnected (b1, b2))
		return;
	const int N = 4;
	dContact contact[N];
	int n = dCollide (o1, o2, N, &contact[0].geom, sizeof (dContact));
	if (n > 0)
		for (unsigned short i = 0; i < n; i++) {
			contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
				dContactSoftERP | dContactSoftCFM | dContactApprox1;
			contact[i].surface.mu = dInfinity;
			contact[i].surface.slip1 = 0.0;
			contact[i].surface.slip2 = 0.0;
			contact[i].surface.soft_erp = 0.2;
			contact[i].surface.soft_cfm = 0.0001;
			dJointID c = dJointCreateContact (phys_world.world,
					phys_world.contact_group, &contact[i]);
			dJointAttach (c, b1, b2);
		}
}

bool CoreGame::OnEvent (const SEvent& ev) {
	// I (kornerr) don't know whether break is necessary after return,
	// so let's be consistent

	// Key events
	// One-time press
	if (ev.EventType == EET_KEY_INPUT_EVENT &&
			ev.KeyInput.PressedDown == false)
		switch (ev.KeyInput.Key) {
			case KEY_F10:
				quit = true;
				return true;
				break;
			case KEY_ESCAPE:
				ToggleGUI ();
				return true;
				break;
			case KEY_KEY_C: {
				core::vector3df cpos =
					smgr->getActiveCamera ()->getPosition ();
				core::vector3df ctar =
					smgr->getActiveCamera ()->getTarget ();
				PrintVector (cpos);
				PrintVector (ctar);
			}
			return true;
			break;
			default:
				return false;
		}

	// GUI events
	if (ev.EventType == EET_GUI_EVENT) {
		s32 id = ev.GUIEvent.Caller->getID ();
		switch (ev.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EV_QUIT:
						quit = true;
						break;
					case EV_OPTIONS:
						mm->GetOptionsMenu ()->Show (true);
						break;
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
					case EV_ABOUT:
						mm->GetAboutMenu ()->Show (true);
						break;
					case EV_ABOUT_OK:
						mm->GetAboutMenu ()->Show (false);
						break;
				}
				return true;
				break;
			case EGET_EDITBOX_ENTER:
				switch (id) {
					case EV_OPTIONS_STEP:
					case EV_OPTIONS_ITERS:
					case EV_OPTIONS_GRAVITY:
						mm->GetOptionsMenu ()->EnableApplyBtns (true);
						break;
				}
				return true;
				break;
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
			default:
				return false;
		}
	}
	// Mouse events
	if (!gui_shown && ev.EventType == EET_MOUSE_INPUT_EVENT) {
		switch (ev.MouseInput.Event) {
			case EMIE_LMOUSE_LEFT_UP:
				Shoot ();
				return true;
				break;
			default:
				return false;
		}
	}
	return false;
}

bool CoreGame::Quit () {
	// Store cam state between restarts
	if (red_play) {
		cam_pos = smgr->getActiveCamera ()->getPosition ();
		cam_target = smgr->getActiveCamera ()->getTarget ();
	}
	else {
		cam_pos2 = smgr->getActiveCamera ()->getPosition ();
		cam_target2 = smgr->getActiveCamera ()->getTarget ();
	}
	// Delete Irrlicht device
	dev->drop ();
	// Delete main menu
	delete mm;
	if (restart) {
		first_run = false;
		// Recreate game over logo
		game_over = false;
		return true;
	}
	return false;
}

void CoreGame::Run () {
	// These variables are used to calculate correct timestep
	// Article about timestep: http://gaffer.org/articles/
	// May be you're the one who can make "The final touch"?
	float accum = 0,
		  delta = 0;
	u32 cur_time = 0;
	const int ITERS = settings->GetIters ();
	const float STEP = settings->GetStep ();
	ITimer *timer = dev->getTimer ();
	cur_time = timer->getRealTime ();

	while (dev->run () && !quit && !restart) {
		drv->beginScene (true, true, SColor (0, 200, 200, 200));

		if (!gui_shown) {
#ifdef USE_TIMESTEP
			delta = (timer->getRealTime () - cur_time) * 0.001f;
			// If FPS are too low, lower delta
			/*
			if (delta > STEP * 40)
				delta = STEP * 40;
			*/
			cur_time = timer->getRealTime ();
			accum += delta;
			while (accum >= settings->GetStep ()) {
#endif
				Simulate (STEP, ITERS);
#ifdef USE_TIMESTEP
				accum -= STEP;
			}
#endif
			// Update graphics representations
			for (unsigned short i = 0; i < amount; i++) {
				box[i]->Update ();
			}
			sphere->Update ();
			UpdateScore ();
		}
#ifdef USE_TIMESTEP
		else
			cur_time = timer->getRealTime ();
#endif
		// Draw scene nodes
		smgr->drawAll ();
		// Draw some additional main menu stuff
		mm->DrawAddons (red_play);
		// Update FPS
		mm->ShowFPS (drv->getFPS ());
		// Draw GUI
		env->drawAll ();
		drv->endScene ();
	}
}

void CoreGame::Shoot () {
	if (game_over)
		return;
	if (shooted)
		return;
	ICameraSceneNode *cam = smgr->getActiveCamera ();
	core::vector3df pos = cam->getPosition ();
	/*
	const int DIST = 10;
	pos.X += DIST;
	pos.Y += DIST;
	pos.Z += DIST;
	*/
	sphere->Shoot (pos, (cam->getTarget () - cam->getPosition ()),
			abs (settings->GetGravity ()) * 1);
	shooted = true;
}

void CoreGame::Simulate (float dt, int iters) {
	dSpaceCollide (phys_world.space, this, NearCallback);
	dWorldSetQuickStepNumIterations (phys_world.world, iters);
	dWorldQuickStep (phys_world.world, dt);
	dJointGroupEmpty (phys_world.contact_group);
}

void CoreGame::Start () {
	restart = false;
	// Create renderer
	dev = createDevice (settings->GetDriverType (),
			core::dimension2du (settings->GetScreenWidth (),
				settings->GetScreenHeight ()), settings->GetBPP (),
			settings->IsFullScreen (), true, false, this);
	drv = dev->getVideoDriver ();
	smgr = dev->getSceneManager ();
	env = dev->getGUIEnvironment ();
	// Window caption
	dev->setWindowCaption (L"Chapaev");
	// Create main menu
	mm = new MainMenu (dev, settings, open_window);
	open_window = 0;
	// Create camera
	SKeyMap key_map[8];

	key_map[0].Action = EKA_MOVE_FORWARD;
	key_map[0].KeyCode = KEY_UP;
	key_map[1].Action = EKA_MOVE_FORWARD;
	key_map[1].KeyCode = KEY_KEY_W;

	key_map[2].Action = EKA_MOVE_BACKWARD;
	key_map[2].KeyCode = KEY_DOWN;
	key_map[3].Action = EKA_MOVE_BACKWARD;
	key_map[3].KeyCode = KEY_KEY_S;

	key_map[4].Action = EKA_STRAFE_LEFT;
	key_map[4].KeyCode = KEY_LEFT;
	key_map[5].Action = EKA_STRAFE_LEFT;
	key_map[5].KeyCode = KEY_KEY_A;

	key_map[6].Action = EKA_STRAFE_RIGHT;
	key_map[6].KeyCode = KEY_RIGHT;
	key_map[7].Action = EKA_STRAFE_RIGHT;
	key_map[7].KeyCode = KEY_KEY_D;

	smgr->addCameraSceneNodeFPS (0, 50, 10, -1, key_map, 8);
	if (red_play) {
		smgr->getActiveCamera ()->setPosition (cam_pos);
		smgr->getActiveCamera ()->setTarget (cam_target);
	}
	else {
		smgr->getActiveCamera ()->setPosition (cam_pos2);
		smgr->getActiveCamera ()->setTarget (cam_target2);
	}

	// Show GUI
	gui_shown = false;
	ToggleGUI ();

	// Initialize graphics and physics (if necessary)
	if (first_run) {
		/*
		map = new WrapMap (phys_world.space, dev,
				core::vector3df (-1300, -144, -1249),
				"data/maps/map-20kdm2.pk3", "20kdm2.bsp");
		*/
		map = new WrapMap (phys_world.space, dev, core::vector3df (0, 0, 0),
				"data/maps/map.obj");
		// Create "blue" draughts
		for (unsigned short i = 0; i < amount / 2; i++) {
			WrapBox *tmp = new WrapBox (phys_world.world,
					phys_world.space,
					core::vector3df (-8.75 + 2.5 * i, 4, 8.75), 2, 2,
					0.5, 0.5, 200, dev, "data/textures/blue.jpg");
			// We need this in NearCallback
			tmp->SetData (blue_data);
			box.push_back (tmp);
		}
		// Create "blue" draughts
		for (unsigned short i = amount / 2; i < amount; i++) {
			WrapBox *tmp = new WrapBox (phys_world.world,
					phys_world.space,
					core::vector3df (-28.75 + 2.5 * i, 4, -8.75), 2, 2,
					0.5, 0.5, 200, dev, "data/textures/red.jpg");
			// We need this in NearCallback
			tmp->SetData (red_data);
			box.push_back (tmp);
		}
		// Create shoot ball
		sphere = new WrapSphere (phys_world.world, phys_world.space,
				core::vector3df (0, 5, 0), 5, 0.5, 10, 100, 200, dev,
				"data/textures/wall.jpg");
	}
	else {
		map->CreateNode (dev);
		for (unsigned short i = 0; i < amount; i++)
			box[i]->CreateNode (dev);
		sphere->CreateNode (dev);
	}
}

void CoreGame::ToggleGUI () {
	mm->Show ((gui_shown = !gui_shown));
	smgr->getActiveCamera ()->setInputReceiverEnabled (!gui_shown);
	dev->getCursorControl ()->setVisible (gui_shown);
	// If we switched from GUI, then position mouse to center,
	// so that its movements while GUI was on don't affect our
	// game movements
	if (!gui_shown)
		dev->getCursorControl ()->setPosition (
				settings->GetScreenWidth () / 2,
				settings->GetScreenHeight () / 2);
}

void CoreGame::UpdateScore () {
	// Update when all boxes are disabled
	// If you use software renderer, you need to lower disable steps
	for (unsigned short i = 0; i < amount; i++)
		if (box[i]->IsEnabled ())
			return;
	unsigned short bs = 0,
				   rs = 0;
	// red score (how many blue boxes has fallen)
	for (unsigned short i = 0; i < amount / 2; i++) {
		const dReal *pos = box[i]->GetPosition ();
		// If Y coordinate is less then the chess board
		if (pos[1] < -2)
			rs++;
	}
	// blue score (how many red boxes has fallen)
	for (unsigned short i = amount / 2; i < amount; i++) {
		const dReal *pos = box[i]->GetPosition ();
		// If Y coordinate is less then the chess board
		if (pos[1] < -2)
			bs++;
	}
	mm->SetBlueScore (bs);
	mm->SetRedScore (rs);
	// Don't eat video memory
	if (game_over)
		return;
	if (shooted)
		if (!sphere->IsEnabled () || sphere->GetPosition ()[1] < -2) {
			shooted = false;
			// The red couldn't made a blue box fall
			if (red_play) {
				if (rs == prev_rs) {
					// Then we change team
					red_play = !red_play;
					// Position camera "correctly"
					cam_pos = smgr->getActiveCamera ()->getPosition ();
					cam_target = smgr->getActiveCamera ()->getTarget ();
					smgr->getActiveCamera ()->setPosition (cam_pos2);
					smgr->getActiveCamera ()->setTarget (cam_target2);
				}
				// Otherwise keep the red's turn to play
			}
			// Do the same for the blue team
			else {
				if (bs == prev_bs) {
					red_play = !red_play;
					cam_pos2 = smgr->getActiveCamera ()->getPosition ();
					cam_target2 = smgr->getActiveCamera ()->getTarget ();
					smgr->getActiveCamera ()->setPosition (cam_pos);
					smgr->getActiveCamera ()->setTarget (cam_target);
				}
			}
		}
	prev_bs = bs;
	prev_rs = rs;
	// Game over
	if (bs == 8 && rs != 8) {
		game_over = true;
		env->addImage (drv->getTexture ("data/textures/the_blue_won.png"),
					core::position2d<s32> (
						settings->GetScreenWidth () / 2 - 256, 20));
	}
	if (rs == 8 && bs != 8) {
		game_over = true;
		env->addImage (drv->getTexture ("data/textures/the_red_won.png"),
					core::position2d<s32> (
						settings->GetScreenWidth () / 2 - 256, 20));
	}
	if (rs == 8 && bs == 8) {
		game_over = true;
		env->addImage (drv->getTexture ("data/textures/draw.png"),
					core::position2d<s32> (
						settings->GetScreenWidth () / 2 - 64, 20));
	}
}

int main () {
	// ATTENTION: Make sure this "core" doesn't collide with
	// Irrlicht's "core" class
	CoreGame *core;
restart_label:
	core->Start ();
	core->Run ();
	if (core->Quit ())
		goto restart_label;

	return 0;
}

