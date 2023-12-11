
#ifndef RAY_COLLISION_H
#define RAY_COLLISION_H

#include "litha_internal.h"
#include <ode/ode.h>
#include "collision_structs.h"
#include <vector>

class ICollisionGeometry;
class IPhysics;

// Collide a ray against all geoms in the given space, returning a list of all collisions.
// NOTE, this currently assumes space only contains geoms (no sub-spaces).
// including - only include the given geometies in collisions
// excluding - include all but these geometries
// layer - the layer the ray should be considered a part of
// physics - need a pointer to this to check which layers can collide with each other
// It doesn't make sense to set both including and excluding at the same time.
const std::vector<RayCollision> &ray_cast(dGeomID rayGeom, dSpaceID space,
		const Set<ICollisionGeometry *> *including,
		const Set<ICollisionGeometry *> *excluding,
		u32 layer,
		IPhysics *p);

// This version creates the ODE ray geom for you.
// Still need to know the ODE space though.
const std::vector<RayCollision> &ray_cast(const core::line3df &ray, dSpaceID space,
		const Set<ICollisionGeometry *> *including,
		const Set<ICollisionGeometry *> *excluding,
		u32 layer,
		IPhysics *p);

/*
// Collide a ray geom against all geoms in the given space, finding the nearest collision point.
// Returns true if a collision occurred, false if none occurred.
// NOTE, this currently assumes space only contains geoms (no sub-spaces).
// including - only include the given geometies in collisions
// excluding - include all but these geometries
// It doesn't make sense to set both including and excluding at the same time.
bool closest_ray_collision(dGeomID rayGeom, dSpaceID space, RayCollision *collisionResult = NULL,
		const Set<ICollisionGeometry *> *including = NULL,
		const Set<ICollisionGeometry *> *excluding = NULL);
*/

#endif

