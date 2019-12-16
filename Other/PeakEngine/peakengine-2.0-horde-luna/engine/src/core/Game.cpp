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

#include "core/Game.h"
#include "core/Script.h"
#include "core/GameEngine.h"
#include "core/Entity.h"
#include "core/SettingsManager.h"
#include "network/NetworkHost.h"
#include "network/NetworkClient.h"
#include "network/BroadcastHost.h"
#include "network/Connection.h"
#include "physics/World.h"
#include "physics/Callback.h"
#include "core/NetworkData.h"
#include "core/Logger.h"
#include "core/Level.h"
#include "graphics/MeshSceneNode.h"

namespace peak
{
	Game *Game::get(void)
	{
		static Game game;
		return &game;
	}
	Game::~Game()
	{
	}
	
	bool Game::init(bool client)
	{
		// Create network host
		nethost = new NetworkHost;
		if (!nethost->init(GAME_PORT))
		{
			LERROR("Could not create server.\n");
			delete nethost;
			nethost = 0;
			return false;
		}
		// Create physics world
		world = new World;
		world->init();
		
		Level::get()->init(SettingsManager::get()->getString("server.level"), true, client);
		
		// Run server script
		server = true;
		this->client = client;
		std::string filename = GameEngine::get()->getRootDirectory() + "/" + SettingsManager::get()->getString("scripts.server");
		serverscript = new Script;
		serverscript->run(filename);
		serverscript->callFunction("create_server");
		if (client)
		{
			// Run client script
			clientscript = new Script;
			std::string filename = GameEngine::get()->getRootDirectory() + "/" + SettingsManager::get()->getString("scripts.client");
			clientscript->run(filename);
			clientscript->callFunction("create_client");
		}
		
		stopgame = false;
		timesinceupdate = 0;
		
		return true;
	}
	bool Game::init(Address addr)
	{
		// Fill in standard port
		if (addr.getPort() == 0) addr.setPort(GAME_PORT);
		// Connect over network
		netclient = new NetworkClient;
		clientconn = netclient->init(addr);
		if (!clientconn)
		{
			LERROR("Could not connect.\n");
			delete netclient;
			netclient = 0;
			return false;
		}
		// Create physics world
		world = new World;
		world->init();
		// Get level from server
		Buffer *levelmsg = 0;
		while (!levelmsg)
		{
			netclient->doWork();
			levelmsg = clientconn->readData();
		}
		std::string levelname = levelmsg->readString();
		delete levelmsg;
		
		Level::get()->init(levelname, false, true);
		
		// Run client script
		this->client = true;
		clientscript = new Script;
		std::string filename = GameEngine::get()->getRootDirectory() + "/" + SettingsManager::get()->getString("scripts.client");
		clientscript->run(filename);
		clientscript->callFunction("create_client");
		
		stopgame = false;
		
		
		return true;
	}
	bool Game::shutdown(void)
	{
		Level::get()->destroy();
		
		if (!server && !client) return true;
		
		actionreceivers.clear();
		actions.clear();
		
		if (bchost)
		{
			bchost->shutdown();
			delete bchost;
			bchost = 0;
		}
		
		// Destroy network client/server
		if (server)
		{
			nethost->shutdown();
			delete nethost;
			serverconns.clear();
			nethost = 0;
			server = false;
			client = false;
		}
		else if (client)
		{
			// Disconnect
			Buffer msg;
			msg.writeByte(ENM_Disconnect);
			//clientconn->sendData(&msg, true);
			clientconn->disconnect();
			for (int i = 0; i < 1000; i++)
			{
				netclient->doWork();
				if (!netclient->isConnected())
				{
					break;
				}
			}
			// Destroy client
			netclient->shutdown();
			delete netclient;
			netclient = 0;
			client = false;
		}
		// Delete entities
		std::list<Entity*>::iterator it = entities.begin();
		while (it != entities.end())
		{
			//(*it)->destroy();
			delete *it;
			it++;
		}
		entities.clear();
		// Call scripts
		if (serverscript)
		{
			serverscript->callFunction("destroy_server");
			delete serverscript;
			serverscript = 0;
		}
		if (clientscript)
		{
			clientscript->callFunction("destroy_client");
			delete clientscript;
			clientscript = 0;
		}
		
		// Delete world
		if (world)
		{
			world->destroy();
			delete world;
			world = 0;
		}
		
		return true;
	}
	void Game::stopGame(void)
	{
		stopgame = true;
	}
	
	bool Game::isServer(void)
	{
		return server;
	}
	bool Game::isClient(void)
	{
		return client;
	}
	
	void Game::setVisible(bool visible)
	{
		if (server)
		{
			if (visible && !bchost)
			{
				bchost = new BroadcastHost;
				bchost->init(27273);
			}
			else if (!visible && bchost)
			{
				bchost->shutdown();
				delete bchost;
				bchost = 0;
			}
		}
	}
	bool Game::isVisible(void)
	{
		return (bchost != 0);
	}
	
	void Game::setFocus(Vector3D focus)
	{
		this->focus = focus;
	}
	Vector3D Game::getFocus(void)
	{
		return focus;
	}
	
