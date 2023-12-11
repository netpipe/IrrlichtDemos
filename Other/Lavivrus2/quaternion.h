#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>
#include "matrix.h"

#ifndef PIOVER180
#define PIOVER180 0.0174532925f
#endif

//! Math namespace containing Vector, Matrix and Quaternion classes.
namespace Math
{
    template<class T> class Vector3;
    template<class T> class Matrix3;
    template<class T> class Quaternion3;

	template<class T>
	class Quaternion
	{
		public:
			T w, x, y, z;

			Quaternion() {}

			Quaternion(const T &W, const T &X, const T &Y, const T &Z)
			{
				w = W; x = X; y = Y; z = Z;
			}

			Quaternion(const Quaternion<T> &rhs)
			{
				w = rhs.w; x = rhs.x; y = rhs.y; z = rhs.z;
			}

			Matrix3<T> toMat3() const
			{
				return Matrix3<T>( 1 - (2 * (y * y)) - (2 * (z * z)), (2 * (x * y)) - (2 * (w * z)), (2 * (x * z)) + (2 * (w * y)),
								   (2 * (x * y)) + (2 * (w * z)), 1 - (2 * (x * x)) - (2 * (z * z)), (2 * (y * z)) - (2 * (w * x)),
								   (2 * (x * z)) - (2 * (w * y)), (2 * (y * z)) + (2 * (w * x)), 1 - (2 * (x * x)) - (2 * (y * y)) );
			}

			T length() const
			{
				return sqrt((w * w) + (x * x) + (y * y) + (z * z));
			}

			Quaternion<T>& normalize()
			{
			    T temp = sqrt((w * w) + (x * x) + (y * y) + (z * z));
			    w /= temp;
			    x /= temp;
			    y /= temp;
			    z /= temp;

			    return *this;
			}

			Quaternion<T> rotate(const T &angle, const T &X, const T &Y, const T &Z) const
			{
				T ang = angle/2;
				return Quaternion<T>( cos(ang), x * sin(ang), y * sin(ang), z * sin(ang));
			}

			T operator*(const Quaternion<T> &rhs) const
			{
				return (((w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z))
					   +((w * rhs.x) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y))
					   +((w * rhs.y) - (x * rhs.z) + (y * rhs.w) + (z * rhs.x))
					   +((w * rhs.z) + (x * rhs.y) - (y * rhs.x) + (z * rhs.w)));
			}

			Quaternion<T> operator+(const T &rhs) const
			{
				return Quaternion<T>( w + rhs, x + rhs, y + rhs, z + rhs );
			}

			Quaternion<T> operator-(const T &rhs) const
			{
				return Quaternion<T>( w - rhs, x - rhs, y - rhs, z - rhs );
			}

			Quaternion<T> operator*(const T &rhs) const
			{
				return Quaternion<T>( w * rhs, x * rhs, y * rhs, z * rhs );
			}

			Quaternion<T> operator/(const T &rhs) const
			{
				return Quaternion<T>( w / rhs, x / rhs, y / rhs, z / rhs );
			}

			Quaternion<T>& operator=(const Quaternion<T> &rhs)
			{
				w = rhs.w; x = rhs.x; y = rhs.y; z = rhs.z;
				return *this;
			}
	};

    template<typename T>
	Quaternion<T> lerp(const Quaternion<T> &lhs, const Quaternion<T> &rhs, const T &interval)
	{
	    return ((lhs * (1 - interval)) + (rhs * interval)).normalize();
	}

	template<typename T>
	Quaternion<T> slerp(const Quaternion<T> &lhs, const Quaternion<T> &rhs, const T &interval)
	{
	    T angle = acos(lhs * rhs);
	    return ((lhs * ((sin(angle * PIOVER180) * (1 - interval))/sin(angle * PIOVER180))) + (rhs * ((sin(angle * PIOVER180) * interval) / sin(angle * PIOVER180))));
	}
};

#endif
