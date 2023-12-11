// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_MAP_LIGHT_H
#define OSRPG_EDITOR_MAP_LIGHT_H

#include "format.h"
#include "item.h"

class MapLight: public MapItem {
	public:
		MapLight (Registry *reg, const char *name,
				const irr::core::vector3df pos, const float rad,
				const irr::video::SColorf col);
		MapLight (Registry *reg, const OMFLight l);
		~MapLight ();
		void Edit (const char *name, const irr::core::vector3df pos,
				const float rad, const irr::video::SColorf col);
		OMFLight GetCopy () const;
		char GetType () const;
		void Select (bool state);
		void SetName (const char *name);
		void SetPosition (const irr::core::vector3df pos);
		void SetRadius (const float rad);
		// Diffuse color
		void SetColor (const irr::video::SColorf col);

	private:
		OMFLight light;
		irr::scene::ILightSceneNode *ln;
};

#endif // OSRPG_EDITOR_MAP_LIGHT_H