	int Game::getClientCount(void)
	{
		return serverconns.size();
	}
	Vector3D Game::getClientFocus(int client)
	{
		return serverconns[client]->focus;
	}
	Connection *Game::getClientConnection(int client)
	{
		return serverconns[client]->conn;
	}
	
	Connection *Game::getServerConnection(void)
	{
		return clientconn;
	}
	
	Entity *Game::createEntity(std::string name, int owner)
	{
		if (!server) return 0;
		Entity *entity = new Entity;
		entity->setOwner(owner);
		entity->setLocal(owner == 0);
		if (!entity->load(name))
		{
			return 0;
		}
		entities.push_back(entity);
		
		// Send entity to clients
		Buffer msg;
		msg.writeByte(ENM_CreateEntity);
		msg.writeWord(entity->getID());
		msg.writeByte(0);
		msg.writeString(name);
		entity->writeStartData(&msg);
		printf("Creating entity %d.\n", entity->getID());
		
		for (unsigned int i = 0; i < serverconns.size(); i++)
		{
			Buffer networkdata = msg;
			if (serverconns[i]->owner == owner)
			{
				// Set owner
				networkdata.getData()[3] = 1;
			}
			serverconns[i]->conn->sendData(&networkdata, true);
		}
		
		return entity;
	}
	Entity *Game::getEntity(int id)
	{
		std::list<Entity*>::iterator it = entities.begin();
		while (it != entities.end())
		{
			if ((*it)->getID() == id)
			{
				return *it;
			}
			it++;
		}
		return 0;
	}
	bool Game::deleteEntity(Entity *entity, bool now)
	{
		if (!now)
		{
			deleting.push_back(entity);
			return true;
		}
		std::list<Entity*>::iterator it = entities.begin();
		while (it != entities.end())
		{
			if (*it == entity)
			{
				// Send delete message
				Buffer msg;
				msg.writeByte(ENM_DeleteEntity);
				msg.writeWord(entity->getID());
				sendDataToClients(&msg);
				// Delete entity
				//(*it)->destroy();
				delete *it;
				entities.erase(it);
				
				return true;
			}
			it++;
		}
		return false;
	}
	
	int Game::getEntityCount(std::string name)
	{
		int count = 0;
		std::list<Entity*>::iterator it = entities.begin();
		while (it != entities.end())
		{
			if ((*it)->getName() == name)
			{
				count++;
			}
			it++;
		}
		return count;
	}
	Entity *Game::getEntity(std::string name, int index)
	{
		int i = 0;
		std::list<Entity*>::iterator it = entities.begin();
		while (it != entities.end())
		{
			if ((*it)->getName() == name)
			{
				if (i == index)
				{
					return *it;
				}
				i++;
			}
			it++;
		}
		return 0;
	}
	
	World *Game::getWorld(void)
	{
		return world;
	}
	
	void Game::doWork(float msecs)
	{
		if (!server && !client) return;
		// Shutdown game?
		if (stopgame)
		{
			shutdown();
			return;
		}
		
		// Answer broadcast messages
		if (bchost)
		{
			bchost->doWork();
		}
		
		if (server)
		{
			nethost->doWork();
			
			// Look for disconnected clients
			for (unsigned int i = 0; i < serverconns.size(); i++)
			{
				if (!serverconns[i]->conn->isConnected())
				{
					// Delete connection
					int id = serverconns[i]->owner;
					nethost->closeConnection(serverconns[i]->conn);
					delete serverconns[i];
					serverconns.erase(serverconns.begin() + i);
					destroyEntities(id);
					i--;
				}
			}
			
			// Look for new clients
			Connection *newconn;
			while ((newconn = nethost->getNewConnection()) != 0)
			{
				LINFO("New connection!\n");
				// Send level name
				Buffer levelmsg;
				levelmsg.writeString(SettingsManager::get()->getString("server.level"));
				newconn->sendData(&levelmsg, true);
				
				int id = ++lastclientid;
				serverconns.push_back(new ClientInfo(newconn, id, Vector3D(0, 0, 0)));
				// Send existing objects
				Buffer msg;
				msg.writeByte(ENM_CreateEntity);
				std::list<Entity*>::iterator it = entities.begin();
				while (it != entities.end())
				{
					msg.writeWord((*it)->getID());
					msg.writeByte(0);
					msg.writeString((*it)->getName());
					(*it)->writeStartData(&msg);
					if (msg.getDataSize() > 600)
					{
						newconn->sendData(&msg, true);
						msg.clear();
						msg.writeByte(ENM_CreateEntity);
					}
					it++;
				}
				if (msg.getDataSize() > 1)
				{
					newconn->sendData(&msg, true);
				}
				
				// Call server script
				serverscript->callFunction("client_connected", id);
			}
			
			// Get data from clients
			for (unsigned int i = 0; i < serverconns.size(); i++)
			{
				Buffer *msg;
				while ((msg = serverconns[i]->conn->readData()) != 0)
				{
					if (!parseClientData(serverconns[i]->owner, msg))
					{
						// Delete connection
						int id = serverconns[i]->owner;
						nethost->closeConnection(serverconns[i]->conn);
						delete serverconns[i];
						serverconns.erase(serverconns.begin() + i);
						destroyEntities(id);
						break;
					}
					delete msg;
				}
			}
			
			// Update entities
			Entity::updateAll(msecs);

			// Update callbacks
			Callback::updateAll();
			
			// Update animations
			MeshSceneNode::updateAll(msecs);
			
			// Send updates to clients
			increasePriority();
			timesinceupdate += msecs;
			if (timesinceupdate > 16)
			{
				int time = (int)timesinceupdate / 16;
				sendServerUpdates(time * 16);
				timesinceupdate -= time * 16;
			}
			
			// Delete entities
			std::list<Entity*>::iterator entit = deleting.begin();
			while (entit != deleting.end())
			{
				deleteEntity(*entit, true);
				entit++;
			}
			deleting.clear();
		}
		else if (client)
		{
			netclient->doWork();
			
			// Get data from server
			Buffer *msg;
			while ((msg = clientconn->readData()) != 0)
			{
				LDEBUG("Data: %d bytes.\n", msg->getDataSize());
				parseServerData(msg);
				delete msg;
			}
			
			// Run client script
			if (clientscript->isFunction("update"))
				clientscript->callFunction("update");
			
			// Update entities
			Entity::updateAll(msecs);
			
			// Update animations
			MeshSceneNode::updateAll(msecs);
		}
	}
	
