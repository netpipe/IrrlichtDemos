#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>

#include <map>

class Physics
{
    public:
        Physics();
        ~Physics();
        void unregisterObject(const unsigned long int &PhysicObjectID);
        void tick();

        static Physics* getInstance() { return instance; }
    private:
        static Physics* instance;


        unsigned long int refId;
        bool firstTick;

        unsigned long int lasttick;

        btDiscreteDynamicsWorld *dynamicsWorld;
        btDefaultCollisionConfiguration *collisionConfiguration;
        btCollisionDispatcher *dispatcher;
        btBroadphaseInterface *broadphase;
        btSequentialImpulseConstraintSolver *solver;
};

#endif
