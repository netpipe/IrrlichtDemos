#ifndef __RIGID_BODY_H_INCLUDED__
#define __RIGID_BODY_H_INCLUDED__

#include "common.h"
#include "collisionshape.h"
#include "collisionobject.h"


struct SRigidBodyConstructionInfo
{
    irr::core::vector3df startVector;
    ICollisionShape *collisionShape;
    irr::f64 angularDamping;
    irr::f64 linearDamping;
};


class IRigidBody : public ICollisionObject
{
public:
    IRigidBody(ICollisionShape *s);
    IRigidBody(SRigidBodyConstructionInfo info);
    ~IRigidBody();

    //////////////////////////
    // The 'dos' and 'sets' //
    //////////////////////////

    void translate(const irr::core::vector3df &v);

    void updateDeactivation(irr::f64 timeStep);

    bool wantsSleeping();

    void getAabb(irr::core::vector3df &aabbMin, irr::core::vector3df &aabbMax) const;

    void setLinearVelocity(const irr::core::vector3df &linVel);

    void setAngularVelocity(const irr::core::vector3df &angVel);

    void saveKinematicState(irr::f64 step);

    void applyCentralForce(const irr::core::vector3df &force);

    void applyForce(const irr::core::vector3df &force, const irr::core::vector3df &relPos);

    void applyCentralImpulse(const irr::core::vector3df &impulse);

    void applyImpulse(const irr::core::vector3df &impulse, const irr::core::vector3df &relPos);

    void applyTorque(const irr::core::vector3df &torque);

    void applyTorqueImpulse(const irr::core::vector3df &torque);

    void internalApplyImpulse(const irr::core::vector3df &linearComponent,
        const irr::core::vector3df &angularComponent, irr::f64 impulseMagnitude);

    void clearForces();

    void updateInertiaTensor();

    void setDamping(const irr::f64 lin_damping, const irr::f64 ang_damping);

    void applyGravity();

    void setGravity(const irr::core::vector3df &grav);

    void applyDamping(irr::f32 timeStep);

    void setMassProps(irr::f64 mass, const irr::core::vector3df &inertia);

    void setLinearFactor(const irr::core::vector3df &linearFactor);

    void setInvInertiaDiagLocal(const irr::core::vector3df &diagInvInertia);

    void setSleepingThresholds(irr::f64 linear, irr::f64 angular);

    void setAngularFactor(const irr::core::vector3df &angFac);

    void setAngularFactor(irr::f64 angFac);




    ////////////////
    // The 'gets' //
    ////////////////
    btRigidBody *getPointer() const;

    irr::core::vector3df getCenterOfMassPosition() const;

    irr::core::vector3df getLinearVelocity() const;

    irr::core::vector3df getAngularVelocity() const;

    irr::core::vector3df getVelocityInLocalPoint(const irr::core::vector3df &relPos) const;

    irr::f64 computeImpulseDenominator(const irr::core::vector3df &pos, const irr::core::vector3df &normal) const;

    irr::f64 computeAngularImpulseDenominator(const irr::core::vector3df &axis) const;

    irr::core::vector3df getGravity() const;

    irr::f64 getLinearDamping() const;

    irr::f64 getAngularDamping() const;

    irr::f64 getLinearSleepingThreshold() const;

    irr::f64 getAngularSleepingThreshold() const;

    irr::core::vector3df getLinearFactor() const;

    irr::f64 getInvMass() const;

    void integrateVelocities(irr::f64 step);

    irr::core::vector3df getTotalForce() const;

    irr::core::vector3df getTotalTorque();

    irr::core::vector3df getInvInertiaDiagLocal() const;

    irr::core::vector3df getAngularFactor() const;

    bool isInWorld() const;

    bool checkCollideWithOverride(ICollisionObject *co);

    irr::u32 getNumConstraintRefs() const;
};




#endif // __RIGID_BODY_H_INCLUDED__
