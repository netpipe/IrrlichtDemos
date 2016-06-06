#include "physics.h"



btDiscreteDynamicsWorld* makeworld()
{
	//create the collision boundries
	btVector3 worldAabbMin(-1000,-1000,-1000);
	btVector3 worldAabbMax(1000,1000,1000);

	//A broadphase is a way to handle collision
	btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, 3000);

	btDefaultCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();

	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver; //the physics solver

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfig); //Finally create the world

	return dynamicsWorld;
}

