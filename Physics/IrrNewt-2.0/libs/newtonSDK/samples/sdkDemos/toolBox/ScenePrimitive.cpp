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



// ScenePrimitive.cpp: implementation of the ScenePrimitive class.
//
//////////////////////////////////////////////////////////////////////
#include <toolbox_stdafx.h>
#include "OpenGlUtil.h"
#include "../OGLMesh.h"
#include "DebugDisplay.h"
#include "ScenePrimitive.h"
//#include "PlaneCollision.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#define USE_TEST_SERIALIZATION

#ifdef USE_TEST_SERIALIZATION

static const char* MAGIC_NUMBER = "serialize data";
static void SerializeFile (void* serializeHandle, const void* buffer, int size)
{
	fwrite (buffer, size, 1, (FILE*) serializeHandle);
}

static void DeSerializeFile (void* serializeHandle, void* buffer, int size)
{
	fread (buffer, size, 1, (FILE*) serializeHandle);
}

#endif


#ifdef USE_TEST_ALL_FACE_USER_RAYCAST_CALLBACK
static dFloat AllRayHitCallback (dFloat interseption, dFloat* normal, int faceId, void* userData)
{
	return interseption;
}
#endif




ScenePrimitive::ScenePrimitive(const NewtonWorld* nWorld, const char* name, int optimized)
	:OGLModel()
{
	NewtonCollision* scene;

	// create a body and with a scene collision
	scene = NewtonCreateSceneCollision (nWorld, 0);

	// add a collision tree to the scene
	AddCollisionTree(scene, nWorld, name, optimized);

	// add some shapes
	AddPrimitives(scene, nWorld);

	// finish the scene construction, optimize the collision scene
	NewtonSceneCollisionOptimize (scene);

	// set the world size base on the scene size
	dVector boxP0; 
	dVector boxP1; 
	// get the position of the aabb of this geometry
	dMatrix matrix (GetMatrix());
	NewtonCollisionCalculateAABB (scene, &matrix[0][0], &boxP0.m_x, &boxP1.m_x); 

	// add some extra padding the world size
	boxP0.m_x -=  50.0f;
	boxP0.m_y -=  50.0f;
	boxP0.m_z -=  50.0f;
	boxP1.m_x +=  50.0f;
	boxP1.m_y += 500.0f;
	boxP1.m_z +=  50.0f;

	// set the world size
	NewtonSetWorldSize (nWorld, &boxP0.m_x, &boxP1.m_x); 

	// set the application level callback
#ifdef USE_STATIC_MESHES_DEBUG_COLLISION
 	NewtonStaticCollisionSetDebugCallback (collision, DebugCallback);
#endif


#ifdef USE_TEST_SERIALIZATION
	{
		FILE* file;
		// save the collision file
		char fullPathName[256];
		GetWorkingFileName ("collisiontest.bin", fullPathName);

		file = fopen (fullPathName, "wb");
		SerializeFile (file, MAGIC_NUMBER, int (strlen (MAGIC_NUMBER)) + 1);
		NewtonCollisionSerialize (nWorld, scene, SerializeFile, file);
		fclose (file);

		// load the collision file
		NewtonReleaseCollision (nWorld, scene);

		file = fopen (fullPathName, "rb");
		char magicNumber[256]; 
		DeSerializeFile (file, magicNumber, int (strlen (MAGIC_NUMBER)) + 1);
		scene = NewtonCreateCollisionFromSerialization (nWorld, DeSerializeFile, file);
		fclose (file);

		NewtonCollisionInfoRecord collisionInfo;
		NewtonCollisionGetInfo (scene, &collisionInfo);
		NewtonCollisionGetInfo (scene, &collisionInfo);
	}
#endif

	// create the level body and add it to the world
	m_level = NewtonCreateBody (nWorld, scene);
	NewtonReleaseCollision (nWorld, scene);

	// set the global position of this body
	NewtonBodySetMatrix (m_level, &matrix[0][0]); 
}

ScenePrimitive::~ScenePrimitive()
{
	for (dList<VisualProxy>::dListNode* node = m_pieces.GetFirst(); node; node = node->GetNext()) {
		const VisualProxy& visualProxy = node->GetInfo();
		visualProxy.m_mesh->Release();
	}
}


void ScenePrimitive::AddPrimitives(NewtonCollision* scene, const NewtonWorld* nWorld)
{
	dMatrix rotMat (dRollMatrix (0.5f * 3.141592f));
	NewtonCollision* collision2 = NewtonCreateCylinder (nWorld, 2.0f, 0.5f, 0, &rotMat[0][0]);
	OGLMesh* mesh2 = new OGLMesh ("cylinder_1", collision2, "smilli.tga", "smilli.tga", "smilli.tga");

	NewtonCollision* collision1 = NewtonCreateBox (nWorld, 4.0f, 0.5, 4.0f, 0, NULL);
	OGLMesh* mesh1 = new OGLMesh ("box_1", collision1, "smilli.tga", "smilli.tga", "smilli.tga");


	dMatrix matrix (GetIdentityMatrix());
	for (int y = 0; y < 10; y ++) {
		for (int x = 0; x < 10; x ++) {
			for (int z = 0; z < 10; z ++) {
				OGLMesh* mesh;
				NewtonCollision* collision;
				NewtonSceneProxy* sceneProxy;
				matrix.m_posit.m_y = 5.0f + y * 4.0f ; 
				matrix.m_posit.m_x = x * 6.0f - 20.0f; 
				matrix.m_posit.m_z = z * 6.0f - 24.0f;  
				
				if (rand () & 0x010) {
					mesh = mesh1;
					collision = collision1;
				} else {
					mesh = mesh2;
					collision = collision2;
				}

				sceneProxy = NewtonSceneCollisionCreateProxy (scene, collision);
				NewtonSceneProxySetMatrix (sceneProxy, &matrix[0][0]);
				VisualProxy& shape = m_pieces.Append()->GetInfo();
				shape.m_sceneProxi = sceneProxy;
				shape.m_mesh = mesh;
				mesh->AddRef();
				shape.m_matrix = matrix;
			}
		}
	}

	mesh1->Release();
	mesh2->Release();
	NewtonReleaseCollision (nWorld, collision1);
	NewtonReleaseCollision (nWorld, collision2);
}


