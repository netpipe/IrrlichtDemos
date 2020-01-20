// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "joint_bs.h"
using namespace irr;
using namespace core;

MapJointBS::MapJointBS (const dWorldID wr, MapMesh *mm1, MapMesh *mm2,
		const vector3df anchor, const bool constrain_x,
		const bool constrain_y, const bool constrain_z,
		const short x_low, const short x_high,
		const short y_low, const short y_high,
		const short z_low, const short z_high) {
	world = wr;
	joint = 0;
	m1 = 0;
	m2 = 0;

	Edit (mm1, mm2, anchor, constrain_x, constrain_y, constrain_z,
			x_low, x_high, y_low, y_high, z_low, z_high);
}

void MapJointBS::Edit (MapMesh *mm1, MapMesh *mm2, const vector3df anchor,
		const bool constrain_x, const bool constrain_y,
		const bool constrain_z, const short x_low, const short x_high,
		const short y_low, const short y_high,
		const short z_low, const short z_high) {
	m1 = mm1;
	m2 = mm2;
	OMFMesh m = m1->GetCopy ();
	strcpy (jb.mesh1_name, m.name);
	m = m2->GetCopy ();
	strcpy (jb.mesh2_name, m.name);
	jb.anchor.a = anchor.X;
	jb.anchor.b = anchor.Y;
	jb.anchor.c = anchor.Z;
	jb.constrain_x = constrain_x;
	jb.constrain_y = constrain_y;
	jb.constrain_z = constrain_z;
	jb.x_low = x_low;
	jb.x_high = x_high;
	jb.y_low = y_low;
	jb.y_high = y_high;
	jb.z_low = z_low;
	jb.z_high = z_high;

	RemoveJoint ();
	joint = dJointCreateBall (world, 0);
	dJointAttach (joint, m1->GetBody (), m2->GetBody ());
	dJointSetBallAnchor (joint, jb.anchor.a, jb.anchor.b, jb.anchor.c);
	// Constraints are not yet implemented
}

OMFJointBallSocket MapJointBS::GetCopy () const {
	return jb;
}

