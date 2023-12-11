#ifndef VECTOR3_H
#define VECTOR3_H

#include "matrix.h"
#include <math.h>

#ifndef PIOVER180
#define PIOVER180 0.0174532925f
#endif

namespace Math
{
    template<class T> class Vector3;
    template<class T> class Matrix3;
    template<class T> class Quaternion3;

	template<class T>
	class Vector3
	{
		public:
			T x, y, z;

			Vector3() {}

			Vector3(const T &X, const T &Y, const T &Z)
			{
				x = X; y = Y; z = Z;
			}

			Vector3(const Vector3<T> &rhs)
			{
				x = rhs.x; y = rhs.y; z = rhs.z;
			}

			T length() const
			{
				return sqrt((x*x) + (y*y) + (z*z));
			}

			Vector3<T>& normalize()
			{
				T len = this->length();
				x /= len;
				y /= len;
				z /= len;
				return *this;
			}

			T dot(const Vector3<T> &rhs) const
			{
				return ((x * rhs.x) + (y * rhs.y) + (z * rhs.z));
			}

			Vector3<T> cross(const Vector3<T> &rhs) const
			{
				return Vector3<T>(((y * rhs.z) - (z * rhs.y)),
									((z * rhs.x) - (x * rhs.z)),
									((x * rhs.y) - (y * rhs.x)));
			}

			Vector3<T> operator+(const Vector3<T> &rhs) const
			{
				return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
			}

			Vector3<T> operator-(const Vector3<T> &rhs) const
			{
				return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
			}

			Vector3<T> operator*(const T &rhs) const
			{
				return Vector3<T>(x * rhs, y * rhs, z * rhs);
			}

			Vector3<T> operator/(const T &rhs) const
			{
				return Vector3<T>(x / rhs, y / rhs, z / rhs);
			}

			Vector3<T> operator*(const Matrix3<T> &rhs) const
			{
                return Vector3<T>(  (x * rhs.m[0][0]) + (y * rhs.m[0][1]) + (z * rhs.m[0][2]),
                                    (x * rhs.m[1][0]) + (y * rhs.m[1][1]) + (z * rhs.m[1][2]),
                                    (x * rhs.m[2][0]) + (y * rhs.m[2][1]) + (z * rhs.m[2][2]) );
			}

			Vector3<T> operator=(const Vector3<T> &rhs)
			{
				x = rhs.x; y = rhs.y; z = rhs.z;
			}
	};

	template<typename T>
	inline Vector3<T> vSquared(const Vector3<T> &rhs)
	{
	    return Vector3<T>(rhs * rhs);
	}

	template<typename T>
	Vector3<T> vPow(const Vector3<T> &rhs, const unsigned int &coeff)
	{
        Vector3<T> temp(rhs);
	    for (unsigned int i = 0; i < coeff; ++i)
            temp = temp * rhs;

        return temp;
	}
};

#endif
