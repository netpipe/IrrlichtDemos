//CBulletPhysicsWorld
//The physical world. No real need to have this as a separate class, except for clarity

#include "BulletPhysicsWorld.h"
#include "btBulletDynamicsCommon.h"
#include "WorldObject.h"
#include "WorldObjectMgr.h"
#include "CollisionObjectMgr.h"
#include "utils.h"
#include "App.h"

extern IrrlichtDevice* g_pIrrDevice;
#define GRAVITY -9.81f

//create the collision object manager for sharing collision geometry
CCollisionObjectMgr* g_pCollisionObjectMgr=0;

CBulletPhysicsWorld::CBulletPhysicsWorld(CWorldObjectMgr * wmgr)
{
	const int maxProxies = 32766;
	const int maxOverlap = 65535;

	m_pMgr = wmgr;

	g_pCollisionObjectMgr = new CCollisionObjectMgr;

	m_pCollisionDispatcher=0;
	m_pBroadphase=0;
	m_pDynamicsWorld=0;
	m_pSolver=0;
btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	m_pCollisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
	btVector3 worldAabbMin(-10000,-10000,-10000);
	btVector3 worldAabbMax(10000,10000,10000);

	m_pBroadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);
	m_pSolver = new btSequentialImpulseConstraintSolver;
	m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pCollisionDispatcher,m_pBroadphase,m_pSolver, collisionConfiguration);
	m_pDynamicsWorld->setGravity(btVector3(0,GRAVITY,0));

}

CBulletPhysicsWorld::~CBulletPhysicsWorld(void)
{
	SAFE_DELETE(m_pDynamicsWorld);
	SAFE_DELETE(m_pSolver);
	SAFE_DELETE(m_pBroadphase);
	SAFE_DELETE(m_pCollisionDispatcher);
	SAFE_DELETE(g_pCollisionObjectMgr);
}

s32 CBulletPhysicsWorld::Update(irr::f32 elapsedTime)
{
	//remember, Bullet uses a fixed internal timestep and interpolates
	// if the delta time is different. Damn useful.
#ifdef USE_QUICKPROF
	btProfiler::beginBlock(PROF_BULLET);
	btProfiler::beginBlock(PROF_BULLET_PERCENT);
#endif
	m_pDynamicsWorld->stepSimulation(elapsedTime,1);
#ifdef USE_QUICKPROF
	btProfiler::endBlock(PROF_BULLET);
	btProfiler::endBlock(PROF_BULLET_PERCENT);
#endif

//	btProfiler::beginBlock(PROF_GAME);


	// apply the new positions and rotations to the scenenodes
	m_pDynamicsWorld->updateAabbs();

	//get contact points between the 2 rigid bodies specified in CBulletPhysicsWorld::RegisterCollision()
	int numManifolds = m_pDynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)   {
		btPersistentManifold* contactManifold = m_pDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
		contactManifold->refreshContactPoints(obA->getWorldTransform(),obB->getWorldTransform());

		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)	{
			btRigidBody* pBody0 = (btRigidBody*) contactManifold->getBody0();
			btRigidBody* pBody1 = (btRigidBody*) contactManifold->getBody1();
			if ((pBody0 == m_pBody0 && pBody1 == m_pBody1) || (pBody1 == m_pBody0 && pBody0 == m_pBody1)) {
				char c[256];
				sprintf(c, "%d contacts\n", numContacts);
//!				OutputDebugString(c);
				//can do alot more here, see collision interface demo (which this code comes from)
			}
		}

		//you can un-comment out this line, and then all points are removed
		//contactManifold->clearManifold();
	}

	CWorldObject* pObj;
	CWorldObject* pRemoveObj = 0;

	for (pObj=m_pMgr->GetFirst(); pObj; pObj=m_pMgr->GetNext()) {
		//do the graphics update
		pObj->UpdateTransform();

		//show inactive objects as dark
		//first see if object is deactivating
		//would probably need to compare last state with current state to
		// determine if re-activating, de-activating etc.
		if (pObj->m_pNode->getMaterialCount()>0) {
			if (pObj->m_pRigidBody->getActivationState() == ISLAND_SLEEPING) { //deactivated
				GET_DRIVER;
				video::SMaterial& mat = pObj->m_pNode->getMaterial(0);
				mat.SpecularColor = video::SColor(0,0,0,0);
			}
			else {
				GET_DRIVER;
				//can't seem to set some objects to darker or transparent,
				// so using a lighter specular colour when active
				video::SMaterial& mat = pObj->m_pNode->getMaterial(0);
				mat.SpecularColor = video::SColor(2,2,2,0);
			}
		}


		//remove objects if no longer needed
		if (pObj->GetPosition().Y < -50) {
			pRemoveObj = pObj;
		}
	}

	if (pRemoveObj) m_pMgr->Remove(pRemoveObj);

	return 1;
}

CWorldObject* CBulletPhysicsWorld::RayTest(irr::core::vector3df& _start, irr::core::vector3df& _end)
{
	//convert to btVector3
	btVector3 start(_start.X, _start.Y, _start.Z);
	btVector3 end(_end.X, _end.Y, _end.Z);

	btCollisionWorld::ClosestRayResultCallback rayCallback(start,end);

	m_pDynamicsWorld->rayTest(start,end,rayCallback);

	//get world object from btRigidBody
	CWorldObject* pObj=NULL;
	if (rayCallback.hasHit()) {
		btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);
		if (body) {
			return (CWorldObject*) body->getUserPointer(); //we set this during initialisation of physics object
		}
	}
	return NULL;
}

void CBulletPhysicsWorld::RegisterCollision(CWorldObject* pObj1, CWorldObject* pObj2)
{
	//this is a very simple example of how to get collision notification.
	//not really a 'registration' process
	//should really have an array of objects or some other system
	//see Update() function for notification of collision
	m_pBody0=pObj1->m_pRigidBody;
	m_pBody1=pObj2->m_pRigidBody;
}
