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

#ifndef _HEIGHTFIELD_H_
#define _HEIGHTFIELD_H_

#include <string>

#include "physics/Geometry.h"

//tolua_begin
namespace peak
{
	class Image;
	/**
	 * \brief Class representing a heightfield based on a heightmap.
	 */
	class Heightfield: public Geometry
	{
		public:
			Heightfield();
			~Heightfield();
			
			/**
			 * \brief Creates a heightfield.
			 *
			 * The heightmap must be 2^x + 1 pixels big (257*257, 129*129 etc.). 
			 * \param world World to place the heightfield into
			 * \param heightmap Path to heightmap image
			 * \param scale Scale of the heightfield
			 */
			bool init(World *world, std::string heightmap, Vector3D scale);
			virtual bool destroy(void);
			
			/**
			 * \brief Returns the loaded heightmap image.
			 *
			 * For internal use only.
			 */
			Image *getHeightmap(void);
			/**
			 * \brief Returns the scale of the heightfield.
			 */
			Vector3D getScale(void);
		private:
			//tolua_end
			Image *heightmapimage;
			
			Vector3D scale;
			//tolua_begin
	};
}
//tolua_end

#endif

