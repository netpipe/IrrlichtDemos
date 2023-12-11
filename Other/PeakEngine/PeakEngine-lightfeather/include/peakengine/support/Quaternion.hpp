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
/*
Parts taken from the lightfeather 3d engine
Copyright 2005-2008 by Lightfeather-Team
Originally written by Nikolaus Gebhardt for the Irrlicht engine

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications,
and to alter it and redistribute it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.
       If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _PEAKENGINE_SUPPORT_QUATERNION_HPP_
#define _PEAKENGINE_SUPPORT_QUATERNION_HPP_

#include "Vector3.hpp"

namespace peak
{
	/**
	 * Quaternion class used to store rotation data.
	 */
	class Quaternion
	{
		public:
			/**
			 * Constructor.
			 */
			Quaternion()
			{
				q[0] = 0;
				q[1] = 0;
				q[2] = 0;
				q[3] = 1;
			}
			/**
			 * Constructor.
			 */
			Quaternion(float x, float y, float z, float w)
			{
				q[0] = x;
				q[1] = y;
				q[2] = z;
				q[3] = w;
			}
			/**
			 * Constructor.
			 */
			Quaternion(const Vector3F &euler)
			{
				/*
				 * Taken from the lightfeather 3d engine
				 */
				Vector3F euler2 = euler * (3.1415 / 180.0);
				float angle;

				angle = euler2.x * 0.5f;
				float sr = (float)sin(angle);
				float cr = (float)cos(angle);

				angle = euler2.y * 0.5f;
				float sp = (float)sin(angle);
				float cp = (float)cos(angle);

				angle = euler2.z * 0.5f;
				float sy = (float)sin(angle);
				float cy = (float)cos(angle);

				float cpcy = cp * cy;
				float spcy = sp * cy;
				float cpsy = cp * sy;
				float spsy = sp * sy; //quaternion fix by jox

				q[0] = sr * cpcy - cr * spsy;
				q[1] = cr * spcy + sr * cpsy;
				q[2] = cr * cpsy - sr * spcy;
				q[3] = cr * cpcy + sr * spsy;

				normalize();
			}
			/**
			 * Copy constructor.
			 */
			Quaternion(const Quaternion &other)
			{
				q[0] = other.q[0];
				q[1] = other.q[1];
				q[2] = other.q[2];
				q[3] = other.q[3];
			}

			/**
			 * Normalizes the quaternion.
			 */
			void normalize()
			{
				float s = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
				*this /= sqrtf(s);
			}
			/**
			 * Sets the quaternion to the linear interpolation between a and b.
			 * \param a First quaternion
			 * \param b Second quaternion
			 * \param d Interpolation factor. If it is 0, then the quaternion
			 * will be set to a, if it is 1, the quaternion will be b.
			 */
			Quaternion &interpolate(const Quaternion &a, const Quaternion &b,
				float d)
			{
				Quaternion dv = b - a;
				*this = a + dv * d;
				return *this;
			}

			Quaternion operator*(float s) const
			{
				return Quaternion(q[0] * s, q[1] * s, q[2] * s, q[3] * s);
			}
			Quaternion &operator*=(float s)
			{
				q[0] = q[0] * s;
				q[1] = q[1] * s;
				q[2] = q[2] * s;
				q[3] = q[3] * s;
				return *this;
			}
			Quaternion operator/(float s) const
			{
				return Quaternion(q[0] / s, q[1] / s, q[2] / s, q[3] / s);
			}
			Quaternion &operator/=(float s)
			{
				q[0] = q[0] / s;
				q[1] = q[1] / s;
				q[2] = q[2] / s;
				q[3] = q[3] / s;
				return *this;
			}
			Quaternion operator+(const Quaternion &other) const
			{
				return Quaternion(q[0] + other.q[0], q[1] + other.q[1],
					q[2] + other.q[2], q[3] + other.q[3]);
			}
			Quaternion &operator+=(const Quaternion &other)
			{
				q[0] = q[0] + other.q[0];
				q[1] = q[1] + other.q[1];
				q[2] = q[2] + other.q[2];
				q[3] = q[3] + other.q[3];
				return *this;
			}
			Quaternion operator-(const Quaternion &other) const
			{
				return Quaternion(q[0] - other.q[0], q[1] - other.q[1],
					q[2] - other.q[2], q[3] - other.q[3]);
			}
			Quaternion &operator-=(const Quaternion &other)
			{
				q[0] = q[0] - other.q[0];
				q[1] = q[1] - other.q[1];
				q[2] = q[2] - other.q[2];
				q[3] = q[3] - other.q[3];
				return *this;
			}
			Quaternion &operator=(const Quaternion &other)
			{
				for (unsigned int i = 0; i < 4; i++)
					q[i] = other.q[i];
				return *this;
			}
			bool operator==(const Quaternion &other) const
			{
				for (unsigned int i = 0; i < 4; i++)
					if (other.q[i] != q[i])
						return false;
				return true;
			}
			bool operator!=(const Quaternion &other) const
			{
				return !(*this == other);
			}

			float q[4];
	};
}

#endif
