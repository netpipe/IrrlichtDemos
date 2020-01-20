// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_MAP_PLACE_TAG_H
#define OSRPG_GAME_MAP_PLACE_TAG_H

#include "format.h"

class MapPlaceTag {
	public:
		MapPlaceTag (const OMFPlaceTag p);
		~MapPlaceTag ();
		void Edit (const OMFPlaceTag p);
		OMFPlaceTag GetCopy () const;

	private:
		OMFPlaceTag pt;
};

#endif // OSRPG_GAME_MAP_PLACE_TAG_H

