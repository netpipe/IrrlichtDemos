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



// HeightFieldPrimitive.cpp: implementation of the HeightFieldPrimitive class.
//
//////////////////////////////////////////////////////////////////////
#include <toolbox_stdafx.h>
#include "OpenGlUtil.h"
#include "../OGLMesh.h"
#include "TargaToOpenGl.h"
#include "DebugDisplay.h"
#include "HeightFieldPrimitive.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define CELL_SIZE			12.0f
#define TEXTURE_SCALE		(1.0f / 16.0f)


#define ELEVATION_SCALE		 256.0f 
#define ELEVATION_SCALE_INV  (1.0f / ELEVATION_SCALE) 



#ifdef USE_TEST_ALL_FACE_USER_RAYCAST_CALLBACK
static dFloat AllRayHitCallback (dFloat interseption, dFloat* normal, int faceId, void* userData)
{
	return interseption;
}
#endif



//static char* MAGIC_NUMBER = "this is height field collision ";
#ifdef USE_TEST_SERIALIZATION

static void SerializeFile (void* serializeHandle, const void* buffer, int size)
{
	_ASSERTE ((size % 4) == 0);

	// this is only for ppc Macs
	SWAP_FLOAT32_ARRAY ((void*)buffer, dInt32 (size));

	fwrite (buffer, size, 1, (FILE*) serializeHandle);

	// this is only for ppc Macs
	SWAP_FLOAT32_ARRAY ((void*)buffer, dInt32 (size));
}

static void DeSerializeFile (void* serializeHandle, void* buffer, int size)
{
	_ASSERTE ((size % 4) == 0);
	fread (buffer, size, 1, (FILE*) serializeHandle);

	// this is only for ppc Mack
	SWAP_FLOAT32_ARRAY (buffer, dInt32 (size));
}

#endif




