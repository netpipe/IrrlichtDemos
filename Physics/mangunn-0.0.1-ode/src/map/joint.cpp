// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "joint.h"

MapJoint::~MapJoint () {
	RemoveJoint ();
}

void MapJoint::RemoveJoint () {
	if (joint) {
		dJointDestroy (joint);
		joint = 0;
	}
}

