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

#ifdef _USE_IRRLICHT_
#include <quaternion.h>
using namespace irr;
#endif
#ifdef _USE_LIGHTFEATHER_
#include "lf/core/quaternion.h"
using namespace lf;
#endif

namespace peak
{
	Quaternion::Quaternion()
	{
	}
	Quaternion::Quaternion(float w, float x, float y, float z)
	{
		q[0] = w;
		q[1] = x;
		q[2] = y;
		q[3] = z;
	}
	Quaternion::Quaternion(float x, float y, float z)
	{
		#ifdef _USE_IRRLICHT_
		core::quaternion quat(x, y, 0);
		quat = core::quaternion(0, 0, z) * quat;
		q[0] = quat.W;
		q[1] = quat.X;
		q[2] = quat.Y;
		q[3] = quat.Z;
		#endif
	}
	Quaternion::Quaternion(Vector3D euler)
	{
		#ifdef _USE_IRRLICHT_
		core::quaternion quat(euler.x, euler.y, 0);
		quat = core::quaternion(0, 0, euler.z) * quat;
		q[0] = quat.W;
		q[1] = quat.X;
		q[2] = quat.Y;
		q[3] = quat.Z;
		#endif

	}
	Quaternion::Quaternion(float *q)
	{
		this->q[0] = q[0];
		this->q[1] = q[1];
		this->q[2] = q[2];
		this->q[3] = q[3];
	}
	Quaternion::Quaternion(float angle, Vector3D axis)
	{
		fromAngleAxis(angle, axis);
	}

	void Quaternion::fromAngleAxis(float angle, Vector3D axis)
	{
		#ifdef _USE_IRRLICHT_
		core::quaternion quat;
		quat.fromAngleAxis(angle, core::vector3df(axis.x, axis.y, axis.z));
		q[0] = quat.W;
		q[1] = quat.X;
		q[2] = quat.Y;
		q[3] = quat.Z;
		#endif
	}
	void Quaternion::toAngleAxis(float &angle, Vector3D &axis)
	{
		#ifdef _USE_IRRLICHT_
		core::vector3df irraxis;
		core::quaternion quat(q[1], q[2], q[3], q[0]);
		quat.toAngleAxis(angle, irraxis);
		axis = Vector3D(irraxis.X, irraxis.Y, irraxis.Z);
		#endif
	}

	float Quaternion::getYaw(void)
	{
		#ifdef _USE_IRRLICHT_
		// FIXME: This is horribly broken.
		core::quaternion quat(q[1], q[2], q[3], q[0]);
		core::vector3df euler;
		quat.toEuler(euler);
		return euler.Y;
		#endif
		#ifdef _USE_LIGHTFEATHER_
		core::quaternion quat(q[1], q[2], q[3], q[0]);
		return quat.toEuler().Y;
		#endif
	}
	Vector3D Quaternion::getEuler(void)
	{
		#ifdef _USE_IRRLICHT_
		core::quaternion quat(q[1], q[2], q[3], q[0]);
		core::vector3df euler;
		quat.toEuler(euler);
		return Vector3D(euler.X, euler.Y, euler.Z);
		#endif
		#ifdef _USE_LIGHTFEATHER_
		core::quaternion quat(q[1], q[2], q[3], q[0]);
		return Vector3D(quat.toEuler().X, quat.toEuler().Y, quat.toEuler().Z);
		#endif
	}

	Vector3D Quaternion::rotateVector(Vector3D vector)
	{
		core::vector3df irrvec(vector.x, vector.y, vector.z);
		core::quaternion quat(q[1], q[2], q[3], q[0]);
		core::vector3df rotated;
		quat.getMatrix().rotateVect(rotated, irrvec);
		return Vector3D(rotated.X, rotated.Y, rotated.Z);
	}

	Quaternion Quaternion::slerp(Quaternion other, float value)
	{
		#ifdef _USE_IRRLICHT_
		core::quaternion q1(q[1], q[2], q[3], q[0]);
		core::quaternion q2(other.q[1], other.q[2], other.q[3], other.q[0]);
		core::quaternion result;
		result.slerp(q1, q2, value);
		return Quaternion(result.W, result.X, result.Y, result.Z);
		#endif
	}

	float Quaternion::getDifference(Quaternion &q2)
	{
		float diff = fabs(q[0] - q2.q[0]);
		diff += fabs(q[1] - q2.q[1]);
		diff += fabs(q[2] - q2.q[2]);
		diff += fabs(q[3] - q2.q[3]);
		return diff;
	}

	Quaternion &Quaternion::operator*=(Quaternion other)
	{
				#ifdef _USE_IRRLICHT_
		core::quaternion q1(q[1], q[2], q[3], q[0]);
		core::quaternion q2(other.q[1], other.q[2], other.q[3], other.q[0]);
		q1 *= q2;
		q[0] = q1.W;
		q[1] = q1.X;
		q[2] = q1.Y;
		q[3] = q1.Z;
		#endif
		return *this;
	}
}

