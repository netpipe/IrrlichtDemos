// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_REGISTRY_H
#define OSRPG_GAME_REGISTRY_H

#include <vector>
#include "settings.h"

class Registry {
	public:
		Registry (irr::IrrlichtDevice *dev, Settings *settings);
		void AddFreedID (unsigned int freed_id);
		unsigned int GetFreeID ();
		void Reinit (irr::IrrlichtDevice *dev);

		irr::IrrlichtDevice *dev;
		irr::video::IVideoDriver *drv;
		irr::gui::IGUIEnvironment *env;
		irr::scene::ISceneManager *smgr;
		irr::scene::ICameraSceneNode *cam;
		Settings *settings;
		/// Should we quit application?
		bool quit,
		/// Should we restart renderer?
			 restart,
		/// Is GUI on?
			 gui_shown;
		char openwin;
		dWorldID world;
		dSpaceID space;
		dJointGroupID jg;

	private:
		/// Next id (if no free ids available)
		unsigned int id;
		/// Vector of freed ids
		std::vector<unsigned int> freed_ids;
};

#endif // OSRPG_GAME_REGISTRY_H

