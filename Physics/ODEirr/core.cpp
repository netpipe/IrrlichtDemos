// Part of the OSRPG project (http://osrpgcreation.sf.net)
// The code is distributed under the terms of the GNU General Public License
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "core.h"

//=====================================
// GameCore class methods
//=====================================

GameCore::GameCore () {
	first_run = true;
	quit = false;
	restart = false;
	submit_ray = false;
	submit_player_move = false;
	ray_height = 20;

	settings = new GameSettings ("_data/settings");
	settings->Load ();

/*
	rot_x = 0;
	rot_y = 0;
	rot_z = 0;
*/

	LeaveWindowOpen ();
	CreateWorld ();

	// We assign the values to variables, instead of useing function
	// calls everywhere, because changed physics parameters will be
	// saved inside GameSettings. So they should be accessible only
	// whild application initialization
	wbam = settings->GetBoxesAmount ();
	iters = settings->GetIters ();
	wbsize = settings->GetBoxSize ();
	wbmass = settings->GetBoxMass ();
	step = settings->GetStep ();
	capsule_mass = settings->GetCapsuleMass ();
	gravity = settings->GetGravity ();
	// Allocate wbam amount of wall boxes
	wall_box = new BaseBox*[wbam];
	wbon = new bool[wbam];

	pd = new MiscPlayerDirection ();
}

GameCore::~GameCore () {
	// Save settings before exit
	settings->Save ();
	// Destroy world before exit
	DestroyWorld ();
}

void GameCore::LeaveWindowOpen (int index) {
	leave_window_open = index;
}

bool GameCore::GUIIsShown () {
	return show_gui;
}

void GameCore::Jump () {
	ray_height = 60;
}

void GameCore::ShowGUI (bool state) {
	show_gui = state;
	menu->ShowGUI (show_gui);
	// No cursor when we're playing
	dev->getCursorControl ()->setVisible (show_gui);
	// Make camera not rotate when we're in GUI
	smgr->getActiveCamera ()->setInputReceiverEnabled (!show_gui);
	// Position mouse cursor to screen center so that our mouse movements
	// in GUI don't affect movements in a game
	if (!show_gui) {
		dev->getCursorControl ()->setPosition (
				settings->GetScreenWidth () / 2,
				settings->GetScreenHeight () / 2);
	}
}

void GameCore::ToggleGUI () {
	ShowGUI (!show_gui);
}

void GameCore::CreateRenderer () {
	// TODO: Add other params to the Video menu
	dev = createDevice (settings->GetDriverType (),
			core::dimension2du (800,
				600), 32, 0);
	drv = dev->getVideoDriver ();
	smgr = dev->getSceneManager ();
	// Make our EvRec class instance handle events
	//dev->setEventReceiver (&receiver);
	dev->setWindowCaption (L"OSRPG First Person Shooter (incomplete yet :P)");
	env = dev->getGUIEnvironment ();
}

void GameCore::CreateInterface ()  {
	menu = new MainMenu ();
	menu->Create (env, settings, leave_window_open);
}

