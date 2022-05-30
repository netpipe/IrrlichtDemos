#ifndef Matrix3_H
#define Matrix3_H

#include "vector.h"

#ifndef PIOVER180
#define PIOVER180 0.0174532925f
#endif

namespace Math
{
    template<class T> class Vector3;
    template<class T> class Matrix3;
    template<class T> class Quaternion3;

	template<class T>
	class Matrix3
	{
		public:
			T m[3][3];

			Matrix3() {}

			Matrix3(const T &m1, const T &m2, const T &m3,
					const T &m4, const T &m5, const T &m6,
					const T &m7, const T &m8, const T &m9)
			{
				m[0][0] = m1; m[0][1] = m2; m[0][2] = m3;
				m[1][0] = m4; m[1][1] = m5; m[1][2] = m6;
				m[2][0] = m7; m[2][1] = m8; m[2][2] = m9;
			}

			Matrix3<T>& identity()
			{
				m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
				m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
				m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
				return *this;
			}

			Matrix3<T> transpose()
			{
                return Matrix3<T>( m[0][0], m[1][0], m[2][0],
                                   m[0][1], m[1][1], m[2][1],
                                   m[0][2], m[1][2], m[2][2] );
			}

			T determinant() const
			{
				return ((m[0][0] * ((m[1][1] * m[2][2]) - (m[1][2] * m[2][1]))) - (m[0][1] * ((m[1][0] * m[2][2]) - (m[1][2] * m[2][0]))) + (m[0][2] * ((m[1][0] * m[2][1]) - (m[1][1] * m[2][0]))));
			}

			Matrix3<T> inverse()
			{
				T det = T(1)/((m[0][0] * ((m[1][1] * m[2][2]) - (m[1][2] * m[2][1]))) - (m[0][1] * ((m[1][0] * m[2][2]) - (m[1][2] * m[2][0]))) + (m[0][2] * ((m[1][0] * m[2][1]) - (m[1][1] * m[2][0]))));
				Matrix3<T> temp;

				temp.m[0][0] = det * ((m[1][1] * m[2][2]) - (m[1][2] * m[2][1]));
				temp.m[0][1] = det * ((m[0][2] * m[2][1]) - (m[0][1] * m[2][2]));
				temp.m[0][2] = det * ((m[0][1] * m[1][2]) - (m[0][2] * m[1][1]));

				temp.m[1][0] = det * ((m[1][2] * m[2][0]) - (m[1][0] * m[2][2]));
				temp.m[1][1] = det * ((m[0][0] * m[2][2]) - (m[0][2] * m[2][0]));
				temp.m[1][2] = det * ((m[0][2] * m[1][0]) - (m[0][0] * m[1][2]));

				temp.m[2][0] = det * ((m[1][0] * m[2][1]) - (m[1][1] * m[2][0]));
				temp.m[2][1] = det * ((m[0][1] * m[2][0]) - (m[0][0] * m[2][1]));
				temp.m[2][2] = det * ((m[0][0] * m[1][1]) - (m[0][1] * m[1][0]));

				return temp;
			}

			Matrix3<T> operator+(const T &rhs) const
			{
				return Matrix3<T>( m[0][0] + rhs, m[0][1] + rhs, m[0][2] + rhs,
								   m[1][0] + rhs, m[1][1] + rhs, m[1][2] + rhs,
								   m[2][0] + rhs, m[2][1] + rhs, m[2][2] + rhs );
			}

			Matrix3<T> operator-(const T &rhs) const
			{
				return Matrix3<T>( m[0][0] - rhs, m[0][1] - rhs, m[0][2] - rhs,
								   m[1][0] - rhs, m[1][1] - rhs, m[1][2] - rhs,
								   m[2][0] - rhs, m[2][1] - rhs, m[2][2] - rhs );
			}

			Matrix3<T> operator*(const T &rhs) const
			{
				return Matrix3<T>( m[0][0] * rhs, m[0][1] * rhs, m[0][2] * rhs,
								   m[1][0] * rhs, m[1][1] * rhs, m[1][2] * rhs,
								   m[2][0] * rhs, m[2][1] * rhs, m[2][2] * rhs );
			}

