// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_MAP_JOINT_BALL_SOCKET_H
#define OSRPG_EDITOR_MAP_JOINT_BALL_SOCKET_H

#include "mesh.h"

class MapJointBallSocket: public MapItem {
	public:
		MapJointBallSocket (Registry *reg, MapMesh *m1, MapMesh *m2,
				const irr::core::vector3df anchor,
				const bool constrain_x, const bool constrain_y,
				const bool constrain_z, const short x_low, const short x_high,
				const short y_low, const short y_high, const short z_low,
				const short z_high);
		void DrawAddons ();
		void Edit (MapMesh *m1, MapMesh *m2,
				const irr::core::vector3df anchor,
				const bool constrain_x, const bool constrain_y,
				const bool constrain_z, const short x_low, const short x_high,
				const short y_low, const short y_high, const short z_low,
				const short z_high);
		OMFJointBallSocket GetCopy () const;
		char GetType () const;
		void Select (bool state);

	private:
		OMFJointBallSocket joint;
		MapMesh *m1,
				*m2;
		bool selected;
		irr::video::SMaterial mat;
		irr::core::matrix4 m;
};

#endif // OSRPG_EDITOR_MAP_JOINT_BALL_SOCKET_H

