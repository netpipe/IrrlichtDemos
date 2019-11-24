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

#ifndef _PEAKENGINE_ENTITY_ENTITYMAP_HPP_
#define _PEAKENGINE_ENTITY_ENTITYMAP_HPP_

#include "../support/Mutex.hpp"

namespace peak
{
	class Entity;

	/**
	 * Custom container for entities, contains entity-id pairs in order to be
	 * able to quickly retrieve a certain Entity. For iterating through the
	 * entities, EntityMap::Iterator is used, example:
	 * @code
	 * // Create a map with some entities
	 * EntityMap map;
	 * int id0 = map.addEntity(entity0);
	 * entity0->setID(id0);
	 * map.addEntity(entity1);
	 * entity1->setID(id1);
	 * // Loop through all entities
	 * EntityMap::Iterator it(map);
	 * Entity *next = it.next();
	 * while (next)
	 * {
	 *     std::cout << "Entity: " << next->getID() << std::endl;
	 *     next = it.next();
	 * }
	 * @endcode
	 */
	class EntityMap
	{
		public:
			/**
			 * Constructor.
			 */
			EntityMap();
			/**
			 * Destructor.
			 */
			~EntityMap();

			/**
			 * Picks the next free ID and inserts the entity at this position.
			 * Entity::setID() is not called, this has to be done by the user!
			 * @return ID of the entity.
			 */
			unsigned int addEntity(Entity *entity);
			/**
			 * Adds an entity at a specific position, overwriting existing
			 * entites with this id.
			 */
			void addEntity(Entity *entity, unsigned int id);
			/**
			 * Removes the entity with the given ID from the container.
			 */
			void removeEntity(unsigned int id);
			/**
			 * Retrieves the entity with the given ID, returns 0 if this ID is
			 * currently unused.
			 */
			Entity *getEntity(unsigned int id);

			/**
			 * Returns the number of entities in the container.
			 */
			unsigned int getEntityCount();

			/**
			 * Class to iterate through the content of an EntityMap. If the map
			 * is changed after the iterator has been initialized, entities
			 * might be missed.
			 */
			class Iterator
			{
				public:
					/**
					 * Constructor.
					 */
					Iterator(EntityMap &map);
					/**
					 * Destructor.
					 */
					~Iterator();

					/**
					 * Returns the pointer to the next entity.
					 */
					Entity *next();
				private:
					EntityMap &map;
					int lastid;
			};
		private:
			unsigned int getID();

			Entity *entities[65535];
			unsigned short used1[256];
			unsigned int used2[8];
			unsigned int size;

			Mutex mutex;
	};
}

#endif