void GameCore::Start ()  {
	CreateRenderer ();

	CreateNOEBText ();
	CreateRayText ();
	CreateSpeedTexts ();

	if (!first_run) {
		// This takes place after renderer restart, so recreate only
		// nodes here
		rag_doll->CreateNode (smgr);
		map->CreateNode (dev);
		// Recreate wall
		for (int i = 0; i < wbam / 10; i++)
			for (int j = 0; j < 10; j++) {
				wall_box[i * 10 + j]->CreateNode (dev,
						"_data/media/wall.jpg");
			}
		shot->CreateNode (dev, "_data/media/water.jpg");
        //player->CreateNode (smgr);
        //weapon->Create (dev);
	}
	else {
		// First run, so create nodes and physics here
		first_run = false;
		rag_doll = new BaseRagDoll ();
		rag_doll->Create (world, space, 10, 30, 10, smgr);
		map = new BaseMap ();
		map->Create (space, 0, 0, 0, dev);
		player = new BasePlayer ();
		player->Create (world, space, 0, 40, 30, capsule_mass );// = mass , smgr
		// Create wall
		// Vertically
		const int CF = 5;
		for (int i = 0; i < CF; i++)
			for (int j = 0; j < wbam / CF; j++) {
				wall_box[i + j * CF] = new BaseBox ();
				wall_box[i + j * CF]->Create (world, space, i * wbsize + 50,
						- 70 - wbsize / 2 + j * wbsize, 0, wbsize, wbmass, dev,
						"_data/media/wall.jpg");
				// test
				wbon[i + j * CF] = true;
			}
		shot = new BaseBox ();
		shot->Create (world, space, 10, 10, 10, wbsize, wbmass, dev,
				"_data/media/water.jpg");
		ca[0] = new BaseCamAttachment ();
		ca[1] = new BaseCamAttachment ();
        //weapon->Create (dev);
	}

	camera.Create (smgr);

	// Hand
	ca[0]->Create (dev, &camera, "_data/models/righthand.obj", 0,
			core::vector3df (15, -10, 25), core::vector3df (-90, 168, 0));
	ca[0]->GetMeshNode ()->GetSceneNode ()->setScale (core::vector3df (2, 2, 2));
	// Weapon
	ca[1]->Create (dev, &camera, "_data/models/smg/smgweapon.ms3d",
			"_data/models/smg/smgtexture.jpg", core::vector3df (13, -3, 30),
			core::vector3df (22, 246, -16));

	/*
	camera.SetPosition (core::vector3df (0, 0, 0));
	camera.SetTarget (core::vector3df (0, 0, 1));

	// Hand
	LoadHandModel ();
	hand_node->GetSceneNode ()->setParent (camera.GetCamera ());
	hand_node->GetSceneNode ()->setScale (core::vector3df (2, 2, 2));
	core::vector3df pos (camera.GetPosition ());
	pos.Y -= 15;
	pos.X += 10;
	pos.Z += 25;
	hand_node->SetPosition (pos);
	hand_node->SetRotation (core::vector3df (-90, 168, 0));

	// Weapon
	weapon->GetMeshNode ()->GetSceneNode ()->setParent (camera.GetCamera ());
	pos = camera.GetPosition ();
	pos.Y -= 7.5;
	pos.X += 7.5;
	pos.Z += 30;
	weapon->GetMeshNode ()->SetPosition (pos);
	rot_x = 22;
	rot_y = 246;
	rot_z = -16;
	weapon->GetMeshNode ()->SetRotation (core::vector3df (rot_x, rot_y, rot_z));
	*/

	CreateInterface ();
	ShowGUI (true);
}

void GameCore::Restart () {
#ifdef DEBUG
	cout << "Restarting...\n";
#endif
	restart = true;

	camera.SavePosition ();
}
/*
void GameCore::LoadHandModel () {
	//hand_node = new MeshNode (smgr, "_data/models/handrigged.obj");
	hand_node = new MeshNode (smgr, "_data/models/righthand.obj");
}
*/

RunMsg GameCore::Run () {
	int last_fps = -1;

	// In the loop we use technique described here: http://www.gaffer.org/articles/
	// named "Fix you timestep!" It describes how to make game run at the same speed
	// on various PCs. Although, I don't know how to do "Final touch". May be YOU know?
	accum = 0;
	cur_time = 0;

	const float DIVISOR = 10000 * step;

	timer = dev->getTimer ();
	timer->tick ();
	float cur_time = timer->getTime ();

	while (dev->run () && !quit && !restart) {
		drv->beginScene (true, true, SColor (0, 200, 200, 200));

		if (!show_gui) {
			float new_time = timer->getTime ();
			float delta_time = (new_time - cur_time) / DIVISOR;
			cur_time = new_time;
			accum += delta_time;
			// physics
			while (accum >= step) {
				Simulate (step);
				// Update ray
				CommitPlayerMove ();
				CommitRay ();
				UpdateCamPos ();
				accum -= step;
			}
			// renderer
			rag_doll->Update (!show_gui);
			for (int i = 0; i <	wbam; i++)
				wall_box[i]->Update (!show_gui);
			shot->Update (!show_gui);
			player->Update (!show_gui);
			UpdateSpeedTexts ();
		}
		// When we do this, we don't have to wait for a long time after we used GUI
		// You can check it out yourself: comment the following 2 lines, run app,
		// use GUI for some time, then return to game. This is also described in
		// the above article
		else
			cur_time = timer->getTime ();

		smgr->drawAll ();
		// Draw the cross
		DrawAim ();
		dev->getGUIEnvironment ()->drawAll ();

		UpdateNumOfEnabledBoxes ();
		drv->endScene ();

		int fps = drv->getFPS ();
		if (fps != last_fps) {
			last_fps = fps;
			menu->UpdateFPS (fps);
		}
	}
	dev->drop ();

	if (restart) {
		restart = false;
		return RM_RESTART_RENDERER;
	}
	return RM_QUIT_APPLICATION;
}

