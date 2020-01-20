// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "joint_hinge.h"
#include "../misc/help.h"
using namespace irr;
using namespace core;

MapJointHinge::MapJointHinge (const dWorldID wr, MapMesh *mm1, MapMesh *mm2,
		const vector3df anchor, const vector3df axis, const bool constrain,
		const short angle_low, const short angle_high) {
	world = wr;
	joint = 0;
	m1 = 0;
	m2 = 0;

	Edit (mm1, mm2, anchor, axis, constrain, angle_low, angle_high);
}

void MapJointHinge::Edit (MapMesh *mm1, MapMesh *mm2,
		const vector3df anchor, const vector3df axis, const bool constrain,
		const short angle_low, const short angle_high) {
	m1 = mm1;
	m2 = mm2;
	OMFMesh m = m1->GetCopy ();
	strcpy (jh.mesh1_name, m.name);
	m = m2->GetCopy ();
	strcpy (jh.mesh2_name, m.name);
	jh.anchor.a = anchor.X;
	jh.anchor.b = anchor.Y;
	jh.anchor.c = anchor.Z;
	jh.constrain = constrain;
	jh.angle_low = angle_low;
	jh.angle_high = angle_high;

	RemoveJoint ();
	joint = dJointCreateHinge (world, 0);
	dJointAttach (joint, m1->GetBody (), m2->GetBody ());
	dJointSetHingeAnchor (joint, jh.anchor.a, jh.anchor.b, jh.anchor.c);
	dJointSetHingeAxis (joint, jh.axis.a, jh.axis.b, jh.axis.c);
	if (jh.constrain) {
		dJointSetHingeParam (joint, dParamLoStop, DegreeToRadian (jh.angle_low));
		dJointSetHingeParam (joint, dParamHiStop, DegreeToRadian (jh.angle_high));
	}
}

OMFJointHinge MapJointHinge::GetCopy () const {
	return jh;
}