HeightFieldPrimitive::HeightFieldPrimitive(NewtonWorld* nWorld)
	:RenderPrimitive()
{
_ASSERTE (0);
/*

	int width;
	int height;
	unsigned short* elevations;
	OGLMesh* geometry;
	dSubMesh* segment;
	NewtonCollision* collision;

	collision = NULL;

	FILE* file;
	char fullPathName[2048];

	//load from raw data
	GetWorkingFileName ("h2.raw", fullPathName);
	file = fopen (fullPathName, "rb");
	_ASSERTE (file);

	width = 256;
	height = 256;
	char* attibutes;

	// load the data;
	elevations = (unsigned short*) malloc (width * height * sizeof (unsigned short));
	attibutes = (char*) malloc (width * height * sizeof (char));
	memset (attibutes, 0, width * height * sizeof (char));
	fread (elevations, sizeof (unsigned short), width * height, file);

	for (int i = 0; i < width * height; i ++) {
		//elevations[i] = 0;
		elevations[i] = SWAP_INT16 (elevations[i]);
	}

	collision = NewtonCreateHeightFieldCollision (nWorld, width, height, 0, elevations, attibutes, CELL_SIZE, ELEVATION_SCALE_INV, 0);
	free (elevations);
	free (attibutes);
	fclose (file);

#ifdef USE_TEST_SERIALIZATION
	// save the collision file
	GetWorkingFileName ("heightField.bin", fullPathName);
	file = fopen (fullPathName, "wb");
//	SerializeFile (file, MAGIC_NUMBER, strlen (MAGIC_NUMBER) + 1);
	NewtonCollisionSerialize (nWorld, collision, SerializeFile, file);
	fclose (file);
	NewtonReleaseCollision (nWorld, collision);


	//load from a serialized height map collision
	file = fopen (fullPathName, "rb");
//	char magicNumber[256]; 
//	DeSerializeFile (file, magicNumber, int (strlen (MAGIC_NUMBER) + 1));
	collision = NewtonCreateCollisionFromSerialization (nWorld, DeSerializeFile, file);
	fclose (file);
#endif

#ifdef USE_STATIC_MESHES_DEBUG_COLLISION
	NewtonStaticCollisionSetDebugCallback (collision, DebugCallback);
#endif


	NewtonCollisionInfoRecord collisionInfo;
	// keep the compiler happy
	memset (&collisionInfo, 0, sizeof (NewtonCollisionInfoRecord));
	NewtonCollisionGetInfo (collision, &collisionInfo);

	width = collisionInfo.m_heightField.m_width;
	height = collisionInfo.m_heightField.m_height;
	elevations = collisionInfo.m_heightField.m_elevation;


	geometry = new OGLMesh("hightMap");
	AddMesh (geometry);
	geometry->Release();

	geometry->AllocVertexData(width * height);
	for (int z = 0; z < height; z ++) {
		dInt32 z0;
		dInt32 z1;
		z0 = ((z - 1) < 0) ? 0 : z - 1;
		z1 = ((z + 1) > (height - 1)) ? height - 1 : z + 1 ;
		for (int x = 0; x < width; x ++) {
			dInt32 x0;
			dInt32 x1;

			x0 = ((x - 1) < 0) ? 0 : x - 1;
			x1 = ((x + 1) > (width - 1)) ? width - 1 : x + 1 ;

			dVector p0 (CELL_SIZE * x0, elevations[z * width + x1] * ELEVATION_SCALE_INV, CELL_SIZE * z);
			dVector p1 (CELL_SIZE * x1, elevations[z * width + x0] * ELEVATION_SCALE_INV, CELL_SIZE * z);
			dVector x10 (p1 - p0);

			dVector q0 (CELL_SIZE * x, elevations[z0 * width + x] * ELEVATION_SCALE_INV, CELL_SIZE * z0);
			dVector q1 (CELL_SIZE * x, elevations[z1 * width + x] * ELEVATION_SCALE_INV, CELL_SIZE * z1);
			dVector z10 (q1 - q0);

			dVector normal (z10 * x10);
			normal = normal.Scale (dSqrt (1.0f / (normal % normal)));
			dVector point (CELL_SIZE * x, elevations[z * width + x] * ELEVATION_SCALE_INV, CELL_SIZE * z);

			geometry->m_vertex[(z * width + x) * 3 + 0] = point.m_x;
			geometry->m_vertex[(z * width + x) * 3 + 1] = point.m_y;
			geometry->m_vertex[(z * width + x) * 3 + 2] = point.m_z;

			geometry->m_normal[(z * width + x) * 3 + 0] = normal.m_x;
			geometry->m_normal[(z * width + x) * 3 + 1] = normal.m_y;
			geometry->m_normal[(z * width + x) * 3 + 2] = normal.m_z;

			geometry->m_uv[(z * width + x) * 2 + 0] = x * TEXTURE_SCALE;
			geometry->m_uv[(z * width + x) * 2 + 1] = z * TEXTURE_SCALE;
		}
	}
	
	segment = geometry->AddSubMesh();
	segment->m_textureHandle = LoadTexture("GrassAndDirt.tga");

	int index;
	int vertexIndex;
	
	index = 0;
	vertexIndex = 0;
	segment->AllocIndexData(((width - 1) * (height - 1)) * 6);

	for (int z = 0; z < height - 1; z ++) {
		vertexIndex = z * width;
		for (int x = 0; x < width - 1; x ++) {
			segment->m_indexes[index + 0] = GLushort (vertexIndex);
			segment->m_indexes[index + 1] = GLushort (vertexIndex + width);
			segment->m_indexes[index + 2] = GLushort (vertexIndex + 1);
			index += 3;

			segment->m_indexes[index + 0] = GLushort (vertexIndex + 1);
			segment->m_indexes[index + 1] = GLushort (vertexIndex + width);
			segment->m_indexes[index + 2] = GLushort (vertexIndex + width + 1);
			index += 3;
			vertexIndex ++;
		}
	}

	// Optimize the mesh for hardware rendering if possible
	geometry->OptimizeForRender(); 

	dVector boxP0; 
	dVector boxP1; 
	// get the position of the aabb of this geometry
	dMatrix matrix (GetMatrix());
	NewtonCollisionCalculateAABB (collision, &matrix[0][0], &boxP0.m_x, &boxP1.m_x); 
	matrix.m_posit = (boxP0 + boxP1).Scale (-0.5f);
	matrix.m_posit.m_w = 1.0f;
	SetMatrix (matrix);

	// create the level rigid body
	m_level = NewtonCreateBody(nWorld, collision);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData (m_level, this);

	// release the collision tree (this way the application does not have to do book keeping of Newton objects
	NewtonReleaseCollision (nWorld, collision);

	// set the global position of this body
	NewtonBodySetMatrix (m_level, &matrix[0][0]); 

	// set the destructor for this object
	NewtonBodySetDestructorCallback (m_level, Destructor);

	// get the position of the aabb of this geometry
	NewtonCollisionCalculateAABB (collision, &matrix[0][0], &boxP0.m_x, &boxP1.m_x); 

	// add some extra padding the world size
	boxP0.m_x -=  10.0f;
	boxP0.m_y -=  10.0f;
	boxP0.m_z -=  10.0f;
	boxP1.m_x +=  10.0f;
	boxP1.m_y += 400.0f;
	boxP1.m_z +=  10.0f;

	// set the world size
	NewtonSetWorldSize (nWorld, &boxP0.m_x, &boxP1.m_x); 

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


//NewtonCollisionGetInfo(NewtonBodyGetCollision(m_level), &collisionInfo);
//_ASSERT(collisionInfo.m_collisionType == SERIALIZE_ID_HEIGHTFIELD);


#if 0
// test collision ID
NewtonCollisionSetUserID(collision, 1000);  
 NewtonBody* body = NULL;
 for (body=NewtonWorldGetFirstBody(nWorld) ; body!=NULL ; body=NewtonWorldGetNextBody(nWorld, body))
 {
//  GFXAPI().SetColor3f(NewtonBodyGetFreezeState(body)==0 ? TColor::Red() : TColor(0.5f, 0.0f, 0.0f));
//  TMatrix4d matrix;
//  NewtonBodyGetMatrix(body, matrix.Ptr());
  NewtonCollision* collision = NewtonBodyGetCollision(body);
  unsigned userId ;
  userId = NewtonCollisionGetUserID(collision);  
  userId = NewtonCollisionGetUserID(collision);  
  NewtonCollisionInfoRecord info;

  NewtonCollisionGetInfo (collision, &info);
  NewtonCollisionGetInfo (collision, &info);
}
#endif
*/
}

