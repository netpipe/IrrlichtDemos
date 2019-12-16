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

#ifndef _PEAKENGINE_ENTITY_PROPERTY_HPP_
#define _PEAKENGINE_ENTITY_PROPERTY_HPP_

#include "../support/Buffer.hpp"

namespace peak
{
	class Entity;

	class Property
	{
		public:
			Property(Entity *entity);
			virtual ~Property();

			virtual void serialize(BufferPointer buffer) = 0;
			virtual void deserialize(BufferPointer buffer) = 0;

			virtual bool hasChanged() = 0;

			void setDirty(bool dirty);
			bool isDirty();

			void setLastChange(unsigned int time);
			unsigned int getLastChange();
		protected:
			void setChanged();
		private:
			bool dirty;
			Entity *entity;
			unsigned int changetime;
	};
}

#endif
