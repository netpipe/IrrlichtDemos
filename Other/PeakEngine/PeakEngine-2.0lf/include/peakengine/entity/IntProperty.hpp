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

#ifndef _PEAKENGINE_ENTITY_INTPROPERTY_HPP_
#define _PEAKENGINE_ENTITY_INTPROPERTY_HPP_

#include "Property.hpp"

namespace peak
{
	class IntProperty : public Property
	{
		public:
			IntProperty(Entity *entity);
			virtual ~IntProperty();

			void init(int defaultval, unsigned int bits = 32);

			virtual void serialize(BufferPointer buffer);
			virtual void deserialize(BufferPointer buffer);

			virtual bool hasChanged();

			void set(int value);
			int get();
		private:
			int defaultval;
			int value;
			unsigned int bits;
	};
}

#endif
