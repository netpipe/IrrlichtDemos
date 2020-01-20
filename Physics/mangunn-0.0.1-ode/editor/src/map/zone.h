// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_MAP_ZONE_H
#define OSRPG_EDITOR_MAP_ZONE_H

#include "format.h"
#include "item.h"

class MapZone: public MapItem {
	public:
		MapZone (Registry *reg, const char *name, const char *model_file,
				const char *script_name, const irr::core::vector3df pos,
				const irr::core::vector3df rot);
		MapZone (Registry *reg, const OMFZone z);
		void Edit (const char *name, const char *model_file,
				const char *script_name, const irr::core::vector3df pos,
				const irr::core::vector3df rot);
		OMFZone GetCopy () const;
		char GetType () const;
		void SetModelFile (const char *model_file);
		void SetName (const char *name);
		void SetPosition (const irr::core::vector3df pos);
		void SetRotation (const irr::core::vector3df rot);
		void SetScriptName (const char *script_name);

	private:
		OMFZone zone;
};

#endif // OSRPG_EDITOR_MAP_ZONE_H