void GameCore::UpdateCamPos () {
	const dReal *pos = dBodyGetPosition (player->GetPhysicsPlayer ()->player_body);
	camera.SetPosition (core::vector3df (pos[0], pos[1] + 30, pos[2]));

}

void GameCore::UpdateNumOfEnabledBoxes () {
	int num = 0;
	for (int i = 0; i < wbam; i++) {
		if (wall_box[i]->IsEnabled ())
			num++;
	}
	core::stringw s = L"Enabled boxes: ";
	s += num;
	s += L"     X/Z: ";
	s += core::stringw (camera.GetXZProportion ());
	noeb_text->setText (s.c_str ());
}

void GameCore::UpdateRayLength (float ray) {
	core::stringw s = L"Ray: ";
	s += core::stringw (ray);
	ray_text->setText (s.c_str ());
	SubmitRay (ray);
}

void GameCore::UpdateSpeedTexts () {
	const dReal *lvel = dBodyGetLinearVel (player->GetPhysicsPlayer ()->player_body);
	const dReal *force = dBodyGetForce (player->GetPhysicsPlayer ()->player_body);
	core::stringw s = L"AddForce: ";
	core::vector3df af (force[0], 0, force[2]);
	s += core::stringw (af.X);
	s += "  ";
	s += core::stringw (af.Z);
	add_force->setText (s.c_str ());

	s = L"LinearVel: ";
	core::vector3df lv (lvel[0], 0, lvel[2]);
	s += core::stringw (lv.X);
	s += "  ";
	s += core::stringw (lv.Z);
	linear_vel->setText (s.c_str ());
}

void GameCore::SubmitRay (float ray) {
	submit_ray = true;
	submitted_ray = ray;
}

void GameCore::SubmitPlayerMove (unsigned short index) {
	submit_player_move = true;
	if (index == 0)
		pd->forward = true;
	if (index == 1)
		pd->backward = true;
	if (index == 2)
		pd->left = true;
	if (index == 3)
		pd->right = true;
}

void GameCore::CommitPlayerMove () {
	if (!submit_player_move)
		return;
	submit_player_move = false;
	core::vector3df cam_vec (camera.GetTarget () - camera.GetPosition ());
	core::vector3df force_vec;
	if (pd->forward) {
		force_vec += cam_vec;
	}
	if (pd->backward) {
		force_vec -= cam_vec;
	}
	if (pd->left) {
		force_vec += core::vector3df (-cam_vec.Z, cam_vec.Y, cam_vec.X);
	}
	if (pd->right) {
		force_vec -= core::vector3df (-cam_vec.Z, cam_vec.Y, cam_vec.X);
	}
	force_vec.Y = 0;
	//const dReal *lvel = dBodyGetLinearVel (player->GetPhysicsPlayer ()->player_body);
	//float b = lvel[0] + lvel[1] + lvel[2];
	// We don't need capsule's mass, since it's stands on a ray, so no mass
	float a = 10000;
	//if (lvel[0] * lvel[0] + lvel[2] * lvel[2] < 50)
	//a /= (lvel [0] * lvel[0] + lvel[2] * lvel[2]);
	// TODO: We must somehow get speed and then decide how much force to apply
	dBodyAddForce (player->GetPhysicsPlayer ()->player_body, a * force_vec.X, a * force_vec.Y, a * force_vec.Z);
	//dBodySetLinearVel (player->GetPhysicsPlayer ()->player_body, a * force_vec.X, lvel[1], a * force_vec.Z);
	pd->Reset ();
}

