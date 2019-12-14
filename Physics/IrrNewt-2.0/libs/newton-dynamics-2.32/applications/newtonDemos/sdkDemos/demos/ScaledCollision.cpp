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


static void AddScaleShape (NewtonFrame& system, const dVector& origin, PrimitiveType type)
{
_ASSERTE (0);
/*

	int xCount;
	int zCount;
	int defaultMaterialID;
	dFloat x;
	dFloat z;
	dFloat spacing;
	
	xCount = 2;
	zCount = 2;
	spacing = 4.0f;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (system.m_world);

	dVector size (1.0f, 0.25f, 0.5f);
	if (type == _SPHERE_PRIMITIVE) {
		size.m_y = 1.0f;
		size.m_z = 1.0f;
	}

	dMatrix matrix (GetIdentityMatrix());
	//	dVector origin (cameraEyepoint + cameraDir.Scale (80.0f));
	for (int i = 0; i < xCount; i ++) {
		x = origin.m_x + (i - xCount / 2) * spacing;
		for (int j = 0; j < zCount; j ++) {
			NewtonBody* body;
			RenderPrimitive* node;
			NewtonCollision* collision;
			NewtonCollision* modifier;
			z = origin.m_z + (j - zCount / 2) * spacing;

			// create a simple primitive
			matrix.m_posit.m_x = x;
			matrix.m_posit.m_y = FindFloor (system.m_world, x, z) + 5.0f;
			matrix.m_posit.m_z = z;
			body = CreateGenericSolid (system.m_world, &system, "test", 1.0f, matrix, size, type, defaultMaterialID);

			// create a shape modifier and replace the collision shape of the body
			collision = NewtonBodyGetCollision(body);
			modifier = NewtonCreateConvexHullModifier (system.m_world, collision, 0);
			NewtonBodySetCollision (body, modifier);
			NewtonReleaseCollision (system.m_world, modifier);

			// set the scaled matrix of the shape
			dMatrix scaleMatrix;
			NewtonConvexHullModifierGetMatrix (modifier, &scaleMatrix[0][0]);
			scaleMatrix[0][0] *= (2.0f + RandomVariable (1.0f));
			scaleMatrix[1][1] *= (2.0f + RandomVariable (1.0f));
//			scaleMatrix[2][2] *= (2.0f + RandomVariable (1.0f));
			if(i == ((xCount -1)) && (j == (zCount -1))) {
				scaleMatrix[0][2] = 2.0f;
			}
			NewtonConvexHullModifierSetMatrix (modifier, &scaleMatrix[0][0]);

			// now make a visual representation to match the collision shape
			OGLMesh* geo;
			node = (RenderPrimitive*) NewtonBodyGetUserData (body);

			char meshName[256];
			sprintf (meshName, "shape%d", rand());
			geo = new OGLMesh(meshName, modifier, "wood_0.tga", "wood_0.tga", "wood_1.tga");
			node->RemoveMesh(node->GetMesh());
			node->AddMesh(geo);
			geo->Release();
		}
	}
*/
}


void ScaledCollision (NewtonFrame& system)
{
	NewtonWorld* world;
	world = system.m_world;

	// create the sky box and the floor,
	BuildFloorAndSceneRoot (system);
	dVector location (cameraEyepoint + cameraDir.Scale (20.0f));

	AddScaleShape (system, location, _SPHERE_PRIMITIVE);
	AddScaleShape (system, location, _BOX_PRIMITIVE);
	AddScaleShape (system, location, _CONE_PRIMITIVE);
	AddScaleShape (system, location, _CYLINDER_PRIMITIVE);
	AddScaleShape (system, location, _CAPSULE_PRIMITIVE);
	AddScaleShape (system, location, _CHAMFER_CYLINDER_PRIMITIVE);
	AddScaleShape (system, location, _RANDOM_CONVEX_HULL_PRIMITIVE);
	AddScaleShape (system, location, _REGULAR_CONVEX_HULL_PRIMITIVE);
}





