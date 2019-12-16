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

#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "core/Vector3D.h"

//tolua_begin
namespace peak
{
	/**
	 * \brief Quaternion class used for rotation
	 */
	class Quaternion
	{
		public:
			Quaternion();
			Quaternion(float x, float y, float z, float w);
			Quaternion(float x, float y, float z);
			Quaternion(Vector3D euler);
			Quaternion(float *q);
			/**
			 * \brief Converts the rotation to euler format.
			 */
			Vector3D getEuler(void);
			
			/**
			 * \brief Rotates a Vector3D.
			 */
			Vector3D rotateVector(Vector3D vector);
			
			/**
			 * \brief Interpolates between this quaternion and another.
			 * \param other Other quaternion
			 * \param value Sets how much of both quaternions should be used.
			 * 0 means 100% of this quaternion, 1 means 100% of the other quaternion.
			 */
			Quaternion slerp(Quaternion other, float value);
			
			/**
			 * \brief Returns the total difference to another quaternion.
			 *
			 * This function is only used to find out whether a rotation has
			 * changed.
			 */
			float getDifference(Quaternion &q2);
			
			/**
			 * \brief The four components of the quaternion.
			 */
			float x, y, z, w;
			
			//tolua_end
			Quaternion &operator*=(Quaternion other);
			Quaternion operator*(Quaternion other);
			//tolua_begin
			
	};
}
//tolua_end

#endif