void GameCore::CommitRay () {
	if (!submit_ray)
		return;
	submit_ray = false;
	// If ray is not higher than 15 units from below object and not "inside" this
	// below object, place it at the certain height
	float m =  10 * 9.81 * capsule_mass * (ray_height - submitted_ray);
	if (submitted_ray < ray_height && submitted_ray > -1) {
		dBodyAddForce (player->GetPhysicsPlayer ()->player_body, 0, m, 0);
		const dReal *lvel = dBodyGetLinearVel (player->GetPhysicsPlayer ()->player_body);
		dBodySetLinearVel (player->GetPhysicsPlayer ()->player_body, lvel[0], 0, lvel[2]);
	}
	if (ray_height == 60 && m < 3000)
		ray_height = 20;
	/*
	if (submitted_ray >= ray_height && submitted_ray <= 1000) {
		dBodyAddForce (player->GetPhysicsPlayer ()->player_body, 0, -2000, 0);
		//const dReal *lvel = dBodyGetLinearVel (player->GetPhysicsPlayer ()->player_body);
		//dBodySetLinearVel (player->GetPhysicsPlayer ()->player_body, lvel[0], -1.1 * abs (lvel[1]), lvel[2]);
	}
	*/
}

void GameCore::CreateSpeedTexts () {
	add_force = env->addStaticText (L"AddForce: ", core::rect<s32> (
				10, 10, 200, 30), false, false, 0);
	linear_vel = env->addStaticText (L"LinearVel: ", core::rect<s32> (
				10, 40, 200, 60), false, false, 0);
}

void GameCore::CreateNOEBText () {
	noeb_text = env->addStaticText (L"Enabled boxes: ", core::rect<s32> (
				10,
				settings->GetScreenHeight () - 30,
				200,
				settings->GetScreenHeight () - 10),
			false, false, 0);
	noeb_text->setOverrideColor (SColor (255, 200, 100, 100));
}

void GameCore::CreateRayText () {
	ray_text = env->addStaticText (L"Ray: ", core::rect<s32> (
				10,
				settings->GetScreenHeight () - 60,
				200,
				settings->GetScreenHeight () - 40),
			false, false, 0);
	ray_text->setOverrideColor (SColor (255, 255, 100, 100));
}

void GameCore::Quit () {
#ifdef DEBUG
	cout << "Quitting...\n";
#endif
	quit = true;
}

void GameCore::Simulate (float dt) {
	dSpaceCollide (space, 0, &NearCallback);
	//dWorldStep (world, 0.04);
	dWorldSetQuickStepNumIterations (world, iters);
	dWorldQuickStep (world, dt);

	dJointGroupEmpty (contact_group);
}

void GameCore::Shoot () {
	ICameraSceneNode *cam = smgr->getActiveCamera ();
	shot->Shoot (cam->getPosition (), cam->getTarget (), 100);
}

