// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "world.h"

void PhysWorld::Create (float gr) {
	world = dWorldCreate ();
	space = dHashSpaceCreate (0);
	contact_group = dJointGroupCreate (0);
	// X axis goes right, Y axis goes up, Z axis goes forward
	dWorldSetGravity (world, 0, gr, 0);
}

void PhysWorld::Destroy () {
	dJointGroupDestroy (contact_group);
	dSpaceDestroy (space);
	dWorldDestroy (world);
}

