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

#ifndef __DEMO_ENTITY_H__
#define __DEMO_ENTITY_H__

#include "DemoEntityManager.h"
class DemoMesh;

class DemoEntity: public dHierarchy<DemoEntity>, virtual public dClassInfo, public dVariableList   
{
public:
	DemoEntity(DemoEntity* parent);
	DemoEntity(DemoEntityManager& world, const dScene* scene, dScene::dTreeNode* rootSceneNode, dTree<DemoMesh*, dScene::dTreeNode*>& meshCache, 
			   DemoEntityManager::EntityDictionary& entityDictionary, DemoEntity* parent = NULL);
	virtual ~DemoEntity(void);

	DemoMesh* GetMesh() const;
	void SetMesh(DemoMesh* m_mesh);
//	virtual void SetMatrix(const dMatrix& matrix);

	dMatrix GetCurrentMatrix () const;
	dMatrix CalculateGlobalMatrix (const DemoEntity* const root = NULL) const;
	virtual void SetMatrix(DemoEntityManager& world, const dQuaternion& rotation, const dVector& position);

	virtual void Render(dFloat timeStep) const;

	void InterpolateMatrix (DemoEntityManager& world, dFloat param);

	protected:
	mutable dMatrix m_matrix;			// interpolated matrix
	dVector m_curPosition;				// position one physics simulation step in the future
	dVector m_nextPosition;             // position at the current physics simulation step
	dQuaternion m_curRotation;          // rotation one physics simulation step in the future  
	dQuaternion m_nextRotation;         // rotation at the current physics simulation step  
	unsigned m_lock;

	DemoMesh* m_mesh;

	static void SetTransformCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex);
	//static void PhysicsApplyGravityForce (const NewtonBody* body, dFloat timestep, int threadIndex);
	//static void PhysicsBodyDestructor (const NewtonBody* body);

	dAddRtti(dClassInfo);

	friend class DemoEntityManager;
};

#endif