			Matrix3<T> operator/(const T &rhs) const
			{
				return Matrix3<T>( m[0][0] / rhs, m[0][1] / rhs, m[0][2] / rhs,
								   m[1][0] / rhs, m[1][1] / rhs, m[1][2] / rhs,
								   m[2][0] / rhs, m[2][1] / rhs, m[2][2] / rhs );
			}

			Vector3<T> operator*(const Vector3<T> &rhs) const
			{
				return Vector3<T>(  (m[0][0] * rhs.x) + (m[0][1] * rhs.y) + (m[0][2] * rhs.z),
                                    (m[1][0] * rhs.x) + (m[1][1] * rhs.y) + (m[1][2] * rhs.z),
                                    (m[2][0] * rhs.x) + (m[2][1] * rhs.y) + (m[2][2] * rhs.z) );
			}

			Matrix3<T> operator*(const Matrix3<T> &rhs) const
			{
				return Matrix3<T>( m[0][0] * rhs.m[0][0], m[0][1] * rhs.m[0][1], m[0][2] * rhs.m[0][2],
								   m[1][0] * rhs.m[1][0], m[1][1] * rhs.m[1][1], m[1][2] * rhs.m[1][2],
								   m[2][0] * rhs.m[2][0], m[2][1] * rhs.m[2][1], m[2][2] * rhs.m[2][2] );
			}

			Matrix3<T> operator/(const Matrix3<T> &rhs) const
			{
				return Matrix3<T>( m[0][0] / rhs.m[0][0], m[0][1] / rhs.m[0][1], m[0][2] / rhs.m[0][2],
								   m[1][0] / rhs.m[1][0], m[1][1] / rhs.m[1][1], m[1][2] / rhs.m[1][2],
								   m[2][0] / rhs.m[2][0], m[2][1] / rhs.m[2][1], m[2][2] / rhs.m[2][2] );
			}

			Matrix3<T>& operator=(const Matrix3<T> &rhs)
			{
				m[0][0] = rhs.m[0][0]; m[0][1] = rhs.m[0][1]; m[0][2] = rhs.m[0][2];
				m[1][0] = rhs.m[1][0]; m[1][1] = rhs.m[1][1]; m[1][2] = rhs.m[1][2];
				m[2][0] = rhs.m[2][0]; m[2][1] = rhs.m[2][1]; m[2][2] = rhs.m[2][2];
				return *this;
			}
	};

	template<typename T>
	Matrix3<T> matrixRotateX(const T &angle)
	{
        T c = cos(angle * PIOVER180);
	    T s = sin(angle * PIOVER180);
	    return Matrix3<T> ( T(1), 0, 0,
                            0,  T(c), T(-s),
                            0,  T(s), T(c) );
	}

    template<typename T>
	Matrix3<T> matrixRotateY(const T &angle)
	{
        T c = cos(angle * PIOVER180);
	    T s = sin(angle * PIOVER180);
	    return Matrix3<T> ( T(c), 0, T(s),
                            0, T(1), 0,
                            T(-s), 0, T(c) );
	}

	template<typename T>
	Matrix3<T> matrixRotateZ(const T &angle)
	{
	    T c = cos(angle * PIOVER180);
	    T s = sin(angle * PIOVER180);
	    return Matrix3<T> ( T(c), T(-s), 0,
                            T(s), T(c), 0,
                            0, 0, T(1));
	}

	template<typename T>
	Matrix3<T> matrixRotateArb(const T &angle, const T &x, const T &y, const T &z)
	{
	    T c = cos(angle * PIOVER180);
	    T s = sin(angle * PIOVER180);
        return Matrix3<T>( c + ((1 - c) * (x * x)), ((1 - c) * (x * y)) - (s * z), ((1 - c) * (x * z)) + (s * y),
                           ((1 - c) * (x * y)) + (s * z), c + ((1 - c) * (y * y)), ((1 - c) * (y * z)) - (s * x),
                           ((1 - c) * (x * z)) - (s * y), ((1 - c) * (y * z)) + (s * x), c + ((1 - c) * (z * z)) );
	}
};

#endif

