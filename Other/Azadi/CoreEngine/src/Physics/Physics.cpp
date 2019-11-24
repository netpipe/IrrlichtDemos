#include "Physics.h"

Physics* Physics::instance = NULL;

Physics::Physics()
{
    instance = this;
    refId = 0;

    console << GUI::Console::log << GUI::Console::medium << "Initializing Physics engine" << GUI::Console::endl();

    btVector3 worldAabbMin(-1000,-1000,-1000);
	btVector3 worldAabbMax(1000,1000,1000);
	int maxProxies = 32766;

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
    //dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase,solver);

  //  dynamicsWorld->getDispatchInfo().m_enableSPU = true;
  //  dynamicsWorld->setGravity(btVector3(0, -10, 0));

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

unsigned long int Physics::registerObject(const PhysicsObject* newObj)
{
    while (PhysicsObjects[refId] != NULL || refId == 0)
        ++refId;

    PhysicsObjects[refId] = newObj;
    return refId;
}

void Physics::unregisterObject(const unsigned long int &objectID)
{
    if (PhysicsObjects[objectID] != NULL)
        PhysicsObjects.erase(objectID);
}



void Physics::tick()
{
    if (firstTick)
    {
        lasttick = SDL_GetTicks();
        return;
    }

    float step = ((SDL_GetTicks() - lasttick) / 1000) / 60;

 //   dynamicsWorld->stepSimulation(step,0);
    lasttick = SDL_GetTicks();
}
