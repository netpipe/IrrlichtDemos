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
			 * \brief Creates an entity (server only)
			 * \param name Name of the entity
			 * \param owner ID of the owner of the entity
			 */
			Entity *createEntity(std::string name, int owner);
			/**
			 * \brief Returns the entity with the given ID
			 */
			Entity *getEntity(int id);
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
			 * \brief Injects an action event.
			 */
			void injectAction(std::string action, bool state);
			/**
			 * \brief Injects a mouse movement.
			 */
			void injectMouseMovement(float x, float y, float z);
			
			/**
			 * \brief Registers an entity for a specific action.
			 *
			 * The function "action_input(action, state)" in the client script is
			 * then called whenever the action is triggered, with action being the
			 * action name and state being either 1 or 0.
			 */
			void registerEntityActionInput(Entity *entity, std::string action);
			/**
			 * \brief Registers an entity for mouse events.
			 *
			 * The function "mouse_moved(x, y, z)" is then called, with x, y, and z
			 * being the relative mouse movement.
			 */
			void registerEntityMouseInput(Entity *entity);
			/**
			 * \brief Returns the entity which is registered for a specific action.
			 */
			Entity *getRegisteredEntity(std::string action);
			
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
		private:
			//tolua_end
			Game();
			
			bool parseServerData(Buffer *data);
			bool sendClientUpdates(float msecs);
			
			bool sendServerUpdates(float msecs);
			bool parseClientData(int conn, Buffer *data);
			void sendDataToClients(Buffer *data, bool reliable = false);
			void destroyEntities(int conn);
			
			void increasePriority(void);
			
			bool server;
			bool client;
			Script *serverscript;
			NetworkHost *nethost;
			std::map<int, Connection*> serverconns;
			Script *clientscript;
			NetworkClient *netclient;
			Connection *clientconn;
			
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

