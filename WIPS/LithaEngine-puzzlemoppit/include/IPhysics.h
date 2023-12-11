
#ifndef I_PHYSICS_H
#define I_PHYSICS_H

#include "litha_internal.h"
#include "collision_structs.h"
#include <vector>

class ICollisionGeometry;
class IMeshCollisionGeometry;
class IBoxCollisionGeometry;
class ISphereCollisionGeometry;
class IMesh;
class IStaticBody;
class IDynamicBody;
class ICollisionMaterial;
class CollisionMaterialInteraction;

// Stuff that is commented out will (*may*) be implemented later...
class IPhysics : public virtual IReferenceCounted
{
public:
	virtual ~IPhysics(){}
	
	// The value passed as dt is always the same (timestep is fixed)
	// This is used internally by the engine.
	virtual void Step(f32 dt) = 0;
	
	// Set the maximum number of contact points to be generated when colliding two objects.
	// Setting this is a tradeoff between speed and accuracy. (more contacts = slower, but more accurate)
	// Default is 12.
	virtual void SetMaxContacts(s32 amount) = 0;
	
	virtual void SetGravity(const core::vector3df &grav) = 0;
	
	virtual ICollisionMaterial *AddCollisionMaterial() = 0;
	virtual void SetCollisionMaterialInteraction(ICollisionMaterial *m1, ICollisionMaterial *m2,
			const CollisionMaterialInteraction &interaction) = 0;
	
	virtual ICollisionMaterial &GetDefaultCollisionMaterial() = 0;
	virtual CollisionMaterialInteraction *GetCollisionMaterialInteraction(ICollisionMaterial *m1,
			ICollisionMaterial *m2) = 0;
	
	virtual IMeshCollisionGeometry *CreateMeshCollisionGeometry(IMesh *mesh) = 0;
	
	virtual IBoxCollisionGeometry *CreateBoxCollisionGeometry(const core::vector3df &size) = 0;
	virtual IBoxCollisionGeometry *CreateBoxCollisionGeometryFromBB(IMesh *mesh) = 0;
	
	virtual ISphereCollisionGeometry *CreateSphereCollisionGeometry(f32 radius) = 0;
	
	// Create a sphere from a bounding box.
	// If inside == true, then the sphere fits inside the box. Otherwise it encompasses the box.
	virtual ISphereCollisionGeometry *CreateSphereCollisionGeometryFromBB(IMesh *mesh, bool inside = true) = 0;
	
	// mass of the dynamic body defaults to a sphere of density 1.0 and radius 1.0
	// Is an ITransformable and is added to the world.
	// Can be removed with IWorld.RemoveTransformable(..)
	virtual IDynamicBody *AddDynamicBody() = 0;
	
	// An immovable physical body, useful for permanent collidable scene geometry (e.g. the ground, walls etc...)
	// Is an ITransformable and is added to the world.
	// Can be removed with IWorld.RemoveTransformable(..)
	virtual IStaticBody *AddStaticBody() = 0;
	
	/*
	virtual ILinearThruster *CreateLinearThruster(core::vector3df offset, core::vector3df force) = 0;
	
	virtual IPositionController *CreatePositionController(core::vector3df pos, float k1, float k2, float k3) = 0;
	
	virtual IRotationController *CreateRotationController(core::quaternion rot, float k1, float k2, float k3) = 0;
	
	virtual ILinearThrustRotationController *CreateLinearThrustRotationController(
			core::vector3df offset, core::vector3df force, core::quaternion rot, float k1, float k2, float k3) = 0;
	*/
	
	// Defines if collisions between the two layers are enabled.
	// By default all layers collide with all others.
	virtual void SetLayerCollisions(u32 layer1, u32 layer2, bool enabled) = 0;
	
	// are collisions between two layers enabled?
	virtual bool GetLayerCollisions(u32 layer1, u32 layer2) = 0;
	
	// Perform a ray cast against all physical objects.
	// Returns true if an intersection occurs.
	// If collisionResult is set, it will be filled with the result of the closest collision.
	// layer - the layer this ray is considered a member of.
	virtual bool RayCast(const core::line3df &ray, RayCollision *collisionResult = NULL, u32 layer = 0) = 0;
	
	// Ray cast against only the specified geometry.
	virtual bool RayCastIncluding(const core::line3df &ray,
			const Set<ICollisionGeometry *> &includingGeometry,
			RayCollision *collisionResult = NULL,
			u32 layer = 0) = 0;
	
	// Ray cast against all geometry excluding that specified.
	virtual bool RayCastExcluding(const core::line3df &ray,
			const Set<ICollisionGeometry *> &excludingGeometry,
			RayCollision *collisionResult = NULL,
			u32 layer = 0) = 0;
	
	// Same as other ray cast methods but returns a list of *all* collisions that occurred, not just the closest.
	// Maximum of one collision (the first) per geometry.
	virtual std::vector<RayCollision> RayCast(const core::line3df &ray, u32 layer = 0) = 0;
	
	virtual std::vector<RayCollision> RayCastIncluding(const core::line3df &ray,
			const Set<ICollisionGeometry *> &includingGeometry,
			u32 layer = 0) = 0;
	
	virtual std::vector<RayCollision> RayCastExcluding(const core::line3df &ray,
			const Set<ICollisionGeometry *> &excludingGeometry,
			u32 layer = 0) = 0;
};

#endif

