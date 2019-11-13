#include "bulletworld.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

irrBulletWorld::irrBulletWorld(IrrlichtDevice* const Device) : device(Device)
{
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	collisionConfiguration->setConvexConvexMultipointIterations();
	btDispatcher*	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface*  pairCache = new btDbvtBroadphase();
	btConstraintSolver*	constraintSolver = new btSequentialImpulseConstraintSolver();

    world = new btDiscreteDynamicsWorld(dispatcher, pairCache,
        constraintSolver, collisionConfiguration);


    // TODO: Fix IPhysicsDebugDraw so that it doesn't draw contact points even when debugging is off
    debug = 0;

    debug = new IPhysicsDebugDraw(device);
    /*debug->setDebugMode(
        IPhysicsDebugDraw::DBG_DrawAabb |
        IPhysicsDebugDraw::DBG_DrawContactPoints |
        IPhysicsDebugDraw::DBG_DrawWireframe);*/
    world->setDebugDrawer(debug);

    world->setGravity(btVector3(0.0,-10.0,0.0));

    // For displaying debugging properties
    propertyText = device->getGUIEnvironment()->addStaticText(L"",
            rect<s32>(10,10,120,240), false);

    printf("irrBullet %i.%i.%i\n", IRRBULLET_VER_MAJOR, IRRBULLET_VER_MINOR, IRRBULLET_VER_SUB);
}


int irrBulletWorld::stepSimulation(f64 timeStep, u32 maxSubSteps, f64 fixedTimeStep)
{
    getPointer()->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);

    return 0;
}


//////////////////////////
// The 'dos' and 'sets' //
//////////////////////////
void irrBulletWorld::removeRigidBody(IRigidBody *body)
{
    getPointer()->removeRigidBody(body->getPointer());
}

void irrBulletWorld::clearForces()
{
    getPointer()->clearForces();
}

void irrBulletWorld::debugDrawWorld(bool b)
{
    if(b == true)
    {
        getPointer()->debugDrawWorld();
    }
}

void irrBulletWorld::debugDrawProperties(bool b, SColor col)
{
    if(b == true)
    {
        // TODO: Percentage calculation needs fixed, sometimes returns greater than 100%
        f32 diff = getNumObjects(EAS_ACTIVE) - getNumObjects(EAS_SLEEPING);
        u32 perc = round32(diff / f32(getNumObjects(EAS_ACTIVE)) * 100);


        stringw str = "FPS: ";
        str += device->getVideoDriver()->getFPS();
        str += "\nObjects: ";
        str += getNumObjects(EAS_ACTIVE) + getNumObjects(EAS_SLEEPING);
        str += "\nActive: ";
        str += getNumObjects(EAS_ACTIVE);
        str += "\nSleeping: ";
        str += getNumObjects(EAS_SLEEPING);
        str += "\nPercentage: ";
        str += (perc < 101) ? stringw(perc) + "%" : "N/A";

        propertyText->setText(str.c_str());
    }

    else
    if(b == false)
    {
        if(propertyText->getText() != L"")
        {
            propertyText->setText(L"");
        }
    }
}


void irrBulletWorld::synchronizeMotionStates()
{
    getPointer()->synchronizeMotionStates();
}

void irrBulletWorld::synchronizeSingleMotionState(IRigidBody *body)
{
    getPointer()->synchronizeSingleMotionState(body->getPointer());
}

void irrBulletWorld::setGravity(const vector3df &gravity)
{
    getPointer()->setGravity(btVector3(gravity.X, gravity.Y, gravity.Z));
}


////////////////
// The 'gets' //
////////////////
IRigidBody* irrBulletWorld::addRigidBody(ICollisionShape *shape)
{
    IRigidBody* b = new IRigidBody(shape);
    rigidBodies.push_back(b);
    getPointer()->addRigidBody(b->getPointer());

    return b;
}

IRigidBody* irrBulletWorld::addRigidBody(ICollisionShape *shape, s32 group, s32 mask)
{
    IRigidBody* b = new IRigidBody(shape);
    rigidBodies.push_back(b);
    getPointer()->addRigidBody(b->getPointer(), group, mask);
    return b;
}


u32 irrBulletWorld::getNumObjects(EActivationState state)
{
    u32 num = 0;

    list<IRigidBody*>::Iterator it = rigidBodies.begin();

    for(; it != rigidBodies.end(); ++it)
    {
        const bool isActive = (*it)->getPointer()->isActive();
        switch(isActive)
        {
            case true:
            {
                if(state == EAS_ACTIVE)
                {
                    num += 1;
                }
                break;
            }

            case false:
            {
                if(state == EAS_SLEEPING)
                {
                    num += 1;
                }
                break;
            }

            default:
                break;
        }
    }

    return num;
}

irrBulletWorld::~irrBulletWorld()
{
    list<IRigidBody*>::Iterator it = rigidBodies.begin();

    for(; it != rigidBodies.end(); ++it)
    {
        delete (*it);
        (*it) = 0;
    }

    if(debug != 0)
    {
        delete debug;
    }
    delete world;
}
