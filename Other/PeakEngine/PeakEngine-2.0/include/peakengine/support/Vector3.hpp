/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKENGINE_SUPPORT_VECTOR3_HPP_
#define _PEAKENGINE_SUPPORT_VECTOR3_HPP_

#include <cmath>

namespace peak
{
	/**
	 * Class for three-dimensional vectors, used for positions/rotation.
	 */
	template<typename T> class Vector3
	{
		public:
			/**
			 * Constructor.
			 */
			Vector3() : x(0), y(0), z(0)
			{
			}
			/**
			 * Constructor.
			 */
			Vector3(T x, T y, T z) : x(x), y(y), z(z)
			{
			}

			/**
			 * Returns the squared length of the vector. Much less
			 * computationally expensive than getLength(). Good for comparing
			 * vector lengths.
			 */
			float getLengthSquared()
			{
				return x * x + y * y + z * z;
			}
			/**
			 * Returns the length of the vector.
			 */
			float getLength()
			{
				return sqrt(getLengthSquared());
			}

			/**
			 * Rotates the vector around the z axis.
			 * @param angle Angle in degrees.
			 */
			void rotateXY(float angle)
			{
				angle *= 3.1415f / 180.0f;
				float s = sin(angle);
				float c = cos(angle);
				*this = Vector3<T>(c * x - s * y, s * x + c * y, z);
			}
			/**
			 * Rotates the vector around the y axis.
			 * @param angle Angle in degrees.
			 */
			void rotateXZ(float angle)
			{
				angle *= 3.1415f / 180.0f;
				float s = sin(angle);
				float c = cos(angle);
				*this = Vector3<T>(-c * x + s * z, y, s * x + c * z);
			}
			/**
			 * Rotates the vector around the x axis.
			 * @param angle Angle in degrees.
			 */
			void rotateYZ(float angle)
			{
				angle *= 3.1415f / 180.0f;
				float s = sin(angle);
				float c = cos(angle);
				*this = Vector3<T>(x, c * y - s * z, s * y + c * z);
			}
			/**
			 * Rotates the vector using the given angles. First z rotation is
			 * applied, then x and then z.
			 */
			void rotate(const Vector3<float> &angles)
			{
				rotateXY(angles.z);
				rotateYZ(angles.x);
				rotateXZ(angles.y);
			}

			/**
			 * Dot product.
			 */
			float dot(const Vector3<T> other)
			{
				return x * other.x + y * other.y + z * other.z;
			}
			/**
			 * Cross product.
			 */
			Vector3<T> cross(const Vector3<T> other)
			{
				return Vector3<T>(y * other.z - z * other.y,
				                  z * other.x - x * other.z,
				                  x * other.y - y * other.x);
			}

			/**
			 * Returns the rotation of this vector relative to (0/0/1).
			 */
			Vector3<float> getAngle()
			{
				Vector3<float> angle;
				angle.y = atan2(x, z) * 180.0f / 3.1415f;
				angle.x = atan2(sqrt(x * x + z * z), y) * 180.0f / 3.1415f - 90;
				if (angle.y < 0)
					angle.y += 360;
				if (angle.y >= 360)
					angle.y -= 360;
				if (angle.x < 0)
					angle.x += 360;
				if (angle.x >= 360)
					angle.x -= 360;
				return angle;
			}

			/**
			 * Sets the vector to the linear interpolation between a and b.
			 * \param a First vector
			 * \param b Second vector
			 * \param d Interpolation factor. If it is 0, then the vector will
			 * be set to a, if it is 1, the vector will be b.
			 */
			Vector3<T> &interpolate(const Vector3<T> &a, const Vector3<T> &b,
				float d)
			{
				Vector3<T> dv = b - a;
				*this = a + dv * d;
				return *this;
			}

			template<typename T2> Vector3<T> operator*(T2 s) const
			{
				return Vector3<T>((T)(x * s), (T)(y * s), (T)(z * s));
			}
			template<typename T2> Vector3<T> &operator*=(T2 s)
			{
				x = x * s;
				y = y * s;
				z = z * s;
				return *this;
			}
			template<typename T2> Vector3<T> operator/(T2 s) const
			{
				return Vector3<T>((T)(x / s), (T)(y / s), (T)(z / s));
			}
			template<typename T2> Vector3<T> &operator/=(T2 s)
			{
				x = x / s;
				y = y / s;
				z = z / s;
				return *this;
			}
			template<typename T2> Vector3<T> operator+(const Vector3<T2> &v) const
			{
				return Vector3<T>((T)(x + v.x), (T)(y + v.y), (T)(z + v.z));
			}
			template<typename T2> Vector3<T> operator-(const Vector3<T2> &v) const
			{
				return Vector3<T>((T)(x - v.x), (T)(y - v.y), (T)(z - v.z));
			}
			template<typename T2> Vector3<T> &operator=(const Vector3<T2> &v)
			{
				x = (T)v.x;
				y = (T)v.y;
				z = (T)v.z;
				return *this;
			}
			template<typename T2> Vector3<T> &operator+=(const Vector3<T2> &v)
			{
				x += v.x;
				y += v.y;
				z += v.z;
				return *this;
			}
			template<typename T2> Vector3<T> &operator-=(const Vector3<T2> &v)
			{
				x -= v.x;
				y -= v.y;
				z -= v.z;
				return *this;
			}
			template<typename T2> bool operator==(const Vector3<T2> &v) const
			{
				return x == v.x && y == v.y && z == v.z;
			}
			template<typename T2> bool operator!=(const Vector3<T2> &v) const
			{
				return x != v.x || y != v.y || z != v.z;
			}

			T x;
			T y;
			T z;
	};

	typedef Vector3<int> Vector3I;
	typedef Vector3<float> Vector3F;
}

#endif
