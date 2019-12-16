/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _GAME_H_
#define _GAME_H_

#include "core/Address.h"
#include "core/Vector3D.h"

#define GAME_PORT 60123

#include <list>
#include <map>
#include <vector>

//tolua_begin
namespace peak
{
	class World;
	class Script;
	class NetworkHost;
	class NetworkClient;
	class Entity;
	class Connection;
	class BroadcastHost;
	class Buffer;
	
	/**
	 * \brief Holds the basic info about a client on the server
	 */
	struct ClientInfo
	{
		ClientInfo(Connection *conn, int owner, Vector3D focus)
		{
			this->conn = conn;
			this->owner = owner;
			this->focus = focus;
		}
		Connection *conn;
		int owner;
		Vector3D focus;
	};
	
	/**
	 * \brief Class holding the game itself
	 */
	class Game
	{
		public:
			/**
			 * \brief Returns pointer to the Game class.
			 *
			 * \return Pointer to the game
			 */
			static Game *get(void);
			~Game();
			
			/**
			 * \brief Initializes the game as a local server.
			 * \param client If true, also the client will be started
			 */
			bool init(bool client = true);
			/**
			 * \brief Connects to a remote server
			 * \param addr Address of the server
			 */
			bool init(Address addr);
			/**
			 * \brief Stops the game
			 *
			 * Do not use this function directly from scripts.
			 */
			bool shutdown(void);
			/**
			 * \brief Stops the game
			 *
			 * This function does not do anything at once and is therefore safe
			 * to be used within scripts.
			 */
			void stopGame(void);
			
			/**
			 * \brief Returns whether the game is a local server
			 */
			bool isServer(void);
			/**
			 * \brief Returns whether the game holds client data
			 */
			bool isClient(void);
			
			/**
			 * \brief Sets whether the server is visible via broadcasting
			 *
			 * Defaults to false.
			 */
			void setVisible(bool visible);
			/**
			 * \brief Returns whether the server is visible via broadcasting
			 */
			bool isVisible(void);
			
			/**
			 * \brief Sets the position of the client
			 * \todo Unimplemented
			 */
			void setFocus(Vector3D focus);
			/**
			 * \brief Returns the position of the client
			 * \todo Unimplemented
			 */
			Vector3D getFocus(void);
			
			/**
			 * \brief Returns the number of clients connected to the server
			 */
			int getClientCount(void);
			/**
			 * \brief Returns the position of the client
			 * \todo Unimplemented
			 */
			Vector3D getClientFocus(int client);
			/**
			 * \brief Returns the connection to the client
			 *
			 * \param index Index of the client
			 */
			Connection *getClientConnection(int client);
			
			/**
			 * \brief Returns the connection to the server
			 */
			Connection *getServerConnection(void);
			
			/**
			 * \brief Creates an entity (server only)
			 * \param name Name of the entity
			 * \param owner ID of the owner of the entity
			 */
			Entity *createEntity(std::string name, int owner);
			/**
			 * \brief Returns the entity with the given ID
			 */
			Entity *getEntity(int id);
			/**
			 * \brief Deletes an entity
			 * \param entity Entity to delete
			 * \param now If set to true, the entity is deleted immediately, if not,
			 * it is deleted after the next frame
			 */
			bool deleteEntity(Entity *entity, bool now = false);
			
			/**
			 * \brief Returns the instance number of a given entity type.
			 * \param name Name of the entity
			 */
			int getEntityCount(std::string name);
			/**
			 * \brief Finds an entity with the given name.
			 * \param name Name of the entity
			 * \param index Index of the entity. Must be in the range 0..
			 * getEntityCount(name) - 1.
			 */
			Entity *getEntity(std::string name, int index);
			
			/**
			 * \brief Returns the world used for game physics
			 */
			World *getWorld(void);
			
			/**
			 * \brief Updates the game
			 */
			void doWork(float msecs);
			
			/**
			 * \brief Returns the server script
			 */
			Script *getServerScript(void);
			/**
			 * \brief Returns the client script
			 */
			Script *getClientScript(void);
			//tolua_end
			/**
			 * \brief Sends some data to all clients
			 */
			void sendDataToClients(Buffer *data, bool reliable = false);
		private:
			Game();
			
			bool parseServerData(Buffer *data);
			
			bool sendServerUpdates(float msecs);
			bool parseClientData(int conn, Buffer *data);
			void destroyEntities(int conn);
			
			void increasePriority(void);
			
			bool server;
			bool client;
			Script *serverscript;
			NetworkHost *nethost;
			std::vector<ClientInfo*> serverconns;
			Script *clientscript;
			NetworkClient *netclient;
			Connection *clientconn;
			
			Vector3D focus;
			
			BroadcastHost *bchost;
			
			std::vector<std::string> actions;
			std::vector<Entity*> actionreceivers;
			Entity *mousereceiver;
			
			float timesinceupdate;
			
			bool stopgame;
			
			std::list<Entity*> entities;
			std::list<Entity*> deleting;
			World *world;
			
			static int lastclientid;
			//tolua_begin
	};
}
//tolua_end

#endif

