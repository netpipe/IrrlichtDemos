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
#include "../DemoMesh.h"
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

static LevelPrimitive* LoadLevelAndSceneRoot (NewtonFrame& system, const char* levelName, int optimized)
{
_ASSERTE (0);
return NULL;
/*

	NewtonWorld* world;
	NewtonBody* floorBody;
	LevelPrimitive* level;

	world = system.m_world;
	// /////////////////////////////////////////////////////////////////////
	//
	// create the sky box,
	OGLModel* sky = new SkyBox ();
	system.AddModel___ (sky);
	sky->Release();


	// Load a level geometry

	level = new LevelPrimitive (levelName, world, optimized);


	system.AddModel___(level);
	level->Release();
	floorBody = level->m_level;

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
//	NewtonMaterialSetSurfaceThickness(world, defaultID, defaultID, 0.0f);

	// set the island update callback
	NewtonSetIslandUpdateEvent (world, PhysicsIslandUpdate);

	// save th3 callback
	SetDemoCallbacks (system);

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), dVector (-40.0f, 10.0f, 0.0f));

	return level;
*/
}




static void AddBoxes(SceneManager* system, dFloat mass, const dVector& origin, const dVector& oriSize, int xCount, int zCount, dFloat spacing, PrimitiveType type, int materialID)
{
_ASSERTE (0);
/*

	dVector size (oriSize);
	dMatrix matrix (GetIdentityMatrix());

	// create the shape and visual mesh as a common data to be re used
//	type = _BOX_PRIMITIVE;
	NewtonCollision* boxCollision = CreateConvexCollision (system->m_world, GetIdentityMatrix(), size, type, materialID);

	char name[256];
	sprintf (name, "shape%d", type);
	OGLMesh* boxMesh = new OGLMesh (name, boxCollision, "wood_0.tga", "wood_0.tga", "wood_1.tga");

	for (int i = 0; i < xCount; i ++) {
		dFloat x;
		x = origin.m_x + (i - xCount / 2) * spacing;

		for (int j = 0; j < zCount; j ++) {
			dFloat z;
			z = origin.m_z + (j - zCount / 2) * spacing;

			matrix.m_posit.m_x = x;
			matrix.m_posit.m_y = FindFloor (system->m_world, x, z) + 4.0f;
			matrix.m_posit.m_z = z;
//			CreateGenericSolid (system->m_world, system, mass, matrix, size, type, materialID);
			CreateSimpleSolid (system->m_world, system, boxMesh, mass, matrix, boxCollision, materialID);
		}
	}

	// do not forget to release the assets	
	boxMesh->Release(); 
	NewtonReleaseCollision(system->m_world, boxCollision);
*/
}





