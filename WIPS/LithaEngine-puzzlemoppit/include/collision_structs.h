
#ifndef COLLISION_STRUCTS_H
#define COLLISION_STRUCTS_H

#include "litha_internal.h"

class ICollisionGeometry;

struct Collision
{
	core::vector3df pos;
	core::vector3df normal;
	f32 depth;
};

struct RayCollision
{
	Collision collision;
	
	// The geometry that the ray touched.
	ICollisionGeometry *geometry;
};

#endif

