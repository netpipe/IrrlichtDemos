
#include "DynamicBody.h"
#include "ICollisionGeometry.h"
#include "CollisionGeometry.h"
#include "ode_utility.h"

// All collision geometries should be included here
// (and an appropriate case implemented just below in FillODEMassFromGeometry)
#include "BoxCollisionGeometry.h"
#include "MeshCollisionGeometry.h"
#include "SphereCollisionGeometry.h"

void DynamicBody::FillODEMassFromGeometry(dMass *dmass, f32 density, ICollisionGeometry *geom)
{
	if (BoxCollisionGeometry *boxGeom = dynamic_cast<BoxCollisionGeometry *>(geom))
	{
		const core::vector3df &size = boxGeom->GetSize();
		
		dMassSetBox(dmass, density, size.X,size.Y,size.Z);
	}
	else if (MeshCollisionGeometry *meshGeom = dynamic_cast<MeshCollisionGeometry *>(geom))
	{
		dMassSetTrimesh(dmass, density, meshGeom->GetODEGeom());
	}
	else if (SphereCollisionGeometry *sphereGeom = dynamic_cast<SphereCollisionGeometry *>(geom))
	{
		dMassSetSphere(dmass, density, sphereGeom->GetRadius());
	}
	else
	{
		WARN << "Only built in collision geometries may be used. Will default to a unit sphere.";
		dMassSetSphere(dmass, 1.0, 1.0);
	}
}

DynamicBody::DynamicBody(dWorldID world)
{
	body = dBodyCreate(world);
	
	// Default mass
	dMass dmass;
	dMassSetSphere(&dmass, 1.0, 1.0);
	dBodySetMass(body, &dmass);
	
	SetGravityEnabled(true);
}

DynamicBody::~DynamicBody()
{
	dBodyDestroy(body);
	
	for (u32 i = 0; i < geometry.size(); i ++)
		geometry[i]->drop();
}

void DynamicBody::AddCollisionGeometry(ICollisionGeometry *geom)
{
	geom->grab();
	geometry.push_back(geom);
	
	// attach its dGeomID to this dBodyID
	if (CollisionGeometry *hasGeom = dynamic_cast<CollisionGeometry *>(geom))
	{
		dGeomSetBody(hasGeom->GetODEGeom(), body);
	}
	else
		FAIL << "A collision geometry could not be dynamically cast to CollisionGeometry.";
}

Set<ICollisionGeometry *> DynamicBody::GetCollisionGeometries()
{
	Set<ICollisionGeometry *> geometries;
	
	for (u32 i = 0; i < geometry.size(); i ++)
		geometries.Insert(geometry[i]);
	
	return geometries;
}

void DynamicBody::SetPosition(const core::vector3df &pos)
{
	ODESetBodyPosition(body, pos);
}

void DynamicBody::SetRotation(const core::vector3df &rot)
{
	ODESetBodyRotation(body, rot);
}

core::vector3df DynamicBody::GetPosition()
{
	return ODEGetBodyPosition(body);
}

core::vector3df DynamicBody::GetRotation()
{
	return ODEGetBodyRotation(body);
}

void DynamicBody::SetMass(f32 density, ICollisionGeometry *geom)
{
	dMass dmass;
	FillODEMassFromGeometry(&dmass, density, geom);
	
	dBodySetMass(body, &dmass);
}

void DynamicBody::AddMass(f32 density, ICollisionGeometry *geom)
{
	dMass dmass;
	FillODEMassFromGeometry(&dmass, density, geom);
	
	dMass dmassOld;
	dBodyGetMass(body, &dmassOld);
	
	dMassAdd(&dmassOld, &dmass);
	dBodySetMass(body, &dmassOld);
}

void DynamicBody::SetGravityEnabled(bool enabled)
{
	dBodySetGravityMode(body, enabled);
}

void DynamicBody::SetLinearDamping(f32 amount)
{
	dBodySetLinearDamping(body, amount);
	dBodySetLinearDampingThreshold(body, 0.0);
}

void DynamicBody::SetAngularDamping(f32 amount)
{
	dBodySetAngularDamping(body, amount);
	dBodySetAngularDampingThreshold(body, 0.0);
}

void DynamicBody::ApplyForce(const core::vector3df &force)
{
	if (force.getLengthSQ() == 0.0)
		return;
	
	dBodyEnable(body);
	dBodyAddForce(body, force.X,force.Y,force.Z);
}

void DynamicBody::ApplyTorque(const core::vector3df &torque)
{
	if (torque.getLengthSQ() == 0.0)
		return;
	
	dBodyEnable(body);
	
	// Z is negated. It seems to help. Want +ve Z rotation to roll in the +ve X direction.
	dBodyAddTorque(body, torque.X,torque.Y,-torque.Z);
}

void DynamicBody::SetLinearVelocity(const core::vector3df &vel)
{
	dBodySetLinearVel(body, vel.X,vel.Y,vel.Z);
}

void DynamicBody::SetAngularVelocity(const core::vector3df &vel)
{
	// See note in IDynamicBody for why this is not yet implemented.
	FAIL << "Not yet implemented.";
}

core::vector3df DynamicBody::GetLinearVelocity()
{
	const dReal *vel = dBodyGetLinearVel(body);
	return core::vector3df(vel[0],vel[1],vel[2]);
}

void DynamicBody::ZeroLinearVelocity(s32 axes)
{
	if (axes == ALL_AXES)
		dBodySetLinearVel(body, 0,0,0);
	else
	{
		const dReal *vel = dBodyGetLinearVel(body);
		
		dBodySetLinearVel(body,
				axes & AXIS_X ? 0 : vel[0],
				axes & AXIS_Y ? 0 : vel[1],
				axes & AXIS_Z ? 0 : vel[2]
				);
	}
}

void DynamicBody::ZeroAngularVelocity()
{
	dBodySetAngularVel(body, 0,0,0);
}

void DynamicBody::AddThruster(IThruster *thruster)
{
	//nothing yet
}

