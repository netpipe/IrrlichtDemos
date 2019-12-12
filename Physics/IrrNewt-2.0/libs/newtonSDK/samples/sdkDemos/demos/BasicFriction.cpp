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
#include "RenderPrimitive.h"
#include "../OGLMesh.h"
#include "../MainFrame.h"
#include "../SceneManager.h"
#include "../PhysicsUtils.h"
#include "../toolBox/MousePick.h"
#include "../toolBox/OpenGlUtil.h"
#include "../toolBox/DebugDisplay.h"


static void UserContactFriction (const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
{
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;
	dFloat mass;
	dFloat friction; 
	const NewtonBody* body;
	const NewtonBody* body0;
	const NewtonBody* body1;

	// call  the basic call back
	GenericContactProcess (contactJoint, timestep, threadIndex);

	body0 = NewtonJointGetBody0(contactJoint);
	body1 = NewtonJointGetBody1(contactJoint);

	body = body0;
	NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
	if (mass == 0.0f) {
		body = body1;
	}

	for (void* contact = NewtonContactJointGetFirstContact (contactJoint); contact; contact = NewtonContactJointGetNextContact (contactJoint, contact)) {
		RenderPrimitive* node;
		NewtonMaterial* material;

		material = NewtonContactGetMaterial (contact);
		node = (RenderPrimitive*) NewtonBodyGetUserData (body);
		
		friction = node->m_density;
		NewtonMaterialSetContactFrictionCoef (material, friction + 0.1f, friction, 0);
		NewtonMaterialSetContactFrictionCoef (material, friction + 0.1f, friction, 1);
	}
}

static void SetDemoCallbacks (NewtonFrame& system)
{
	system.m_control = Keyboard;
	system.m_autoSleep = AutoSleep;
	system.m_showIslands = SetShowIslands;
	system.m_showContacts = SetShowContacts; 
	system.m_setMeshCollision = SetShowMeshCollision;
//	system.m_scene = scene;
}


static void BuildFloorAndSceneRoot (NewtonFrame& system)
{
	NewtonWorld* world;
	RenderPrimitive* floor;
	NewtonBody* floorBody;
	NewtonCollision* floorCollision;
	OGLMesh* meshInstance;
//	dSceneNode* scene;

	world = system.m_world;

	// /////////////////////////////////////////////////////////////////////
	//
	// create the sky box,
	OGLModel* sky = new SkyBox ();
	system.AddModel___ (sky);
	sky->Release();



	// create the the floor graphic objects
	dVector floorSize (100.0f, 2.0f, 100.0f);
	dMatrix location (GetIdentityMatrix());
	location.m_posit.m_y = -5.0f; 

	// create a box for floor 
	floorCollision = NewtonCreateBox (world, floorSize.m_x, floorSize.m_y, floorSize.m_z, 0, NULL); 

	meshInstance = new OGLMesh ("ground", floorCollision, "GrassAndDirt.tga", "metal_30.tga", "metal_30.tga");
	floor = new RenderPrimitive (location, meshInstance);
	system.AddModel___ (floor);
	floor->Release();
	meshInstance->Release();

	// create the the floor collision, and body with default values
	floorBody = NewtonCreateBody (world, floorCollision);
	NewtonReleaseCollision (world, floorCollision);


	// set the transformation for this rigid body
	NewtonBodySetMatrix (floorBody, &location[0][0]);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData (floorBody, floor);

	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback (floorBody, PhysicsBodyDestructor);


	// get the default material ID
	int defaultID;
	defaultID = NewtonMaterialGetDefaultGroupID (world);

	// set default material properties
	NewtonMaterialSetDefaultSoftness (world, defaultID, defaultID, 0.05f);
	NewtonMaterialSetDefaultElasticity (world, defaultID, defaultID, 0.4f);
	NewtonMaterialSetDefaultCollidable (world, defaultID, defaultID, 1);
	NewtonMaterialSetDefaultFriction (world, defaultID, defaultID, 1.0f, 0.5f);
	NewtonMaterialSetCollisionCallback (world, defaultID, defaultID, NULL, NULL, GenericContactProcess); 

//	NewtonMaterialSetSurfaceThickness(world, materialID, materialID, 0.1f);
	NewtonMaterialSetSurfaceThickness(world, defaultID, defaultID, 0.0f);

	// set the island update callback
	NewtonSetIslandUpdateEvent (world, PhysicsIslandUpdate);

	// save the callback
	SetDemoCallbacks (system);

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-40.0f, 10.0f, 0.0f));
}


void Friction (NewtonFrame& system)
{
	int zCount;
	int defaultMaterialID;
	dFloat spacing;
	NewtonWorld* world;
	NewtonBody* body;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);

	defaultMaterialID = NewtonMaterialGetDefaultGroupID (system.m_world);
	NewtonMaterialSetCollisionCallback (world, defaultMaterialID, defaultMaterialID, NULL, NULL, UserContactFriction); 

	// create a friction Ramp
	dVector location (cameraEyepoint + cameraDir.Scale (40.0f));
	dMatrix matrix (dPitchMatrix (20.0f * 3.141592f / 180.0f));
	matrix.m_posit = location;
	matrix.m_posit.m_x -= 5.0f;
	matrix.m_posit.m_y -= 8.0f;
	matrix.m_posit.m_z -= 20.0f;
	dVector size (40.0f, 0.2f, 40.0f, 0.0f);
	NewtonCollision* rampCollision = CreateConvexCollision (system.m_world, GetIdentityMatrix(), size, _BOX_PRIMITIVE, defaultMaterialID);
	OGLMesh* rampMesh = new OGLMesh ("incline", rampCollision, "wood_0.tga", "wood_0.tga", "wood_1.tga");
	body = CreateSimpleSolid (system.m_world, &system, rampMesh, 0, matrix, rampCollision, defaultMaterialID);
	rampMesh->Release();
	NewtonReleaseCollision(system.m_world, rampCollision);

	// create some boxes too
	dVector boxSize (1.0f, 0.5f, 2.0f, 0.0f);
	NewtonCollision* boxCollision = CreateConvexCollision (system.m_world, GetIdentityMatrix(), boxSize, _BOX_PRIMITIVE, defaultMaterialID);
	OGLMesh* boxMesh = new OGLMesh ("box", boxCollision, "wood_0.tga", "wood_0.tga", "wood_1.tga");

	zCount = 10;
	spacing = 2.0f;
	dVector origin (matrix.m_posit);
	origin.m_z -= 10.0f;

	// create 
	for (int i = 0; i < zCount; i ++) {
		dFloat z;
		dFloat x;
		dFloat mass;

		z = origin.m_z;
		x = origin.m_x + (i - zCount / 2) * spacing;

		mass = 1.0f;
		matrix.m_posit.m_x = x;
		matrix.m_posit.m_y = FindFloor (system.m_world, x, z) + size.m_y * 0.5f;
		matrix.m_posit.m_z = z;
		
		body = CreateSimpleSolid (system.m_world, &system, boxMesh, mass, matrix, boxCollision, defaultMaterialID);

		// save a coefficient of friction in the the primitive
		RenderPrimitive* node;
		node = (RenderPrimitive*) NewtonBodyGetUserData (body);
		node->m_density = i * 0.03f;
	}

	// release the used collisions
	boxMesh->Release(); 
	NewtonReleaseCollision(system.m_world, boxCollision);

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-70.0f, 10.0f, 0.0f));	
}


