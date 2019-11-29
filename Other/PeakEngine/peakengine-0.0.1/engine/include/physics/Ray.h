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

#ifndef _RAY_H_
#define _RAY_H_

#include "physics/Geometry.h"
#include "core/Vector3D.h"

//tolua_begin
namespace peak
{
	/**
	 * \brief Simple ray, useful for manual raytracing.
	 *
	 * Cannot be used for automatic collision detection.
	 */
	class Ray: public Geometry
	{
		public:
			Ray();
			virtual ~Ray();
			
			/**
			 * \brief Initiates the ray with a given direction/length.
			 *
			 * The direction vector actually changes the rotation of the ray.
			 * \param world World into that the ray is inserted
			 * \param direction Direction into which the ray points
			 * \param length Length of the ray
			 */
			bool init(World *world, Vector3D direction, float length);
			virtual bool destroy(void);
		private:
			//tolua_end
			//tolua_begin
	};
}
//tolua_end

#endif

