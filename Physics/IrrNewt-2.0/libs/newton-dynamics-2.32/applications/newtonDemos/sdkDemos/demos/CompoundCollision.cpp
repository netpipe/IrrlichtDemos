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
#include "../toolBox/LevelPrimitive.h"
#include "../toolBox/HeightFieldPrimitive.h"
#include "../toolBox/UserHeightFieldCollision.h"

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
	NewtonBody* floorBody;

	world = system.m_world;
	// /////////////////////////////////////////////////////////////////////
	//
	// create the sky box,
	OGLModel* sky = new SkyBox ();
	system.AddModel___ (sky);
	sky->Release();

	// create the the floor graphic objects
	
	dMatrix location (GetIdentityMatrix());
	location.m_posit.m_y = -5.0f; 

	
#if 0
	// create a box for floor 
	dVector floorSize (100.0f, 2.0f, 100.0f);
	NewtonCollision* floorCollision = NewtonCreateBox (world, floorSize.m_x, floorSize.m_y, floorSize.m_z, 0, NULL); 

	OGLMesh* meshInstance = new OGLMesh ("ground", floorCollision, "GrassAndDirt.tga", "metal_30.tga", "metal_30.tga");
	RenderPrimitive* level = new RenderPrimitive (location, meshInstance);
	system.AddModel___ (level);
	level->Release();
	meshInstance->Release();

	// create the the floor collision, and body with default values
	floorBody = NewtonCreateBody (world, floorCollision);
	NewtonReleaseCollision (world, floorCollision);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData (floorBody, level);

#else
	HeightFieldPrimitive* level;
	level = new HeightFieldPrimitive (world);
	NewtonBodyGetMatrix(level->GetRigidBody(), &location[0][0]);

	system.AddModel___ (level);
	level->Release();
	floorBody = level->GetRigidBody();

#endif

	// set the transformation for this rigid body
	level->SetMatrix(location);
	NewtonBodySetMatrix (floorBody, &location[0][0]);

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


void CompoundCollision (NewtonFrame& system)
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
	NewtonCollision* compound;

	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);
	dVector location (cameraEyepoint + cameraDir.Scale (20.0f));
	
	xCount = 10;
	zCount = 10;
	spacing = 6.0f;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (system.m_world);

	dVector size (1.0f, 0.25f, 0.5f);
	dVector origin (cameraEyepoint + cameraDir.Scale (20.0f));

	// create the compound pieces;

	NewtonCollision* snowMan[5];
	snowMan[0] = NewtonCreateSphere (system.m_world, 1.0f, 1.0f, 1.0f, 0, NULL);

	dMatrix offset (GetIdentityMatrix());
	offset.m_posit.m_y += 1.0f + 0.75f;
	snowMan[1] = NewtonCreateSphere (system.m_world, 0.75f, 0.75f, 0.75f, 0, &offset[0][0]);

	offset.m_posit.m_y += 0.75f + 0.5;
	snowMan[2] = NewtonCreateSphere (system.m_world, 0.5f, 0.6f, 0.5f, 0, &offset[0][0]);

	offset.m_posit.m_y -= 0.75f + 0.25;
	offset.m_posit.m_z += 1.0f;
	offset = dYawMatrix (90.0f * 3.141592f / 180.0f) * offset;
	snowMan[3] = NewtonCreateCapsule (system.m_world, 0.35f, 2.0f, 0, &offset[0][0]);

	offset.m_posit.m_z -= 2.0f;
	snowMan[4] = NewtonCreateCapsule (system.m_world, 0.35f, 2.0f, 0, &offset[0][0]);

	// make a compound collision
	compound = NewtonCreateCompoundCollision (system.m_world, sizeof (snowMan) / sizeof (snowMan[1]), snowMan, 0);
	for (int i = 0; i < int (sizeof (snowMan) / sizeof (snowMan[0])); i ++) {
		NewtonReleaseCollision (system.m_world, snowMan[i]);
	}


	// create the visual shape
	OGLMesh* geo;
	geo = new OGLMesh("coumpound", compound, "wood_0.tga", "wood_0.tga", "wood_1.tga");

//xCount = 7;
//zCount = 8;

	dMatrix matrix (GetIdentityMatrix());
	for (int i = 0; i < xCount; i ++) {
		x = origin.m_x + (i - xCount / 2) * spacing;
		for (int j = 0; j < zCount; j ++) {
			dFloat mass;
			NewtonBody* body;
			RenderPrimitive* node;
			
			z = origin.m_z + (j - zCount / 2) * spacing;

			mass = 1.0f;
			// create a simple primitive
			matrix.m_posit.m_x = x;
			matrix.m_posit.m_y = FindFloor (system.m_world, x, z) + 5.0f;
			matrix.m_posit.m_z = z;
			body = CreateGenericSolid (system.m_world, &system, "compound", mass, matrix, size, _SPHERE_PRIMITIVE, defaultMaterialID);

			// make a compound collision
			NewtonBodySetCollision (body, compound);


			// now make a visual representation to match the collision shape
			node = (RenderPrimitive*) NewtonBodyGetUserData (body);
//			node->RemoveMesh(node->m_meshList.GetFirst()->GetInfo().m_data.GetFirst()->GetInfo());
			node->RemoveMesh (node->GetMesh());
			node->AddMesh(geo);

			dVector origin;
			dVector inertia;
			NewtonConvexCollisionCalculateInertialMatrix (compound, &inertia[0], &origin[0]);	
			inertia = inertia.Scale (mass);
			NewtonBodySetMassMatrix (body, mass, inertia.m_x, inertia.m_y, inertia.m_z); 

			dVector com (0.0f, -1.0f, 0.0f, 0.0f);
			NewtonBodySetCentreOfMass (body, &com[0]);


#if 1
			int count;
			dMatrix matrixA (matrix);
			dMatrix matrixB (matrix);
			matrixA.m_posit.m_y += 1.0f;
			matrixB.m_posit.m_z += 0.0f;

			dVector contacts[20];
			dVector normals[20];
			dFloat depths[20];
			NewtonCollision* collisionB = NewtonBodyGetCollision(body);
			count = NewtonCollisionCollide (system.m_world, 10, compound, &matrixA[0][0], collisionB, &matrixB[0][0], &contacts[0][0], &normals[0][0], &depths[0], 0);
			_ASSERTE (count);
#endif

		}
	}

	geo->Release();
	NewtonReleaseCollision (system.m_world, compound);

//	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-44.0f, 4.0f, 0.0f));
	dVector p (dVector (origin.m_x - 20.0f, 0.0f, origin.m_z));
	p.m_y = FindFloor (system.m_world, p.m_x, p.m_z) + 5.0f;
	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), p);
*/
}



