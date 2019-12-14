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
#include "CustomPlayerController.h"
#include "../OGLMesh.h"
#include "../MainFrame.h"
#include "../SceneManager.h"
#include "../PhysicsUtils.h"
#include "../toolBox/MousePick.h"
#include "../toolBox/OpenGlUtil.h"
#include "../toolBox/DebugDisplay.h"
#include "../toolBox/LevelPrimitive.h"
#include "../toolBox/PlaneCollision.h"
#include "../toolBox/HeightFieldPrimitive.h"
#include "../toolBox/UserHeightFieldCollision.h"


static CustomPlayerController* player;

#define THIRD_PERSON_VIEW	

#define PLAYER_SPEED		10.0f
#define PLAYER_SIDE_SPEED	 5.0f
#define PLAYER_JOINT_ID		0xEF38AB01

class MyPlayerController: public CustomPlayerController
{
	public:

	MyPlayerController (const dMatrix& globalFrame, NewtonBody* player, dFloat stairStepFactor, dFloat cushion)
		:CustomPlayerController (globalFrame, player, stairStepFactor, cushion),
		m_eyePoint (0.0f, 2.0f, 0.0f, 0.0f)
	{
		SetJointID (PLAYER_JOINT_ID);

		SetMaxSlope (30.0f * 3.1416f / 180.0f);

		// change the transform callback to display the internal collision shapes
		NewtonBodySetTransformCallback (player, SetTransform);
	}

	~MyPlayerController()
	{
	}

	// over load the player controller to apply some customizations
	virtual void SubmitConstraints (dFloat timestep, int threadId)
	{
		dFloat velocity;
		dFloat strafeVeloc;
		dFloat headinAngle;
		
		velocity = 0;
//		if (dGetKeyState (KeyCode_w)) {
//			velocity = PLAYER_SPEED;
//		} else if (dGetKeyState (KeyCode_s)) {
//			velocity = -PLAYER_SPEED;
//		}

		strafeVeloc = 0.0f;
//		if (dGetKeyState (KeyCode_d)) {
//			strafeVeloc = PLAYER_SIDE_SPEED;
//		} else if (dGetKeyState (KeyCode_a)) {
//			strafeVeloc = -PLAYER_SIDE_SPEED;
//		}

		// now set the desired player velocity and heading 
		headinAngle = yawAngle;
//velocity = -PLAYER_SPEED;
//velocity = -1.0f;
		SetVelocity (velocity, strafeVeloc, headinAngle);

		CustomPlayerController::SubmitConstraints (timestep, threadId);
	}


	static void SetTransform (const NewtonBody* body, const dFloat* matrix, int threadId)
	{
		MyPlayerController* player;

		// find the player joint
		player = NULL;
		for (NewtonJoint* joint = NewtonBodyGetFirstJoint(body); joint; joint = NewtonBodyGetNextJoint(body, joint)) {
			MyPlayerController* tmp;
			tmp = (MyPlayerController*) NewtonJointGetUserData(joint);
			if (tmp->GetJointID() == PLAYER_JOINT_ID) {
				player = tmp;
				break;
			}
		}

		// get the visual matrix for this model 
#if 0
		dMatrix visualMatrix (player->CalculateVisualMatrix ());
#else
		const dMatrix& visualMatrix = *((dMatrix*) matrix);
#endif

		// call the generic transform callback
		PhysicsSetTransform (body, &visualMatrix[0][0], threadId);


		// now we will set the camera to follow the player
		dVector eyePoint (visualMatrix.TransformVector(player->m_eyePoint));

		eyePoint -= cameraDir.Scale (8.0f);
		dVector target (eyePoint + cameraDir);

//dMatrix xxx (dYawMatrix(-0.4f * 3.1416f));
//eyePoint -= xxx.RotateVector(cameraDir.Scale (6.0f));
//eyePoint.m_y -= 1.0f;
//dVector target (eyePoint + xxx.RotateVector(cameraDir));
		
		SetCamera (eyePoint, target);
//	    GetDGCamera()->Apply();

		// find the player joint;
		if (DebugDisplayOn()) {
			dMatrix shapeMatrix (*((dMatrix*) matrix));
//			DebugDrawCollision (player->GetSensorShape(), shapeMatrix);

			shapeMatrix.m_posit.m_y += player->m_stairHeight;
			DebugDrawCollision (player->GetStairStepShape (), shapeMatrix);
			
//			DebugDrawCollision (player->GetVerticalSensorShape(), shapeMatrix);
		}
	}

