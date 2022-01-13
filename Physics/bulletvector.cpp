// irrbullet.h inspired by blindside

#ifndef IRR_BULLET_H
#define IRR_BULLET_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>

namespace irr {

// convert bullet vector to irrlicht vector
core::vector3df toIrrVec(const btVector3& v);

// convert 
btVector3 toBtVec(const core::vector3df& v);

// get Euler angles in YXZ-order from bullet quaternion
core::vector3df quaternionToEuler(const btQuaternion& q);

// get bullet quaternion from angles in YXZ-order
btQuaternion eulerToQuaternion(const core::vector3df& euler);

// transform irrlicht matrix to bullet transform
btTransform toBtTransform(const core::matrix4& m);

// choose coordinates from vector
core::vector3df vecAxes(const core::vector3df& v, bool x, bool y, bool z);

// null vector
const btVector3 NullVector(0.0f, 0.0f, 0.0f);

} // end namespace irr

#endif

// irrbullet.cpp

#include "irrbullet.h"

using namespace irr;
using namespace core;

vector3df toIrrVec(const btVector3& v)
{
	return (vector3df&)v;
}

btVector3 toBtVec(const vector3df& v)
{
	// btVector requires special byte alignment so better not take my chances.
	return btVector3(v.X, v.Y, v.Z);
}

vector3df quaternionToEuler(const btQuaternion& q) {
  vector3df euler;
  quaternion(q.getX(), q.getY(), q.getZ(), q.getW()).toEuler(euler);
  return euler;
}

btQuaternion eulerToQuaternion(const vector3df& euler) {
  return btQuaternion(euler.Y, euler.X, euler.Z);
}

btTransform toBtTransform(const matrix4& m) {
  btScalar* p = (btScalar*)m.pointer();
  return btTransform( 
    btMatrix3x3(p[0], p[1], p[2], p[4], p[5], p[6], p[8], p[9], p[10]),
    btVector3(p[12], p[13], p[14])
    );
}

core::vector3df vecAxes(const core::vector3df& v, bool x, bool y, bool z) {
  vector3df c;
  if(x) c.X = v.X;
  if(y) c.Y = v.Y;
  if(z) c.Z = v.Z;
  return c;
}
