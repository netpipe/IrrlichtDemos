#include "rigidbody.h"
#include "collisionshape.h"
#include "motionstate.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IRigidBody::IRigidBody(ICollisionShape *s)
{
    shape = s;
    node = shape->getNode();

    const vector3df pos = node->getPosition();


    // Set the initial position of the object
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(pos.X,pos.Y,pos.Z));

	IMotionState *motionState = new IMotionState(transform);
	motionState->setNode(node);

    vector3df in = shape->getLocalInertia();


    object = new btRigidBody(shape->getMass(), motionState, shape->getPointer(),
        btVector3(in.X,in.Y,in.Z));
}


IRigidBody::IRigidBody(SRigidBodyConstructionInfo info)
{
    shape = info.collisionShape;
    node = shape->getNode();

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(info.startVector.X,info.startVector.Y,info.startVector.Z));

    btDefaultMotionState *motionState = new btDefaultMotionState(transform);
    motionState->setWorldTransform(transform);

    vector3df in = shape->getLocalInertia();
    object = new btRigidBody(shape->getMass(), motionState, shape->getPointer(),
        btVector3(in.X,in.Y,in.Z));

    getPointer()->setDamping(info.linearDamping, info.angularDamping);

}


//////////////////////////
// The 'dos' and 'sets' //
//////////////////////////
void IRigidBody::translate(const vector3df &v)
{
    getPointer()->translate(btVector3(v.X,v.Y,v.Z));
}


void IRigidBody::getAabb(vector3df &aabbMin, vector3df &aabbMax) const
{
    btVector3 aabbMinBt, aabbMaxBt;
    getPointer()->getAabb(aabbMinBt, aabbMaxBt);

    aabbMin = vector3df(aabbMinBt.getX(),aabbMinBt.getY(),aabbMinBt.getZ());
    aabbMax = vector3df(aabbMaxBt.getX(),aabbMaxBt.getY(),aabbMaxBt.getZ());
}


void IRigidBody::updateDeactivation(f64 timeStep)
{
    getPointer()->updateDeactivation(timeStep);
}


void IRigidBody::setLinearVelocity(const vector3df &linVel)
{
    getPointer()->setLinearVelocity(btVector3(linVel.X,linVel.Y,linVel.Z));
}


void IRigidBody::setAngularVelocity(const vector3df &angVel)
{
    getPointer()->setAngularVelocity(btVector3(angVel.X,angVel.Y,angVel.Z));
}


void IRigidBody::saveKinematicState(f64 step)
{
    getPointer()->saveKinematicState(step);
}


void IRigidBody::applyCentralForce(const vector3df &force)
{
    getPointer()->applyCentralForce(btVector3(force.X,force.Y,force.Z));
}


void IRigidBody::applyForce(const vector3df &force, const vector3df &relPos)
{
    getPointer()->applyForce(btVector3(force.X,force.Y,force.Z), btVector3(relPos.X,relPos.Y,relPos.Z));
}


void IRigidBody::applyCentralImpulse(const vector3df &impulse)
{
    getPointer()->applyCentralImpulse(btVector3(impulse.X,impulse.Y,impulse.Z));
}


void IRigidBody::applyImpulse(const vector3df &impulse, const vector3df &relPos)
{
    getPointer()->applyImpulse(btVector3(impulse.X,impulse.Y,impulse.Z), btVector3(relPos.X,relPos.Y,relPos.Z));
}


void IRigidBody::applyTorque(const vector3df &torque)
{
    getPointer()->applyTorque(btVector3(torque.X,torque.Y,torque.Z));
}

void IRigidBody::applyTorqueImpulse(const vector3df &torque)
{
    getPointer()->applyTorqueImpulse(btVector3(torque.X,torque.Y,torque.Z));
}


void IRigidBody::internalApplyImpulse(const vector3df &linearComponent,
    const vector3df &angularComponent, f64 impulseMagnitude)
{
    getPointer()->internalApplyImpulse(btVector3(linearComponent.X,linearComponent.Y,linearComponent.Z),
        btVector3(angularComponent.X,angularComponent.Y,angularComponent.Z), impulseMagnitude);
}


void IRigidBody::clearForces()
{
    getPointer()->clearForces();
}


void IRigidBody::updateInertiaTensor()
{
    getPointer()->updateInertiaTensor();
}


void IRigidBody::setDamping(const f64 lin_damping, const f64 ang_damping)
{
    getPointer()->setDamping((btScalar)lin_damping, (btScalar)ang_damping);
}


void IRigidBody::applyGravity()
{
    getPointer()->applyGravity();
}


void IRigidBody::setGravity(const vector3df &grav)
{
    getPointer()->setGravity(btVector3(grav.X,grav.Y,grav.Z));
}


void IRigidBody::applyDamping(f32 timeStep)
{
    getPointer()->applyDamping(timeStep);
}


void IRigidBody::setMassProps(f64 mass, const vector3df &inertia)
{
    getPointer()->setMassProps(mass, btVector3(inertia.X,inertia.Y,inertia.Z));
}


void IRigidBody::setLinearFactor(const vector3df &linearFactor)
{
    getPointer()->setLinearFactor(btVector3(linearFactor.X,linearFactor.Y,linearFactor.Z));
}


