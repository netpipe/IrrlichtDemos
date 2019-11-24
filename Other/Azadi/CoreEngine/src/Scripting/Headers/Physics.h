#ifndef AZPHYSICS_H
#define AZPHYSICS_H

#include "PhysicsObject.h"

class azPhysics
{
    public:
        virtual ~azPhysics() {}

        virtual unsigned long int registerObject(const PhysicsObject*) = 0;
        virtual void unregisterObject(const unsigned long int &PhysicObjectID) = 0;
};

#endif
