/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/


#include <toolbox_stdafx.h>
#include "SkyBox.h"
#include "../DemoEntityManager.h"
#include "../DemoCamera.h"
#include "../PhysicsUtils.h"

#define FRICTION_VAR_NAME "friction"
static unsigned frictionCRC (dCRC (FRICTION_VAR_NAME));

static void UserContactFriction (const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
{
	// call  the basic call back
	GenericContactProcess (contactJoint, timestep, threadIndex);


	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;
	dFloat mass;

	const NewtonBody* body0 = NewtonJointGetBody0(contactJoint);
	const NewtonBody* body1 = NewtonJointGetBody1(contactJoint);
	const NewtonBody* body = body0;
	NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
	if (mass == 0.0f) {
		body = body1;
	}

	DemoEntity* entity = (DemoEntity*) NewtonBodyGetUserData (body);
	dVariable* friction = entity->FindVariable (frictionCRC);
	_ASSERTE (friction);
	dFloat frictionValue = friction->GetFloat();
	for (void* contact = NewtonContactJointGetFirstContact (contactJoint); contact; contact = NewtonContactJointGetNextContact (contactJoint, contact)) {
		NewtonMaterial* material = NewtonContactGetMaterial (contact);
		NewtonMaterialSetContactFrictionCoef (material, frictionValue + 0.1f, frictionValue, 0);
		NewtonMaterialSetContactFrictionCoef (material, frictionValue + 0.1f, frictionValue, 1);
	}
}

/*
static void xxx(NewtonWorld* const world)
{

	// Make collision tree
	NewtonCollision* pCollision = NewtonCreateTreeCollision(world, 0);
	NewtonTreeCollisionBeginBuild(pCollision);

	const int cNumVerts = 8;
	float verts[cNumVerts][3] = 
	{
		0.5,    0,    0,
		-0.5,    0,    0,
		0,    0, -2.5,
		0,    0,  2.5,
		0.5, 0.25,    0,
		-0.5, 0.25,    0,
		0, 0.25, -2.5,
		0, 0.25,  2.5
	};
	const int cNumFaces = 12;
	int indices[cNumFaces * 3] = 
	{
		0, 1, 2,
		0, 3, 1,
		4, 6, 5,
		4, 5, 7,
		5, 6, 2,
		5, 2, 1,
		7, 5, 1,
		7, 1, 3,
		4, 2, 6,
		4, 0, 2,
		4, 7, 3,
		4, 3, 0
	};

	for (int i = 0; i < cNumFaces; i++)
//	for (int i = 0; i < 2; i++)
	{
		int index0 = indices[i * 3];
		int index1 = indices[i * 3 + 1];
		int index2 = indices[i * 3 + 2];

		float faceVerts[3][3];
		faceVerts[0][0] = verts[index0][0];
		faceVerts[0][1] = verts[index0][1];
		faceVerts[0][2] = verts[index0][2];
		faceVerts[1][0] = verts[index1][0];
		faceVerts[1][1] = verts[index1][1];
		faceVerts[1][2] = verts[index1][2];
		faceVerts[2][0] = verts[index2][0];
		faceVerts[2][1] = verts[index2][1];
		faceVerts[2][2] = verts[index2][2];

		NewtonTreeCollisionAddFace(pCollision, 3, &(faceVerts[0][0]), 12, 0);
	}
	NewtonTreeCollisionEndBuild(pCollision, 1);

	// Add chamfer cylinder
	float offsetmtx[16] = { 0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0.1f, 0, 1 };
	NewtonCollision* pCylCollision = NewtonCreateChamferCylinder(world, 0.5f, 0.2f, 1, offsetmtx);

	// Matrices of collision tree and cylinder
	float treeMtx[16] = { 0.70710677f, 0, 0.70710677f, 0, 0, 1, 0, 0, -0.70710677f, 0, 0.70710677f, 0, 26.652052f, 0.5f, 43.713711f, 1 };
	float cylMtx[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		26.0855798f, 0.495723963f, 44.80299f, 1.0f };

	// Do collision
	const int cMaxContacts = 15;
	float contacts[cMaxContacts][3];
	float normals[cMaxContacts][3];
	float penetrations[cMaxContacts];
	int numContacts = NewtonCollisionCollide(world, cMaxContacts, pCylCollision, cylMtx, pCollision, treeMtx, &(contacts[0][0]), &(normals[0][0]), &(penetrations[0]), 0);

	for(int i = 0; i < numContacts; i ++) {
	
	}
}
*/


void Friction (DemoEntityManager* const scene)
{
	// suspend simulation before making changes to the physics world
	scene->StopsExecution ();

	// load the skybox
	scene->Append(new SkyBox());


	// load the scene from and alchemedia file format
	char fileName[2048];
	GetWorkingFileName ("frictionDemo.xml", fileName);
	scene->LoadScene (fileName);


	// set a default material call back
	NewtonWorld* const world = scene->GetNewton();
	int defaultMaterialID = NewtonMaterialGetDefaultGroupID (world);
	NewtonMaterialSetCollisionCallback (world, defaultMaterialID, defaultMaterialID, NULL, NULL, UserContactFriction); 


	// customize the scene after loading
	// set a user friction variable in the body for variable friction demos
	// later this will be done using LUA script
	int index = 0;
	for (NewtonBody* body = NewtonWorldGetFirstBody(world); body; body = NewtonWorldGetNextBody(world, body)) {
		dFloat Ixx;
		dFloat Iyy;
		dFloat Izz;
		dFloat mass;
		NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
		if (mass > 0.0f) {
			DemoEntity* const entity = (DemoEntity*) NewtonBodyGetUserData (body);
			dVariable* const friction = entity->CreateVariable (FRICTION_VAR_NAME);
			_ASSERTE (friction);
			friction->SetValue(dFloat (index) * 0.03f);
			index ++;
		}
	}

	// place camera into position
	dVector origin (-70.0f, 10.0f, 0.0f, 0.0f);
	scene->GetCamera()->m_upVector = dVector (0.0f, 1.0f, 0.0f);
	scene->GetCamera()->m_origin = origin;
	scene->GetCamera()->m_pointOfInterest = origin + dVector (1.0f, 0.0f, 0.0f);

//	scene->SaveScene ("test1.xml");
//	dScene CreateAlchemediaFromPhysic(); 

	// resume the simulation
	scene->ContinueExecution();
}