void SimplePlaneCollision (NewtonFrame& system)
{
	NewtonWorld* world;
	LevelPrimitive *level;
	NewtonCollision* planeColl;

	world = system.m_world;

	// create a material carrier to collision with this object
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (world);

	// create the sky box and the floor,
	level = LoadLevelAndSceneRoot (system, "flatplane.dae", 1);

	// Find the world mesh and replace the collision for a custom plane collision mesh
	dMatrix matrix;
	NewtonBodyGetMatrix (level->m_level, &matrix[0][0]);
	dVector plane (matrix.m_up);
	plane.m_w = - (plane % matrix.m_posit);
	planeColl = CreatePlaneCollidion (world, plane);
	NewtonBodySetCollision(level->m_level, planeColl);
	NewtonReleaseCollision(world, planeColl);


	dVector posit (0.0f, 0.0f, 0.0f, 0.0f);
	posit.m_y = FindFloor (world, posit.m_x, posit.m_z) + 5.0f;

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), posit);

	dVector size (1.0f, 1.0f, 1.0f);
	dVector location (cameraEyepoint + cameraDir.Scale (10.0f));

	AddBoxes (&system, 10.0f, location, size, 3, 3, 10.0f, _SPHERE_PRIMITIVE, defaultMaterialID);
	AddBoxes (&system, 10.0f, location, size, 3, 3, 10.0f, _BOX_PRIMITIVE, defaultMaterialID);
	AddBoxes (&system, 10.0f, location, size, 3, 3, 10.0f, _CONE_PRIMITIVE, defaultMaterialID);
	AddBoxes (&system, 10.0f, location, size, 3, 3, 10.0f, _CYLINDER_PRIMITIVE, defaultMaterialID);
	AddBoxes (&system, 10.0f, location, size, 3, 3, 10.0f, _CAPSULE_PRIMITIVE, defaultMaterialID);
	AddBoxes (&system, 10.0f, location, size, 3, 3, 10.0f, _CHAMFER_CYLINDER_PRIMITIVE, defaultMaterialID);
	AddBoxes (&system, 10.0f, location, size, 3, 3, 10.0f, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
	AddBoxes (&system, 10.0f, location, size, 3, 3, 10.0f, _REGULAR_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
}


void SimpleHeighMapCollision (NewtonFrame& system)
{
_ASSERTE (0);
/*

	NewtonWorld* world;
	LevelPrimitive *level;
	HeightFieldPrimitive* map;

	world = system.m_world;

	// create the sky box and the floor,
	level = LoadLevelAndSceneRoot (system, "flatplane.dae", 0);

	// create a material to collide with this object
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (world);

	// delete the existing levelMesh
	NewtonDestroyBody (world, level->m_level);
	system.RemoveModel (level);

	// add a height map mesh 
	map = new HeightFieldPrimitive (world);
	system.AddModel___ (map);
	map->Release();


	dVector posit (0.0f, 0.0f, 0.0f, 0.0f);
	posit.m_y = FindFloor (world, posit.m_x, posit.m_z) + 5.0f;

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), posit);

	dVector size (1.0f, 1.0f, 1.0f);
	dVector location (cameraEyepoint + cameraDir.Scale (10.0f));

	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _SPHERE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _BOX_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CONE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CYLINDER_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CAPSULE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CHAMFER_CYLINDER_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _REGULAR_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
*/
}

void UserHeighMapColliion (NewtonFrame& system)
{
_ASSERTE (0);
/*

	NewtonWorld* world;
	LevelPrimitive *level;
	UserHeightFieldCollision* map;

	world = system.m_world;

	// create the sky box and the floor,
	level = LoadLevelAndSceneRoot (system, "flatplane.dae", 0);

	// delete the existing levelMesh
	NewtonDestroyBody (world, level->m_level);
	system.RemoveModel (level);


	// add a height map mesh 
	map = new UserHeightFieldCollision (world);
	system.AddModel___ (map);
	map->Release();

	// create a material to collision with this object
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (world);


	dVector posit (0.0f, 0.0f, 0.0f, 0.0f);
	posit.m_y = FindFloor (world, posit.m_x, posit.m_z) + 5.0f;

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), posit);

	dVector size (1.0f, 1.0f, 1.0f);
	dVector location (cameraEyepoint + cameraDir.Scale (10.0f));

	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _SPHERE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _BOX_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CONE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CYLINDER_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CAPSULE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CHAMFER_CYLINDER_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _REGULAR_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
*/
}




void OptimizedMeshLevelCollision (NewtonFrame& system)
{
	NewtonWorld* world;

	world = system.m_world;

	// create the sky box and the floor,
	//LoadLevelAndSceneRoot (system, "flatplane.dae", 1);
	//LoadLevelAndSceneRoot (system, "dungeon.dae", 1);
	//LoadLevelAndSceneRoot (system, "pitpool.dae", 1);
	LoadLevelAndSceneRoot (system, "pitpool.dae", 1);
	//LoadLevelAndSceneRoot (system, "dungeon.dae", 1);


	// create a material to collide with this object
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (world);

	dVector posit (0.0f, 0.0f, 0.0f, 0.0f);
	posit.m_y = FindFloor (world, posit.m_x, posit.m_z) + 5.0f;

	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), posit);

	dVector size (1.0f, 1.0f, 1.0f);
	dVector location (cameraEyepoint + cameraDir.Scale (10.0f));

	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _SPHERE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _BOX_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CONE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CYLINDER_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CAPSULE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CHAMFER_CYLINDER_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _REGULAR_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
}


void SimpleMeshLevelCollision (NewtonFrame& system)
{
	NewtonWorld* world;

	world = system.m_world;

	// create the sky box and the floor,
//	LoadLevelAndSceneRoot (system, "flatplane.dae", 0);
//	LoadLevelAndSceneRoot (system, "dungeon.dae", 0);
//	LoadLevelAndSceneRoot (system, "xxx.dae", 1);
//	LoadLevelAndSceneRoot (system, "xxx1.dae", 1);
//	LoadLevelAndSceneRoot (system, "pitpool1.dae", 0);
	LoadLevelAndSceneRoot (system, "pitpool.dae", 0);
//	LoadLevelAndSceneRoot (system, "playground.dae", 0);
	

	// create a material to collide with this object
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (world);

	dVector posit (0.0f, 0.0f, 0.0f, 0.0f);
	posit.m_y = FindFloor (world, posit.m_x, posit.m_z) + 5.0f;


	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), posit);

	dVector size (1.0f, 1.0f, 1.0f);
	dVector location (cameraEyepoint + cameraDir.Scale (10.0f));

	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _SPHERE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _BOX_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CONE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CYLINDER_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CAPSULE_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _CHAMFER_CYLINDER_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
	AddBoxes(&system, 10.0f, location, size, 3, 3, 10.0f, _REGULAR_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
}
#endif