	dVector m_eyePoint;
};


static void NoPlayerAutoSleep (SceneManager& me, int mode)
{
	// set auto sleep mode 
	AutoSleep (me, mode);

	// make sure the player do not go to auto sleep node
	if (player) {
		NewtonBodySetAutoSleep (player->GetBody0(), 0);
	}
}


//	Keyboard handler. 
static void PlayerKeyboard(SceneManager& me)
{

_ASSERTE (0);
/*
	NewtonWorld* world;

	world = me.m_world;
	// read the mouse position and set the camera direction

	dMOUSE_POINT mouse1;
	dInt32 mouseLeftKey;
	static dMOUSE_POINT mouse0;

//	dVector target (cameraEyepoint + cameraDir);
//	SetCamera (cameraEyepoint, target);

	GetCursorPos(mouse1);

	// this section control the camera object picking
	mouseLeftKey = dGetKeyState (KeyCode_L_BUTTON);
	if (!MousePick (world, mouse1, mouseLeftKey, 0.125f, 1.0f)) {
		// we are not in mouse pick mode, then we are in camera tracking mode
		if (mouseLeftKey) {
			// when click left mouse button the first time, we reset the camera
			// convert the mouse x position to delta yaw angle
			if (mouse1.x > (mouse0.x + 1)) {
				yawAngle += 1.0f * 3.141592f / 180.0f;
				if (yawAngle > (360.0f * 3.141592f / 180.0f)) {
					yawAngle -= (360.0f * 3.141592f / 180.0f);
				}
			} else if (mouse1.x < (mouse0.x - 1)) {
				yawAngle -= 1.0f * 3.141592f / 180.0f;
				if (yawAngle < 0.0f) {
					yawAngle += (360.0f * 3.141592f / 180.0f);
				}
			}

			if (mouse1.y > (mouse0.y + 1)) {
				rollAngle += 1.0f * 3.141592f / 180.0f;
				if (rollAngle > (80.0f * 3.141592f / 180.0f)) {
					rollAngle = 80.0f * 3.141592f / 180.0f;
				}
			} else if (mouse1.y < (mouse0.y - 1)) {
				rollAngle -= 1.0f * 3.141592f / 180.0f;
				if (rollAngle < -(80.0f * 3.141592f / 180.0f)) {
					rollAngle = -80.0f * 3.141592f / 180.0f;
				}
			}
			dMatrix cameraDirMat (dRollMatrix(rollAngle) * dYawMatrix(yawAngle));
			cameraDir = cameraDirMat.m_front;
		}
	}

	// save mouse position and left mouse key state for next frame
	mouse0 = mouse1;

	// now set the camera matrix
	dVector target (cameraEyepoint + cameraDir);
	SetCamera (cameraEyepoint, target);
	*/
} 



static void SetDemoCallbacks (SceneManager& system)
{
	system.m_control = PlayerKeyboard;
	system.m_autoSleep = NoPlayerAutoSleep;
	system.m_showIslands = SetShowIslands;
	system.m_showContacts = SetShowContacts; 
	system.m_setMeshCollision = SetShowMeshCollision;
}

static LevelPrimitive* LoadLevelAndSceneRoot (SceneManager& system, const char* levelName, int optimized)
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

	// save the callback
	SetDemoCallbacks (system);

	return level;
*/
}


