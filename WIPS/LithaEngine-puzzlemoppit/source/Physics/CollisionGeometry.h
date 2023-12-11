
#ifndef COLLISION_GEOMETRY
#define COLLISION_GEOMETRY

#include "ICollisionGeometry.h"
#include <ode/ode.h>

// Base implementation for all collision geometries.
class CollisionGeometry : public virtual ICollisionGeometry
{
	u32 collisionLayer;
	ICollisionMaterial *material;
	
	dGeomID geom;
	
protected:
	// This method should be called *once* by derived classes after
	// creating a specific ODE geometry.
	// As well as setting the private geom attribute it sets the geom's userdata
	// to point to this ICollisionGeometry.
	void SetODEGeom(dGeomID geom);
	
public:
	CollisionGeometry(dSpaceID space);
	virtual ~CollisionGeometry();
	
	// Used by DynamicBody (and possibly derived classes too)
	dGeomID GetODEGeom();
	
	void SetMaterial(ICollisionMaterial *material);
	ICollisionMaterial *GetMaterial();
	
	void SetCollisionLayer(u32 layer);
	u32 GetCollisionLayer();
	
	core::aabbox3df GetAABB();
};

#endif