void ScenePrimitive::AddCollisionTree(NewtonCollision* scene, const NewtonWorld* nWorld, const char* name, int optimized)
{
	NewtonCollision* collision;
	NewtonSceneProxy* sceneProxy;
	
	// open the level data
	char fullPathName[2048];
	GetWorkingFileName (name, fullPathName);

	OGLLoaderContext context;
	dMatrix matrix (dYawMatrix (-3.14159265f * 0.5f));
	LoadCollada(fullPathName, context, matrix, 1.0f);

	// create the collision tree geometry
	collision = NewtonCreateTreeCollision(nWorld, 0);

	// prepare to create collision geometry
	NewtonTreeCollisionBeginBuild(collision);

	dMatrix pallete[64];
	UpdateMatrixPalette (GetIdentityMatrix(), pallete, sizeof (m_matrixPalete) / sizeof (dMatrix));

	// iterate the entire geometry an build the collision
//	for (ModelComponentList<dList<dMesh*> >::dListNode* list = m_meshList.GetFirst(); list; list = list->GetNext()) {
	for (dList<dMeshInstance>::dListNode* node = m_meshList.GetFirst(); node; node = node->GetNext()) { 
		dFloat* vertex;
	
		OGLMesh* mesh = (OGLMesh*)node->GetInfo().m_mesh;

		vertex = mesh->m_vertex;
		const dMatrix& globalMatrix = pallete[mesh->m_boneID];
		for (dMesh::dListNode* nodes = mesh->GetFirst(); nodes; nodes = nodes->GetNext()) {
			dSubMesh& segment = nodes->GetInfo();
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

				globalMatrix.TransformTriplex (face, sizeof (dVector), face, sizeof (dVector), 3);

				// stress test the collision builder
//				matID = matID == 2 ? 1 : 2 ;
				matID = 0;
				NewtonTreeCollisionAddFace(collision, 3, &face[0].m_x, sizeof (dVector), matID);
			}
		}
	}
	NewtonTreeCollisionEndBuild(collision, optimized);

	// create a Scene proxy to contain this mesh
	sceneProxy = NewtonSceneCollisionCreateProxy (scene, collision);

	// release the collision tree (this way the application does not have to do book keeping of Newton objects
	NewtonReleaseCollision (nWorld, collision);

	// set proxy relative matrix 
	dMatrix proxyMatrix (GetIdentityMatrix());
	NewtonSceneProxySetMatrix (sceneProxy, &proxyMatrix[0][0]);

	// save the pointer to the graphic object with the body.
//	NewtonBodySetUserData (m_level, this);


#ifdef USE_TEST_ALL_FACE_USER_RAYCAST_CALLBACK
	// set a ray cast callback for all face ray cast 
	NewtonTreeCollisionSetUserRayCastCallback (collision, AllRayHitCallback);
	dVector p0 (0,  100, 0, 0);
	dVector p1 (0, -100, 0, 0);
	dVector normal;
	int id;
	dFloat parameter;
	parameter = NewtonCollisionRayCast (collision, &p0[0], &p1[0], &normal[0], &id);
#endif
}



void ScenePrimitive::DebugCallback (const NewtonBody* bodyWithTreeCollision, const NewtonBody* body, int faceID, int vertexCount, const dFloat* vertex, int vertexstrideInBytes)
{
	// the application can use this function for debugging purpose by writing the 
	// face to a global variable then display which face of the mesh are been used for collision.
	// ,,,,,,,,,,,
	// ..........

	// the application can use this function also to modify the collision geometry by changing the face ID
	// this could be uses full to make surface change for example from dry to wet, or breaking glass
	// for this the application should used the functions:
	// int id = NewtonTreeCollisionGetFaceAtribute (treeCollision, indexArray); 
	// NewtonTreeCollisionSetFaceAtribute (treeCollision, indexArray, new id);

	if (DebugDisplayOn()) {
		dVector face[64];
		int stride = vertexstrideInBytes / sizeof (dFloat);
		for (int j = 0; j < vertexCount; j ++) {
			face [j] = dVector (vertex[j * stride + 0], vertex[j * stride + 1] , vertex[j * stride + 2]);
		}
		DebugDrawPolygon (vertexCount, face);
	}
}


void ScenePrimitive::Render()
{
	OGLModel::Render();  

	for (dList<VisualProxy>::dListNode* node = m_pieces.GetFirst(); node; node = node->GetNext()) {
		const VisualProxy& visualProxy = node->GetInfo();

		glPushMatrix();
//		dMatrix matrix;
//		dMatrix matrix (NewtonSceneProxyGetMatrix (visualProxy.m_sceneProxi, matrix)
		glMultMatrix(&visualProxy.m_matrix[0][0]);
		visualProxy.m_mesh->Render (NULL);
		glPopMatrix();
	}
}

