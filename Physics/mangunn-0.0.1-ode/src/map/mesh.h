// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_MAP_MESH_H
#define OSRPG_GAME_MAP_MESH_H

#include "format.h"
#include "entity.h"

class MapMesh: public MapEntity {
	public:
		MapMesh (Registry *rg, const dWorldID wr, const dSpaceID sp,
				const OMFMesh m);
		virtual ~MapMesh ();
		void CreateNode (const OMFMesh *m = 0);
		void Edit (const OMFMesh m);
		OMFMesh GetCopy () const;
		void Update ();

	private:
		void PositionNode (const OMFMesh m);
		void PositionPhys (const OMFMesh m);
		void RemoveNode ();
		void SetMass (const float m);

		OMFMesh mesh;
		dWorldID world;
		dSpaceID space;
		irr::scene::ISceneNode *sn;
		irr::scene::IAnimatedMesh *am;
		irr::core::stringc model_dir;
};

#endif // OSRPG_GAME_MAP_MESH_H

