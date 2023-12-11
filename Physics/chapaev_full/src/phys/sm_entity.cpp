// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "sm_entity.h"
#include "../help.h"
using namespace irr;

PhysSMEntity::~PhysSMEntity () {
	dGeomDestroy (geom);
}

void PhysSMEntity::DisableCheck () {
	// If linear and/or angular velocities are/is more than
	// allowed thresholds, "enable body" by zeroing disable steps
	if (!CheckForDisabling (body, dlvel, davel))
		ds_cur = 0;
	else
		// If body is disabled but steps amount is not yet enough,
		// increase it
		if (ds_cur < ds_given)
			ds_cur++;
	// If body was disabled for quite a while and disable steps reached
	// its threshold, then disable body
	if (ds_cur >= ds_given)
		Disable ();
}

dBodyID PhysSMEntity::GetBody () const {
	return body;
}

dGeomID PhysSMEntity::GetGeom () const {
	return geom;
}

void PhysSMEntity::SetDisableParams (float lvel, float avel,
	   const unsigned short steps) {
	ds_given = steps;
	dlvel = lvel;
	davel = avel;
}

void PhysSMEntity::Shoot (const core::vector3df pos,
		const core::vector3df dir, const float force) {
	dBodyEnable (body);
	ds_cur = 0;
	dBodySetPosition (body, pos.X, pos.Y, pos.Z);
	// Unfortunately, I can't figure out how to make correct shooting
	// with force application, so I set linear velocity, which isn't correct
	//dBodyAddForce (body, force * dir.X, force * dir.Y, force * dir.Z);
	dBodySetLinearVel (body, force * dir.X, force * dir.Y, force * dir.Z);
}

void PhysSMEntity::Disable () {
	dBodyDisable (body);
}

