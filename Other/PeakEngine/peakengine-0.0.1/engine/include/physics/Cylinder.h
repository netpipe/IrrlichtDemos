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

#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "physics/Geometry.h"
#include "core/Quaternion.h"

//tolua_begin
namespace peak
{
	/**
	 * \brief Class representing a solid cylinder.
	 */
	class Cylinder: public Geometry
	{
		public:
			Cylinder();
			virtual ~Cylinder();
			
			/**
			 * \brief Initializes the cylinder.
			 * \param world World to insert the cylinder into
			 * \param radius Radius of the cylinder
			 * \param length Length of the cylinder
			 */
			bool init(World *world, float radius, float length);
			/**
			 * \brief Initializes the cylinder and instantly applies a rotation.
			 * \param world World to insert the cylinder into
			 * \param radius Radius of the cylinder
			 * \param length Length of the cylinder
			 * \param rot Rotation
			 */
			bool init(World *world, float radius, float length, Quaternion rot);
			virtual bool destroy(void);
		private:
			//tolua_end
			//tolua_begin
	};
}
//tolua_end

#endif

