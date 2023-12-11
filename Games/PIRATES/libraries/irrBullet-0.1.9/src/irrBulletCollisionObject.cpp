// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <ISceneManager.h>
#include <IFileSystem.h>
#include <IMeshSceneNode.h>
#include "irrBulletWorld.h"
#include "irrBulletCollisionObject.h"
#include "irrBulletCompileConfig.h"

using namespace irr;
using namespace core;

ICollisionObject::ICollisionObject()
{
    static u32 collisionObjectID;
    collisionObjectID++;

    uniqueID = collisionObjectID;

    attributes = 0;

    IncludeNodeOnRemoval = true;
    LiquidSimulationEnabled = true;
}

void ICollisionObject::updateObject()
{
    /*list<ICollisionObjectAffector*>::ConstIterator it = animators.begin();

    for(; it != animators.end(); it++)
    {
        (*it)->onAnimate();
    }*/
}

void ICollisionObject::setWorldTransform(const irr::core::matrix4& irrmat)
{
    btTransformFromIrrlichtMatrix(irrmat, internalTransform);

	if (getObjectType() == ECollisionObjectType::ECOT_RIGID_BODY)
        getPointer()->setWorldTransform(internalTransform);

	else if (getObjectType() == ECollisionObjectType::ECOT_SOFT_BODY)
        static_cast<btSoftBody*>(getPointer())->transform(internalTransform);
}

void ICollisionObject::setCcdValues(const f32 motionThreshold, const f32 radius)
{
    getPointer()->setCcdMotionThreshold(motionThreshold);
    getPointer()->setCcdSweptSphereRadius(radius);
}

/*void ICollisionObject::applyCalculatedCcdValues(irr::f32 motionThreshold)
{
    vector3df scale(1,1,1);
    if(getObjectType() == ECOT_RIGID_BODY)
        scale = static_cast<IRigidBody*>(this)->getCollisionShape()->getSceneNode()->getScale();

    else if(getObjectType() == ECOT_SOFT_BODY)
        scale = static_cast<ISoftBody*>(this)->getSceneNode()->getScale();

    f32 f = 0.0;

    if(scale.X > f)
        f = scale.X;
    else if(scale.Y > f)
        f = scale.Y;
    else if(scale.Z > f)
        f = scale.Z;
    getPointer()->setCcdMotionThreshold(motionThreshold);
	getPointer()->setCcdSweptSphereRadius(f * 0.5);
}*/


void ICollisionObject::setAnisotropicFriction(const vector3df &friction)
{
    getPointer()->setAnisotropicFriction(btVector3(friction.X,friction.Y,friction.Z));
}


void ICollisionObject::setContactProcessingThreshold(f32 cpt)
{
    getPointer()->setContactProcessingThreshold(cpt);
}


void ICollisionObject::setActivationState(EActivationState newState)
{
    getPointer()->setActivationState(static_cast<int>(newState));
}


void ICollisionObject::forceActivationState(EActivationState newState)
{
	getPointer()->forceActivationState(static_cast<int>(newState));
}


void ICollisionObject::activate(bool forceActivation)
{
    getPointer()->activate(forceActivation);
}


void ICollisionObject::setRestitution(f32 rest)
{
    getPointer()->setRestitution(rest);
}


void ICollisionObject::setInterpolationLinearVelocity(const vector3df &linvel)
{
    getPointer()->setInterpolationLinearVelocity(btVector3(linvel.X,linvel.Y,linvel.Z));
}


void ICollisionObject::setFriction(f32 friction)
{
    getPointer()->setFriction(friction);
}


void ICollisionObject::setInterpolationAngularVelocity(const vector3df &angvel)
{
    getPointer()->setInterpolationAngularVelocity(btVector3(angvel.X,angvel.Y,angvel.Z));
}


void ICollisionObject::setIslandTag(u32 tag)
{
    getPointer()->setIslandTag(tag);
}


void ICollisionObject::setCompanionId(u32 id)
{
    getPointer()->setCompanionId(id);
}


void ICollisionObject::setHitFraction(f32 hitFraction)
{
    getPointer()->setHitFraction(hitFraction);
}


void ICollisionObject::setCollisionFlags(ECollisionFlag flags)
{
    getPointer()->setCollisionFlags(static_cast<int>(flags));
}

void ICollisionObject::addAffector(ICollisionObjectAffector* const affector)
{
    affectors.push_back(affector);
}

void ICollisionObject::removeAffector(ICollisionObjectAffector* const affector)
{
    // Remove the collision object
    if(affector)
    {
        for(auto& it : std::move(affectors) )
        {
            if(it == affector)
            {
                #ifdef IRRBULLET_DEBUG_MODE
                    printf("irrBullet: Removing animator (BODY: %i)\n", this->getUniqueID());
                #endif
                delete it;
                it = 0;

                it = *affectors.erase(affectors.begin(), affectors.end());
            }

            else
                it++;
        }
    }
}

void ICollisionObject::removeAffectors()
{
    for(auto it :  affectors)
    {
        #ifdef IRRBULLET_DEBUG_MODE
            printf("irrBullet: Removing animator (BODY: %i)\n", this->getUniqueID());
        #endif
        delete it;
        it = 0;

        it = *affectors.erase(affectors.begin(), affectors.end());
    }
}

