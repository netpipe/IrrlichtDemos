// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "basic.h"
dWorldID world;
dSpaceID space;
dJointGroupID contact_group;
dGeomID map;

void CreateWorld () {
	world = dWorldCreate ();
	space = dHashSpaceCreate (0);
	contact_group = dJointGroupCreate (0);
	// We set gravity along the Y axis since it's what Irrlicht uses
	// FYI: Y points up, Z points forwards, X points right
	dWorldSetGravity (world, 0, settings->GetGravity (), 0);
}

void DestroyWorld () {
	dJointGroupDestroy (contact_group);
	dSpaceDestroy (space);
	dWorldDestroy (world);
}

