// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "rag_doll.h"

PhysicsRagDoll::PhysicsRagDoll () {
	dis_steps_cur = 0;
	dis_steps_given = 0;
	dis_avel = 0;
	dis_lvel = 0;
}

PhysicsRagDoll::~PhysicsRagDoll () {

}

/*
void PhysicsRagDoll::CreateHead (dWorldID world, dSpaceID space) {
	head_body = dBodyCreate (world);
	dMassSetSphere (&m, 1, 1);
	dMassAdjust (&m, 0.2);
	dBodySetMass (head_body, &m);
	head_geom = dCreateSphere (space, 1);
	dGeomSetBody (head_geom, head_body);
}
*/

void PhysicsRagDoll::CreateBody (dWorldID world, dSpaceID space,
		IAnimatedMesh *mesh) {
	// We don't have box here, but for simplicity...
	body_body = dBodyCreate (world);
	dMassSetBox (&m, 1, 6, 6, 2);
	dMassAdjust (&m, 6);
	dBodySetMass (body_body, &m);
	dTriMeshDataID data = GetGeomFromMesh (mesh->getMesh (0));
	// Build geom from mesh
	body_geom =	dCreateTriMesh (space, data, 0, 0, 0);
	dGeomSetBody (body_geom, body_body);
}

void PhysicsRagDoll::CreateLegs (dWorldID world, dSpaceID space) {
	for (int i = 0; i < 4; i++) {
		leg_body[i] = dBodyCreate (world);
		dMassSetBox (&m, 1, 2, 6, 2);
		dMassAdjust (&m, 2);
		dBodySetMass (leg_body[i], &m);
		leg_geom[i] = dCreateBox (space, 2, 6, 2);
		dGeomSetBody (leg_geom[i], leg_body[i]);
	}
}

void PhysicsRagDoll::CreateHands (dWorldID world, dSpaceID space) {
	for (int i = 0; i < 4; i++) {
		hand_body[i] = dBodyCreate (world);
		dMassSetBox (&m, 1, 2, 6, 2);
		dMassAdjust (&m, 1);
		dBodySetMass (hand_body[i], &m);
		hand_geom[i] = dCreateBox (space, 2, 6, 2);
		dGeomSetBody (hand_geom[i], hand_body[i]);
	}
}

