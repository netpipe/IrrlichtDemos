
#include "SphereCollisionGeometry.h"

SphereCollisionGeometry::SphereCollisionGeometry(dSpaceID space, f32 radius)
	: CollisionGeometry(space)
{
	SetODEGeom( dCreateSphere(space, radius) );
	
	this->radius = radius;
}

SphereCollisionGeometry::~SphereCollisionGeometry()
{
}

core::vector3df SphereCollisionGeometry::GetSize()
{
	return core::vector3df(radius*2.0);
}

f32 SphereCollisionGeometry::GetRadius()
{
	return radius;
}
