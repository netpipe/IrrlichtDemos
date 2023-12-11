/*

BMaths.cpp

*/

#include "BMaths.h"

// rotates a bone
core::vector3df matrixRotation(core::vector3df rotation, core::vector3df vec)
{
	f32 pitch=vec.X;
	f32 yaw=vec.Y;
	f32 roll=vec.Z;
	matrix4 m,t;
	m.setRotationDegrees(rotation);

	t.setRotationDegrees(core::vector3df(0,0,roll));
	m*=t;

	t.setRotationDegrees(core::vector3df(pitch,0,0));
	m*=t;

	t.setRotationDegrees(core::vector3df(0,yaw,0));
	t*=m;

	return t.getRotationDegrees();
};