void IRigidBody::integrateVelocities(f64 step)
{
    getPointer()->integrateVelocities(step);
}


void IRigidBody::setInvInertiaDiagLocal(const vector3df &diagInvInertia)
{
    getPointer()->setInvInertiaDiagLocal(btVector3(diagInvInertia.X,diagInvInertia.Y,diagInvInertia.Z));
}


void IRigidBody::setSleepingThresholds(f64 linear, f64 angular)
{
    getPointer()->setSleepingThresholds(linear, angular);
}


/*
void IRigidBody::setCenterOfMassTransform(const ITransform &xform)
{
    // TODO: Actually do something here once the ITransform class is finished.
}*/


void IRigidBody::setAngularFactor(const vector3df &angFac)
{
    getPointer()->setAngularFactor(btVector3(angFac.X,angFac.Y,angFac.Z));
}


void IRigidBody::setAngularFactor(f64 angFac)
{
    getPointer()->setAngularFactor(angFac);
}



////////////////
// The 'gets' //
////////////////
btRigidBody *IRigidBody::getPointer() const
{
    return static_cast<btRigidBody*>(object);
}


vector3df IRigidBody::getCenterOfMassPosition() const
{
    const btVector3 pos = getPointer()->getCenterOfMassPosition();

    return vector3df(pos.getX(),pos.getY(),pos.getZ());
}


/*
vector3df IRigidBody::getOrientation() const
{
    // TODO: Actually return an orientation here.
}*/

/*
ITransform IRigidBody::getCenterOfMassTransform() const
{
    // TODO: Return a transform once the ITransform class is finished.
}*/


vector3df IRigidBody::getLinearVelocity() const
{
    const btVector3 linVec = getPointer()->getLinearVelocity();

    return vector3df(linVec.getX(),linVec.getY(),linVec.getX());
}


vector3df IRigidBody::getAngularVelocity() const
{
    const btVector3 angVec = getPointer()->getAngularVelocity();

    return vector3df(angVec.getX(),angVec.getY(),angVec.getZ());
}


vector3df IRigidBody::getVelocityInLocalPoint(const vector3df &relPos) const
{
    const btVector3 locVel = getPointer()->getVelocityInLocalPoint(btVector3(relPos.X,relPos.Y,relPos.Z));

    return vector3df(locVel.getX(),locVel.getY(),locVel.getZ());
}


f64 IRigidBody::computeImpulseDenominator(const vector3df &pos, const vector3df &normal) const
{
    return f64(getPointer()->computeImpulseDenominator(
        btVector3(pos.X,pos.Y,pos.Z), btVector3(normal.X,normal.Y,normal.Z)));
}


f64 IRigidBody::computeAngularImpulseDenominator(const vector3df &axis) const
{
    return f64(getPointer()->computeAngularImpulseDenominator(btVector3(axis.X,axis.Y,axis.Z)));
}


bool IRigidBody::wantsSleeping()
{
    return getPointer()->wantsSleeping();
}


vector3df IRigidBody::getTotalForce() const
{
    const btVector3 force = getPointer()->getTotalForce();

    return vector3df(force.getX(),force.getY(),force.getZ());
}

vector3df IRigidBody::getTotalTorque()
{
    const btVector3 torque = getPointer()->getTotalTorque();

    return vector3df(torque.getX(),torque.getY(),torque.getZ());
}


vector3df IRigidBody::getGravity() const
{
    btVector3 grav = static_cast<btRigidBody*>(object)->getGravity();

    return vector3df(grav.getX(), grav.getY(), grav.getZ());
}


f64 IRigidBody::getLinearDamping() const
{
    return f64(getPointer()->getLinearDamping());
}


f64 IRigidBody::getAngularDamping() const
{
    return f64(getPointer()->getAngularDamping());
}


f64 IRigidBody::getLinearSleepingThreshold() const
{
    return f64(getPointer()->getLinearSleepingThreshold());
}


f64 IRigidBody::getAngularSleepingThreshold() const
{
    return f64(getPointer()->getAngularSleepingThreshold());
}


vector3df IRigidBody::getLinearFactor() const
{
    const btVector3 linearFactor = getPointer()->getLinearFactor();

    return vector3df(linearFactor.getX(),linearFactor.getY(),linearFactor.getZ());
}


f64 IRigidBody::getInvMass() const
{
    return f64(getPointer()->getInvMass());
}


vector3df IRigidBody::getInvInertiaDiagLocal() const
{
    const btVector3 diagInvInertia = getPointer()->getInvInertiaDiagLocal();

    return vector3df(diagInvInertia.getX(),diagInvInertia.getY(),diagInvInertia.getZ());
}


vector3df IRigidBody::getAngularFactor() const
{
    const btVector3 angFac = getPointer()->getAngularFactor();

    return vector3df(angFac.getX(),angFac.getY(),angFac.getZ());
}


bool IRigidBody::isInWorld() const
{
    return getPointer()->isInWorld();
}


bool IRigidBody::checkCollideWithOverride(ICollisionObject *co)
{
    return getPointer()->checkCollideWithOverride(co->getPointer());
}


u32 IRigidBody::getNumConstraintRefs() const
{
    return u32(getPointer()->getNumConstraintRefs());
}


IRigidBody::~IRigidBody()
{
}


