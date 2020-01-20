
#ifndef BOX_COLLISION_GEOMETRY
#define BOX_COLLISION_GEOMETRY

#include "IBoxCollisionGeometry.h"
#include "CollisionGeometry.h"

class BoxCollisionGeometry : public IBoxCollisionGeometry, public CollisionGeometry
{
	core::vector3df size;
	
public:
	BoxCollisionGeometry(dSpaceID space, const core::vector3df &size);
	~BoxCollisionGeometry();
	
	core::vector3df GetSize();
};

#endif