void ICollisionObject::remove()
{
    dynamicsWorld->addToDeletionQueue(this);
}



btCollisionObject* ICollisionObject::getPointer() const
{
    return object;
}


bool ICollisionObject::mergesSimulationIslands() const
{
    return getPointer()->mergesSimulationIslands();
}


const vector3df ICollisionObject::getAnisotropicFriction() const
{
    const btVector3 friction = getPointer()->getAnisotropicFriction();

    return vector3df(friction.getX(), friction.getY(), friction.getZ());
}


bool ICollisionObject::hasAnisotropicFriction() const
{
    return getPointer()->hasAnisotropicFriction();
}


f32 ICollisionObject::getContactProcessingThreshold() const
{
    return f32(getPointer()->getContactProcessingThreshold());
}


bool ICollisionObject::hasContactResponse() const
{
    return getPointer()->hasContactResponse();
}


EActivationState ICollisionObject::getActivationState() const
{
	EActivationState state = EActivationState::EAS_SLEEPING;
    switch(getPointer()->getActivationState())
    {
        case 1:
			state = EActivationState::EAS_ACTIVE;
            break;
        case 2:
			state = EActivationState::EAS_SLEEPING;
            break;
        case 3:
			state = EActivationState::EAS_WANTS_DEACTIVATION;
            break;
        case 4:
			state = EActivationState::EAS_DISABLE_DEACTIVATION;
            break;
        case 5:
			state = EActivationState::EAS_DISABLE_SIMULATION;
    }

    return state;
}


bool ICollisionObject::isActive() const
{
    return getPointer()->isActive();
}


irr::f32 ICollisionObject::getRestitution() const
{
    return f32(getPointer()->getRestitution());
}


irr::f32 ICollisionObject::getFriction() const
{
    return f32(getPointer()->getFriction());
}


const vector3df ICollisionObject::getInterpolationLinearVelocity() const
{
    const btVector3 vel = getPointer()->getInterpolationLinearVelocity();

    return vector3df(vel.getX(),vel.getY(),vel.getZ());
}


const vector3df ICollisionObject::getInterpolationAngularVelocity() const
{
    const btVector3 vel = getPointer()->getInterpolationAngularVelocity();

    return vector3df(vel.getX(),vel.getY(),vel.getZ());
}


u32 ICollisionObject::getIslandTag() const
{
    return u32(getPointer()->getIslandTag());
}


u32 ICollisionObject::getCompanionId() const
{
    return u32(getPointer()->getCompanionId());
}


f32 ICollisionObject::getHitFraction() const
{
    return f32(getPointer()->getHitFraction());
}


ECollisionFlag ICollisionObject::getCollisionFlags() const
{
    return ECollisionFlag(getPointer()->getCollisionFlags());
}

bool ICollisionObject::checkCollideWith(ICollisionObject* const collObj) const
{
    return getPointer()->checkCollideWith(collObj->getPointer());
}

irr::u32 ICollisionObject::getNumAffectors() const
{
    u32 num = 0;

    for(auto it : affectors)
    {
        num++;
    }

    return num;
}

ICollisionObjectAffector* ICollisionObject::getAffector(irr::u32 index)
{
    auto it = affectors.begin();

    it += index;
    ICollisionObjectAffector* affector = (*it);
    if(affector)
        return affector;
    return 0;
}

/*bool ICollisionObject::hasCollidedWithAttribute(irr::c8 *attributeName) const
{
    for(irr::u32 i=0; i < dynamicsWorld->getNumManifolds(); i++)
    {
        btPersistentManifold* manifold = dynamicsWorld->getPointer()->getDispatcher()->getManifoldByIndexInternal(i);
        void* pointer = static_cast<btCollisionObject*>(manifold->getBody0())->getUserPointer();

        bool whichOne = (getUniqueID() == static_cast<SCollisionObjectIdentification*>(pointer)->ID);

        ICollisionObject* other = (!whichOne) ?
        dynamicsWorld->getCollisionObjectByID(
            static_cast<SCollisionObjectIdentification*>(static_cast<btCollisionObject*>(manifold->getBody0())->getUserPointer())->ID) :
        dynamicsWorld->getCollisionObjectByID(
            static_cast<SCollisionObjectIdentification*>(static_cast<btCollisionObject*>(manifold->getBody1())->getUserPointer())->ID);



        if(other->getAttributes()->existsAttribute(attributeName))
            return true;
    }

    return false;
}*/

ICollisionObject::~ICollisionObject()
{
    #ifdef IRRBULLET_DEBUG_MODE
        switch(this->getObjectType())
        {
		case ECollisionObjectType::ECOT_RIGID_BODY:
                printf("irrBullet: Deleting rigid body (%i)\n", this->getUniqueID());
                break;
		case ECollisionObjectType::ECOT_SOFT_BODY:
                printf("irrBullet: Deleting soft body (%i)\n", this->getUniqueID());
                break;
            default:
                break;
        }
    #endif
    removeAffectors();

    if(object)
        delete object;

    if(attributes)
        attributes->drop();

    if(collID)
        delete collID;
}
