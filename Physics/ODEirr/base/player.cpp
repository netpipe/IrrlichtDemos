// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "player.h"

BasePlayer::BasePlayer () {

}

BasePlayer::~BasePlayer () {
	Destroy ();
}

void BasePlayer::Create (dWorldID world, dSpaceID space, float x, float y,
		float z, float mass/*, ISceneManager *smgr*/) {
	phys_pl = new PhysicsPlayer ();
	//CreateNode (smgr);
	phys_pl->CreatePlayer (world, space, x, y, z, mass);
}

/*
void BasePlayer::CreateNode (ISceneManager *smgr) {
	capsule_node = new MeshNode (smgr, "_data/models/capsule.obj");
}
*/

void BasePlayer::Update (bool run) {
	if (!run)
		return;
	//UpdateObject (phys_pl->player_geom, capsule_node->GetSceneNode ());
	// We must keep the capsule vertically, but manually setting its degree
	// to 90 isn't correct, since the collision may be broken sometimes
	// when capsule lyes on the floor, and then is turned, it will be
	// half-inside the floor
	// We must apply force to keep it vertically
	// TODO: do as /h+ suggests about keeping capsule vertically with forces
	// /h+'s doesn't seem to work :(
	dQuaternion q;
	dQFromAxisAndAngle (q, 1, 0, 0, DegreeToRadian (90));
	dBodySetQuaternion (phys_pl->player_body, q);
	dBodySetAngularVel (phys_pl->player_body, 0, 0, 0);
	/*
	const dReal *avel = dBodyGetAngularVel (phys_pl->player_body);
	core::vector3df ang_vel (avel[0], avel[1], avel[2]);
	dGeomGetQuaternion (phys_pl->player_geom, q);
	core::vector3df rot;
	QuaternionToEuler (q, rot);
	const float T = 0.2;
	const float C = 1;
	rot += T * ang_vel;
	dBodyAddTorque (phys_pl->player_body, -rot.X * C, -rot.Y * C, -rot.Z * C);
	*/
	const dReal *pos = dGeomGetPosition (phys_pl->player_geom);
	// Place ray right below the capsule
	dGeomRaySet (phys_pl->player_ray, pos[0], pos[1] + 30, pos[2], 0, -1, 0);

	// Friction
	const dReal *lvel = dBodyGetLinearVel (phys_pl->player_body);
	dBodySetLinearVel (phys_pl->player_body, lvel[0] * 0.95, lvel[1], lvel[2] * 0.95);
}

PhysicsPlayer* BasePlayer::GetPhysicsPlayer () {
	return phys_pl;
}

core::vector3df BasePlayer::GetPosition () {
	return phys_pl->GetPosition ();
}

void BasePlayer::Shoot (core::vector3df force_vec, float force) {
	phys_pl->Shoot (force_vec, force);
}

void BasePlayer::Destroy () {
	phys_pl->DestroyPlayer ();
}

/*
ISceneNode* BasePlayer::GetSceneNode () {
	return capsule_node->GetSceneNode ();
}
*/

