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


#if 0
static void SetDemoCallbacks (NewtonFrame& system)
{
	system.m_control = Keyboard;
	system.m_autoSleep = AutoSleep;
	system.m_showIslands = SetShowIslands;
	system.m_showContacts = SetShowContacts; 
	system.m_setMeshCollision = SetShowMeshCollision;
}

static void BuildFloorAndSceneRoot (NewtonFrame& system)
{
_ASSERTE (0);
/*

	NewtonWorld* world;
	RenderPrimitive* floor;
	NewtonBody* floorBody;
	NewtonCollision* floorCollision;
	OGLMesh* meshInstance;

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

	//	meshInstance = OGLMesh::MakeBox (world, size.m_x, size.m_y, size.m_z, "GrassAndDirt.tga");
	meshInstance = new OGLMesh ("floor", floorCollision, "GrassAndDirt.tga", "metal_30.tga", "metal_30.tga");
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
*/
}


static void BasicCollisionStacks (NewtonFrame& system, NewtonCollision* collision, int high)
{
_ASSERTE (0);
/*

	dFloat mass;
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;
	dVector origin;
	dVector inertia;
	NewtonWorld* world;
	OGLMesh* meshInstance;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);

	// set the initial size
	meshInstance = new OGLMesh ("stackShape", collision, "smilli.tga", "smilli.tga", "smilli.tga");

	dMatrix rotate (dRollMatrix (3.141592f * 0.5));
	dMatrix location (GetIdentityMatrix());

	mass = 1.0f;
	NewtonConvexCollisionCalculateInertialMatrix (collision, &inertia[0], &origin[0]);	
	Ixx = mass * inertia[0];
	Iyy = mass * inertia[1];
	Izz = mass * inertia[2];


	// get the box spacing
	dVector size;
	for (int i = 0; i < 3; i ++) {
		dVector p0;
		dVector p1;
		dVector dir (0.0f, 0.0f, 0.0f, 0.0f);

		dir[i] = -1.0f;
		NewtonCollisionSupportVertex (collision, &dir[0], &p0[0]);
		dir[i] = 1.0f;
		NewtonCollisionSupportVertex (collision, &dir[0], &p1[0]);
		size[i] = p1[i] - p0[i]; 
	}

	size.m_x *= 4.0f;
	size.m_z *= 4.0f;

	#define X_COUNT 8
	#define Z_COUNT 8

	// create 100 stacks of 10 boxes each
	location.m_posit.m_x = -(X_COUNT * size.m_x * 0.5f); 
	for (int k = 0; k < Z_COUNT; k ++) { 
		location.m_posit.m_z = -(Z_COUNT * size.m_z * 0.5f); 

		for (int j = 0; j < X_COUNT; j ++) { 
			location.m_posit.m_y = FindFloor(system.m_world, location.m_posit.m_x, location.m_posit.m_z) + size.m_y * 0.5f;

			for (int i = 0; i < high; i ++) {
				RenderPrimitive* box;
				NewtonBody* boxBody;

				// create a graphic box
				dMatrix matrix (dYawMatrix(i * 0.1f * 90.0f * 3.141592f / 180.0f));

				matrix.m_posit = location.m_posit;
				box  = new RenderPrimitive (location, meshInstance);
				system.AddModel___ (box);
				box->Release();

				//create the rigid body
				boxBody = NewtonCreateBody (world, collision);

				// save the pointer to the graphic object with the body.
				NewtonBodySetUserData (boxBody, box);

				// set a destructor for this rigid body
				NewtonBodySetDestructorCallback (boxBody, PhysicsBodyDestructor);

				// set the transform call back function
				NewtonBodySetTransformCallback (boxBody, PhysicsSetTransform);

				// set the force and torque call back function
				NewtonBodySetForceAndTorqueCallback (boxBody, PhysicsApplyGravityForce);

				// set the mass matrix
				//NewtonBodySetMassMatrix (boxBody, 1.0f, 1.0f / 6.0f, 1.0f / 6.0f, 1.0f  / 6.0f);
				NewtonBodySetMassMatrix (boxBody, mass, Ixx, Iyy, Izz);

				// set the matrix for both the rigid body and the graphic body
				NewtonBodySetMatrix (boxBody, &matrix[0][0]);
				PhysicsSetTransform (boxBody, &matrix[0][0], 0);

				float space;
				space = 1.0f;
				location.m_posit.m_y += size.m_y * space;
			}
			location.m_posit.m_z += size.m_z; 	
		}
		location.m_posit.m_x += size.m_x; 
	}


	// release the collision geometry when not need it
	meshInstance->Release();

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-40.0f, 10.0f, 0.0f));
*/
}



void BasicCylinderStacks (NewtonFrame& system)
{
	NewtonCollision* collision;
	dVector size (1.0f, 1.0f, 1.0f);

	dMatrix matrix (dgGrammSchmidt (dVector (0.0f, 1.0f, 0.0, 0.0)));

	collision = NewtonCreateCylinder (system.m_world, size.m_x, size.m_y, 0, &matrix[0][0]); 
	BasicCollisionStacks (system, collision, 10);
	NewtonReleaseCollision (system.m_world, collision);
}

void BasicConvexStacks (NewtonFrame& system)
{
	dInt32 i;
	dInt32 j;
	dInt32 size;
	dInt32 pointsCount;
	dFloat y;
	dVector convexShape[2048];
	NewtonCollision* collision;

	y = -0.5f;
	size = 8;
	pointsCount = 0;
	dMatrix rotation (dYawMatrix(2.0f * 3.141592f / size));
	for (i = 0; i < 4; i ++) {
		dFloat pad;
		pad = ((i == 1) || (i == 2)) * 0.25f;
		dVector p (0.5f + pad, y, 0.0f);
		y += 0.3333f;
		dMatrix acc (GetIdentityMatrix());
		for (j = 0; j < size; j ++) {
			convexShape[pointsCount] = acc.RotateVector(p);
			acc = acc * rotation;
			pointsCount ++;
		}
	}

	collision = NewtonCreateConvexHull (system.m_world, pointsCount, &convexShape[0].m_x, sizeof (dVector), 0.01f, 0, NULL); 

	BasicCollisionStacks (system, collision, 10);
	NewtonReleaseCollision (system.m_world, collision);
}
#endif



void BasicBoxStacks (DemoEntityManager* const scene)
{
	// suspend simulation before making changes to the physics world
	scene->StopsExecution ();

	// load the skybox
	scene->Append(new SkyBox());


	// load the scene from and alchemedia file format
	char fileName[2048];
	//GetWorkingFileName ("boxStacks_1.xml", fileName);
	//GetWorkingFileName ("boxStacks_3.xml", fileName);
	GetWorkingFileName ("boxStacks.xml", fileName);
	//GetWorkingFileName ("pyramid40x40.xml", fileName);
	scene->LoadScene (fileName);

	// place camera into position
	dVector origin (-40.0f, 10.0f, 0.0f, 0.0f);
	scene->GetCamera()->m_upVector = dVector (0.0f, 1.0f, 0.0f);
	scene->GetCamera()->m_origin = origin;
	scene->GetCamera()->m_pointOfInterest = origin + dVector (1.0f, 0.0f, 0.0f);

	// resume the simulation
	scene->ContinueExecution();
}

