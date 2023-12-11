
#ifndef I_COLLISION_GEOMETRY_H
#define I_COLLISION_GEOMETRY_H

#include "litha_internal.h"
#include "ICollisionMaterial.h"

// NOTE: New collision geometries can only be added internally.
// A case for any new implemented ICollisionGeometry must currently be created
// in DynamicBody.cpp
class ICollisionGeometry : public virtual IReferenceCounted
{
public:
	virtual ~ICollisionGeometry(){}
	
	virtual void SetMaterial(ICollisionMaterial *material) = 0;
	
	// Will return NULL if no material has been set.
	virtual ICollisionMaterial *GetMaterial() = 0;
	
	// Set the collision layer this geometry is a member of.
	// Default is layer 0.
	virtual void SetCollisionLayer(u32 layer) = 0;
	
	virtual u32 GetCollisionLayer() = 0;
	
	// Get a bounding box.
	// Geometry would fit entirely within this box, just touching the sides.
	// This is not transformed in any way.
	virtual core::vector3df GetSize() = 0;
	
	// Get axis aligned bounding box, as positioned within the world.
	// (i.e. depends on where the body that owns this geom is)
	// If this geom is not set on a body, will return the aabb as positioned at {0,0,0}.
	virtual core::aabbox3df GetAABB() = 0;
};

#endif
