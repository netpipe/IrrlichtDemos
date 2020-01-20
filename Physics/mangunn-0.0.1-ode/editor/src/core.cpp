// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "core.h"
#include <fstream>
using namespace irr;
using namespace video;

Core::Core () {
	fps = 0;
	mm = 0;
	std::fstream in ("settings");
	if (in.fail ()) {
		core::stringc path = getenv ("HOME");
		path += "/.config/osrpg/editor/settings";
		settings = new Settings (path.c_str ());
	}
	else {
		in.close ();
		settings = new Settings ("settings");
	}
	dev = createDevice (EDT_OPENGL, core::dimension2d<s32> (800, 600), 32,
			false, true, false, this);
	dev->setResizeAble (true);
	dev->getSceneManager ()->addCameraSceneNodeFPS (0, 50, 200, -1);
	reg = new Registry (dev, settings);
	ln = reg->smgr->addLightSceneNode (0, core::vector3df (0, 0, 0),
			SColorf (1.0f, 1.0f, 1.0f, 1.0f), 30);
	mm = new MainMenu (reg);
}

Core::~Core () {
	delete settings;
}

bool Core::OnEvent (const irr::SEvent &e) {
	// One-time press
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
	// Ctrl + button
	if (e.EventType == EET_KEY_INPUT_EVENT &&
			e.KeyInput.PressedDown == false &&
			e.KeyInput.Control == true)
		switch (e.KeyInput.Key) {
			// Toggle camera light
			case KEY_KEY_L:
				if (ln->getLightData ().Radius == 30)
					ln->getLightData ().Radius = 0;
				else
					ln->getLightData ().Radius = 30;
				return true;
				break;
			default:
				break;
		}
	if (e.EventType == EET_MOUSE_INPUT_EVENT) {
		switch (e.MouseInput.Event) {
			case EMIE_RMOUSE_LEFT_UP:
				mm->Toggle ();
				return true;
				break;
			default:
				break;
		}
	}
	// If mm exists and it has handled an event, return true
	if (mm && mm->OnEvent (e))
		return true;
	return false;
}

void Core::Run () {
	ITimer *timer = dev->getTimer ();
	u32 cur_time = timer->getRealTime ();
	float delta = 0;
	while (dev->run () && !reg->quit)
		if (dev->isWindowActive ()) {
			reg->drv->beginScene (true, true, SColor (150, 50, 50, 50));
			reg->smgr->drawAll ();
			delta = (timer->getRealTime () - cur_time);
			cur_time = timer->getRealTime ();
			mm->Update (delta);
			Update ();
			reg->env->drawAll ();
			reg->drv->endScene ();
		}
}

void Core::Quit () {
	delete mm;

	dev->drop ();
}

void Core::Update () {
	// Place light scene node behind camera
	core::vector3df dir = reg->cam->getPosition () - reg->cam->getTarget ();
	dir.normalize ();
	ln->setPosition (reg->cam->getPosition () + dir * 5);
	// Set window caption
	int ffps = reg->drv->getFPS ();
	if (ffps != fps) {
		fps = ffps;
		core::stringw cap = L"OSRPG Map Editor 0.5.1 [26 Nov 2006]    FPS: ";
		cap += fps;
		reg->dev->setWindowCaption (cap.c_str ());
	}
}

int main () {
	Core core;
	core.Run ();
	core.Quit ();

	return 0;
}

