
#ifndef I_DYNAMIC_BODY_H
#define I_DYNAMIC_BODY_H

#include "IBody.h"

class IThruster;

class IDynamicBody : public IBody
{
public:
	virtual ~IDynamicBody(){}
	
	// Sets mass using dimensions automatically determined from some collision geometry
	virtual void SetMass(f32 density, ICollisionGeometry *geom) = 0;
	// Add to the existing mass (without replacing it)
	virtual void AddMass(f32 density, ICollisionGeometry *geom) = 0;
	
	// Whether the world's gravity should affect this body.
	// Default is true.
	virtual void SetGravityEnabled(bool enabled) = 0;
	
	virtual void SetLinearDamping(f32 amount) = 0;
	virtual void SetAngularDamping(f32 amount) = 0;
	
	virtual void ApplyForce(const core::vector3df &force) = 0;
	virtual void ApplyTorque(const core::vector3df &torque) = 0;
	
	virtual void SetLinearVelocity(const core::vector3df &vel) = 0;
	
	// Not implemented yet. I probably want to convert from Irrlicht euler angles to
	// the axis-magnitude representation used by ODE.
	//virtual void SetAngularVelocity(const core::vector3df &vel) = 0;
	
	virtual core::vector3df GetLinearVelocity() = 0;
	
	// Make stationary.
	// axes: the axes to zero. Default is all of them.
	virtual void ZeroLinearVelocity(s32 axes = ALL_AXES) = 0;
	virtual void ZeroAngularVelocity() = 0;
	
	virtual void AddThruster(IThruster *thruster) = 0;
};

#endif
