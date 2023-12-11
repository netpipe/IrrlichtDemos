#include "Physics.h"

Physics* Physics::instance = NULL;

Physics::Physics()
{
	instance = this;
	refId = 0;

	btVector3 worldAabbMin ( -1000,-1000,-1000 );
	btVector3 worldAabbMax ( 1000,1000,1000 );
	int maxProxies = 32766;

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher ( collisionConfiguration );
	solver = new btSequentialImpulseConstraintSolver;
	broadphase = new btAxisSweep3 ( worldAabbMin, worldAabbMax, maxProxies );
	dynamicsWorld = new btDiscreteDynamicsWorld ( dispatcher, broadphase,solver,collisionConfiguration );

	dynamicsWorld->getDispatchInfo().m_enableSPU = true;
	dynamicsWorld->setGravity ( btVector3 ( 0, -10, 0 ) );

	firstTick = true;
}

Physics::~Physics()
{
	delete dynamicsWorld;
	delete broadphase;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
}

void Physics::unregisterObject ( const unsigned long int &objectID )
{

}



void Physics::tick()
{

}