	Script *Game::getServerScript(void)
	{
		return serverscript;
	}
	Script *Game::getClientScript(void)
	{
		return clientscript;
	}
	
	Game::Game()
	{
		server = false;
		client = false;
		serverscript = 0;
		nethost = 0;
		clientscript = 0;
		netclient = 0;
		bchost = 0;
		world = 0;
		mousereceiver = 0;
	}
	
	bool Game::parseServerData(Buffer *data)
	{
		int type = data->readByte();
		switch (type)
		{
			case ENM_Physics:
			{
				world->update(data);
				break;
			}
			case ENM_CreateEntity:
			{
				LDEBUG("ENM_CreateEntity!\n");
				LDEBUG("Size: %d\n", data->getDataSize());
				while (data->getPosition() != data->getDataSize())
				{
					// Create entity
					int id = data->readWord();
					bool local = data->readByte();
					std::string name = data->readString();
					Entity *entity = new Entity;
					entity->setLocal(local);
					if (!entity->load(name, data))
					{
						return 0;
					}
					entity->setID(id);
					entities.push_back(entity);
				}
				break;
			}
			case ENM_DeleteEntity:
			{
				LDEBUG("ENM_DeleteEntity!\n");
				while (data->getPosition() != data->getDataSize())
				{
					int id = data->readWord();
					Entity *entity = getEntity(id);
					if (!entity)
					{
						LERROR("Unknown entity (%d).\n", id);
						break;
					}
					std::list<Entity*>::iterator it = entities.begin();
					while (it != entities.end())
					{
						if (*it == entity)
						{
							//(*it)->destroy();
							delete *it;
							entities.erase(it);
							break;
						}
						it++;
					}
				}
				break;
			}
			case ENM_UpdateProperty:
			{
				Entity::updateProperty(data);
				break;
			}
			default:
			{
				LERROR("Unknown server command: %d\n", type);
			}
		}
		
		return true;
	}
	bool Game::sendServerUpdates(float msecs)
	{
		if (serverconns.size() == 0) return true;
		// Bandwidth limitation
		//int datasize = 200000 / 8 * msecs / 1000 / serverconns.size();
		world->sendUpdates(msecs);
		return true;
	}
	bool Game::parseClientData(int conn, Buffer *data)
	{
		int type = data->readByte();
		switch (type)
		{
			case ENM_UpdateProperty:
			{
				Entity::updateProperty(data);
				break;
			}
			case ENM_Link:
			{
				Entity::updateLink(data);
				break;
			}
			default:
			{
				LERROR("Unknown client command: %d\n", type);
			}
		}
		return true;
	}
	void Game::sendDataToClients(Buffer *data, bool reliable)
	{
		for (unsigned int i = 0; i < serverconns.size(); i++)
		{
			Buffer networkdata = *data;
			serverconns[i]->conn->sendData(&networkdata, reliable);
		}
	}
	void Game::destroyEntities(int conn)
	{
		std::list<Entity*>::iterator it = entities.begin();
		while (it != entities.end())
		{
			if ((*it)->getOwner() == conn)
			{
				std::list<Entity*>::iterator next = it;
				next++;
				deleteEntity(*it);
				it = next;
			}
			else
			{
				it++;
			}
		}
	}
	
	void Game::increasePriority(void)
	{
		/*std::list<Entity*>::iterator it = entities.begin();
		while (it != entities.end())
		{
			if ((*it)->getCurrentPriority() > 0)
			{
				(*it)->setCurrentPriority((*it)->getCurrentPriority() - 1);
			}
			it++;
		}*/
	}
	
	int Game::lastclientid = 0;
}

