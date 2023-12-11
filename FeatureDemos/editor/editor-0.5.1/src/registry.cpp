// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "registry.h"
using namespace irr;

Registry::Registry (IrrlichtDevice *dev, Settings *settings) {
	Registry::settings = settings;
	Registry::dev = dev;
	drv = dev->getVideoDriver ();
	env = dev->getGUIEnvironment ();
	smgr = dev->getSceneManager ();
	cam = smgr->getActiveCamera ();

	id = 0;
	freed_ids.clear ();
	quit = false;
}

void Registry::AddFreedID (unsigned int freed_id) {
	freed_ids.push_back (freed_id);
}

unsigned int Registry::GetFreeID () {
	if (freed_ids.size () == 0)
		return id++;
	// If some ids were freed, take them back into account
	else {
		unsigned int i = freed_ids[freed_ids.size () - 1];
		freed_ids.pop_back ();
		return i;
	}
}

