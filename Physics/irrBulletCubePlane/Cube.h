#ifndef __Cube__
#define __Cube__
#include "object.h"

class Cube : public Object
{
	
	Cube(const btScalar& radius, const btTransform& anglePos, const btScalar& mass);
	virtual void createPhysicsInfo(const btScalar& radius, const btTransform& anglePos, const btScalar& mass);
}




#endif