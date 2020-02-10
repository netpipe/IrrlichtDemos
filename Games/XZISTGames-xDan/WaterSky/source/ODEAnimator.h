
#ifndef __ODEAnimator_h
#define __ODEAnimator_h

#include "ODEPhysics.h"
#include "ISceneNode.h"

enum
{
    ODEANIM_MESH_COMBINE        = 1 << 0,
    ODEANIM_CREATE_BODIES       = 1 << 1,
    ODEANIM_CENTRE_NODE         = 1 << 2,   // centre irrlicht scene node to ode body
    ODEANIM_NO_ROTATION         = 1 << 3,
    ODEANIM_NO_ROTATIONXZ       = 1 << 4    // Y rotation *is* taken from ODE
};

class ODEPhysics;

class ODEAnimator : public scene::ISceneNodeAnimator
{
public:
    ODEAnimator(scene::ISceneNode *, ODEPhysics *, u32);
    ~ODEAnimator();
    void animateNode(scene::ISceneNode *, u32);
    void takeTransformation(scene::ISceneNode *); // calls updateAbsolutePosition() first
    void putTransformation(scene::ISceneNode *);
    
    //core::array <dGeomID> *getGeoms() { return &dGeoms; }
    //core::array <dBodyID> *getBodies() { return &dBodies; }
    
    void endUpdate();
    void addForce(core::vector3df);
    void addTorque(core::vector3df);
    void addTorqueDamping(f32);
    
    void setLinearVel(core::vector3df);
    void setAngularVel(core::vector3df);
    
    // I like to use these methods for example to keep a body stationary
    // (e.g. a moving platform at rest... when its not moving)
    // Values passed here are set before each step (multiple times per updatePhysics)
    // If you just wanted to set initial conditions of a body use setLinear/AngularVel;
    void maintainLinearVel(core::vector3df);
    void maintainAngularVel(core::vector3df);
    
protected:
    ODEPhysics *dPhysics;
    u32 creationFlags;
    core::array <dGeomID> dGeoms;
    core::array <dBodyID> dBodies;
    
    virtual void clearAll();
    
private:
    u32 lastTimeMs;
    
    // Would need some custom structures for things like addForceRel...
    core::vector3df force;
    core::vector3df torque;
    f32 torqueDamping;
    
    core::vector3df linearVel;
    core::vector3df angularVel;
    bool linearVelSet;
    bool angularVelSet;
	
	friend class ODEPhysics;
	
	// methods
	void preStep();
};

#endif