HeightFieldPrimitive::~HeightFieldPrimitive()
{
}


// rigid body destructor
void HeightFieldPrimitive::Destructor (const NewtonBody* body)
{
//	HeightFieldPrimitive* level;
	// get the graphic object form the rigid body
//	level = (HeightFieldPrimitive*) NewtonBodyGetUserData (body);

	// destroy the graphic object
//	delete level;
}



NewtonBody* HeightFieldPrimitive::GetRigidBody() const
{
	return m_level;
}


void HeightFieldPrimitive::DebugCallback (const NewtonBody* bodyWithTreeCollision, const NewtonBody* body, int faceID, int vertexCount, const dFloat* vertex, int vertexstrideInBytes)
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
		NewtonWorld* world;
		dVector face[64];
		int stride = vertexstrideInBytes / sizeof (dFloat);

		// if we are display debug info we need to block other threads from writing the data at the same time
		world = NewtonBodyGetWorld (bodyWithTreeCollision);
		NewtonWorldCriticalSectionLock(world);

		for (int j = 0; j < vertexCount; j ++) {
			face [j] = dVector (vertex[j * stride + 0], vertex[j * stride + 1] , vertex[j * stride + 2]);
		}
		// Do not forget to unlock the critical section
		NewtonWorldCriticalSectionUnlock(world);


		DebugDrawPolygon (vertexCount, face);
	}
}



