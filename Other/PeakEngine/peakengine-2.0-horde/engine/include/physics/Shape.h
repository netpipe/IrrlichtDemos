/*
Copyright (C) 2008   Christian Reiser, Mathias Gottschlag

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

#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "core/Vector3D.h"

class btCollisionShape;
class btTransform;
class btVector3;

//tolua_begin
namespace peak
{
	/**
	 * \brief Physical shape of an object, used for collision detection
	 */
	class Shape
	{
		public:
			Shape();
			~Shape();
			
			/**
			 * \brief Destroys the shape
			 */
			virtual bool destroy(void);
			
			/**
			 * \brief Returns the mass of the shape
			 */
			float getMass();
			/**
			 * \brief Returns the inertia of the shape
			 */
			Vector3D getInertia();
			
			/**
			 * \brief Returns the radius of the bounding sphere of the shape.
			 */
			void getBoundingSphere(float& radius, Vector3D& center);
			
			//tolua_end
			/**
			 * \brief Returns a pointer to the btCollisionShape
			 */
			btCollisionShape *getShape(void);
			/**
			 * \brief Returns the position/rotation of the shape
			 */
			btTransform getTransform(void);
		protected:
			btCollisionShape *shape;
			btTransform *transform;
			float mass;
			Vector3D inertia;
			//tolua_begin
	};
}
//tolua_end

#endif
