// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_MAP_SCENE_H
#define OSRPG_GAME_MAP_SCENE_H

#include "mesh.h"
#include "light.h"
#include "place_tag.h"
#include "joint_bs.h"
#include "joint_hinge.h"
#include "player.h"
#include "sky.h"

class MapScene {
	public:
		MapScene (Registry *rg);
		~MapScene ();
		bool AddJointBS (const char *name1, const char *name2,
				const irr::core::vector3df anchor, const bool constrain_x,
				const bool constrain_y, const bool constrain_z,
				const short x_low, const short x_high, const short y_low,
				const short y_high, const short z_low, const short z_high);
		bool AddJointBS (const OMFJointBallSocket b);
		bool AddJointHinge (const char *name1, const char *name2,
				const irr::core::vector3df anchor, const irr::core::vector3df axis,
				const bool constrain, const short angle_low,
				const short angle_high);
		bool AddJointHinge (const OMFJointHinge h);
		void AddLight (OMFLight l);
		void AddMesh (OMFMesh m);
		void AddPlaceTag (OMFPlaceTag p);
		void Clear ();
		irr::core::vector3df GetGravity () const;
#ifdef USE_PLAYER_RAY
		float GetRayLength () const;
#endif
		static void NearCallback (void *data, dGeomID o1, dGeomID o2);
		void NearCallback (dGeomID o1, dGeomID o2);
		bool OnEvent (const irr::SEvent &e);
		void Open (const char *fn);
		void RecreateNodes ();
		void SetGravity (irr::core::vector3df gr);
		void Simulate (float dt, int iters);
		void Update ();

	private:
		Registry *reg;
		std::vector<MapMesh*> meshes;
		std::vector<MapLight*> lights;
		std::vector<MapPlaceTag*> ptags;
		std::vector<MapJointBS*> jbs;
		std::vector<MapJointHinge*> jhs;
		MapSky *sky;
		MapPlayer *player;
		OMFHeader header;
		irr::core::vector3df gravity;
#ifdef USE_PLAYER_RAY
		float dist;
#endif
};

#endif // OSRPG_GAME_MAP_SCENE_H

