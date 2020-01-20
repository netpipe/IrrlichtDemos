// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_MAP_ENTITY_H
#define OSRPG_GAME_MAP_ENTITY_H

#include "../registry.h"

class MapEntity {
	public:
		dBodyID GetBody () const;
		dGeomID GetGeom () const;
		void Move (const irr::core::vector3df dir, const float force,
				const bool use_af = false, const bool use_pos = false,
				const irr::core::vector3df pos =
				irr::core::vector3df (0, 0, 0));
		void SetDisableParams (const float lvel, const float avel,
				const unsigned short steps);
		virtual void Update () = 0;

	protected:
		void Disable ();
		void DisableCheck ();
		void RemoveBody ();
		void RemoveGeom ();

		Registry *reg;
		dBodyID body;
		dGeomID geom;
		unsigned short ds_cur,
					   ds_given;
		float dlvel,
			  davel;
};

#endif // OSRPG_GAME_MAP_ENTITY_H

