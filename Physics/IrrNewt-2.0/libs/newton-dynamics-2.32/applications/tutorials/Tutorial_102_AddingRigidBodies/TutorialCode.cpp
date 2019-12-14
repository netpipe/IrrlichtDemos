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

#include "StdAfx.h"
#include "Entity.h"
#include "TutorialCode.h"
#include "SceneManager.h"
#include "RigidBodyUtil.h"
#include "CollisionShapeUtil.h"
#include "CollectInputAndUpdateCamera.h"

void CreateScene (NewtonWorld* world, SceneManager* sceneManager)
{
	Entity* floor;
	Entity* smilly;
	Entity* frowny;
	NewtonBody* floorBody;
	NewtonBody* smillyBody;
	NewtonBody* frownyBody;
	NewtonCollision* shape;

	// initialize the camera
	InitCamera (dVector (-15.0f, 5.0f, 0.0f, 0.0f), dVector (1.0f, 0.0f, 0.0f));

	// Create a large body to be the floor
	floor = sceneManager->CreateEntity();
	floor->LoadMesh ("FloorBox.dat");

	// add static floor Physics
	shape = CreateNewtonBox (world, floor, 0);
	floorBody = CreateRigidBody (world, floor, shape, 0.0f);
	NewtonReleaseCollision (world, shape);

	// set the Transformation Matrix for this rigid body
	dMatrix matrix (floor->m_curRotation, floor->m_curPosition);
	NewtonBodySetMatrix (floorBody, &matrix[0][0]);

	// now we will use the properties of this body to set a proper world size.
	dVector minBox;
	dVector maxBox;
	NewtonCollisionCalculateAABB (shape, &matrix[0][0], &minBox[0], &maxBox[0]);

	// add some extra padding
	minBox.m_x -=  50.0f;
	minBox.m_y -= 500.0f;
	minBox.m_z -=  50.0f;

	maxBox.m_x +=  50.0f;
	maxBox.m_y += 500.0f;
	maxBox.m_z +=  50.0f;

	// set the new world size
	NewtonSetWorldSize (world, &minBox[0], &maxBox[0]);


	// add some visual entities.
	smilly = sceneManager->CreateEntity();
	smilly->LoadMesh ("Smilly.dat");
	smilly->m_curPosition.m_y = 10.0f;
	smilly->m_prevPosition = smilly->m_curPosition;

	// add a body with a box shape
	shape = CreateNewtonBox (world, smilly, 0);
	smillyBody = CreateRigidBody (world, smilly, shape, 10.0f);
	NewtonReleaseCollision (world, shape);


	// add some visual entities.
	frowny = sceneManager->CreateEntity();
	frowny->LoadMesh ("Frowny.dat");
	frowny->m_curPosition.m_z = 0.4f;
	frowny->m_curPosition.m_y = 10.0f + 0.4f;
	frowny->m_prevPosition = frowny->m_curPosition;

	// add a body with a Convex hull shape
	shape = CreateNewtonConvex (world, frowny, 0);
	frownyBody = CreateRigidBody (world, frowny, shape, 10.0f);
	NewtonReleaseCollision (world, shape);
}



