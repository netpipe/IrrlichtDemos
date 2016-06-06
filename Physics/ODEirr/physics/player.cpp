// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "player.h"

PhysicsPlayer::PhysicsPlayer () {

}

PhysicsPlayer::~PhysicsPlayer () {

}

void PhysicsPlayer::CreatePlayer (dWorldID world, dSpaceID space, float x, float y,
		float z, float mass) {
	player_body = dBodyCreate (world);
	//dMassSetCapsule (&m, 1, 2/* direction = along Y */, 15, 30);
	dMassAdjust (&m, mass);
	dBodySetMass (player_body, &m);
	//player_geom = dCreateCapsule (space, 15, 30);


	/*
	dQuaternion q;
	dQFromAxisAndAngle (q, 1, 0, 0, DegreeToRadian (90));
	dGeomSetQuaternion (player_geom, q);
	*/
	dGeomSetBody (player_geom, player_body);
	dBodySetPosition (player_body, x, y, z);

	player_ray = dCreateRay (space, 1500);
	dGeomRaySet (player_ray, x, y + 30, z, 0, -1, 0);
}

void PhysicsPlayer::Shoot (core::vector3df force_vec, float force) {
	dBodySetAngularVel (player_body, 0, 0, 0);
	dBodySetLinearVel (player_body, force * force_vec.X, force * force_vec.Y, force * force_vec.Z);
}

core::vector3df PhysicsPlayer::GetPosition () {
	const dReal *pos = dBodyGetPosition (player_body);
	return core::vector3df (pos[0], pos[1], pos[2]);
}

void PhysicsPlayer::DestroyPlayer () {
	dGeomDestroy (player_geom);
}

