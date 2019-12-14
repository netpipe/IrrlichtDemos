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
#include "../DemoMesh.h"
#include "../toolBox/OpenGlUtil.h"

class ClosestDistanceNode: public DemoEntity
{
	public:
	ClosestDistanceNode (NewtonBody* const body, NewtonCollision* const castShape, DemoEntityManager* const scene)
		:DemoEntity (NULL)
	{
		m_pith = RandomVariable(3.1416f * 2.0f);
		m_yaw = RandomVariable(3.1416f * 2.0f);
		m_roll = RandomVariable(3.1416f * 2.0f);
		m_step = 15.0f * (dAbs (RandomVariable(0.25f)) + 0.0001f) * 3.1416f/180.0f;


		m_body = body;
		m_shape = castShape;

		DemoMesh* const geometry = new DemoMesh("castingMesh", castShape, "wood_0.tga", "wood_0.tga", "wood_1.tga");
		SetMesh(geometry);
		scene->Append(this);

		geometry->Release();
		
		// add a reference to this collision
		NewtonAddCollisionReference (castShape);
	}

	~ClosestDistanceNode ()
	{
		// release the collision 
		NewtonReleaseCollision (NewtonBodyGetWorld (m_body), m_shape);
	}


	void Render(dFloat timeStep) const
	{
		dMatrix matrix;

		NewtonBodyGetMatrix (m_body, &matrix[0][0]);

		dFloat speed = m_step * timeStep * 60.0f; 

		m_pith = dMod (m_pith + speed, 3.1416f * 2.0f);
		m_yaw = dMod (m_yaw + speed, 3.1416f * 2.0f);
		m_roll = dMod (m_roll + speed, 3.1416f * 2.0f);
		//dMatrix matrix1(matrix);
		dMatrix matrix1(dPitchMatrix(m_pith) * dYawMatrix(m_yaw) * dRollMatrix(m_roll));
		matrix1.m_posit = matrix.m_posit;
		matrix1.m_posit.m_y = 20.0f;
		m_matrix = matrix1;

		// render the shape
		DemoEntity::Render(timeStep);

		NewtonCollision* const collisionA = NewtonBodyGetCollision(m_body);

		dVector contact0;
		dVector contact1;
		dVector normal;
		NewtonWorld* const world = NewtonBodyGetWorld (m_body);
		if (NewtonCollisionClosestPoint(world, collisionA, &matrix[0][0], m_shape, &matrix1[0][0], &contact0[0], &contact1[0],  &normal[0], 0)) {
			ShowMousePicking (contact0, contact1);
		}
	}

	dFloat m_step;
	mutable dFloat m_pith;
	mutable dFloat m_yaw;
	mutable dFloat m_roll;
	NewtonBody* m_body;
	NewtonCollision* m_shape;
};

static void PhysicsSpinBody (const NewtonBody* body, dFloat timestep, int threadIndex)
{
	dVector omega (0.0f, 0.f, 1.0f, 0.0f);
	NewtonBodySetOmega (body, &omega[0]);
}



// create physics scene
void ClosestDistance (DemoEntityManager* const scene)
{

	// suspend simulation before making changes to the physics world
	scene->StopsExecution ();

	// load the skybox
	scene->Append(new SkyBox());


	// load the scene from and alchemedia file format
	char fileName[2048];
	GetWorkingFileName ("closestDistance.xml", fileName);
	scene->LoadScene (fileName);

	// customize the scene after loading
	// set a user friction variable in the body for variable friction demos
	// later this will be done using LUA script
	NewtonWorld* const world = scene->GetNewton();
	dVector shapeSize (4.0f, 4.0f, 4.0f, 0.0f);
	dMatrix offsetMatrix (GetIdentityMatrix());
	offsetMatrix.m_posit.m_y = -shapeSize.m_y * 0.5f;
#if 0
	NewtonCollision* const castShape = NewtonCreateBox(world, 4.0f, 4.0f, 4.0f, 0, &offsetMatrix[0][0]);
#else
	#if 0
		NewtonCollision* parts[3];
		parts[0] = NewtonCreateBox(world, 1.5f, 8.0f, 1.5f, 0, NULL);
		parts[1] = NewtonCreateCylinder(world, 1.5, 5, 0, NULL);
		dMatrix offset (dYawMatrix(3.1416f * 0.5f));
		parts[2] = NewtonCreateCapsule(world, 1.5, 5, 0, &offset[0][0]);
		NewtonCollision* const castShape = NewtonCreateCompoundCollision(world, 3, parts, 0);
		NewtonReleaseCollision(world, parts[0]);
		NewtonReleaseCollision(world, parts[1]);
		NewtonReleaseCollision(world, parts[2]);
	#else

		dScene compoundTestMesh (world);
		GetWorkingFileName ("bessel_1.xml", fileName);

		compoundTestMesh.Deserialize(fileName);

		dMeshNodeInfo* mesh = NULL;
		for (dScene::dTreeNode* node = compoundTestMesh.GetFirstNode (); node; node = compoundTestMesh.GetNextNode (node)) {
			dNodeInfo* info = compoundTestMesh.GetInfoFromNode(node);
			if (info->GetTypeId() == dMeshNodeInfo::GetRttiType()) {
				mesh = (dMeshNodeInfo*) info;
				break;
			}
		}
		_ASSERTE (mesh);
		dMatrix matrix (GetIdentityMatrix());
		matrix[0][0] = 40.0f;
		matrix[1][1] = 40.0f;
		matrix[2][2] = 40.0f;
		mesh->BakeTransform (matrix);
		NewtonMesh* const newtonMesh = mesh->GetMesh();
		
		int count = 0;
		NewtonCollision* parts[256];

		NewtonMesh* nextPart = NULL;
		for (NewtonMesh* part = NewtonMeshCreateFirstSingleSegment (newtonMesh); part; part = nextPart) {
			nextPart = NewtonMeshCreateNextSingleSegment (newtonMesh, part);
			parts[count] = NewtonCreateConvexHullFromMesh (world, part, 0.0f, 0);
			count ++;
			NewtonMeshDestroy(part);
		} 
		NewtonCollision* const castShape = NewtonCreateCompoundCollision(world, count, parts, 0);
		for (int i = 0; i < count; i ++) {
			NewtonReleaseCollision(world, parts[i]);
		}
	#endif
#endif

	// here we will change the standard gravity force callback and apply null and add a 
	// spin the body in the transform callback
	for (NewtonBody* body = NewtonWorldGetFirstBody(world); body; body = NewtonWorldGetNextBody(world, body)) {
		NewtonBodySetForceAndTorqueCallback(body, PhysicsSpinBody);
		NewtonBodySetAutoSleep (body, 0);
		new ClosestDistanceNode (body, castShape, scene);
	}

	NewtonReleaseCollision(world, castShape);

	// place camera into position
	dVector origin (-15.0f, 10.0f, 80.0f, 0.0f);
	scene->GetCamera()->m_upVector = dVector (0.0f, 1.0f, 0.0f);
	scene->GetCamera()->m_origin = origin;
	scene->GetCamera()->m_pointOfInterest = origin + dVector (0.0f, 0.0f, -1.0f);

	// resume the simulation
	scene->ContinueExecution();
}



