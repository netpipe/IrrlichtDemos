
#include "ray_collision.h"
#include "CollisionMaterialInteraction.h"
#include <vector>
#include "ICollisionGeometry.h"
#include "IPhysics.h"

// want a pointer to this to check which layer collisions are enabled
IPhysics *physics = NULL;

dGeomID castRayGeom;
std::vector<RayCollision> collisions;
const Set<ICollisionGeometry *> *includingGeometry;
const Set<ICollisionGeometry *> *excludingGeometry;
u32 rayCollisionLayer;

// Collide two geoms.
// The first geom should be the ray.
void ray_callback(void *data, dGeomID o1, dGeomID o2)
{
	// Don't bother colliding against self...
	if (o1 == o2)
		return;
	
	// Should both be geoms
	ASSERT( !dGeomIsSpace(o1) );
	ASSERT( !dGeomIsSpace(o2) );
	
	// First should always be the original ray...
	ASSERT( o1 == castRayGeom );
	
	// Don't bother testing against another ray.
	if (dGeomGetClass(o2) == dRayClass )
		return;
	
	// Are collisions actually enabled between the ray's layer and the geom's layer?
	
	ASSERT( dGeomGetData(o2) );
	ASSERT( physics );
	
	if (!physics->GetLayerCollisions(rayCollisionLayer, ((ICollisionGeometry *)dGeomGetData(o2))->GetCollisionLayer()))
		return;
	
	
	// Include or exclude some particular geoms
	// (remember, each ODE geom has ICollisionGeometry set as its data pointer)
	
	ASSERT( !(includingGeometry && excludingGeometry) );
	
	// We only test collisions against the given geometry.
	if (includingGeometry)
	{
		// If includingGeometry doesn't contain this geom, then ignore this collision.
		if ( !includingGeometry->Contains((ICollisionGeometry *)dGeomGetData(o2)) )
			return;
	}
	// We include all geometry except certain geometry which is excluded.
	else if (excludingGeometry)
	{
		// If exludingGeometry does contain this geom, then ignore this collision.
		if ( excludingGeometry->Contains((ICollisionGeometry *)dGeomGetData(o2)) )
			return;
	}
	
	
	dContact contact;
	
	s32 contactCount = dCollide(o1, o2, 1, &contact.geom, sizeof(dContact));
	
	// A ray collision so maximum of one contact point.
	ASSERT( contactCount <= 1 );
	
	if (contactCount == 1)
	{
		RayCollision rayCollision;
		
		Collision &collision = rayCollision.collision;
		collision.pos = core::vector3df( contact.geom.pos[0],contact.geom.pos[1],contact.geom.pos[2] );
		collision.normal = core::vector3df( contact.geom.normal[0],contact.geom.normal[1],contact.geom.normal[2] );
		collision.depth = contact.geom.depth;
		
		rayCollision.geometry = (ICollisionGeometry *)dGeomGetData(o2);
		
		collisions.push_back(rayCollision);
	}
}

const std::vector<RayCollision> &ray_cast(dGeomID rayGeom, dSpaceID space,
		const Set<ICollisionGeometry *> *including,
		const Set<ICollisionGeometry *> *excluding,
		u32 layer,
		IPhysics *p)
{
	ASSERT( dGeomGetClass(rayGeom) == dRayClass );
	
	// Stored for debugging...
	castRayGeom = rayGeom;
	
	// Store pointers
	includingGeometry = including;
	excludingGeometry = excluding;
	
	// Store layer
	rayCollisionLayer = layer;
	
	// Store pointer to physics
	physics = p;
	
	// Reset temporary list of collisions
	collisions.clear();
	
	// Collide ray with each geom in the space
	dSpaceCollide2(rayGeom, (dGeomID)space, NULL, ray_callback);
	
	return collisions;
}

const std::vector<RayCollision> &ray_cast(const core::line3df &ray, dSpaceID space,
		const Set<ICollisionGeometry *> *including,
		const Set<ICollisionGeometry *> *excluding,
		u32 layer,
		IPhysics *p)
{
	// Create ray geom
	dGeomID rayGeom = dCreateRay(space, ray.getLength());
	core::vector3df dir = ray.getVector();
	dir.normalize();
	dGeomRaySet(rayGeom, ray.start.X,ray.start.Y,ray.start.Z, dir.X,dir.Y,dir.Z);
	// Don't use the first contact, use the closest, and also do backface culling (ignore backface)
	dGeomRaySetParams(rayGeom, false, true);
	// Want closest point on a trimesh
	dGeomRaySetClosestHit(rayGeom, true);
	
	ray_cast(rayGeom, space, including, excluding, layer, p);
	dGeomDestroy(rayGeom);
	
	return collisions;
}

/*
// Previous implementation that returned only the closest collision point.
bool closest_ray_collision(dGeomID rayGeom, dSpaceID space, RayCollision *collisionResult,
		const Set<ICollisionGeometry *> *including,
		const Set<ICollisionGeometry *> *excluding)
{
	ASSERT( dGeomGetClass(rayGeom) == dRayClass );
	
	// Store pointers
	includingGeometry = including;
	excludingGeometry = excluding;
	
	// Reset temporary list of collisions
	collisions.clear();
	
	// Collide ray with each geom in the space
	dSpaceCollide2(rayGeom, (dGeomID)space, NULL, ray_callback);
	
	if (!collisions.size())
		return false;
	
	// Find the closest collision
	if (collisionResult)
	{
		RayCollision *closest = &collisions[0];
		
		for (u32 i = 0; i < collisions.size(); i ++)
		{
			if (collisions[i].collision.depth < closest->collision.depth)
				closest = &collisions[i];
		}
		
		*collisionResult = *closest;
	}
	
	return true;
}
*/