static void CreateLevelMeshBody (NewtonWorld* const world, DemoEntity* const ent, bool optimization)
{

	// measure the time to build a collision tree
	unsigned64 timer0 = dHeightResolutionTimer::GetTimeInMicrosenconds();


	// create the collision tree geometry
	NewtonCollision* const collision = NewtonCreateTreeCollision(world, 0);


	// set the application level callback
#ifdef USE_STATIC_MESHES_DEBUG_COLLISION
//	NewtonStaticCollisionSetDebugCallback (collision, DebugCallback);
#endif

	// prepare to create collision geometry
	NewtonTreeCollisionBeginBuild(collision);

	int faceCount = 0;
	// iterate the entire geometry an build the collision
	for (DemoEntity* model = ent->GetFirst(); model; model = model->GetNext()) {

		dMatrix matrix (model->CalculateGlobalMatrix(ent));
		DemoMesh* const mesh = model->GetMesh();
		dFloat* const vertex = mesh->m_vertex;
		for (DemoMesh::dListNode* nodes = mesh->GetFirst(); nodes; nodes = nodes->GetNext()) {

			DemoSubMesh& segment = nodes->GetInfo();
			for (int i = 0; i < segment.m_indexCount; i += 3) {
				int index;	
				int matID;
				dVector face[3];

				index = segment.m_indexes[i + 0] * 3;
				face[0] = dVector (vertex[index + 0], vertex[index + 1], vertex[index + 2]);

				index = segment.m_indexes[i + 1] * 3;
				face[1] = dVector (vertex[index + 0], vertex[index + 1], vertex[index + 2]);

				index = segment.m_indexes[i + 2] * 3;
				face[2] = dVector (vertex[index + 0], vertex[index + 1], vertex[index + 2]);

				matrix.TransformTriplex (face, sizeof (dVector), face, sizeof (dVector), 3);

				// stress test the collision builder
				// matID = matID == 2 ? 1 : 2 ;
				matID = 0;
				faceCount ++;
				NewtonTreeCollisionAddFace(collision, 3, &face[0].m_x, sizeof (dVector), matID);
			}
		}
	}
	NewtonTreeCollisionEndBuild(collision, optimization ? 1 : 0);

	// measure the time to build a collision tree
	timer0 = (dHeightResolutionTimer::GetTimeInMicrosenconds() - timer0) / 1000;


	// Get the root Matrix
	dMatrix matrix (ent->CalculateGlobalMatrix(NULL));

	// create the level rigid body
	NewtonBody* const level = NewtonCreateBody(world, collision, &matrix[0][0]);

	// set the global position of this body
	//NewtonBodySetMatrix (level, &matrix[0][0]); 

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData (level, ent);

	// set a destructor for this rigid body
	//NewtonBodySetDestructorCallback (m_level, Destructor);

	dVector boxP0; 
	dVector boxP1; 
	// get the position of the aabb of this geometry
	NewtonCollisionCalculateAABB (collision, &matrix[0][0], &boxP0.m_x, &boxP1.m_x); 

	// add some extra padding the world size
	boxP0.m_x -=  50.0f;
	boxP0.m_y -=  50.0f;
	boxP0.m_z -=  50.0f;
	boxP1.m_x +=  50.0f;
	boxP1.m_y += 500.0f;
	boxP1.m_z +=  50.0f;

	// set the world size
	NewtonSetWorldSize (world, &boxP0.m_x, &boxP1.m_x); 

	// release the collision tree (this way the application does not have to do book keeping of Newton objects
	NewtonReleaseCollision (world, collision);
}


static void SimpleMeshLevel (DemoEntityManager* const scene, bool optimization)
{
	// suspend simulation before making changes to the physics world
	scene->StopsExecution ();

	// load the skybox
	scene->Append(new SkyBox());


	// load the scene from and alchemedia file format
	char fileName[2048];
	GetWorkingFileName ("sponza.xml", fileName);
//	GetWorkingFileName ("cattle.xml", fileName);
//	GetWorkingFileName ("playground.xml", fileName);
	scene->LoadScene (fileName);

	for (DemoEntityManager::dListNode* node = scene->GetFirst(); node; node = node->GetNext()) {
		DemoEntity* const ent = node->GetInfo();
		DemoMesh* const mesh = ent->GetMesh();
		if (mesh) {
			char name[2048];
			mesh->GetName(name);
			if (!strcmp (name, "levelGeometry_mesh")) {
				CreateLevelMeshBody (scene->GetNewton(), ent, optimization);
				break;
			}
		}
	}
	


	// place camera into position
	dVector origin (0.0f, 1.0f, 0.0f, 0.0f);
	scene->GetCamera()->m_upVector = dVector (0.0f, 2.0f, 0.0f);
	scene->GetCamera()->m_origin = origin;
	scene->GetCamera()->m_pointOfInterest = origin + dVector (1.0f, 0.0f, 0.0f);

	// resume the simulation
	scene->ContinueExecution();
}

void OptimizedMeshLevelCollision (DemoEntityManager* const scene)
{
	SimpleMeshLevel (scene, true);
}

void SimpleMeshLevelCollision (DemoEntityManager* const scene)
{
	SimpleMeshLevel (scene, false);
}