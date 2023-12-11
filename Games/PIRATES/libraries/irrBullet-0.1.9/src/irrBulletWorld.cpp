// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include <IrrlichtDevice.h>
#include <IGUIEnvironment.h>
#include <IGUIStaticText.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletSoftBody/btSoftSoftCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include "irrBulletLiquidBody.h"

#include "irrBulletWorld.h"
#include "irrBulletRayCastVehicle.h"
#include "irrBulletCompileConfig.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

irrBulletWorld::irrBulletWorld(irr::IrrlichtDevice* const Device, bool useGImpact, bool useDebugDrawer)
{
	device.reset(Device);

    collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	//collisionConfiguration->setConvexConvexMultipointIterations();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	pairCache = new btDbvtBroadphase();
	pairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	constraintSolver = new btSequentialImpulseConstraintSolver();

    world.reset(new btSoftRigidDynamicsWorld(dispatcher, pairCache,
        constraintSolver, collisionConfiguration));

    // Initialize the softbody world info
    softBodyWorldInfo.m_broadphase = pairCache;
    softBodyWorldInfo.m_dispatcher = dispatcher;

    softBodyWorldInfo.m_sparsesdf.Initialize();
    softBodyWorldInfo.m_gravity.setValue(0,-10.0,0);
    softBodyWorldInfo.air_density = (btScalar)1.2;
    softBodyWorldInfo.water_density = 0;
    softBodyWorldInfo.water_offset = 0;
    softBodyWorldInfo.water_normal = btVector3(0,0,0);

    LiquidBodyCount = 0;
    CollisionObjectCount = 0;


    isPaused = false;

    if(useGImpact == true)
    {
        gimpactEnabled = true;
        btGImpactCollisionAlgorithm::registerAlgorithm((btCollisionDispatcher*)dispatcher);
    }

    else
    {
        gimpactEnabled = false;
    }


    debug = 0;

    if(useDebugDrawer)
    {
        debug = new IPhysicsDebugDraw(device);
        world->setDebugDrawer(debug);

        debugMat.Lighting = false;
    }

    // For displaying debugging properties - by default on TOP_LEFT
	propertyText = device->getGUIEnvironment()->addStaticText(L"",
		rect<s32>(10, 10, 120, 240), false);
	TextPropertiesPosition = EDPT_POSITION::EDPT_TOP_LEFT;

    printf("irrBullet %i.%i.%i\n", IRRBULLET_VER_MAJOR, IRRBULLET_VER_MINOR, IRRBULLET_VER_MICRO);
}

irrBulletWorld::irrBulletWorld(std::shared_ptr<irr::IrrlichtDevice> Device, bool useGImpact, bool useDebugDrawer)
{
	device = std::move(Device);

	collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	//collisionConfiguration->setConvexConvexMultipointIterations();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	pairCache = new btDbvtBroadphase();
	pairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	constraintSolver = new btSequentialImpulseConstraintSolver();

	world.reset(new btSoftRigidDynamicsWorld(dispatcher, pairCache,
		constraintSolver, collisionConfiguration));

	// Initialize the softbody world info
	softBodyWorldInfo.m_broadphase = pairCache;
	softBodyWorldInfo.m_dispatcher = dispatcher;

	softBodyWorldInfo.m_sparsesdf.Initialize();
	softBodyWorldInfo.m_gravity.setValue(0, -10.0, 0);
	softBodyWorldInfo.air_density = (btScalar)1.2;
	softBodyWorldInfo.water_density = 0;
	softBodyWorldInfo.water_offset = 0;
	softBodyWorldInfo.water_normal = btVector3(0, 0, 0);

	LiquidBodyCount = 0;
	CollisionObjectCount = 0;


	isPaused = false;

	if (useGImpact == true)
	{
		gimpactEnabled = true;
		btGImpactCollisionAlgorithm::registerAlgorithm((btCollisionDispatcher*)dispatcher);
	}

	else
	{
		gimpactEnabled = false;
	}


	debug = 0;

	if (useDebugDrawer)
	{
		debug = new IPhysicsDebugDraw(device);
		world->setDebugDrawer(debug);

		debugMat.Lighting = false;
	}

	// For displaying debugging properties - by default on TOP_LEFT
	propertyText = device->getGUIEnvironment()->addStaticText(L"",
		rect<s32>(10, 10, 120, 240), false);
	TextPropertiesPosition = EDPT_POSITION::EDPT_TOP_LEFT;

	printf("irrBullet %i.%i.%i\n", IRRBULLET_VER_MAJOR, IRRBULLET_VER_MINOR, IRRBULLET_VER_MICRO);
}

