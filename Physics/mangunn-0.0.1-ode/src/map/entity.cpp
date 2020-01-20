// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "entity.h"
#include "../misc/help.h"
using namespace irr;
using namespace core;

dBodyID MapEntity::GetBody () const {
	return body;
}

dGeomID MapEntity::GetGeom () const {
	return geom;
}

void MapEntity::Move (const vector3df dir, const float force,
		const bool use_af, const bool use_pos, const vector3df pos) {
	dBodyEnable (body);
	ds_cur = 0;
	if (use_pos)
		dBodySetPosition (body, pos.X, pos.Y, pos.Z);
	if (use_af)
		dBodyAddForce (body, force * dir.X, force * dir.Y,
				force * dir.Z);
	else
		dBodySetLinearVel (body, force * dir.X, force * dir.Y,
				force * dir.Z);
}

void MapEntity::SetDisableParams (const float lvel, const float avel,
		const unsigned short steps) {
	ds_given = steps;
	dlvel = lvel;
	davel = avel;
}

void MapEntity::Disable () {
	dBodyDisable (body);
}

void MapEntity::DisableCheck () {
	if (!CheckForDisabling (body, dlvel, davel))
		ds_cur = 0;
	else
		if (ds_cur < ds_given)
			ds_cur++;
	if (ds_cur >= ds_given)
		Disable ();
}

void MapEntity::RemoveBody () {
	if (body) {
		dBodyDestroy (body);
		body = 0;
	}
}

void MapEntity::RemoveGeom () {
	if (geom) {
		dGeomDestroy (geom);
		geom = 0;
	}
}

