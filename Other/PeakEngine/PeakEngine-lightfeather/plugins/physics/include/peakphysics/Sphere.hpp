/*
Copyright (c) 2009-2010, Mathias Gottschlag, Christian Reiser

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

#ifndef _PEAKPHYSICS_SPHERE_HPP_
#define _PEAKPHYSICS_SPHERE_HPP_

#include "Shape.hpp"

namespace peak
{
	/**
	 * Sphere collision shape.
	 */
	class Sphere : public Shape
	{
		public:
			Sphere();
			~Sphere();

			/**
			 * Creates the sphere
			 * @param radius Radius of the sphere
			 * @param mass Mass of the sphere (0 = static)
			 */
			bool init(float radius, float mass = 0.0f);
	};
}

#endif