u32 irrBulletWorld::stepSimulation(f32 timeStep, u32 maxSubSteps, f32 fixedTimeStep)
{
    if(isPaused == false)
    {
        u32 i = getPointer()->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);
        updateCollisionObjects();
        updateLiquidBodies();
        return i;
    }

    return 0;
}


void irrBulletWorld::updateLiquidBodies()
{
	for (auto& it : std::move(liquidBodies))
    {
        it->updateLiquidBody();
    }
}


void irrBulletWorld::updateCollisionObjects()
{
	for (auto& cbit : std::move(collisionObjects))
    {
        auto obj = cbit;

		if (obj->getObjectType() == ECollisionObjectType::ECOT_SOFT_BODY)
        {
            static_cast<ISoftBody*>(obj)->updateSoftBody();
        }

        if(obj)
        {
            for(u32 j=0; j < obj->getNumAffectors(); j++)
            {
                auto affector = obj->getAffector(j);
                if(affector->hasFinished() == false)
                {
                    affector->affectObject(obj, device->getTimer()->getTime());
                }
            }
        }
    }

	auto dlit = std::move(deletionList.begin());

	for (; dlit != std::move(deletionList.end());)
    {
        this->removeCollisionObject((*dlit));
		dlit = std::move(deletionList.erase(dlit));
    }
}


void irrBulletWorld::registerGImpactAlgorithm()
{
    if(gimpactEnabled == false)
    {
        gimpactEnabled = true;
        btGImpactCollisionAlgorithm::registerAlgorithm((btCollisionDispatcher*)getPointer()->getDispatcher());
    }
}


IRigidBody* irrBulletWorld::addRigidBody(ICollisionShape* shape)
{
    auto b = new IRigidBody(this, shape);
    collisionObjects.push_back(b);
    getPointer()->addRigidBody(b->getPointer());

    CollisionObjectCount++;

    return b;
}


IRigidBody* irrBulletWorld::addRigidBody(ICollisionShape *shape, s32 group, s32 mask)
{
    auto b = new IRigidBody(this, shape);
    collisionObjects.push_back(b);
    getPointer()->addRigidBody(b->getPointer(), group, mask);

    CollisionObjectCount++;

    return b;
}


ISoftBody* irrBulletWorld::addSoftBody(IMeshSceneNode* const node)
{
    auto b = new ISoftBody(this, node);
    collisionObjects.push_back(b);
    getPointer()->addSoftBody(b->getPointer());

    CollisionObjectCount++;

    return b;
}


void irrBulletWorld::addToDeletionQueue(ICollisionObject* obj)
{
    if(!obj)
        return;
    deletionList.push_back(obj);
}

ILiquidBody* irrBulletWorld::addLiquidBody(const irr::core::vector3df& pos, const irr::core::aabbox3df& aabb,
    irr::f32 waveFrequency, irr::f32 density)
{
    auto liquidBody = new ILiquidBody(this, pos, aabb, waveFrequency, density);

    liquidBodies.push_back(liquidBody);

    LiquidBodyCount++;

    return liquidBody;
}


