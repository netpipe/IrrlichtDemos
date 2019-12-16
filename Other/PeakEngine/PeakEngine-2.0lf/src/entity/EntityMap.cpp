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

#include "peakengine/entity/EntityMap.hpp"
#include "peakengine/support/ScopedLock.hpp"

namespace peak
{
	EntityMap::EntityMap() : size(0)
	{
		// Empty map
		for (unsigned int i = 0; i < 65535; i++)
			entities[i] = 0;
		for (unsigned int i = 0; i < 256; i++)
			used1[i] = 0;
		for (unsigned int i = 0; i < 8; i++)
			used2[i] = 0;
	}
	EntityMap::~EntityMap()
	{
	}

	unsigned int EntityMap::addEntity(Entity *entity)
	{
		ScopedLock lock(mutex);
		// Insert entity
		unsigned int id = getID();
		if (id == 65535)
			return 0;
		entities[id] = entity;
		// Increment counters
		used1[id / 256]++;
		used2[id / 8192]++;
		size++;
		return id + 1;
	}
	void EntityMap::addEntity(Entity *entity, unsigned int id)
	{
		id -= 1;
		ScopedLock lock(mutex);
		// Insert entity
		if (entities[id])
		{
			// Simply replace it
			entities[id] = entity;
			return;
		}
		entities[id] = entity;
		// Increment counters
		used1[id / 256]++;
		used2[id / 8192]++;
		size++;
	}
	void EntityMap::removeEntity(unsigned int id)
	{
		ScopedLock lock(mutex);
		if (id == 0)
			return;
		id--;
		if (!entities[id])
			return;
		// Remove entity
		entities[id] = 0;
		// Decrement counters
		used1[id / 256]--;
		used2[id / 8192]--;
		size--;
	}
	Entity *EntityMap::getEntity(unsigned int id)
	{
		ScopedLock lock(mutex);
		if (id == 0)
			return 0;
		id--;
		return entities[id];
	}

	unsigned int EntityMap::getEntityCount()
	{
		return size;
	}

	EntityMap::Iterator::Iterator(EntityMap &map) : map(map), lastid(-1)
	{
	}
	EntityMap::Iterator::~Iterator()
	{
	}

	Entity *EntityMap::Iterator::next()
	{
		ScopedLock lock(map.mutex);
		// Do not return the same entity again
		lastid++;
		if (lastid >= 65535)
			return 0;
		while (true)
		{
			// Groups empty?
			while (map.used2[lastid / 8192] == 0)
			{
				lastid += 8192 - (lastid % 8192);
				if (lastid >= 65535)
					return 0;
			}
			while (map.used1[lastid / 256] == 0)
			{
				lastid += 256 - (lastid % 256);
				if (lastid >= 65535)
					return 0;
			}
			// Find the next entity from this position
			do
			{
				if (map.entities[lastid])
					return map.entities[lastid];
				lastid++;
				if (lastid >= 65535)
					return 0;
			}
			while (lastid % 256);
		}
	}

	unsigned int EntityMap::getID()
	{
		// Walk over the table in large steps
		unsigned int i2, i1, id;
		for (i2 = 0; i2 < 8; i2++)
		{
			if (used2[i2] < 8192)
				break;
		}
		// We have one entry less than the used tables pretend to have
		if (i2 == 7 && used2[7] == 8191)
			return 65535;
		// Find a free 256-entity-group
		for (i1 = 0; i1 < 32; i1++)
		{
			if (used1[i2 * 32 + i1] < 256)
				break;
		}
		// Find a free ID
		for (id = 0; id < 256; id++)
		{
			if (!entities[i2 * 8192 + i1 * 256 + id])
				return i2 * 8192 + i1 * 256 + id;
		}
		// We should never end up here
		// TODO: Assert?
		return 65535;
	}
}
