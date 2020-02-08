// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "core.h"
#include <fstream>
//#include <iostream>
#include "misc/events.h"
//using namespace std;
using namespace irr;
using namespace gui;
using namespace scene;
using namespace video;

#define DEBUG

Core::Core () {
	std::ifstream in ("settings");
	if (in.fail ()) {
		core::stringc path = getenv ("HOME");
		path += "/.config/osrpg/maggun/settings";
		settings = new Settings (path.c_str ());
	}
	else {
		in.close ();
		settings = new Settings ("settings");
	}
	reg = 0;
	mm = 0;
	ms = 0;
}

Core::~Core () {
	if (ms)
		delete ms;
	/*
	dJointGroupDestroy (reg->jg);
	dSpaceDestroy (reg->space);
	dWorldDestroy (reg->world);
	*/
	delete settings;
}

bool Core::OnEvent (const SEvent &e) {
	// One-time key press
	if (e.EventType == EET_KEY_INPUT_EVENT &&
			e.KeyInput.PressedDown == false)
		switch (e.KeyInput.Key) {
			case KEY_F10:
				reg->quit = true;
				return true;
				break;
			default:
				break;
		}
	if (mm && mm->OnEvent (e))
		return true;
	if (ms && ms->OnEvent (e))
		return true;
	return false;
}

bool Core::Quit () {
	// Delete Irrlicht device
	dev->drop ();
	// Delete main menu
	delete mm;
	if (reg->restart)
		return true;
	return false;
}

void Core::Run () {
	// These variables are used to calculate correct timestep
	// Article about timestep: http://gaffer.org/articles/
	// May be you're the one who can make "The final touch"?
	float accum = 0,
		  delta = 0;
	u32 cur_time = 0;
	const int ITERS = 10;
	const float STEP = 0.001;
	ITimer *timer = dev->getTimer ();
	cur_time = timer->getRealTime ();

	while (dev->run () && !reg->quit && !reg->restart) {
		reg->drv->beginScene (true, true, SColor (0, 200, 200, 200));
		if (!reg->gui_shown) {
			delta = (timer->getRealTime () - cur_time) * 0.001f;
			cur_time = timer->getRealTime ();
			accum += delta;
			while (accum >= STEP) {
				ms->Simulate (STEP, ITERS);
				accum -= STEP;
			}
		}
		else
			cur_time = timer->getRealTime ();
		ms->Update ();
		// Draw scene nodes
		reg->smgr->drawAll ();
		mm->DrawAddons ();
		reg->env->drawAll ();
		reg->drv->endScene ();
		//dev->sleep(1);
	}
}

void Core::Start () {
	// Create renderer
	dev = createDevice (EDT_OPENGL, core::dimension2d<u32> (
				settings->GetScreenWidth (),settings->GetScreenHeight ()),
			32, 0, true, false, this);
	// Window caption
	dev->setWindowCaption (L"OSRPG Project: Maggun 0.0.1");
//	dev->setResizeAble (false);
	// Create camera
	cam = dev->getSceneManager ()->addCameraSceneNodeFPS (0, 50, 10);
	// ATTENTION: This must be done ONLY after setting up
	// dev, cam, settings
	if (reg == 0) {
		reg = new Registry (dev, settings);
		reg->quit = false;
		/*
		reg->world = dWorldCreate ();
		reg->space = dSimpleSpaceCreate (0);
		reg->jg = dJointGroupCreate (0);
		*/
		reg->openwin = -1;
		ms = new MapScene (reg);
		core::stringc data_dir = settings->GetDataDir ();
		core::stringc path = data_dir + "maps/05_other_dome.omf";
		ms->Open (path.c_str ());
		//MapMesh
	}
	else {
		reg->Reinit (dev);
		ms->RecreateNodes ();
	}
	reg->restart = false;
	// Create main menu
	mm = new MainMenu (reg, ms);
}

/*
void Core::Update () {
		// Show node id camera looks at
		n = smgr->getSceneCollisionManager ()->getSceneNodeFromCameraBB (cam);
		s32 id;
		if (n) {
			id = n->getID ();
			txt->setText (core::stringw (id).c_str ());
		}
		if (n && id != -1 && pulling && !holding && !editing) {
			core::line3d<f32> l (cam->getPosition (), n->getPosition ());
			float ll = l.getLength ();
			player->PullBody (id, -l.getVector (), ll);
			if (ll < 15) {
				player->HoldBody (id);
				holding = true;
			}
		}
}
*/

int main () {
	// ATTENTION: Make sure this "core" doesn't clash with
	// Irrlicht's "core" class
	Core core;
restart_label:
	core.Start ();
	core.Run ();
	if (core.Quit ())
		goto restart_label;

	return 0;
}

