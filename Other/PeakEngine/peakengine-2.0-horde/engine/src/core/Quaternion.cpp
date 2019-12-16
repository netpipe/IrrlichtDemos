/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "core/Quaternion.h"

#include <support/quaternion.h>

namespace peak
{
	Quaternion::Quaternion() : x(0), y(0), z(0), w(0)
	{
	}
	Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
	{
	}
	Quaternion::Quaternion(float x, float y, float z)
	{
		/*core::quaternion quat(x, y, 0);
		quat = core::quaternion(0, 0, z) * quat;
		q[0] = quat.W;
		q[1] = quat.X;
		q[2] = quat.Y;
		q[3] = quat.Z;*/
		Quaternion roll(sinf(x / 2), 0, 0, cosf(x / 2));
		Quaternion pitch(0, sinf(y / 2), 0, cosf(y / 2));
		Quaternion yaw(0, 0, sinf(z / 2), cosf(z / 2));
		*this = pitch * roll * yaw;
	}
	Quaternion::Quaternion(Vector3D euler)
	{
		irr::core::quaternion quat(euler.x, euler.y, 0);
		quat = irr::core::quaternion(0, 0, euler.z) * quat;
		w = quat.W;
		x = quat.X;
		y = quat.Y;
		z = quat.Z;
	}
	Quaternion::Quaternion(float *q)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	
	Vector3D Quaternion::getEuler(void)
	{
		irr::core::quaternion quat(x, y, z, w);
		irr::core::vector3df euler;
		quat.toEuler(euler);
		return Vector3D(euler.X, euler.Y, euler.Z);
	}

	Vector3D Quaternion::rotateVector(Vector3D vector)
	{
		irr::core::vector3df irrvec(vector.x, vector.y, vector.z);
		irr::core::quaternion quat(x, y, z, w);
		irr::core::vector3df rotated = quat * irrvec;
		return Vector3D(rotated.X, rotated.Y, rotated.Z);
	}

	Quaternion Quaternion::slerp(Quaternion other, float value)
	{
		irr::core::quaternion q1(x, y, z, w);
		irr::core::quaternion q2(other.x, other.y, other.z, other.w);
		irr::core::quaternion result;
		result.slerp(q1, q2, value);
		return Quaternion(result.X, result.Y, result.Z, result.W);
	}

	float Quaternion::getDifference(Quaternion &q2)
	{
		// TODO: User correct angle here
		float diff = fabs(x - q2.x);
		diff += fabs(y - q2.y);
		diff += fabs(z - q2.z);
		diff += fabs(w - q2.w);
		return diff;
	}

	Quaternion &Quaternion::operator*=(Quaternion other)
	{
		irr::core::quaternion q1(x, y, z, w);
		irr::core::quaternion q2(other.x, other.y, other.z, other.w);
		q1 *= q2;
		w = q1.W;
		x = q1.X;
		y = q1.Y;
		z = q1.Z;
		return *this;
	}
	Quaternion Quaternion::operator*(Quaternion other)
	{
		irr::core::quaternion q1(x, y, z, w);
		irr::core::quaternion q2(other.x, other.y, other.z, other.w);
		q1 *= q2;
		return Quaternion(q1.X, q1.Y, q1.Z, q1.W);
	}
}

