#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>
#include "../Scripting/Headers/Physics.h"
#include "../GUI/Console.h"

#include <map>

class Physics : public azPhysics
{
    public:
        Physics();
        ~Physics();
        unsigned long int registerObject(const PhysicsObject*);
        void unregisterObject(const unsigned long int &PhysicObjectID);
        void tick();

        static Physics* getInstance() { return instance; }
    private:
        static Physics* instance;

        std::map<unsigned long int, const PhysicsObject *> PhysicsObjects;

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
