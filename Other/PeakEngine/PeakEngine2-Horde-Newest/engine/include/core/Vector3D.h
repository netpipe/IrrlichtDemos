/*
Copyright (C) 2008   Mathias Gottschlag

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

#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

//tolua_begin
namespace peak
{
	/**
	 * \brief 3-dimensional vector class
	 */
	class Vector3D
	{
		public:
			Vector3D();
			Vector3D(float x, float y, float z);
			~Vector3D();

			/**
			 * \brief Rotates the vector around the y-axis.
			 * \param radians Rotation angle
			 */
			void rotateXZ(float radians);
			/**
			 * \brief Rotates the vector around the x-axis.
			 * \param radians Rotation angle
			 */
			void rotateYZ(float radians);
			/**
			 * \brief Rotates the vector around the z-axis.
			 * \param radians Rotation angle
			 */
			void rotateXY(float radians);
			
			/**
			 * \brief Rotates the vector.
			 * \param rotation Rotation in euler angles
			 */
			void rotate(Vector3D rotation);
			
			/**
			 * \brief Returns the rotation of the vector.
			 *
			 * If you would rotate the vector 0/0/1 by this rotation, it would have
			 * the same direction as this vector.
			 */
			Vector3D getRotation(void);
			
			/**
			 * \brief Returns the squared length of the vector
			 */
			float getLengthSq(void);
			
			float x;
			float y;
			float z;

			//tolua_end
			Vector3D &operator*=(float scale);
			Vector3D operator+(const Vector3D &v);
			Vector3D operator-(const Vector3D &v);
			Vector3D operator*(const float scale);
			//tolua_begin
	};
}
//tolua_end

#endif

