// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_REGISTRY_H
#define OSRPG_EDITOR_REGISTRY_H

#include <irrlicht.h>
#include <vector>
#include "settings.h"

class Registry {
	public:
		Registry (irr::IrrlichtDevice *dev, Settings *settings);
		void AddFreedID (unsigned int freed_id);
		unsigned int GetFreeID ();

		irr::IrrlichtDevice *dev;
		irr::video::IVideoDriver *drv;
		irr::gui::IGUIEnvironment *env;
		irr::scene::ISceneManager *smgr;
		irr::scene::ICameraSceneNode *cam;
		Settings *settings;
		bool quit;

	private:
		unsigned int id;
		std::vector<unsigned int> freed_ids;
};

#endif // OSRPG_EDITOR_REGISTRY_H