IRaycastVehicle* irrBulletWorld::addRaycastVehicle(IRigidBody* const body, const vector3d<s32>& coordSys)
{
    auto vehicle = new IRaycastVehicle(body, coordSys);

    raycastVehicles.push_back(vehicle);

    getPointer()->addVehicle(vehicle->getPointer());

    return vehicle;
}

IRaycastVehicle* irrBulletWorld::addRaycastVehicle(IRigidBody* const body, btVehicleRaycaster* const raycaster, const vector3d<s32>& coordSys)
{
    auto vehicle = new IRaycastVehicle(body, raycaster, coordSys);

    raycastVehicles.push_back(vehicle);

    getPointer()->addVehicle(vehicle->getPointer());

    return vehicle;
}

void irrBulletWorld::removeCollisionObject(ICollisionObject* const obj, bool deleteObject)
{
    if(obj)
    {
        auto cbit = collisionObjects.begin();

        for(; cbit != collisionObjects.end(); )
        {
            if((*cbit) == obj)
            {
				if ((*cbit)->getObjectType() == ECollisionObjectType::ECOT_RIGID_BODY)
                {
                    #ifdef IRRBULLET_DEBUG_MODE
                        printf("irrBullet: Removing rigid body (%i)\n", obj->getUniqueID());
                    #endif
                    if(static_cast<IRigidBody*>(obj)->getVehicleReference() != 0)
                        removeRaycastVehicle(static_cast<IRigidBody*>(obj)->getVehicleReference());
                    getPointer()->removeRigidBody(static_cast<IRigidBody*>(obj)->getPointer());
                }

                else
				if ((*cbit)->getObjectType() == ECollisionObjectType::ECOT_SOFT_BODY)
                {
                    #ifdef IRRBULLET_DEBUG_MODE
                        printf("irrBullet: Removing soft body (%i)\n", obj->getUniqueID());
                    #endif
                    getPointer()->removeSoftBody(static_cast<ISoftBody*>(obj)->getPointer());
                }

                if(deleteObject == true)
                {
					if ((*cbit)->getObjectType() == ECollisionObjectType::ECOT_RIGID_BODY)
                    {
                        delete static_cast<IRigidBody*>(*cbit);
                        (*cbit) = 0;
                    }

                    else
                    {
                        delete static_cast<ISoftBody*>(*cbit);
                        (*cbit) = 0;
                    }
                }
                cbit = collisionObjects.erase(cbit);

                CollisionObjectCount--;

                break;
            }

            else
                cbit++;
        }
    }
}

void irrBulletWorld::removeRaycastVehicle(IRaycastVehicle* const vehicle)
{
    if(vehicle)
    {
        auto rvit = raycastVehicles.begin();

        for(; rvit != raycastVehicles.end(); )
        {
            if((*rvit) == vehicle)
            {
                #ifdef IRRBULLET_DEBUG_MODE
                    printf("irrBullet: Removing raycast vehicle (BODY: %i)\n", vehicle->getRigidBody()->getUniqueID());
                #endif
                getPointer()->removeVehicle(vehicle->getPointer());

                delete (*rvit);
                (*rvit) = 0;
                rvit = raycastVehicles.erase(rvit);

                break;
            }

            else
                rvit++;
        }
    }
}

void irrBulletWorld::removeLiquidBody(ILiquidBody* const liquidBody)
{
    if(liquidBody)
    {
        auto rvit = liquidBodies.begin();

        for(; rvit != liquidBodies.end(); )
        {
            if((*rvit) == liquidBody)
            {
                #ifdef IRRBULLET_DEBUG_MODE
                    printf("irrBullet: Removing raycast vehicle (BODY: %i)\n", liquidBody->getUniqueID());
                #endif

                delete (*rvit);
                (*rvit) = 0;
                rvit = liquidBodies.erase(rvit);

                break;
            }

            else
                rvit++;
        }
    }
}

void irrBulletWorld::clearForces()
{
    getPointer()->clearForces();
}


void irrBulletWorld::setDebugMode(irrPhysicsDebugMode mode)
{
    if(debug != 0)
        debug->setDebugMode(static_cast<int>(mode));
}

