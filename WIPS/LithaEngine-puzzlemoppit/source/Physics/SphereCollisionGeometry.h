
#ifndef SPHERE_COLLISION_GEOMETRY
#define SPHERE_COLLISION_GEOMETRY

#include "ISphereCollisionGeometry.h"
#include "CollisionGeometry.h"

class SphereCollisionGeometry : public ISphereCollisionGeometry, public CollisionGeometry
{
	f32 radius;
	
public:
	SphereCollisionGeometry(dSpaceID space, f32 radius);
	~SphereCollisionGeometry();
	
	core::vector3df GetSize();
	
	f32 GetRadius();
};

#endif
