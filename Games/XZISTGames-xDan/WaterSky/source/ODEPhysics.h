
#ifndef __ODEPhysics_h
#define __ODEPhysics_h

#include <irrlicht.h>
#include <ode/ode.h>

using namespace irr;

class ODEAnimator;

class ODEPhysics
{
public:
    ODEPhysics();
    ~ODEPhysics();
    
    void updatePhysics(f32); // time in seconds
    
    // util methods
    void setRotation(core::CMatrix4<f32>, dMatrix3);
    void setRotation(const dMatrix3, core::CMatrix4<f32> &);
    
protected:
    friend class ODEAnimator;
    friend class ODEMeshAnimator;
    friend class ODESphereAnimator;
    friend class ODEBoxAnimator;
    
    dWorldID dWorld;
    dSpaceID dMainSpace;
    dJointGroupID dPerStepContactJointGroup;
    
private:
    f32 accumulator;
    core::array <ODEAnimator *> dAnimators;
    
    static void dNearCallback(void *, dGeomID, dGeomID);
    void geomCollide(dGeomID, dGeomID);
};

#endif
