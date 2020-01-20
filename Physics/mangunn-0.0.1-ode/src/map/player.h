// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_MAP_PLAYER_H
#define OSRPG_GAME_MAP_PLAYER_H

#include "entity.h"

//#define USE_PLAYER_RAY

class MapPlayer: public MapEntity {
	public:
		MapPlayer (Registry *rg, const dWorldID wr, const dSpaceID sp);
		const dReal* GetPosition () const;
#ifdef USE_PLAYER_RAY
		dGeomID GetRay () const;
#endif
		void MoveInDir (char i, irr::core::vector3df gr);
		void SetMass (float m);
		void SetPosition (irr::core::vector3df pos);
		void Update ();
#ifdef USE_PLAYER_RAY
		void UpdateRayLength (float a);
#endif

	private:
		irr::core::vector3df GetForwardVec ();
		float radius,
			  length,
			  mass,
			  scale;
#ifdef USE_PLAYER_RAY
		dGeomID ray;
		float rdist;
		bool do_rdist;
#endif
};

#endif // OSRPG_GAME_MAP_PLAYER_H

