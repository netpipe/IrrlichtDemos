// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_MAP_JOINT_H
#define OSRPG_GAME_MAP_JOINT_H

#include "mesh.h"

class MapJoint {
	public:
		virtual ~MapJoint ();

	protected:
		void RemoveJoint ();

		MapMesh *m1,
				*m2;
		dWorldID world;
		dJointID joint;
};

#endif // OSRPG_GAME_MAP_JOINT_H

