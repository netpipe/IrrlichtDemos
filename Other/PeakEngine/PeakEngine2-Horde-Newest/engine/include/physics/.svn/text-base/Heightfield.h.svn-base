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

#ifndef _HEIGHTFIELD_H_
#define _HEIGHTFIELD_H_

#include <string>
#include "core/Vector3D.h"
#include "physics/Shape.h"

class btHeightfieldTerrainShape;

//tolua_begin
namespace peak
{
	/**
	 * \brief Heightfield terrain collision shape
	 */
	class Heightfield : public Shape
	{
		public:
			Heightfield();
			~Heightfield();

			/**
			 * \brief Creates the heightfield
			 * \param heightmap Heightmap texture
			 * \param mass Mass of the heightfield (0 = static)
			 * \param scale Size of the heightfield
			 */
			bool create(std::string heightmap, float mass = 0.0f, Vector3D scale = Vector3D(1, 1, 1));
			virtual bool destroy();
			//tolua_end
		private:
			btHeightfieldTerrainShape *terrain;
			//tolua_begin
	};
}
//tolua_end

#endif
