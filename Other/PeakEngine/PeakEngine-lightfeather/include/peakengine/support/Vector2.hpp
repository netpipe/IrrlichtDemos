/*
Copyright (C) 2009  Mathias Gottschlag

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

#ifndef _PEAKENGINE_SUPPORT_VECTOR2_HPP_
#define _PEAKENGINE_SUPPORT_VECTOR2_HPP_

#include <string>
#include <cstdlib>
#include <cmath>

namespace peak
{
	/**
	 * Utility class for 2D vectors.
	 */
	template<typename T> class Vector2
	{
		public:
			/**
			 * Constructor.
			 */
			Vector2()
			{
				x = 0;
				y = 0;
			}
			/**
			 * Constructor.
			 */
			template <class T2> Vector2(T2 x, T2 y)
			{
				this->x = (T)x;
				this->y = (T)y;
			}
			/**
			 * Constructor.
			 */
			Vector2(const char *s)
			{
				set(s);
			}
			/**
			 * Constructor.
			 */
			Vector2(const std::string &s)
			{
				set(s);
			}
			/**
			 * Constructor.
			 */
			template<typename T2> Vector2(const Vector2<T2> &v)
			{
				x = (T)v.x;
				y = (T)v.y;
			}
			/**
			 * Destructor.
			 */
			~Vector2()
			{
			}

			template<typename T2> Vector2<T> operator*(T2 s) const
			{
				return Vector2<T>((T)(x * s),(T)(y * s));
			}
			template<typename T2> Vector2<T> &operator*=(T2 s)
			{
				x = x * s;
				y = y * s;
				return *this;
			}
			template<typename T2> Vector2<T> operator/(T2 s) const
			{
				return Vector2<T>((T)(x / s),(T)(y / s));
			}
			template<typename T2> Vector2<T> &operator/=(T2 s)
			{
				x = x / s;
				y = y / s;
				return *this;
			}
			template<typename T2> Vector2<T> operator+(const Vector2<T2> &v) const
			{
				return Vector2<T>((T)(x + v.x),(T)(y + v.y));
			}
			template<typename T2> Vector2<T> operator-(const Vector2<T2> &v) const
			{
				return Vector2<T>((T)(x - v.x),(T)(y - v.y));
			}
			template<typename T2> Vector2<T> &operator=(const Vector2<T2> &v)
			{
				x = (T)v.x;
				y = (T)v.y;
				return *this;
			}
			Vector2<T> &operator=(std::string s)
			{
				set(s);
				return *this;
			}
			Vector2<T> &operator=(const char *s)
			{
				set(s);
				return *this;
			}
			template<typename T2> Vector2<T> &operator+=(const Vector2<T2> &v)
			{
				x += v.x;
				y += v.y;
				return *this;
			}
			template<typename T2> Vector2<T> &operator-=(const Vector2<T2> &v)
			{
				x -= v.x;
				y -= v.y;
				return *this;
			}
			template<typename T2> bool operator==(const Vector2<T2> &v) const
			{
				return x == v.x && y == v.y;
			}
			template<typename T2> bool operator!=(const Vector2<T2> &v) const
			{
				return x != v.x || y != v.y;
			}

			void set(const std::string &s)
			{
				size_t separator = s.find("/");
				if (separator == std::string::npos)
				{
					x = 0;
					y = 0;
				}
				else
				{
					x = (T)atof(s.c_str());
					const char *ys = s.c_str() + separator + 1;
					y = (T)atof(ys);
				}
			}

			/**
			 * Returns the squared length of the vector. Much less
			 * computationally expensive than getLength(). Good for comparing
			 * vector lengths.
			 */
			float getLengthSquared()
			{
				return x * x + y * y;
			}
			/**
			 * Returns the length of the vector.
			 */
			float getLength()
			{
				return sqrt(getLengthSquared());
			}

			/**
			 * Returns the angle between (0, 1) and this vector in degrees.
			 */
			float getRotation()
			{
				return atan2(y, x) * 180 / 3.1415;
			}

			/**
			 * Rotates the vector around (0, 0).
			 */
			void rotate(float angle)
			{
				angle *= 3.1415 / 180;
				T newx = (T)((float)x * cos(angle) - (float)y * sin(angle));
				T newy = (T)((float)x * sin(angle) + (float)y * cos(angle));
				x = newx;
				y = newy;
			}

			/**
			 * X coordinate of the vector.
			 */
			T x;
			/**
			 * Y coordinate of the vector.
			 */
			T y;
	};

	typedef Vector2<int> Vector2I;
	typedef Vector2<float> Vector2F;
}

#endif
