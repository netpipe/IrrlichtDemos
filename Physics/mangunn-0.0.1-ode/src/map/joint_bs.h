// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_MAP_JOINT_BS_H
#define OSRPG_GAME_MAP_JOINT_BS_H

#include "joint.h"

class MapJointBS: public MapJoint {
	public:
		MapJointBS (const dWorldID wr, MapMesh *mm1, MapMesh *mm2,
				const irr::core::vector3df anchor, const bool constrain_x,
				const bool constrain_y, const bool constrain_z,
				const short x_low, const short x_high, 
				const short y_low, const short y_high, 
				const short z_low, const short z_high);
		void Edit (MapMesh *mm1, MapMesh *mm2, 
				const irr::core::vector3df anchor, const bool constrain_x,
				const bool constrain_y, const bool constrain_z,
				const short x_low, const short x_high, 
				const short y_low, const short y_high, 
				const short z_low, const short z_high);
		OMFJointBallSocket GetCopy () const;

	private:
		OMFJointBallSocket jb;
};

#endif // OSRPG_GAME_MAP_JOINT_BS_H

