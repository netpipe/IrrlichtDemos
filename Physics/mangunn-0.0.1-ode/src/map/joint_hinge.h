// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_MAP_JOINT_HINGE_H
#define OSRPG_GAME_MAP_JOINT_HINGE_H

#include "joint.h"

class MapJointHinge: public MapJoint {
	public:
		MapJointHinge (const dWorldID wr, MapMesh *mm1, MapMesh *mm2,
				const irr::core::vector3df anchor,
				const irr::core::vector3df axis, const bool constrain,
				const short angle_low, const short angle_high);
		void Edit (MapMesh *mm1, MapMesh *mm2,
				const irr::core::vector3df anchor,
				const irr::core::vector3df axis, const bool constrain,
				const short angle_low, const short angle_high);
		OMFJointHinge GetCopy () const;

	private:
		OMFJointHinge jh;
};

#endif // OSRPG_GAME_MAP_JOINT_HINGE_H

