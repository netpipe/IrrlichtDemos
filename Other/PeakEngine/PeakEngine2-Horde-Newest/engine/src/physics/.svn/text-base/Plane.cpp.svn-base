
#include "physics/Plane.h"

#include <btBulletDynamicsCommon.h>

namespace peak
{
	Plane::Plane()
	{
	}
	
	Plane::~Plane()
	{
	}
	
	bool Plane::create(Vector3D normal , float constant, Vector3D position)
	{
		shape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), constant);
		transform = new btTransform();
		
		transform->setIdentity();
		transform->setOrigin(btVector3(position.x, position.y, position.z));
		
		inertia = Vector3D(0, 0, 0);
		mass = 0.0f;
		return true;
	}
}
