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


/*
static NetwonMesh* BuildCompoundCollisionFromMesh (NewtonFrame& system, char* meshName)
{
	dFloat mass;
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;
	dVector origin;
	dVector inertia;
	NewtonWorld* world;
	OGLMesh* meshInstance;

	world = system.m_world;

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
}
*/

static OGLModel* AddComplexMesh (NewtonFrame& system, char* meshName, dVector location)
{
_ASSERTE (0);
return NULL;
/*

	OGLModel* model = new OGLModel;

	char fullPathName[2048];
	GetWorkingFileName (meshName, fullPathName);
	OGLLoaderContext context;
	dMatrix rotMatrix (dYawMatrix (-3.14159265f * 0.5f));

_ASSERTE (0);
//	model->LoadCollada(fullPathName, context, rotMatrix, 1.0f);

	dMatrix matrix (GetIdentityMatrix());
	matrix.m_posit.m_x = location.m_x;
	matrix.m_posit.m_y = location.m_y;
	matrix.m_posit.m_z = location.m_z;
	model->SetMatrix (matrix);

	system.AddModel___ (model);

	return model;
*/
}



static void CreateBody (NewtonCollision* collision, OGLModel* model, NewtonWorld* world)
{
	//create the rigid body
	NewtonBody* rigidBody = NewtonCreateBody (world, collision);

	NewtonBodySetUserData(rigidBody, model);
	NewtonBodySetTransformCallback (rigidBody, PhysicsSetTransform);

	dMatrix matrix (model->GetMatrix());
	matrix.m_posit.m_y = FindFloor (world, matrix.m_posit.m_x, matrix.m_posit.m_z) + 5.0f;
	NewtonBodySetMatrix(rigidBody, &matrix[0][0]);

	dFloat mass = 1.0f;

	dVector origin;
	dVector inertia;
	NewtonConvexCollisionCalculateInertialMatrix (collision, &inertia[0], &origin[0]);	
	inertia = inertia.Scale (mass);

	NewtonBodySetMassMatrix (rigidBody, mass, inertia.m_x, inertia.m_y, inertia.m_z); 
	NewtonBodySetCentreOfMass (rigidBody, &origin[0]);

}





static NewtonCollision* BuildCompounCollisionFromMesh (OGLMesh* geometry, NewtonWorld* world)
{
_ASSERTE (0);
return NULL;
/*

	NewtonCollision* collision;
	NewtonCollision* compounentHulls[128];

	// Build a Newton Mesh from the visual geometry
	NewtonMesh* mesh = geometry->BuildMesh (world);
	

	// build an array of convex hulls from this mesh.
	int compounentsCount = NewtonMeshConvexApproximation (mesh, world, 0.1f, 128, compounentHulls) ;
	NewtonMeshDestroy(mesh);

	// create a compound collision form this mesh
	collision = NewtonCreateCompoundCollision (world, compounentsCount, compounentHulls, 0);

	// release all the components
	for (int i = 0; i < compounentsCount; i ++) {
		NewtonReleaseCollision (world, compounentHulls[i]);
	}

	return collision;
*/
}


void ConvexApproximation  (NewtonFrame& system)
{
_ASSERTE (0);
/*
	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);

	// Load a Model 
	OGLModel* model = AddComplexMesh (system, "concave0.dae", dVector (-10, 0, 0));
	model->Release();

	// get the mesh for this model
	OGLMesh* geoMesh = (OGLMesh *) model->m_meshList.GetFirst()->GetInfo().m_mesh;
	NewtonCollision* collision = BuildCompounCollisionFromMesh (geoMesh, system.m_world);
	

	CreateBody (collision, model, system.m_world);

	NewtonReleaseCollision (system.m_world, collision);


	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-40.0f, 10.0f, 0.0f));
*/
}

