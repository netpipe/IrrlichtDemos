#include "GamePhysique.h"
#include "PhysiqueObject/PhysiqueObject.h"
//! the licence is on EntityManager.h

namespace GamePhysique
{
    Physique::Physique(irr::IrrlichtDevice*dev): device(dev)
    {
        CollisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(CollisionConfiguration);
        broadphase = new btAxisSweep3(btVector3(-10000,-10000,-10000),btVector3(10000,10000,10000),1024);
        solver = new btSequentialImpulseConstraintSolver;
        dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher,broadphase,solver,CollisionConfiguration);
            dynamicsWorld->setGravity(btVector3(0,-9.81,0));
            dynamicsWorld->debugDrawWorld();
    }
    Physique::~Physique()
    {
        for(int i=dynamicsWorld->getNumCollisionObjects()-1;i>0;i--)
        {
            btCollisionObject*	obj=dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody*		body=btRigidBody::upcast(obj);
            if(body&&body->getMotionState())
                delete body->getMotionState();
            while(dynamicsWorld->getNumConstraints())
            {
                btTypedConstraint*	pc=dynamicsWorld->getConstraint(0);
                dynamicsWorld->removeConstraint(pc);
                delete pc;
            }
            btSoftBody* softBody = btSoftBody::upcast(obj);
            if (softBody)
                ((btSoftRigidDynamicsWorld*)dynamicsWorld)->removeSoftBody(softBody);
            else
                dynamicsWorld->removeCollisionObject(obj);
            delete obj->getUserPointer();
        }
        delete dynamicsWorld;
        delete solver;
        delete CollisionConfiguration;
        delete dispatcher;
        delete broadphase;
    }
    void Physique::drawAll(float rd, int precision)
    {
        irr::u32 DeltaTime = device->getTimer()->getTime() - TimeStamp;
        getWorldPhysic()->stepSimulation(DeltaTime*0.001f,precision,btScalar(rd));
        for(int i = dynamicsWorld->getNumCollisionObjects()-1; i>0; i-- )
            ((PhysiqueObjet*)dynamicsWorld->getCollisionObjectArray()[i]->getUserPointer())->drawNode();
        TimeStamp = device->getTimer()->getTime();
    }
}