void PhysicsRagDoll::CreateRagDoll (dWorldID world, dSpaceID space, float x,
		float y, float z, IAnimatedMesh *mesh) {
	CreateLegs (world, space);
	CreateBody (world, space, mesh);
	CreateHands (world, space);
	//CreateHead (world, space);
	//DisableRagDoll ();
	
	// Position body parts correctly and connect them
	
	// Left leg
	dBodySetPosition (leg_body[0], x - 2, y - 6, z);
	dBodySetPosition (leg_body[1], x - 2, y - 12, z);
	knee_joint[0] = dJointCreateHinge (world, 0);
	dJointAttach (knee_joint[0], leg_body[0], leg_body[1]);
	dJointSetHingeAnchor (knee_joint[0], x - 2, y - 9, z);
	dJointSetHingeAxis (knee_joint[0], 1, 0, 0);

	// Right leg
	dBodySetPosition (leg_body[2], x + 2, y - 6, z);
	dBodySetPosition (leg_body[3], x + 2, y -12, z);
	knee_joint[1] = dJointCreateHinge (world, 0);
	dJointAttach (knee_joint[1], leg_body[2], leg_body[3]);
	dJointSetHingeAnchor (knee_joint[1], x, y - 9, z);
	dJointSetHingeAxis (knee_joint[1], 1, 0, 0);

	// Some angle constraints for knees
	// Left knee
	dJointSetHingeParam (knee_joint[0], dParamLoStop, DegreeToRadian (-150));
	dJointSetHingeParam (knee_joint[0], dParamHiStop, DegreeToRadian (0));
	// Right knee
	dJointSetHingeParam (knee_joint[1], dParamLoStop, DegreeToRadian (-150));
	dJointSetHingeParam (knee_joint[1], dParamHiStop, DegreeToRadian (0));

	// Body
	dBodySetPosition (body_body, x, y, z);

	// Left part of the hip
	hip_joint[0] = dJointCreateUniversal (world, 0);
	dJointAttach (hip_joint[0], body_body, leg_body[0]);
	dJointSetUniversalAnchor (hip_joint[0], x - 1, y - 3, z);
	dJointSetUniversalAxis1 (hip_joint[0], 0, 0, 1);
	dJointSetUniversalAxis2 (hip_joint[0], 1, 0, 0);

	// Right part of the hip
	hip_joint[1] = dJointCreateUniversal (world, 0);
	dJointAttach (hip_joint[1], body_body, leg_body[2]);
	dJointSetUniversalAnchor (hip_joint[1], x + 1, y - 3, z);
	dJointSetUniversalAxis1 (hip_joint[1], 0, 0, 1);
	dJointSetUniversalAxis2 (hip_joint[1], 1, 0, 0);
	
	// Set some angle constraints for the hip
	// Left part of the hip
	dJointSetUniversalParam (hip_joint[0], dParamLoStop, DegreeToRadian (-90));
	dJointSetUniversalParam (hip_joint[0], dParamHiStop, DegreeToRadian (0));

	dJointSetUniversalParam (hip_joint[0], dParamLoStop2, DegreeToRadian (-160));
	dJointSetUniversalParam (hip_joint[0], dParamHiStop2, DegreeToRadian (45));

	// Left part of the hip
	dJointSetUniversalParam (hip_joint[1], dParamLoStop, DegreeToRadian (0));
	dJointSetUniversalParam (hip_joint[1], dParamHiStop, DegreeToRadian (90));

	dJointSetUniversalParam (hip_joint[1], dParamLoStop2, DegreeToRadian (-160));
	dJointSetUniversalParam (hip_joint[1], dParamHiStop2, DegreeToRadian (45));

	// Left hand
	dBodySetPosition (hand_body[0], x - 4, y, z);
	dBodySetPosition (hand_body[1], x - 4, y - 6, z);
	elbow_joint[0] = dJointCreateHinge (world, 0);
	dJointAttach (elbow_joint[0], hand_body[0], hand_body[1]);
	dJointSetHingeAnchor (elbow_joint[0], x - 4, y - 3, z);
	dJointSetHingeAxis (elbow_joint[0], 1, 0, 0);

	// Right hand
	dBodySetPosition (hand_body[2], x + 4, y, z);
	dBodySetPosition (hand_body[3], x + 4, y - 6, z);
	elbow_joint[1] = dJointCreateHinge (world, 0);
	dJointAttach (elbow_joint[1], hand_body[2], hand_body[3]);
	dJointSetHingeAnchor (elbow_joint[1], x + 4, y - 3, z);
	dJointSetHingeAxis (elbow_joint[1], 1, 0, 0);

	// Set some angle constraints for elbows
	// Left elbow
	dJointSetHingeParam (elbow_joint[0], dParamLoStop, DegreeToRadian (0));
	dJointSetHingeParam (elbow_joint[0], dParamHiStop, DegreeToRadian (150));
	// Right elbow
	dJointSetHingeParam (elbow_joint[1], dParamLoStop, DegreeToRadian (0));
	dJointSetHingeParam (elbow_joint[1], dParamHiStop, DegreeToRadian (150));

	// Left shoulder
	shoulder_joint[0] = dJointCreateUniversal (world, 0);
	dJointAttach (shoulder_joint[0], body_body, hand_body[0]);
	dJointSetUniversalAnchor (shoulder_joint[0], x - 3, y + 3, z);
	dJointSetUniversalAxis1 (shoulder_joint[0], 0, 0, 1);
	dJointSetUniversalAxis2 (shoulder_joint[0], 1, 0, 0);

	// Right shoulder
	shoulder_joint[1] = dJointCreateUniversal (world, 0);
	dJointAttach (shoulder_joint[1], body_body, hand_body[2]);
	dJointSetUniversalAnchor (shoulder_joint[1], x + 3, y + 3, z);
	dJointSetUniversalAxis1 (shoulder_joint[1], 0, 0, 1);
	dJointSetUniversalAxis2 (shoulder_joint[1], 1, 0, 0);

	// Set some angle constraints for shoulders
	// Left shoulder
	dJointSetUniversalParam (shoulder_joint[0], dParamLoStop, DegreeToRadian (-160));
	dJointSetUniversalParam (shoulder_joint[0], dParamHiStop, DegreeToRadian (0));

	dJointSetUniversalParam (shoulder_joint[0], dParamLoStop2, DegreeToRadian (-160));
	dJointSetUniversalParam (shoulder_joint[0], dParamHiStop2, DegreeToRadian (90));

	// Right shoulder
	dJointSetUniversalParam (shoulder_joint[1], dParamLoStop, DegreeToRadian (0));
	dJointSetUniversalParam (shoulder_joint[1], dParamHiStop, DegreeToRadian (160));

	dJointSetUniversalParam (shoulder_joint[1], dParamLoStop2, DegreeToRadian (-160));
	dJointSetUniversalParam (shoulder_joint[1], dParamHiStop2, DegreeToRadian (90));
/*
	//Head
	dBodySetPosition (head_body, 92, -40, 0);
	neck_joint = dJointCreateBall (world, 0);
	dJointAttach (neck_joint, body_body, head_body);
	dJointSetBallAnchor (neck_joint, 92, -41, 0);
*/
	//CreateRagDollSpace (space);
}

