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

#ifndef _PEAKPHYSICS_CAPSULE_HPP_
#define _PEAKPHYSICS_CAPSULE_HPP_

#include "Shape.hpp"

namespace peak
{
	/**
	 * Capsule collision shape.
	 */
	class Capsule : public Shape
	{
		public:
			Capsule();
			~Capsule();

			/**
			 * Creates the capsule.
			 * @param radius Radius of the spheres at the end of the capsule.
			 * @param height Height of the cylinder in the middle of the
			 * capsule.
			 * @param mass Mass of the box (0 = static).
			 */
			bool init(float radius, float height, float mass = 0.0f);
	};
}

#endif
