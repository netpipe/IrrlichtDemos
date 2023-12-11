
#include "CollisionGeometry.h"

CollisionGeometry::CollisionGeometry(dSpaceID space)
{
	collisionLayer = 0;
	material = NULL;
	
	// Geom is set by a call to SetODEGeom in the constructor of derived classes.
	geom = NULL;
}

CollisionGeometry::~CollisionGeometry()
{
	ASSERT(geom);
	dGeomDestroy(geom);
	
	if (material)
		material->drop();
}

void CollisionGeometry::SetODEGeom(dGeomID geom)
{
	this->geom = geom;
	
	// Pointer must be to ICollisionGeometry as that is what we cast back to when getting the data.
	// (If the types don't match exactly we get insidious crashes.)
	dGeomSetData(geom, static_cast<ICollisionGeometry *>(this));
}

dGeomID CollisionGeometry::GetODEGeom()
{
	return geom;
}

void CollisionGeometry::SetMaterial(ICollisionMaterial *material)
{
	SET_REF_COUNTED_POINTER(this->material, material)
}

ICollisionMaterial *CollisionGeometry::GetMaterial()
{
	return material;
}

void CollisionGeometry::SetCollisionLayer(u32 layer)
{
	collisionLayer = layer;
}

u32 CollisionGeometry::GetCollisionLayer()
{
	return collisionLayer;
}

core::aabbox3df CollisionGeometry::GetAABB()
{
	dReal aabb[6];
	dGeomGetAABB(geom, aabb);
	return core::aabbox3df( aabb[0],aabb[2],aabb[4], aabb[1],aabb[3],aabb[5] );
}
