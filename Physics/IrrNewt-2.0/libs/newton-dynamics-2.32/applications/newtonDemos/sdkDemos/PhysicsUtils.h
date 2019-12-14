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

#ifndef __PHYSICS_UTIL__
#define __PHYSICS_UTIL__


/*
enum PrimitiveType
{
	_BOX_PRIMITIVE,
	_CONE_PRIMITIVE,
	_SPHERE_PRIMITIVE,
	_CYLINDER_PRIMITIVE,
	_CAPSULE_PRIMITIVE,
	_CHAMFER_CYLINDER_PRIMITIVE,
	_RANDOM_CONVEX_HULL_PRIMITIVE,
	_REGULAR_CONVEX_HULL_PRIMITIVE,
};


#define CAMERA_SPEED 10.0f

extern dVector cameraDir;
extern dVector cameraEyepoint;
extern dFloat yawAngle;
extern dFloat rollAngle;

class OGLMesh;
class SceneManager;
class NewtonCustomJoint;



void InitEyePoint (const dVector& dir, const dVector& origin);

void AutoSleep (SceneManager& me, int mode);
void SetShowIslands (SceneManager& me, int mode);
void SetShowContacts (SceneManager& me, int mode);
void SetShowMeshCollision (SceneManager& me, int mode);

void ShowBodyContacts (const NewtonBody* body);

void ShowJointInfo(const NewtonCustomJoint* joint);

void ConvexCastPlacement (NewtonBody* body);
dFloat FindFloor (const NewtonWorld* world, dFloat x, dFloat z);

//void CalculateAABB (const NewtonBody* body, dVector& minP, dVector& maxP);
void CalculateAABB (const NewtonCollision* collision, const dMatrix& matrix, dVector& minP, dVector& maxP);


void Keyboard(SceneManager& me);

void PhysicsSetTransform (const NewtonBody* body, const dFloat* matrix, int threadIndex);

int  PhysicsIslandUpdate (const NewtonWorld* world, const void* islandHandle, int bodyCount);


NewtonCollision* CreateConvexCollision (NewtonWorld* world, const dMatrix& offsetMatrix, const dVector& size, PrimitiveType type, int materialID);
NewtonBody* CreateSimpleSolid (NewtonWorld* world, SceneManager* scene, OGLMesh* mesh, dFloat mass, const dMatrix& matrix, NewtonCollision* collision, int materialId);

NewtonBody* CreateGenericSolid (NewtonWorld* world, SceneManager* scene, const char* meshName, dFloat mass, const dMatrix& matrix, const dVector& size, PrimitiveType type, int materialID);



// this callback is called for every contact between the two bodies
int GenericContactProcess (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1, dFloat timestep, int threadIndex);
int OnAABBOverlap (const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1, int threadIndex);


NewtonMesh* CreateCollisionTreeDoubleFaces (NewtonWorld* world, NewtonCollision* optimizedDoubelFacesTree);
*/

#endif

dFloat RandomVariable(dFloat amp);

void PhysicsBodyDestructor (const NewtonBody* body);
void PhysicsApplyGravityForce (const NewtonBody* body, dFloat timestep, int threadIndex);

void ShowJointContacts (const NewtonJoint* contactJoint);
void GenericContactProcess (const NewtonJoint* contactJoint, dFloat timestep, int threadIndex);