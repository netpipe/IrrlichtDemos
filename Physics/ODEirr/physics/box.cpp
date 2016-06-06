// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "box.h"

PhysicsBox::PhysicsBox () {
	dis_steps_cur = 0;
	dis_steps_given = 0;
	dis_lvel = 0;
	dis_avel = 0;
}

PhysicsBox::~PhysicsBox () {

}

int PhysicsBox::GetDisSteps () {
	return dis_steps_cur;
}

void PhysicsBox::CreateBox (dWorldID world, dSpaceID space, float x, float y,
		float z, float size, float mass) {
	PhysicsBox::size = size;
	PhysicsBox::mass = mass;

	box_body = dBodyCreate (world);
	dMassSetBox (&m, 1, size, size, size);
	dMassAdjust (&m, mass);
	dBodySetMass (box_body, &m);
	box_geom = dCreateBox (space, size, size, size);
	dGeomSetBody (box_geom, box_body);
	dBodySetPosition (box_body, x, y, z);
	//DisableBox ();
}

void PhysicsBox::DestroyBox () {
	dGeomDestroy (box_geom);
}

void PhysicsBox::SetDisableParams (float lvel, float avel, int steps) {
	dis_steps_given = steps;
	dis_lvel = lvel;
	dis_avel = avel;
}

void PhysicsBox::DisableCheck (bool run) {
	if (!run)
		return;
	if (!CheckForDisabling (box_body, dis_lvel, dis_avel))
		ResetDisSteps ();
	if (dis_steps_cur < dis_steps_given) {
		if (!CheckForDisabling (box_body, dis_lvel, dis_avel)) {
			ResetDisSteps ();
			return;
		}
		dis_steps_cur++;
	}
	if (dis_steps_cur >= dis_steps_given)
		DisableBox ();
}

void PhysicsBox::DisableBox () {
	dBodyDisable (box_body);
#ifdef DEBUG
	cout << "Box disabled\n";
#endif
}

void PhysicsBox::ResetDisSteps () {
	dis_steps_cur = 0;
}

void PhysicsBox::Shoot (core::vector3df cam_pos, core::vector3df cam_target,
		float force) {
	dBodyEnable (box_body);
	ResetDisSteps ();
	core::vector3df force_vec = - cam_pos + cam_target;
	int dist = 20;
	dBodySetPosition (box_body, cam_pos.X + dist * force_vec.X,
			cam_pos.Y + dist * force_vec.Y,
			cam_pos.Z + dist * force_vec.Z);
	dBodySetAngularVel (box_body, 0, 0, 0);
	dBodySetLinearVel (box_body, force * force_vec.X, force * force_vec.Y,
			force * force_vec.Z);
}

