// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "sphere.h"
using namespace irr;
using namespace core;

PhysSphere::PhysSphere (const dWorldID world, const dSpaceID space,
		const core::vector3df pos, const float mass,
		const float rad) {
	ds_cur = 0;
	davel = 0;
	dlvel = 0;

	body = dBodyCreate (world);
	dMassSetSphere (&m, 1, rad);
	dMassAdjust (&m, mass);
	dBodySetMass (body, &m);
	geom = dCreateSphere (space, rad);
	dGeomSetBody (geom, body);
	dBodySetPosition (body, pos.X, pos.Y, pos.Z);
}

