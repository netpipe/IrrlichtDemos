// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_MAP_PLACE_TAG_H
#define OSRPG_EDITOR_MAP_PLACE_TAG_H

#include "format.h"
#include "item.h"

class MapPlaceTag: public MapItem {
	public:
		MapPlaceTag (Registry *reg, const char *name,
				const irr::core::vector3df pos);
		MapPlaceTag (Registry *reg, const OMFPlaceTag pt);
		~MapPlaceTag ();
		void Edit (const char *name, const irr::core::vector3df pos);
		OMFPlaceTag GetCopy () const;
		char GetType () const;
		void SetName (const char *name);
		void SetPosition (const irr::core::vector3df pos);

	private:
		OMFPlaceTag pt;
};

#endif // OSRPG_EDITOR_MAP_PLACE_TAG_H

