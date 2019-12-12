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
#include "../PhysicsUtils.h"
#include "../SceneManager.h"
#include "../toolBox/MousePick.h"
#include "../toolBox/OpenGlUtil.h"


class ClosestDistanceNode: public RenderPrimitive
{
	public:
	ClosestDistanceNode (NewtonBody* body, NewtonCollision* castShape, SceneManager& system)
		:RenderPrimitive ()
	{
		OGLMesh *geometry; 

		m_body = body;
		m_shape = castShape;

		geometry = new OGLMesh ("castBox", castShape, "wood_0.tga", "wood_0.tga", "wood_1.tga");
		SetMesh(geometry);

		system.AddModel___ (this);
		Release();


		NewtonAddCollisionReference (castShape);

		geometry->Release();
	}

	~ClosestDistanceNode ()
	{
		NewtonReleaseCollision (NewtonBodyGetWorld (m_body), m_shape);
	}


	void Render()
	{
		dMatrix matrix;

		NewtonBodyGetMatrix (m_body, &matrix[0][0]);
		dMatrix matrix1(matrix);

		matrix1.m_posit.m_y += 20.0f;
		SetMatrix (matrix1);

		// render the shape
		RenderPrimitive::Render();
		
		NewtonCollision* collisionA;
		collisionA = NewtonBodyGetCollision(m_body);

		NewtonWorld* world;
		dVector contact0;
		dVector contact1;
		dVector normal;
		world = NewtonBodyGetWorld (m_body);
		if (NewtonCollisionClosestPoint(world, collisionA, &matrix[0][0], m_shape, &matrix1[0][0], &contact0[0], &contact1[0],  &normal[0], 0)) {
			ShowMousePicking (contact0, contact1);
		}
	}

	NewtonBody* m_body;
	NewtonCollision* m_shape;
};


static void DoNothing (SceneManager& me, int mode)
{
}


static void PhysicsDistanceSetTransform (const NewtonBody* body, const dFloat* matrix, int threadIndex)
{
	RenderPrimitive* primitive;

	// get the graphic object form the rigid body
	primitive = (RenderPrimitive*) NewtonBodyGetUserData (body);

	// set the transformation matrix for this rigid body
	dMatrix& mat = *((dMatrix*)matrix);
	primitive->SetMatrix (mat);

	// animate the body by setting the angular velocity
	dVector omega (mat[2].Scale (1.0f));
	NewtonBodySetOmega (body, &omega[0]); 
}




// create physics scene
void ClosestDistance(NewtonFrame& system)
{
//	dSceneNode* scene;
	NewtonBody* body;
	NewtonWorld* world;
	NewtonCollision* castShape;

	// /////////////////////////////////////////////////////////////////////
	//
	// create the sky box,
	OGLModel* sky = new SkyBox ();
	system.AddModel___ (sky);
	sky->Release();



	world = system.m_world;

	// create a material to collision with this object
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (world);

	// create the cast shape
	dVector shapeSize (4.0f, 4.0f, 4.0f, 0.0f);
	dMatrix offsetMatrix (GetIdentityMatrix());
	offsetMatrix.m_posit.m_y = -shapeSize.m_y * 0.5f;
	castShape =  NewtonCreateBox(world, 4.0f, 4.0f, 4.0f, 0, &offsetMatrix[0][0]);
	dMatrix baseMatrix (GetIdentityMatrix());

//	cameraDir = dVector (0.0f, 0.0f, -1.0f);
//	cameraEyepoint = dVector (0.0f, 0.0f, 40.0f);
	InitEyePoint (dVector (0.0f, 0.0f, -1.0f), dVector (0.0f, 0.0f, 80.0f));

	dVector size(8.0f, 2.0f, 6.0f);

	// place few shapes to be casted
	baseMatrix.m_posit.m_x = -30.0f;
	body = CreateGenericSolid (world, &system, "capsule", 1, baseMatrix, size, _CAPSULE_PRIMITIVE, defaultMaterialID);
	NewtonBodySetForceAndTorqueCallback(body, NULL);
	NewtonBodySetTransformCallback(body, PhysicsDistanceSetTransform);
	NewtonBodySetAutoSleep (body, 0);
	new ClosestDistanceNode (body, castShape, system);

	baseMatrix.m_posit.m_x = -20.0f;
	body = CreateGenericSolid (world, &system,  "box", 1, baseMatrix, size, _BOX_PRIMITIVE, defaultMaterialID);
	NewtonBodySetForceAndTorqueCallback(body, NULL);
	NewtonBodySetTransformCallback(body, PhysicsDistanceSetTransform);
	NewtonBodySetAutoSleep (body, 0);
	new ClosestDistanceNode (body, castShape, system);

	baseMatrix.m_posit.m_x = -10.0f;
	body = CreateGenericSolid (world, &system,  "convexHull", 1, baseMatrix, size, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
	NewtonBodySetForceAndTorqueCallback(body, NULL);
	NewtonBodySetTransformCallback(body, PhysicsDistanceSetTransform);
	NewtonBodySetAutoSleep (body, 0);
	new ClosestDistanceNode (body, castShape, system);

	baseMatrix.m_posit.m_x = 0.0f;
	body = CreateGenericSolid (world, &system,  "cylinder", 1, baseMatrix, size, _CYLINDER_PRIMITIVE, defaultMaterialID);
	NewtonBodySetForceAndTorqueCallback(body, NULL);
	NewtonBodySetTransformCallback(body, PhysicsDistanceSetTransform);
	NewtonBodySetAutoSleep (body, 0);
	new ClosestDistanceNode (body, castShape, system);

	baseMatrix.m_posit.m_x = 15.0f;
	body = CreateGenericSolid (world, &system,  "cone", 1, baseMatrix, size, _CONE_PRIMITIVE, defaultMaterialID);
	NewtonBodySetForceAndTorqueCallback(body, NULL);
	NewtonBodySetTransformCallback(body, PhysicsDistanceSetTransform);
	NewtonBodySetAutoSleep (body, 0);
	new ClosestDistanceNode (body, castShape, system);

	baseMatrix.m_posit.m_x = 30.0f;
	body = CreateGenericSolid (world, &system,  "camferedCylinder", 1, baseMatrix, size, _CHAMFER_CYLINDER_PRIMITIVE, defaultMaterialID);
	NewtonBodySetForceAndTorqueCallback(body, NULL);
	NewtonBodySetTransformCallback(body, PhysicsDistanceSetTransform);
	NewtonBodySetAutoSleep (body, 0);
	new ClosestDistanceNode (body, castShape, system);

	NewtonReleaseCollision(world, castShape);


	system.m_control = Keyboard;
	system.m_autoSleep = DoNothing;
	system.m_showIslands = DoNothing;
	system.m_showContacts = DoNothing; 
	system.m_setMeshCollision = DoNothing;


}