void irrBulletWorld::setPropertiesTextPosition(EDPT_POSITION pos)
{
    TextPropertiesPosition = pos;

	if (propertyText != nullptr)
		propertyText->remove();

    OriginalScreenSize = device->getVideoDriver()->getScreenSize();

    irr::u32 width = OriginalScreenSize.Width;
    irr::u32 height = OriginalScreenSize.Height;

	if (TextPropertiesPosition == EDPT_POSITION::EDPT_TOP_LEFT)
    {
        propertyText = device->getGUIEnvironment()->addStaticText(L"",
            rect<s32>(10, 10, 120, 240), false);
    }

	else if (TextPropertiesPosition == EDPT_POSITION::EDPT_TOP_RIGHT)
    {
        propertyText = device->getGUIEnvironment()->addStaticText(L"",
            rect<s32>(width - 60, 10, width, height), false);
    }

	else if (TextPropertiesPosition == EDPT_POSITION::EDPT_BOTTOM_LEFT)
    {
        propertyText = device->getGUIEnvironment()->addStaticText(L"",
            rect<s32>(10, height - 60, 150, height), false);
    }

	else if (TextPropertiesPosition == EDPT_POSITION::EDPT_BOTTOM_RIGHT)
    {
       propertyText = device->getGUIEnvironment()->addStaticText(L"",
            rect<s32>(width - 60, height - 70, width, height), false);
    }
}

void irrBulletWorld::debugDrawWorld(bool setDriverMaterial)
{
    if(debug != 0)
    {
        if(setDriverMaterial)
        {
            device->getVideoDriver()->setMaterial(debugMat);
            device->getVideoDriver()->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
        }
        getPointer()->debugDrawWorld();
    }
}

