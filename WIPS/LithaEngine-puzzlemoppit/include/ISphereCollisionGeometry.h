
#ifndef I_SPHERE_COLLISION_GEOMETRY_H
#define I_SPHERE_COLLISION_GEOMETRY_H

#include "ICollisionGeometry.h"

class ISphereCollisionGeometry : public virtual ICollisionGeometry
{
public:
	virtual ~ISphereCollisionGeometry(){}
	
	virtual f32 GetRadius() = 0;
};

#endif
