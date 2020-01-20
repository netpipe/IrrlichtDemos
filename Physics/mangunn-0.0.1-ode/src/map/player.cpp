// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "player.h"
#include "../misc/help.h"
#include <ode/common.h>

using namespace irr;
using namespace core;

MapPlayer::MapPlayer (Registry *rg, const dWorldID wr, const dSpaceID sp) {
	reg = rg;
	radius = 15;
	length = 30;
	geom = dCreateCapsule (sp, radius, length);
	body = dBodyCreate (wr);
	SetMass (20);
	dGeomSetBody (geom, body);
	scale = 40;
#ifdef USE_PLAYER_RAY
	ray = dCreateRay (sp, 1000);
	do_rdist = false;
#endif
}

const dReal* MapPlayer::GetPosition () const {
	return dGeomGetPosition (geom);
}

#ifdef USE_PLAYER_RAY
dGeomID MapPlayer::GetRay () const {
	return ray;
}
#endif

void MapPlayer::MoveInDir (char i, vector3df gr) {
	vector3df dir;
	switch (i) {
		// Forward
		case 0:
			dir = GetForwardVec () / mass;
			break;
		// Backward
		case 1:
			dir = -GetForwardVec () / mass;
			break;
		// Left
		case 2:
			{
				vector3df v = GetForwardVec ();
				dir = vector3df (-v.Z, v.Y, v.X) / mass;
			}
			break;
		// Right
		case 3:
			{
				vector3df v = -GetForwardVec ();
				dir = vector3df (-v.Z, v.Y, v.X) / mass;
			}
			break;
		default:
			break;
	}
	// TODO: How to use gravity when moving player?
	Move (dir, scale * mass);
}

void MapPlayer::SetMass (float m) {
	dMass mm;
	dMassSetCapsule (&mm, 1, 2 /* direction = along Y */, radius, length);
	dMassAdjust (&mm, m);
	dBodySetMass (body, &mm);
	mass = m;
}

void MapPlayer::SetPosition (irr::core::vector3df pos) {
	dBodySetPosition (body, pos.X, pos.Y, pos.Z);
}

void MapPlayer::Update () {
	// Make capsule stand
	dQuaternion q;
	dQFromAxisAndAngle (q, 1, 0, 0, DegreeToRadian (90));
	dBodySetQuaternion (body, q);
	const dReal *av = dBodyGetAngularVel (body);
	const float D = 1.2;
	dBodySetAngularVel (body, av[0] / D, av[1] / D, av[2] / D);
	const dReal *pos = dBodyGetPosition (body);
	reg->cam->setPosition (vector3df (pos[0], pos[1] + 20, pos[2]));
#ifdef USE_PLAYER_RAY
	dGeomRaySet (ray, pos[0], pos[1] + 30, pos[2], 0, -1, 0);
	if (do_rdist) {
		do_rdist = false;
		float m = 12 * mass * scale * (20 - rdist);
		if (rdist < 20 && rdist > -1) {
			dBodyAddForce (body, 0, m, 0);
			const dReal *lv = dBodyGetLinearVel (body);
			dBodySetLinearVel (body, lv[0], 0, lv[2]);
		}
	}
#endif
}

#ifdef USE_PLAYER_RAY
void MapPlayer::UpdateRayLength (float a) {
	do_rdist = true;
	rdist = a;
}
#endif

vector3df MapPlayer::GetForwardVec () {
	vector3df v = reg->cam->getTarget () - reg->cam->getPosition ();
	v.Y = 0;
	return v.normalize ();
}

