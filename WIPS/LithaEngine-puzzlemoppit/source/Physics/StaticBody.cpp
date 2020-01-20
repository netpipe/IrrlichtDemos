
#include "StaticBody.h"
#include "ICollisionGeometry.h"
#include "CollisionGeometry.h"
#include "ode_utility.h"

StaticBody::StaticBody()
{
	// Do nothing yet...
}

StaticBody::~StaticBody()
{
	for (u32 i = 0; i < geometry.size(); i ++)
		geometry[i]->drop();
}

void StaticBody::AddCollisionGeometry(ICollisionGeometry *geom)
{
	geom->grab();
	geometry.push_back(geom);
	
	if (CollisionGeometry *hasGeom = dynamic_cast<CollisionGeometry *>(geom))
	{
		dGeomID odeGeom = hasGeom->GetODEGeom();
		odeGeoms.push_back(odeGeom);
		dGeomSetBody(odeGeom, 0);
	}
	else
		FAIL << "A collision geometry could not be dynamically cast to CollisionGeometry.";
}

Set<ICollisionGeometry *> StaticBody::GetCollisionGeometries()
{
	Set<ICollisionGeometry *> geometries;
	
	for (u32 i = 0; i < geometry.size(); i ++)
		geometries.Insert(geometry[i]);
	
	return geometries;
}

void StaticBody::SetPosition(const core::vector3df &pos)
{
	ITransformable::SetPosition(pos);
	
	for (u32 i = 0; i < odeGeoms.size(); i ++)
		ODESetGeomPosition(odeGeoms[i], pos);
}

void StaticBody::SetRotation(const core::vector3df &rot)
{
	ITransformable::SetRotation(rot);
	
	for (u32 i = 0; i < odeGeoms.size(); i ++)
		ODESetGeomRotation(odeGeoms[i], rot);
}

