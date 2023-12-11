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

#include "peakengine/entity/EntityManager.hpp"

namespace peak
{
	EntityManager::EntityManager(Engine *engine) : engine(engine)
	{
	}
	EntityManager::~EntityManager()
	{
	}

	void EntityManager::addEntity(Entity *entity)
	{
		// Add entity to the entity list
		if (!entity->getID())
		{
			unsigned int id = entities.addEntity(entity);
			if (!id)
			{
				// TODO: Assert?
			}
			entity->setID(id);
		}
		else
		{
			entities.addEntity(entity, entity->getID());
		}
	}
	void EntityManager::removeEntity(Entity *entity)
	{
		entities.removeEntity(entity->getID());
	}
	Entity *EntityManager::getEntity(int id)
	{
		return entities.getEntity(id);
	}

	Engine *EntityManager::getEngine()
	{
		return engine;
	}

	void EntityManager::update()
	{
		EntityMap::Iterator it(entities);
		for (Entity *entity = it.next();entity != 0; entity = it.next())
		{
			entity->update();
		}
	}
}
