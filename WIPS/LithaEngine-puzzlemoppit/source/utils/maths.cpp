
#include "utils/maths.h"

namespace utils
{
namespace maths
{

void copy_rotation(const core::matrix4 &source, core::matrix4 &dest)
{
	dest[0] = source[0];
	dest[1] = source[1];
	dest[2] = source[2];
	dest[4] = source[4];
	dest[5] = source[5];
	dest[6] = source[6];
	dest[8] = source[8];
	dest[9] = source[9];
	dest[10] = source[10];
}

core::vector3df quaternion_to_euler(const core::quaternion &quat)
{
	// This code from "3D Math Primer for Graphics and Game Development"
	// http://www.gamemath.com
	// The Irrlicht quaternion.toEuler method doesn't seem to account for gimbal lock??
	
	core::vector3df euler;
	
	f32 sp = -2.0f * (quat.Y*quat.Z - quat.W*quat.X);

	if (fabs(sp) > 0.9999f)
	{
		euler.X = core::HALF_PI * sp;
		euler.Y = atan2(-quat.X*quat.Z + quat.W*quat.Y, 0.5f - quat.Y*quat.Y - quat.Z*quat.Z);
		euler.Z = 0.0f;
	}
	else
	{
		euler.X	= asin(sp);
		euler.Y	= atan2(quat.X*quat.Z + quat.W*quat.Y, 0.5f - quat.X*quat.X - quat.Y*quat.Y);
		euler.Z	= atan2(quat.X*quat.Y + quat.W*quat.Z, 0.5f - quat.X*quat.X - quat.Z*quat.Z);
	}
	return euler * core::RADTODEG;
}

core::quaternion euler_to_quaternion(const core::vector3df &rot)
{
	return core::quaternion(rot * core::DEGTORAD);
}

void interpolate_matrix(const core::matrix4 &a, const core::matrix4 &b, f32 alpha, core::matrix4 &dest)
{
	// I think the reason I interpolate position and rotation separately (rather than using matrix.interpolate)
	// is for slerp.
	
	dest.setTranslation( core::lerp(a.getTranslation(), b.getTranslation(), alpha) );
	dest.setRotationDegrees( interpolate_rotation(a.getRotationDegrees(),b.getRotationDegrees(),alpha) );
}

core::vector3df combine_rotations(const core::vector3df &a, const core::vector3df &b)
{
	return quaternion_to_euler(
			core::quaternion(a*core::DEGTORAD) * core::quaternion(b*core::DEGTORAD)
			);
}

core::vector3df interpolate_position(const core::vector3df &a, const core::vector3df &b, f32 alpha)
{
	return core::lerp(a,b,alpha);
}

core::vector3df interpolate_rotation(const core::vector3df &a, const core::vector3df &b, f32 alpha)
{
	core::quaternion qCurrent(core::DEGTORAD * a);
	core::quaternion qDesired(core::DEGTORAD * b);
	core::quaternion qInterpolated;
	qInterpolated.slerp(qCurrent,qDesired,alpha);
	
	return quaternion_to_euler(qInterpolated);
}

f32 scalar_rotation_difference(const core::vector3df &a, const core::vector3df &b)
{
	core::quaternion q1(core::DEGTORAD * a);
	core::quaternion q2(core::DEGTORAD * b);
	
	// fix: otherwise the dot product never hits exactly 1.0 (and thus acos(dp) never reaches 0.0).
	// probably due to rounding errors
	if (q1 == q2)
		return 0.0;
	
	f32 dp = fabs(q1.dotProduct(q2));
	
	if (dp > 1.0)
		dp = 1.0;
	
	return 2.0 * acos(dp) * core::RADTODEG;
}

// this function is untested
core::vector3df rotation_difference(const core::vector3df &a, const core::vector3df &b)
{
	core::quaternion q1(core::DEGTORAD * a);
	core::quaternion q2(core::DEGTORAD * b);
	
	// Delta
	q1.makeInverse();
	core::quaternion qDelta = q1 * q2;
	
	return quaternion_to_euler(qDelta);
}

core::vector3df rotation_required(const core::vector3df &vec, const core::vector3df &opposite)
{
	const core::vector3df startVec(0,0,1);
	
	if (vec == -startVec)
		return opposite;
	
	core::quaternion quat;
	quat.rotationFromTo(startVec, vec);
	
	return quaternion_to_euler(quat);
}

f32 rotation_required_2d(const core::vector2df &vec)
{
	// atan2(1,0) == PI/2.0
	return (core::HALF_PI - atan2(vec.Y, vec.X)) * core::RADTODEG;
}

core::vector3df rotation_to_direction(const core::vector3df &rot)
{
	return core::quaternion(rot * core::DEGTORAD) * core::vector3df(0,0,1);
}

// interpolate a single angle in degrees
// handles the wrap around.
f32 interpolate_angle(f32 a, f32 b, f32 alpha)
{
	// put between 0 and 360
	a = fmod(a, 360.0);
	b = fmod(b, 360.0);
	
	// The smallest arc doesn't cross the 0/360 point?
	if (fabs(b-a) <= 180.0)
		return a + (b-a)*alpha;
	
	f32 delta;
	
	if (a > b)
	{
		delta = 360.0 - a;
		a = 0.0; // equivalent to a += delta
		b += delta;
	}
	else // b > a
	{
		delta = 360.0 - b;
		b = 0.0; // equivalent to b += delta
		a += delta;
	}
	
	return fmod( ( a + (b-a)*alpha ) - delta, 360.0 );
}

f32 extract_y_rotation(const core::vector3df &rot)
{
	core::vector3df vec(0,0,1);
	core::quaternion quat(rot * core::DEGTORAD);
	vec = quat * vec;
	
	core::vector2df vec2d(vec.X,vec.Z);
	vec2d.normalize();
	return vec2d.getAngle() + 90.0; // Irrlicht has 0 to the right (1,0) rather than up (0,1).
}

core::vector2df get_horizontal_vec(const core::vector3df &rot)
{
	return vec_from_angle( extract_y_rotation(rot) );
}

core::vector2df vec_from_angle(f32 angle)
{
	angle *= core::DEGTORAD;
	return core::vector2df(sin(angle),cos(angle));
}


/*
// This is not really tested.
// Nor does it have a purpose in this Engine, I just dumped it here for
// something to do with it.
void SetAbsoluteRotation(scene::ISceneNode *node, core::vector3df absoluteRot)
{
	// Could perhaps recurse.
	if (node->getParent())
		node->getParent()->updateAbsolutePosition();

	node->updateAbsolutePosition();

	// Old absolute rotation
	core::quaternion qOldRotAbs( node->getAbsoluteTransformation().getRotationDegrees() * core::DEGTORAD );

	// Desired new absolute rotation
	core::quaternion qDesiredRotAbs( absoluteRot * core::DEGTORAD );

	// Delta
	core::quaternion qDelta;
	core::quaternion qOldRotAbsInv = qOldRotAbs;
	qOldRotAbsInv.makeInverse();
	qDelta = qOldRotAbsInv * qDesiredRotAbs;

	// Relative rotation
	core::quaternion qOldRelative(node->getRotation() * core::DEGTORAD);

	// Result!
	core::quaternion result = qOldRelative * qDelta;

	// Set
	node->setRotation( quaternion_to_euler(result) );
}
*/

}
}


