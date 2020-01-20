// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_MAP_LIGHT_H
#define OSRPG_GAME_MAP_LIGHT_H

#include "format.h"
#include "../registry.h"

class MapLight {
	public:
		MapLight (Registry *rg, const OMFLight l);
		~MapLight ();
		void CreateNode ();
		void Edit (const OMFLight l);
		OMFLight GetCopy () const;

	private:
		OMFLight light;
		Registry *reg;
		irr::scene::ILightSceneNode *ln;
};

#endif // OSRPG_GAME_MAP_LIGHT_H