BasePlayer* GameCore::GetPlayer () {
	return player;
}
/*
void GameCore::RotX (bool state) {
	if (state)
		rot_x += 1;
	else
		rot_x -= 1;
	core::vector3df rot (weapon->GetMeshNode ()->GetRotation ());
	rot.X = rot_x;
	weapon->GetMeshNode ()->SetRotation (rot);
}

void GameCore::RotY (bool state) {
	if (state)
		rot_y += 1;
	else
		rot_y -= 1;
	core::vector3df rot (weapon->GetMeshNode ()->GetRotation ());
	rot.Y = rot_y;
	weapon->GetMeshNode ()->SetRotation (rot);
}

void GameCore::RotZ (bool state) {
	if (state)
		rot_z += 1;
	else
		rot_z -= 1;
	core::vector3df rot (weapon->GetMeshNode ()->GetRotation ());
	rot.Z = rot_z;
	weapon->GetMeshNode ()->SetRotation (rot);
}

void GameCore::PrintRot () {
	PrintVector (weapon->GetMeshNode ()->GetRotation ());
}
*/
void GameCore::DrawAim () {
	if (GUIIsShown ())
		return;
	drv->draw2DLine (
			core::position2d<s32> (
				settings->GetScreenWidth () / 2 - 9,
				settings->GetScreenHeight () / 2),
			core::position2d<s32> (
				settings->GetScreenWidth () / 2 - 2,
				settings->GetScreenHeight () / 2),
			SColor (255, 255, 0, 0)
			);
	drv->draw2DLine (
			core::position2d<s32> (
				settings->GetScreenWidth () / 2 + 3,
				settings->GetScreenHeight () / 2),
			core::position2d<s32> (
				settings->GetScreenWidth () / 2 + 10,
				settings->GetScreenHeight () / 2),
			SColor (255, 255, 0, 0)
			);
	drv->draw2DLine (
			core::position2d<s32> (
				settings->GetScreenWidth () / 2,
				settings->GetScreenHeight () / 2 + 9),
			core::position2d<s32> (
				settings->GetScreenWidth () / 2,
				settings->GetScreenHeight () / 2 + 3),
			SColor (255, 255, 0, 0)
			);
	drv->draw2DLine (
			core::position2d<s32> (
				settings->GetScreenWidth () / 2,
				settings->GetScreenHeight () / 2 - 10),
			core::position2d<s32> (
				settings->GetScreenWidth () / 2,
				settings->GetScreenHeight () / 2 - 3),
			SColor (255, 255, 0, 0)
			);
}

//=====================================
// EvRec class methods
//=====================================

bool EvRec::OnEvent (const SEvent& ev) {
	// one-time press
	if (ev.EventType == EET_KEY_INPUT_EVENT && ev.KeyInput.PressedDown == false)
		switch (ev.KeyInput.Key) {
			case KEY_F10:
				game_core.Quit ();
				return true;
				break;
			case KEY_ESCAPE:
				game_core.ToggleGUI ();
				return true;
				break;
			case KEY_KEY_C:
				game_core.camera.PrintCurrentPos ();
				return true;
				break;
			case KEY_LMENU:
				game_core.Jump ();
				return true;
				break;
		}
	// constant press
	if (ev.EventType == EET_KEY_INPUT_EVENT && !game_core.GUIIsShown ()) {
		if (ev.KeyInput.Key == KEY_KEY_W)
			game_core.SubmitPlayerMove (0);
		if (ev.KeyInput.Key == KEY_KEY_S)
			game_core.SubmitPlayerMove (1);
		if (ev.KeyInput.Key == KEY_KEY_A)
			game_core.SubmitPlayerMove (2);
		if (ev.KeyInput.Key == KEY_KEY_D)
			game_core.SubmitPlayerMove (3);
/*
		if (ev.KeyInput.Key == KEY_KEY_U)
			game_core.RotX (true);
		if (ev.KeyInput.Key == KEY_KEY_J)
			game_core.RotX (false);
		if (ev.KeyInput.Key == KEY_KEY_I)
			game_core.RotY (true);
		if (ev.KeyInput.Key == KEY_KEY_K)
			game_core.RotY (false);
		if (ev.KeyInput.Key == KEY_KEY_O)
			game_core.RotZ (true);
		if (ev.KeyInput.Key == KEY_KEY_L)
			game_core.RotZ (false);
*/
	}

	if (ev.EventType == EET_GUI_EVENT) {
		s32 id = ev.GUIEvent.Caller->getID ();
		switch (ev.GUIEvent.EventType) {
			// This is why we have to press Enter after we changed value in an
			// edit box
			case EGET_EDITBOX_ENTER:
				switch (id) {
					case EvBoxesAmount:
					case EvBoxSize:
					case EvBoxMass:
					case EvStep:
					case EvGravity:
					case EvIters:
					case EvCapsule:
						game_core.menu->EnableApply (true);
						game_core.menu->EnableCancel (true);
						break;
				}
				return true;
				break;
			case EGET_COMBO_BOX_CHANGED:
				switch (id) {
					case EvRes:
					case EvVD:
						game_core.menu->EnableApply (true);
						game_core.menu->EnableCancel (true);
						break;
				}
				return true;
				break;
			case EGET_CHECKBOX_CHANGED:
				switch (id) {
					case EvFullScr:
						game_core.menu->EnableApply (true);
						game_core.menu->EnableCancel (true);
						break;
				}
				return true;
				break;
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EvApply:
						if (game_core.menu->Apply () == 0) {
							game_core.LeaveWindowOpen (0);
							game_core.Restart ();
						}
						return true;
						break;
					case EvCancel:
						game_core.menu->Cancel ();
						return true;
						break;
					case EvQuit:
						game_core.Quit ();
						return true;
						break;
					case EvOptions:
						game_core.menu->ShowOptions (true);
						return true;
						break;
					case EvOk:
						if (game_core.menu->Ok () == 0) {
							game_core.LeaveWindowOpen ();
							game_core.Restart ();
						}
						return true;
						break;
				}
		}
	}
	if (!game_core.GUIIsShown ()) {
		if (ev.EventType == EET_MOUSE_INPUT_EVENT) {
			switch (ev.MouseInput.Event) {
				case EMIE_LMOUSE_LEFT_UP:
					game_core.Shoot ();
					return true;
					break;
			}
		}
	}
	return false;
}

