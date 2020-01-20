
#ifndef PHYSICS_H
#define PHYSICS_H

#include "IPhysics.h"
#include <ode/ode.h>
#include <vector>
#include <map>
#include "CollisionMaterialInteraction.h"

class IWorld;

class Physics : public IPhysics
{
public:
	Physics(IWorld *lithaWorld);
	~Physics();
	
	void Step(f32 dt);
	
	void SetMaxContacts(s32 amount);
	
	void SetGravity(const core::vector3df &grav);
	
	ICollisionMaterial *AddCollisionMaterial();
	void SetCollisionMaterialInteraction(ICollisionMaterial *m1, ICollisionMaterial *m2,
			const CollisionMaterialInteraction &interaction);
	
	ICollisionMaterial &GetDefaultCollisionMaterial();
	CollisionMaterialInteraction *GetCollisionMaterialInteraction(ICollisionMaterial *m1,
			ICollisionMaterial *m2);
	
	IMeshCollisionGeometry *CreateMeshCollisionGeometry(IMesh *mesh);
	
	IBoxCollisionGeometry *CreateBoxCollisionGeometry(const core::vector3df &size);
	IBoxCollisionGeometry *CreateBoxCollisionGeometryFromBB(IMesh *mesh);
	
	ISphereCollisionGeometry *CreateSphereCollisionGeometry(f32 radius);
	ISphereCollisionGeometry *CreateSphereCollisionGeometryFromBB(IMesh *mesh, bool inside);
	
	IDynamicBody *AddDynamicBody();
	IStaticBody *AddStaticBody();
	
	void SetLayerCollisions(u32 layer1, u32 layer2, bool enabled);
	bool GetLayerCollisions(u32 layer1, u32 layer2);
	
	bool RayCast(const core::line3df &ray, RayCollision *collisionResult, u32 layer);
	bool RayCastIncluding(const core::line3df &ray, const Set<ICollisionGeometry *> &includingGeometry,
			RayCollision *collisionResult, u32 layer);
	bool RayCastExcluding(const core::line3df &ray, const Set<ICollisionGeometry *> &excludingGeometry,
			RayCollision *collisionResult, u32 layer);
	// internal method only
	bool RayCast(const core::line3df &ray, RayCollision *collisionResult,
			const Set<ICollisionGeometry *> *includingGeometry,
			const Set<ICollisionGeometry *> *excludingGeometry,
			u32 layer);
	
	std::vector<RayCollision> RayCast(const core::line3df &ray, u32 layer);
	std::vector<RayCollision> RayCastIncluding(const core::line3df &ray,
			const Set<ICollisionGeometry *> &includingGeometry, u32 layer);
	std::vector<RayCollision> RayCastExcluding(const core::line3df &ray,
			const Set<ICollisionGeometry *> &excludingGeometry, u32 layer);
	
private:
	
	IWorld *lithaWorld;
	
	dWorldID		world;
	dSpaceID		space; // root space
	dJointGroupID	perStepContactJointGroup;
	
	s32 maxContactsPerCollision;
	
	std::vector<ICollisionMaterial *> materials;
	
	std::map<ICollisionMaterial *, std::map<ICollisionMaterial *, CollisionMaterialInteraction> >
			materialInteractions;
	
	// whether collisions between two layers are enabled.
	std::map<u32, std::map<u32, bool> > layerCollisions;
	
	ICollisionMaterial *defaultMaterial;
	CollisionMaterialInteraction defaultInteraction;
	
	static void ODE_Callback(void *data, dGeomID o1, dGeomID o2)
	{
		// Do we have at least one space?
		if (dGeomIsSpace(o1) || dGeomIsSpace(o2))
		{
			// Collide spaces or a space and a geom.
			dSpaceCollide2(o1, o2, data, ODE_Callback);
			
			// Collide all geoms inside this space with each other
			
			if (dGeomIsSpace(o1))
				dSpaceCollide((dSpaceID)o1, data, ODE_Callback);
			
			if (dGeomIsSpace(o2))
				dSpaceCollide((dSpaceID)o2, data, ODE_Callback);
		}
		else
		{
			// Collide two geoms.
			((Physics *)data)->ODE_GeomCollide(o1,o2);
		}
	}

	void ODE_GeomCollide(dGeomID o1, dGeomID o2);
	
	// Fill the ODE surface parameters struct using the given interaction.
	void SetSurfaceFromInteraction(dSurfaceParameters &surface, const CollisionMaterialInteraction &interaction);
};

#endif
