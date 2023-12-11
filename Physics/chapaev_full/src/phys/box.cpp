// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "box.h"
using namespace irr;
using namespace core;

PhysBox::PhysBox (const dWorldID world, const dSpaceID space,
		const vector3df pos, const float mass, const float size) {
	ds_cur = 0;
	davel = 0;
	dlvel = 0;

	body = dBodyCreate (world);
	dMassSetBox (&m, 1, size, size, size);
	dMassAdjust (&m, mass);
	dBodySetMass (body, &m);
	geom = dCreateBox (space, size, size, size);
	dGeomSetBody (geom, body);
	dBodySetPosition (body, pos.X, pos.Y, pos.Z);
}