//=====================================
// NearCallback ()
//=====================================

void NearCallback (void *data, dGeomID o1, dGeomID o2) {
	// Check for ray collision
	if (dGeomGetClass (o1) == dRayClass) {
		dContactGeom cont;
		if (dCollide (game_core.GetPlayer ()->GetPhysicsPlayer ()->player_ray, o2, 1, &cont, sizeof (dContactGeom))) {
			const dReal *pos = dGeomGetPosition (game_core.GetPlayer ()->GetPhysicsPlayer ()->player_geom);
			float distance = pos[1] - 30 - cont.pos[1];
			game_core.UpdateRayLength (distance);
		}
		return;
	}
	if (dGeomGetClass (o2) == dRayClass) {
		dContactGeom cont;
		if (dCollide (game_core.GetPlayer ()->GetPhysicsPlayer ()->player_ray, o1, 1, &cont, sizeof (dContactGeom))) {
			const dReal *pos = dGeomGetPosition (game_core.GetPlayer ()->GetPhysicsPlayer ()->player_geom);
			float distance = pos[1] - 30 - cont.pos[1];
			game_core.UpdateRayLength (distance);
		}
		return;
	}

	// Check for other collisions
	dBodyID b1 = dGeomGetBody (o1);
	dBodyID b2 = dGeomGetBody (o2);
	// If bodies are connected with a joint, then no collision detection needed
	if (b1 && b2 && dAreConnected (b1, b2))
		return;

	// Maximum possible contacts
	const int N = 4;
	dContact contact[N];
	// How much contacts do we actually have?
	int n = dCollide (o1, o2, N, &contact[0].geom, sizeof (dContact));
	if (n > 0)
		for (int i = 0; i < n; i++) {
			// What these parameters mean, you can read in ODE Manual
			contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
				dContactSoftERP | dContactSoftCFM | dContactApprox1;
			contact[i].surface.mu = dInfinity;
			contact[i].surface.slip1 = 0.0;
			contact[i].surface.slip2 = 0.0;
			contact[i].surface.soft_erp = 0.2;
			contact[i].surface.soft_cfm = 0.001;
			dJointID c = dJointCreateContact (world, contact_group,
					&contact[i]);
			dJointAttach (c, dGeomGetBody (contact[i].geom.g1),
					dGeomGetBody (contact[i].geom.g2));
		}
}

//=====================================
// main ()
//=====================================

int main () {
	// Yep, I use labels, blame me, blame me :^)
restart_label:
	game_core.Start ();
	RunMsg msg = game_core.Run ();
	if (msg == RM_RESTART_RENDERER)
		goto restart_label;
	else
		if (msg == RM_QUIT_APPLICATION) {
#ifdef DEBUG
			cout << "RM_QUIT_APPLICATION\n";
#endif
			return 0;
		}

#ifdef DEBUG
	cout << "return 1\n";
#endif
	return 1;
}