NewtonBody* CreatePlayBody (
	NewtonWorld* world,
	SceneManager* scene, 
	dFloat mass, 
	const dMatrix& srcMatrix, 
	const dVector& size, 
	int matrixlId)
{
_ASSERTE (0);
return NULL;
/*

	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;
	NewtonBody* rigidBody;
	OGLMesh* meshInstance;
	RenderPrimitive* primitive;
	NewtonCollision* collision;
	dVector origin;
	dVector inertia;

	dMatrix matrix (srcMatrix);
	matrix.m_front = matrix.m_front.Scale (1.0f / dSqrt (matrix.m_front % matrix.m_front));
	matrix.m_right = matrix.m_front * matrix.m_up;
	matrix.m_right = matrix.m_right.Scale (1.0f / dSqrt (matrix.m_right % matrix.m_right));
	matrix.m_up = matrix.m_right * matrix.m_front;

	// create the collision 
	dMatrix location (dRollMatrix(-3.1416f * 0.5f));
	collision = NewtonCreateCapsule (world, size.m_y, size.m_x, 0, &location[0][0]); 
//	collision = NewtonCreateCylinder (world, size.m_y, size.m_x, 0, &location[0][0]); 

	meshInstance = new OGLMesh ("Player", collision, "smilli.tga", "smilli.tga", "smilli.tga");

	// create a visual geometry
	primitive = new RenderPrimitive (matrix, meshInstance);
	scene->AddModel___ (primitive);
	primitive->Release();


	// release the instance
	meshInstance->Release();

	// calculate the moment of inertia and the relative center of mass of the solid
	NewtonConvexCollisionCalculateInertialMatrix (collision, &inertia[0], &origin[0]);	
	Ixx = mass * inertia[0];
	Iyy = mass * inertia[1];
	Izz = mass * inertia[2];

	//create the rigid body
	rigidBody = NewtonCreateBody (world, collision);

	// release the collision geometry when not need it
	NewtonReleaseCollision (world, collision);

	// set the correct center of gravity for this body
	NewtonBodySetCentreOfMass (rigidBody, &origin[0]);

	// set the mass matrix
	NewtonBodySetMassMatrix (rigidBody, mass, Ixx, Iyy, Izz);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData (rigidBody, primitive);

	// assign the wood id
	NewtonBodySetMaterialGroupID (rigidBody, matrixlId);

	//  set continue collision mode
	//	NewtonBodySetContinuousCollisionMode (rigidBody, continueCollisionMode);

	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback (rigidBody, PhysicsBodyDestructor);

	// set the transform call back function
	NewtonBodySetTransformCallback (rigidBody, PhysicsSetTransform);

	// set the force and torque call back function
	NewtonBodySetForceAndTorqueCallback (rigidBody, PhysicsApplyGravityForce);


	// set the matrix for both the rigid body and the graphic body
	NewtonBodySetMatrix (rigidBody, &matrix[0][0]);
	PhysicsSetTransform (rigidBody, &matrix[0][0], 0);

	return rigidBody;
*/
}

void PlayerController (NewtonFrame& system)
{
	NewtonWorld* world;
//	dBone* startPoint;
	LevelPrimitive* scene;
	NewtonBody* playerBody;

	world = system.m_world;

	// create the sky box and the floor,
	//scene = LoadLevelAndSceneRoot (system, "flatplane.dae", 1);
	//scene = LoadLevelAndSceneRoot (system, "dungeon.dae", 1);
	//scene = LoadLevelAndSceneRoot (system, "pitpool.dae", 1);
	scene = LoadLevelAndSceneRoot (system, "castle.dae", 1);
	//scene = LoadLevelAndSceneRoot (system, "playground.dae", 1);

	// create a material carrier to could collision with this object
	int defaultMaterialID;
	defaultMaterialID = NewtonMaterialGetDefaultGroupID (world);

	// get the Player position
	dMatrix location (GetIdentityMatrix());

//location.m_posit.m_x += 6;
//location.m_posit.m_z -= 1;
	location.m_posit.m_y = FindFloor (world, location.m_posit.m_x, location.m_posit.m_z) + 4.0f;

//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _SPHERE_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _BOX_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CONE_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CYLINDER_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CAPSULE_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _CHAMFER_CYLINDER_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _RANDOM_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
//	AddBoxes(system, 10.0f, location, size, 3, 3, 10.0f, _REGULAR_CONVEX_HULL_PRIMITIVE, defaultMaterialID);
//	InitEyePoint (dVector (1.0f, 0.0f, 0.0f), location.m_posit);

	dFloat kinematicCushion = 1.0f / 64.0f;  

//	dVector size (1.8f - kinematicCushion, 0.4f - kinematicCushion, 1.0f);
	dVector size (1.8748f, 0.4885f, 1.0f);
	playerBody = CreatePlayBody (world, &system, 100, location, size, defaultMaterialID);

	// make sure the player is active
//	dVector pin (0.0f, 1.0f, 0.0f, 0.0f);
	dMatrix globalFrame (GetIdentityMatrix());
	globalFrame.m_front = dVector (0.0f, 1.0f, 0.0f, 0.0f);        // up direction in global Space 
	globalFrame.m_up    = dVector (1.0f, 0.0f, 0.0f, 0.0f);        // front direction in global Space 
	globalFrame.m_right = globalFrame.m_front * globalFrame.m_up;  // strafing direction in global Space 

	dFloat maxStairStepFactor = 0.7f / size.m_x;
	player = new MyPlayerController (globalFrame, playerBody, maxStairStepFactor, kinematicCushion); 
}



