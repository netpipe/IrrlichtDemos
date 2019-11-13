#ifndef __BULLET_WORLD_H_INCLUDED__
#define __BULLET_WORLD_H_INCLUDED__

#define IRRBULLET_VER_MAJOR 0
#define IRRBULLET_VER_MINOR 1
#define IRRBULLET_VER_SUB 1


#include "common.h"
#include "rigidbody.h"
#include "physicsdebug.h"


class irrBulletWorld
{
public:
    irrBulletWorld(irr::IrrlichtDevice* const Device);

    ~irrBulletWorld();

    //////////////////////////
    // The 'dos' and 'sets' //
    //////////////////////////
    int stepSimulation(irr::f64 timeStep, irr::u32 maxSubSteps=1, irr::f64 fixedTimeStep=irr::f64(1.)/irr::f64(60.));

    void synchronizeMotionStates();

    void synchronizeSingleMotionState(IRigidBody *body);

    void setGravity(const irr::core::vector3df &gravity);

    IRigidBody* addRigidBody(ICollisionShape *shape);

    IRigidBody* addRigidBody(ICollisionShape *shape, irr::s32 group, irr::s32 mask);

    void removeRigidBody(IRigidBody *body);

    void clearForces();

    void debugDrawWorld(bool b);

    void debugDrawProperties(bool b, irr::video::SColor col);


    ////////////////
    // The 'gets' //
    ////////////////
    irr::u32 getNumObjects(EActivationState state);

    btDiscreteDynamicsWorld *getPointer() {return world;};

private:
    btDiscreteDynamicsWorld *world;
    IPhysicsDebugDraw *debug;
    irr::IrrlichtDevice *device;
    irr::gui::IGUIStaticText *propertyText;
    irr::core::list<IRigidBody *> rigidBodies;
};

#endif // __BULLET_WORLD_H_INCLUDED__
