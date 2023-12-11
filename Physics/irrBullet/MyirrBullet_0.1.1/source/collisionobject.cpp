#include "collisionobject.h"

using namespace irr;
using namespace core;

ICollisionObject::ICollisionObject()
{
}


//////////////////////////
// The 'dos' and 'sets' //
//////////////////////////
void ICollisionObject::setCollisionShape(ICollisionShape* const Shape)
{
    shape = Shape;
}


void ICollisionObject::setCcdValues(const f64 mt, const f64 r)
{
    getPointer()->setCcdMotionThreshold(mt);
    getPointer()->setCcdSweptSphereRadius(r);
}

void ICollisionObject::autoCalculateCcdValues()
{
    const vector3df scale = getCollisionShape()->getNode()->getScale();
    f64 f = 0.0;

    if(scale.X > f)
        f = scale.X;
    else if(scale.Y > f)
        f = scale.Y;
    else if(scale.Z > f)
        f = scale.Z;
    getPointer()->setCcdMotionThreshold(0.8);
	getPointer()->setCcdSweptSphereRadius(f * 0.5);
}


void ICollisionObject::setAnisotropicFriction(const vector3df &friction)
{
    getPointer()->setAnisotropicFriction(btVector3(friction.X,friction.Y,friction.Z));
}


void ICollisionObject::setContactProcessingThreshold(f64 cpt)
{
    getPointer()->setContactProcessingThreshold(cpt);
}


void ICollisionObject::setActivationState(EActivationState newState)
{
    getPointer()->setActivationState(newState);
}


void ICollisionObject::forceActivationState(EActivationState newState)
{
    getPointer()->forceActivationState(newState);
}


void ICollisionObject::activate(bool forceActivation)
{
    getPointer()->activate(forceActivation);
}


void ICollisionObject::setRestitution(f64 rest)
{
    getPointer()->setRestitution(rest);
}


void ICollisionObject::setInterpolationLinearVelocity(const vector3df &linvel)
{
    getPointer()->setInterpolationLinearVelocity(btVector3(linvel.X,linvel.Y,linvel.Z));
}


void ICollisionObject::setFriction(f64 frict)
{
    getPointer()->setFriction(frict);
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


void ICollisionObject::setHitFraction(f64 hitFraction)
{
    getPointer()->setHitFraction(hitFraction);
}


void ICollisionObject::setCollisionFlags(ECollisionFlag flags)
{
    getPointer()->setCollisionFlags(flags);
}



////////////////
// The 'gets' //
////////////////
btCollisionObject *ICollisionObject::getPointer() const
{
    return object;
}

ICollisionShape *ICollisionObject::getCollisionShape() const
{
    return shape;
}


bool ICollisionObject::mergesSimulationIslands() const
{
    return getPointer()->mergesSimulationIslands();
}


vector3df ICollisionObject::getAnisotropicFriction() const
{
    const btVector3 friction = getPointer()->getAnisotropicFriction();

    return vector3df(friction.getX(), friction.getY(), friction.getZ());
}


bool ICollisionObject::hasAnisotropicFriction() const
{
    return getPointer()->hasAnisotropicFriction();
}


f64 ICollisionObject::getContactProcessingThreshold() const
{
    return f64(getPointer()->getContactProcessingThreshold());
}


bool ICollisionObject::hasContactResponse() const
{
    return getPointer()->hasContactResponse();
}


EActivationState ICollisionObject::getActivationState() const
{
    EActivationState state = EAS_SLEEPING; // Default value

    if(getPointer()->getActivationState() == 0)
        state = EAS_SLEEPING;

    else if(getPointer()->getActivationState() == 1)
        state = EAS_ACTIVE;

    return state;
}


bool ICollisionObject::isActive() const
{
    return getPointer()->isActive();
}


irr::f64 ICollisionObject::getRestitution() const
{
    return f64(getPointer()->getRestitution());
}


irr::f64 ICollisionObject::getFriction() const
{
    return f64(getPointer()->getFriction());
}


vector3df ICollisionObject::getInterpolationLinearVelocity() const
{
    const btVector3 vel = getPointer()->getInterpolationLinearVelocity();

    return vector3df(vel.getX(),vel.getY(),vel.getZ());
}


vector3df ICollisionObject::getInterpolationAngularVelocity() const
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


f64 ICollisionObject::getHitFraction() const
{
    return f64(getPointer()->getHitFraction());
}


u32 ICollisionObject::getCollisionFlags() const
{
    return u32(getPointer()->getCollisionFlags());
}


ICollisionObject::~ICollisionObject()
{
    if(shape)
        delete shape;

    if(object)
        delete object;
}
