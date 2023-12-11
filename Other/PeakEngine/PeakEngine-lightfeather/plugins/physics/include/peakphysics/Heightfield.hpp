/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKPHYSICS_HEIGHTFIELD_HPP_
#define _PEAKPHYSICS_HEIGHTFIELD_HPP_

#include "Shape.hpp"

#include <string>

class btHeightfieldTerrainShape;

namespace peak
{
	/**
	 * Height field collision shape.
	 */
	class Heightfield : public Shape
	{
		public:
			Heightfield();
			~Heightfield();

			/**
			 * Creates the height field.
			 * @param heightmap Heightmap texture.
			 * @param mass Mass of the heightfield (0 = static)
			 * @param scale Size of the heightfield.
			 */
			bool init(std::string heightmap, float mass = 0.0f,
				Vector3F scale = Vector3F(1, 1, 1));
			virtual bool destroy();
		private:
			btHeightfieldTerrainShape *terrain;
	};
}

#endif
