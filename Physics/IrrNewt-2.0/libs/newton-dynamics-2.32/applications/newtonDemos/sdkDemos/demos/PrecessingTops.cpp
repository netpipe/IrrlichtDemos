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



void PrecessingTops (NewtonFrame& system)
{
_ASSERTE (0);
/*

	int xCount;
	int zCount;
	int defaultMaterialID;
	dFloat x;
	dFloat z;
	dFloat spacing;
	NewtonWorld* world;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);
	dVector location (cameraEyepoint + cameraDir.Scale (20.0f));
	
	xCount = 10;
	zCount = 10;
	spacing = 6.0f;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (system.m_world);

	dVector size (3.0f, 1.0f, 0.5f);
	dVector origin (cameraEyepoint + cameraDir.Scale (25.0f));


	dMatrix matrix (dRollMatrix (- 70.0f * 3.141592f / 180.0f));
	dVector omega (-40.0f, 0.0f, 0.0f);
	omega = matrix.RotateVector (omega);

	// create the shape and visual mesh as a common data to be re used
	NewtonCollision* coneCollision = CreateConvexCollision (system.m_world, GetIdentityMatrix(), size, _CONE_PRIMITIVE, defaultMaterialID);
	OGLMesh* coneMesh = new OGLMesh ("cone", coneCollision, "wood_0.tga", "wood_0.tga", "wood_1.tga");


	for (int i = 0; i < xCount; i ++) {
		x = origin.m_x + (i - xCount / 2) * spacing;
		for (int j = 0; j < zCount; j ++) {
			dFloat mass;
			NewtonBody* body;
			z = origin.m_z + (j - zCount / 2) * spacing;

			mass = 1.0f;
			// create a simple primitive
			matrix.m_posit.m_x = x;
			matrix.m_posit.m_y = FindFloor (system.m_world, x, z) + 5.0f;
			matrix.m_posit.m_z = z;

//			body = CreateGenericSolid (system.m_world, &system, mass, matrix, size, _CONE_PRIMITIVE, defaultMaterialID);
			body = CreateSimpleSolid (system.m_world, &system, coneMesh, mass, matrix, coneCollision, defaultMaterialID);
			NewtonBodySetOmega (body, &omega[0]);

			dVector damp (0.0f, 0.0f, 0.0f);
			NewtonBodySetAngularDamping (body, &damp[0]);
		}
	}

	// do not forget to release the assets	
	coneMesh->Release(); 
	NewtonReleaseCollision(system.m_world, coneCollision);


	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-60.0f, 10.0f, 0.0f));
*/
}