void PhysicsRagDoll::DestroyRagDoll () {
	for (int i = 0; i < 4; i++)
		dGeomDestroy (leg_geom[i]);
	dGeomDestroy (body_geom);
	for (int i = 0; i < 4; i++)
		dGeomDestroy (hand_geom[i]);
	//dGeomDestroy (head_geom);
}

void PhysicsRagDoll::DisableCheck (bool run) {
	if (!run)
		return;
	if (!CheckForDisabling (body_body, dis_lvel, dis_avel))
		dis_steps_cur = 0;
	if (dis_steps_cur < dis_steps_given) {
		for (int i = 0; i < 4; i++) {
			if (!CheckForDisabling (leg_body[i], dis_lvel, dis_avel)) {
				dis_steps_cur = 0;
				return;
			}
			if (!CheckForDisabling (hand_body[i], dis_lvel, dis_avel)) {
				dis_steps_cur = 0;
				return;
			}
		}
		if (!CheckForDisabling (body_body, dis_lvel, dis_avel)) {
			dis_steps_cur = 0;
			return;
		}
		dis_steps_cur++;
	}
	// I doubt we should check if bodies are enabled to disable them,
	// I guess it's more time consuming than only disabling, am I wrong?
	if (dis_steps_cur >= dis_steps_given) 
		DisableRagDoll ();
}

void PhysicsRagDoll::SetDisableParams (float lvel, float avel, int steps) {
	dis_steps_given = steps;
	dis_lvel = lvel;
	dis_avel = avel;
}

void PhysicsRagDoll::DisableRagDoll () {
	for (int i = 0; i < 4; i++) {
		dBodyDisable (leg_body[i]);
		dBodyDisable (hand_body[i]);
	}
	dBodyDisable (body_body);
#ifdef DEBUG
	cout << "Rag doll disabled\n";
#endif
}

/*
void PhysicsRagDoll::Reanimate () {
	dBodyEnable (body_body);
	dis_steps_cur = 0;
	const dReal *pos = dBodyGetPosition (body_body);
	dBodySetPosition (body_body, pos[0] + rand () % 50, pos[1] + rand () % 50,
			pos[2] + rand () % 50);
}
*/

/*
void PhysicsRagDoll::CreateRagDollSpace (dSpaceID space) {
	// WARNING: doesn't work correctly!
	rag_doll_space = dSimpleSpaceCreate (space);
	dSpaceSetCleanup (rag_doll_space, 0);
	for (int i = 0; i < 4; i++) {
		dSpaceAdd (rag_doll_space, hand_geom[i]);
		dSpaceAdd (rag_doll_space, leg_geom[i]);
	}
	dSpaceAdd (rag_doll_space, body_geom);
	//dSpaceAdd (rag_doll_space, head_geom);
}
*/

