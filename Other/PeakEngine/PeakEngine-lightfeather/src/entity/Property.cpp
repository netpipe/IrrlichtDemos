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

#include "peakengine/entity/Property.hpp"
#include "peakengine/entity/Entity.hpp"
#include "peakengine/entity/EntityManager.hpp"

namespace peak
{
	Property::Property(Entity *entity) : dirty(false), entity(entity),
		changetime(0)
	{
	}
	Property::~Property()
	{
	}

	void Property::setDirty(bool dirty)
	{
		this->dirty = dirty;
	}
	bool Property::isDirty()
	{
		return dirty;
	}

	void Property::setLastChange(unsigned int time)
	{
		changetime = time;
	}
	unsigned int Property::getLastChange()
	{
		return changetime;
	}

	void Property::setChanged()
	{
		setLastChange(entity->getManager()->getTime());
	}
}
