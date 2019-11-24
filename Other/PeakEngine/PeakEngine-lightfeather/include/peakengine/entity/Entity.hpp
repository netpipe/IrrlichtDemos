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

#ifndef _PEAKENGINE_ENTITY_ENTITY_HPP_
#define _PEAKENGINE_ENTITY_ENTITY_HPP_

#include "../support/Buffer.hpp"

#include <vector>
#include <string>

namespace peak
{
	class EntityManager;
	class Property;
	class Buffer;

	/**
	 * Dynamic game object, this can be everything from the player to objects
	 * like doors or a scoreboard. The user entity is not derived from this
	 * class but rather from ServerEntity or ClientEntity, depending on where
	 * the code is supposed to be run. See also EntityFactory and
	 * Game::registerFactory() for information on how to register entity types
	 * in the engine.
	 *
	 * Entities are synchronized over the network via Property, properties are
	 * added via addProperty() and the server is in control over the values of
	 * the properties.
	 *
	 * There also are Properties which are controlled by the client and can be
	 * used for example input, use addClientProperty() for these (the properties
	 * must be added only on the server and on the client which owns the
	 * entity).
	 */
	class Entity
	{
		public:
			/**
			 * Constructor.
			 * @param manager EntityManager into which this entity will get
			 * inserted later. This does not insert the entity into the manager
			 * yet!
			 */
			Entity(EntityManager *manager);
			/**
			 * Destructor.
			 */
			virtual ~Entity();

			bool hasChanged(unsigned int time);
			void setState(Buffer *buffer);
			void getState(Buffer *buffer);
			void applyUpdate(Buffer *buffer, unsigned int time);
			void getUpdate(Buffer *buffer, unsigned int time);

			bool hasChangedClient(unsigned int time);
			void applyClientUpdate(Buffer *buffer, unsigned int time);
			void getClientUpdate(Buffer *buffer, unsigned int time);

			/**
			 * Saves a backup of all properties. This backup will be applied
			 * before the next server update is applied. This has to be called
			 * on the server before modifying the properties (e.g. for client
			 * predition).
			 */
			void createBackup();
			/**
			 * Returns true if there has been a backup since the last server
			 * update.
			 */
			bool hasBackup();
			/**
			 * Applies the backup. Usually only called by Client when a server
			 * update arrives. The backup stays valid, discardBackup() is not
			 * called by this function.
			 */
			void applyBackup();
			/**
			 * Drops the backup without applying it.
			 */
			void discardBackup();

			/**
			 * Returns the manager which contains this entity.
			 */
			EntityManager *getManager();

			/**
			 * Sets the ID of the entity. Must not be called by the user. IDs
			 * are automatically created in Server.
			 */
			void setID(unsigned int id);
			/**
			 * Returns the ID of the entity.
			 */
			unsigned int getID();

			/**
			 * Callback which is called whenever the entity received an entity
			 * message (via ServerEntity::sendMessage() or
			 * ClientEntity::sendMessage()).
			 */
			virtual void receiveMessage(Buffer *buffer) = 0;

			/**
			 * Returns a string with the name of the entity (e.g. "player").
			 * Must be implemented by the user and is used for entity
			 * replication on the client.
			 */
			virtual std::string getType() = 0;

			/**
			 * Updates the entity. Can be implemented by the user.
			 */
			virtual void update();

			/**
			 * Adds a property to the property list. Properties have to be added
			 * in the same order on both server and client! Also, there must
			 * not be different counts or types of properties.
			 */
			void addProperty(Property *property);
			/**
			 * Adds a property to the client property list. This has to be
			 * called for all client properties on the server and on the client
			 * owning the entity.
			 */
			void addClientProperty(Property *property);
		private:
			std::vector<Property*> properties;
			std::vector<Property*> clientproperties;
			EntityManager *manager;
			unsigned int id;

			BufferPointer backup;
			bool backupvalid;
	};
}

#endif
