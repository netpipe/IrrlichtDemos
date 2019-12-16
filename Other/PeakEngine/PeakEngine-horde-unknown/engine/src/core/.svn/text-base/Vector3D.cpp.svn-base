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

#include "core/Vector3D.h"

#include <math.h>

#include <support/vector3d.h>

namespace peak
{
	Vector3D::Vector3D()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3D::~Vector3D()
	{
	}
	Vector3D::Vector3D(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	void Vector3D::rotateXZ(float radians)
	{
		float oldx = x;
		float oldz = z;
		x = cos(radians) * oldx + -sin(radians) * oldz;
		z = sin(radians) * oldx + cos(radians) * oldz;
	}
	void Vector3D::rotateYZ(float radians)
	{
		float oldy = y;
		float oldz = z;
		y = cos(radians) * oldy + -sin(radians) * oldz;
		z = sin(radians) * oldy + cos(radians) * oldz;
	}
	void Vector3D::rotateXY(float radians)
	{
		float oldx = x;
		float oldy = y;
		x = cos(radians) * oldx + -sin(radians) * oldy;
		y = sin(radians) * oldx + cos(radians) * oldy;
	}
	
	void Vector3D::rotate(Vector3D rotation)
	{
		rotateXY(rotation.z);
		rotateYZ(rotation.x);
		rotateXZ(rotation.y);
	}
	
	Vector3D Vector3D::getRotation(void)
	{
		irr::core::vector3df vector(x, y, z);
		irr::core::vector3df rotation = vector.getHorizontalAngle();
		rotation *= M_PI / 180;
		return Vector3D(rotation.X, rotation.Y, rotation.Z);
	}
	
	float Vector3D::getLengthSq(void)
	{
		return x * x + y * y + z * z;
	}
	
	Vector3D &Vector3D::operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}
	Vector3D Vector3D::operator+(const Vector3D &v)
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}
	Vector3D Vector3D::operator-(const Vector3D &v)
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}
	Vector3D Vector3D::operator*(const float scale)
	{
		return Vector3D(x * scale, y * scale, z * scale);
	}
}

