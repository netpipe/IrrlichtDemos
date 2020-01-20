// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_MAP_ITEM_H
#define OSRPG_EDITOR_MAP_ITEM_H

#include <irrlicht.h>
#include "../registry.h"

class MapItem {
	public:
		virtual ~MapItem ();
		unsigned int GetID () const;
		// TODO: Should we replace char with enum?
		virtual char GetType () const = 0;
		virtual void Select (bool state);
	
	protected:
		unsigned int id;
		irr::scene::ISceneNode *node;
		Registry *reg;
};

#endif // OSRPG_EDITOR_MAP_ITEM_H