void irrBulletWorld::debugDrawProperties(bool b, const SColor& col)
{
    if(b == true)
    {
        const u32 numObjects = getNumCollisionObjects();
        u32 active = 0;
        u32 sleeping = 0;

        for(u32 i=0; i < numObjects; i++)
        {
			if (getCollisionObjectByIndex(i)->getActivationState() == EActivationState::EAS_ACTIVE)
                active++;

            else
			if (getCollisionObjectByIndex(i)->getActivationState() == EActivationState::EAS_SLEEPING)
                sleeping++;
        }

        // Shows percentage of active objects.
        const f32 diff = f32(active - sleeping);
        const s32 perc = s32((diff / active) * 100);


        stringw str = "MPF: "; // Milliseconds Per Frame
        str += (1000 / (device->getVideoDriver()->getFPS()));
        str += "\nObjects: ";
        str += numObjects;
        str += "\nActive: ";
        str += active;
        str += "\nSleeping: ";
        str += sleeping;
        str += "\nPerc. Active: ";
        str += (perc > 0) ? (stringw(perc) + "%") : "0%";

        propertyText->setText(str.c_str());

        if(propertyText->getOverrideColor() != col)
            propertyText->setOverrideColor(col);


        // Check resized event
        irr::core::dimension2du CurrentScreenSize = device->getVideoDriver()->getScreenSize();
		if (CurrentScreenSize != OriginalScreenSize)
		{
			printf("-- irrBullet: Resized... --\n");
			setPropertiesTextPosition(TextPropertiesPosition);
		}

    }

    else
    if(b == false)
    {
        if(0 != wcscmp(propertyText->getText(), L""))
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


/*u32 irrBulletWorld::getNumCollisionObjects()
{
    u32 num = 0;

    list<ICollisionObject*>::Iterator it = collisionObjects.begin();

    for(; it != collisionObjects.end(); it++)
    {
        num++;
    }

    return num;
}*/


/*u32 irrBulletWorld::getNumCollisionObjects(EActivationState state)
{
    u32 num = 0;

    list<ICollisionObjects*>::Iterator it = collisionObjects.begin();

    for(; it != collisionObjects.end(); it++)
    {
        const bool isActive = (*it)->getPointer()->isActive();
        switch(isActive)
        {
            case true:
            {
                if(state == EAS_ACTIVE)
                {
                    num++;
                }
                break;
            }

            case false:
            {
                if(state == EAS_SLEEPING)
                {
                    num++;
                }
                break;
            }

            default:
                break;
        }
    }

    return num;
}*/


ICollisionObject* irrBulletWorld::getCollisionObjectByIndex(irr::u32 index) const
{
    auto it = collisionObjects.begin();

    it += index;
    auto obj = (*it);
    if(obj)
        return obj;
    return 0;
}

ILiquidBody* irrBulletWorld::getLiquidBodyByIndex(irr::u32 index) const
{
    auto it = liquidBodies.begin();

    it += index;
    auto body = (*it);
    if(body)
        return body;
    return 0;
}

ILiquidBody* irrBulletWorld::getLiquidBodyByID(irr::u32 ID) const
{
	for (auto it : liquidBodies)
    {
        auto obj = it;
        if(obj->getUniqueID() == ID)
            return obj;

    }
    return 0;
}

ICollisionObject* irrBulletWorld::getCollisionObjectByID(irr::u32 ID) const
{
	for (auto it : collisionObjects)
    {
        auto obj = it;
        if(obj->getUniqueID() == ID)
            return obj;

    }
    return 0;
}

ICollisionObject* irrBulletWorld::getCollisionObjectByName(const irr::core::stringc& name) const
{
	for (auto it : collisionObjects)
    {
        auto obj = it;
        if(obj->getIdentification()->getName() == name)
            return obj;
    }

    return 0;
}

ICollisionCallbackInformation* irrBulletWorld::getCollisionCallback(irr::u32 index) const
{
    auto callback = new ICollisionCallbackInformation(dispatcher->getManifoldByIndexInternal(index), (irrBulletWorld*)this);
    return callback;
}


bool irrBulletWorld::isGImpactEnabled() const
{
    return gimpactEnabled;
}


irrBulletWorld::~irrBulletWorld()
{
    printf("-- irrBullet: Freeing memory --\n");

    // remove constraints
    /*for (u32 i = 0; i < world->getNumConstraints(); i++)
	{
		btTypedConstraint* constraint = world->getConstraint(i);
		if(constraint)
		{
		    printf("irrBullet: Removing constraint\n");
            world->removeConstraint(constraint);
            delete constraint;
		}
	}*/

	// remove liquid bodies
    auto wbit = liquidBodies.begin();

	for(; wbit != liquidBodies.end(); )
    {
        auto liquidBody = (*wbit);
        if(liquidBody)
        {
            delete liquidBody;
            liquidBody = 0;
        }

		wbit = liquidBodies.erase(wbit);
    }

    // remove the raycast vehicles
    auto rvit = raycastVehicles.begin();

    for(; rvit != raycastVehicles.end(); )
    {
        auto vehicle = (*rvit);
        if(vehicle)
        {
            getPointer()->removeVehicle(vehicle->getPointer());
            delete vehicle;
            vehicle = 0;
        }

		rvit = raycastVehicles.erase(rvit);
    }


    // Remove the collision objects
    /*list<ICollisionObject*>::Iterator rbit = collisionObjects.begin();

    for(; rbit != collisionObjects.end(); )
    {
        if(*rbit)
        {
            delete (*rbit);
            (*rbit) = 0;

            rbit = collisionObjects.erase(rbit);
        }
    }*/
    while(getNumCollisionObjects() > 0)
    {
        removeCollisionObject(getCollisionObjectByIndex(0));
    }

    if(propertyText)
        propertyText->remove();

    if(debug != 0)
        delete debug;

    delete constraintSolver;
    delete pairCache;
    delete dispatcher;
    delete collisionConfiguration;
    printf("-- irrBullet: Finished freeing memory --\n");
}
