
#ifndef MESH_COLLISION_GEOMETRY
#define MESH_COLLISION_GEOMETRY

#include "IMeshCollisionGeometry.h"
#include "CollisionGeometry.h"
#include "ode_utility.h"

class IMesh;

class MeshCollisionGeometry : public IMeshCollisionGeometry, public CollisionGeometry
{
	ODEMeshData *meshData;
	
public:
	MeshCollisionGeometry(dSpaceID space, IMesh *mesh);
	~MeshCollisionGeometry();
	
	core::vector3df GetSize();
};

#endif
