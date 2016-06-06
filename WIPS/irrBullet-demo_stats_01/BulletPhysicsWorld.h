#pragma once

#include <irrlicht.h>
//using namespace irr;

#include "btBulletDynamicsCommon.h"

class CWorldObjectMgr;
class CWorldObject;

class CBulletPhysicsWorld
{
private:
	btAxisSweep3* m_pBroadphase;
	btCollisionDispatcher* m_pCollisionDispatcher;
	btConstraintSolver* m_pSolver;
	CWorldObjectMgr* m_pMgr;

	btRigidBody* m_pBody0, *m_pBody1; //just a couple of pointers to get contact info, for example purposes
public:
	btDiscreteDynamicsWorld* m_pDynamicsWorld;
public:
	CBulletPhysicsWorld(CWorldObjectMgr*);
	virtual ~CBulletPhysicsWorld(void);
	irr::s32 Update(irr::f32 elapsedTime);
	CWorldObject* RayTest(irr::core::vector3df& start, irr::core::vector3df& end);
	void RegisterCollision(CWorldObject* pObj1, CWorldObject* pObj2);
};
