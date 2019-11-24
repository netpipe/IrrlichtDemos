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

#ifndef _PEAKPHYSICS_COMPOUND_HPP_
#define _PEAKPHYSICS_COMPOUND_HPP_

#include "Shape.hpp"

#include "peakengine/support/Quaternion.hpp"

#include <vector>

namespace peak
{
	/**
	 * Compound collision shape.
	 */
	class Compound : public Shape
	{
		public:
			Compound();
			~Compound();

			/**
			 * Creates the compound shape
			 * @param dynamicaabbtree Is true if dynamic aabb tree should be used, false if not
			 */
			bool init(bool dynamicaabbtree, float mass = 0.0f);
			virtual bool destroy();

			/**
			 * Adds a shape to the compound
			 * @param child The shape that will be added
			 */
			void addShape(Shape *child, Vector3F offset = Vector3F(), Quaternion orientation = Quaternion());
			/**
			 * Removes the given child
			 * @param child The shape that will be removed
			 */
			void removeShape(Shape *child);

		private:
			std::vector<Shape*> childs;

	};
}

#endif
