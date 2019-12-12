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

#ifndef __NEWTON_DEMOS__
#define __NEWTON_DEMOS__
#include "dList.h"
#include "OGLModel.h"

//class SceneManager;
class NewtonFrame;
typedef void (*LaunchSDKDemoCallback) (NewtonFrame& system);

class SceneManager: public dList <OGLModel*>
{
	public:
	SceneManager ();
	~SceneManager ();

	void CloseScene ();

	typedef void (*Create) (SceneManager& me);
	typedef void (*KeyboardControl) (SceneManager& me);
	typedef void (*AutoSleep) (SceneManager& me, int mode);
	typedef void (*SetShowInslandMode) (SceneManager& me, int mode);
	typedef void (*SetShowContacts) (SceneManager& me, int mode);
	typedef void (*SetShowMeshCollision) (SceneManager& me, int mode);

	void Render ();
	void ApplyControl ();
	void SetAutoSleep (int mode);
	void CleanUp();

	void AddModel___ (OGLModel* model);
	void RemoveModel (OGLModel* model);
	NewtonWorld* m_world;

	AutoSleep m_autoSleep;
	KeyboardControl m_control;
	SetShowContacts m_showContacts;
	SetShowInslandMode m_showIslands;
	SetShowMeshCollision m_setMeshCollision;
};


void BasicSphereSpin (NewtonFrame& system);
void PrecessingTops (NewtonFrame& system);
void ConvexCast (NewtonFrame& system);
void Restitution (NewtonFrame& system);
void Friction (NewtonFrame& system);
void ClosestDistance(NewtonFrame& system);
void BasicBoxStacks (NewtonFrame& system);
void BasicCylinderStacks (NewtonFrame& system);
void BasicConvexStacks (NewtonFrame& system);

void CreatePyramid (NewtonFrame& system);
void CreatePyramidTall (NewtonFrame& system);

void CreateWalls (NewtonFrame& system);
void CreateTower (NewtonFrame& system);
void CreateTowerTall (NewtonFrame& system);

void UnstableStacks (NewtonFrame& system);
void UnstableStruture (NewtonFrame& system);

void Jenga (NewtonFrame& system);
void JengaTall (NewtonFrame& system);

void ScaledCollision (NewtonFrame& system);
void SimplePlaneCollision (NewtonFrame& system);
void SimpleMeshLevelCollision (NewtonFrame& system);
void OptimizedMeshLevelCollision (NewtonFrame& system);
void SimpleHeighMapCollision (NewtonFrame& system);
void UserHeighMapColliion (NewtonFrame& system);
void SceneCollision (NewtonFrame& system);
void CompoundCollision (NewtonFrame& system);
void ArchimedesBuoyancy (NewtonFrame& system);
void Magnets (NewtonFrame& system);
void Repulsive (NewtonFrame& system);


void LegacyJoints (NewtonFrame& system);
void BasicCustomJoints (NewtonFrame& system);
void BasicRobots (NewtonFrame& system);
void TracktionJoints (NewtonFrame& system);
void RayCastCar (NewtonFrame& system);
void SkinRagDoll (NewtonFrame& system);
void DescreteRagDoll (NewtonFrame& system);
void PlayerController(NewtonFrame& system);




/*
void RealTimeSimpleDestruction(NewtonFrame& system);
void PrefabSimpleDestruction(NewtonFrame& system);
void BuildingDestruction(NewtonFrame& system);
*/

#endif
