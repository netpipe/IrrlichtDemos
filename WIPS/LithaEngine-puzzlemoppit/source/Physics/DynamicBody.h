
#ifndef DYNAMIC_BODY_H
#define DYNAMIC_BODY_H

#include "IDynamicBody.h"
#include <ode/ode.h>
#include <vector>

class DynamicBody : public IDynamicBody
{
	std::vector<ICollisionGeometry *> geometry;
	
	dBodyID body;
	
	void FillODEMassFromGeometry(dMass *dmass, f32 density, ICollisionGeometry *geom);
	
public:
	DynamicBody(dWorldID world);
	~DynamicBody();
	
	void AddCollisionGeometry(ICollisionGeometry *geom);
	Set<ICollisionGeometry *> GetCollisionGeometries();
	
	void SetPosition(const core::vector3df &pos);
	void SetRotation(const core::vector3df &rot);
	
	core::vector3df GetPosition();
	core::vector3df GetRotation();
	
	void SetMass(f32 density, ICollisionGeometry *geom);
	void AddMass(f32 density, ICollisionGeometry *geom);
	
	void SetGravityEnabled(bool enabled);
	
	void SetLinearDamping(f32 amount);
	void SetAngularDamping(f32 amount);
	
	void ApplyForce(const core::vector3df &force);
	void ApplyTorque(const core::vector3df &torque);
	
	void SetLinearVelocity(const core::vector3df &vel);
	void SetAngularVelocity(const core::vector3df &vel);
	
	core::vector3df GetLinearVelocity();
	
	void ZeroLinearVelocity(s32 axes);
	void ZeroAngularVelocity();
	
	void AddThruster(IThruster *thruster);
};

#endif

