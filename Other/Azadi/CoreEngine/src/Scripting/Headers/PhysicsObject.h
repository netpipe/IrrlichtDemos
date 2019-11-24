#ifndef AZPHYSICSOBJECT_H
#define AZPHYSICSOBJECT_H

class PhysicsObject
{
    public:
        virtual ~PhysicsObject() {}

        float x, y, z;
        float rx, ry, rz;
        float mass;
        float accX, accY, accZ;

        enum collisionType
        {
            box,
            sphere,
            cylinder,
        } cType;
};

#endif
