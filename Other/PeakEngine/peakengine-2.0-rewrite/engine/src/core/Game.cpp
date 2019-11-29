/*
Copyright (C) 2009 Mathias Gottschlag

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
#include "core/Logger.h"
#include "core/Entity.h"
#include "network/NetworkHost.h"
#include "network/NetworkClient.h"
#include "network/BroadcastHost.h"
#include "network/Connection.h"

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
		
		stopgame = false;
		timesinceupdate = 0;
		server = true;
		client = false;
		
		time = 0;
		
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
		// Receive initial time
		Buffer *timemsg = 0;
		while (!timemsg)
		{
			netclient->doWork();
			timemsg = clientconn->readData();
		}
		time = timemsg->readInt();
		delete timemsg;
		printf("Initial time: %d\n", time);
		
		// Receive initial entities
		Buffer *entitydata = 0;
		while (!entitydata)
		{
			netclient->doWork();
			entitydata = clientconn->readData();
		}
		printf("Got %d bytes of entity data.\n", entitydata->getDataSize());
		while (entitydata->getPosition() != entitydata->getDataSize())
		{
			int id = entitydata->readInt();
			std::string type = entitydata->readString();
			createClientEntity(type, id, 0);
		}
		delete entitydata;
		
		stopgame = false;
		server = false;
		client = true;
		
		return true;
	}
	
	bool Game::shutdown(void)
	{
		if (!server && !client) return true;
		
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
			//serverconns.clear();
			nethost = 0;
			server = false;
			client = false;
		}
		else if (client)
		{
			/*// Disconnect
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
			}*/
			// Destroy client
			netclient->shutdown();
			delete netclient;
			netclient = 0;
			client = false;
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

	int Game::getClientCount(void)
	{
		return 0;
	}
	
	Connection *Game::getClientConnection(int client)
	{
		return 0;
	}
	
	Connection *Game::getServerConnection(void)
	{
		return 0;
	}
	
	Entity *Game::createEntity(std::string type, int owner)
	{
		if (!isServer())
		{
			printf("No server.\n");
			return 0;
		}
		// Get create function
		std::map<std::string, Entity *(*)(void)>::iterator it = entitytypes.find(type);
		if (it == entitytypes.end())
		{
			printf("Entity type not found.\n");
			return 0;
		}
		Entity *(*create)(void) = it->second;
		// Create entity
		int id = getNewEntityID();
		Entity *entity = create();
		entity->setID(id);
		if (!entity->init(type))
		{
			printf("Could not initialize entity.\n");
			delete entity;
			return 0;
		}
		entities[id - 1] = entity;
		printf("Created %d.\n", id - 1);
		
		// TODO: Send entity to clients
		
		return entity;
	}
	Entity *Game::createClientEntity(std::string type, int id, int owner)
	{
		// Get create function
		std::map<std::string, Entity *(*)(void)>::iterator it = entitytypes.find(type);
		if (it == entitytypes.end())
		{
			printf("Entity type not found.\n");
			return 0;
		}
		Entity *(*create)(void) = it->second;
		// Create entity
		Entity *entity = create();
		entity->setID(id);
		if (!entity->init(type))
		{
			printf("Could not initialize entity.\n");
			delete entity;
			return 0;
		}
		entities[id - 1] = entity;
		printf("Created %d.\n", id - 1);
		
		return entity;
	}
	Entity *Game::getEntity(int id)
	{
		if ((id == 0) || (id > 65535))
			return 0;
		return entities[id];
	}
	bool Game::deleteEntity(Entity *entity, bool now)
	{
		return false;
	}
	
	int Game::getEntityCount(std::string type)
	{
		return 0;
	}
	Entity *Game::getEntity(std::string name, int index)
	{
		return 0;
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
		
		if (isServer())
		{
			time++;
			
			nethost->doWork();
			
			// Look for disconnected clients
			for (unsigned int i = 0; i < clients.size(); i++)
			{
				if (!clients[i]->conn->isConnected())
				{
					// Delete connection
					//int id = clients[i]->owner;
					nethost->closeConnection(clients[i]->conn);
					delete clients[i];
					clients.erase(clients.begin() + i);
					// TODO: Delete entities
					//destroyEntities(id);
					i--;
				}
			}
			// Look for new clients
			Connection *newconn;
			while ((newconn = nethost->getNewConnection()) != 0)
			{
				LINFO("New connection!\n");
				// Send initial time
				Buffer msg;
				msg.writeInt(time);
				newconn->sendData(&msg, true);
				// Add client to list
				int id = ++lastclientid;
				clients.push_back(new ClientInfo(newconn, id));
				// Send existing objects
				msg.clear();
				for (int i = 0; i < 65535; i++)
				{
					if (entities[i])
					{
						printf("Sending entity %d\n", i + 1);
						msg.writeInt(i + 1);
						msg.writeString(entities[i]->getType());
						if (!entities[i]->sendFullUpdate(&msg))
							break;
					}
				}
				printf("Initial update: %d bytes.\n", msg.getDataSize());
				newconn->sendData(&msg, true);
			}
			// Read incoming data from clients
			for (unsigned int i = 0; i < clients.size(); i++)
			{
				Buffer *msg;
				while ((msg = clients[i]->conn->readData()) != 0)
				{
					int type = msg->readByte();
					if (type == 1)
					{
						if (msg->getDataSize() < 5) return;
						// Client command
						printf("Client command: %d bytes.\n", msg->getDataSize());
						int entity = msg->readWord();
						int id = msg->readWord();
						if (entity == 0)
							break;
						if (!entities[entity - 1])
							break;
						// Insert command into system
						// TODO: Validation
						EntityCommand *command = new EntityCommand;
						command->data = Buffer(msg->getData() + 5, msg->getDataSize() - 5);
						command->id = id;
						command->timestamp = getTime();
						entities[entity - 1]->sendCommand(command);
					}
					delete msg;
				}
			}
			// Physics engine
			// Game logic
			for (int i = 0; i < 65535; i++)
			{
				if (entities[i])
				{
					entities[i]->applyNextCommand();
					entities[i]->update(msecs);
					entities[i]->save();
				}
			}
			// Write updates to clients
			int currententity = nextentity;
			Buffer updatebuffer;
			updatebuffer.writeByte(1);
			updatebuffer.writeInt(getTime());
			for (int i = 0; i < 65535; i++)
			{
				int currententity = (nextentity + i) % 65535;
				if (entities[currententity])
				{
					// Add update to buffer
					updatebuffer.writeWord(currententity + 1);
					updatebuffer.writeWord(entities[currententity]->getLastCommandID());
					updatebuffer.writeWord(getTime() - entities[currententity]->getLastCommandTime());
					if (!entities[currententity]->update(&updatebuffer))
					{
						updatebuffer.setSize(updatebuffer.getDataSize() - 2);
					}
					if (updatebuffer.getDataSize() >= 500)
						break;
				}
			}
			nextentity = currententity;
			printf("Incremental update: %d bytes.\n", updatebuffer.getDataSize());
			if (updatebuffer.getDataSize() > 1)
				sendDataToClients(&updatebuffer, false);
		}
		else
		{
			netclient->doWork();
			
			// Read incoming data from server
			unsigned int newesttime = time;
			Buffer *msg;
			while ((msg = clientconn->readData()) != 0)
			{
				printf("Server data: %d bytes.\n", msg->getDataSize());
				int type = msg->readByte();
				if (type == 1)
				{
					unsigned int time = msg->readInt();
					if (time < newesttime)
					{
						for (int i = 0; i < 65535; i++)
						{
							if (entities[i])
							{
								entities[i]->reset(time);
							}
						}
						newesttime = time;
						this->time = time;
					}
					else
					{
						while (this->time != time)
						{
							this->time++;
							for (int i = 0; i < 65535; i++)
							{
								if (entities[i])
								{
									entities[i]->update(20);
									entities[i]->save();
								}
							}
						}
					}
					// Delta updates
					while (msg->getPosition() < msg->getDataSize())
					{
						// Get entity
						int entity = msg->readWord();
						if (entity == 0)
							break;
						if (!entities[entity - 1])
							break;
						int cmdid = msg->readWord();
						EntityCommand *cmd = 0;
						if (cmdid != 0)
						{
							printf("Command: %d\n", cmdid);
							cmd = entities[entity - 1]->getCommand(cmdid);
						}
						int cmdtime = msg->readWord();
						if (cmd)
						{
							cmdtime += cmd->timestamp;
							printf("Command data %d ticks in the past.\n", time - cmdtime);
							EntityState *state = entities[entity - 1]->getState(cmdtime);
							if (!state || (time - cmdtime <= 0))
							{
								if (state == 0)
								{
									printf("State %d not there (now: %d).\n", cmdtime, time);
								}
								// Force update
								cmd = 0;
							}
							else if (!entities[entity - 1]->checkState(state, msg))
							{
								entities[entity - 1]->applyDeltaUpdate(msg, 0, state);
								// Reset to step to which the update refers to
								for (int i = 0; i < 65535; i++)
								{
									if (entities[i])
									{
										if (i != entity - 1)
										{
											entities[i]->reset(cmdtime);
										}
										else
										{
											entities[i]->reset(cmdtime, true);
										}
									}
								}
								int timedifference = time - cmdtime;
								this->time = cmdtime;
								// Replay last frames
								for (int i = 0; i < timedifference; i++)
								{
									printf(".");
									this->time++;
									for (int i = 0; i < 65535; i++)
									{
										if (entities[i])
										{
											entities[i]->update(20);
											if (i == entity - 1)
											{
												entities[i]->applyCommand(this->time);
												entities[i]->save();
											}
											else
											{
												entities[i]->reset(this->time);
											}
										}
									}
								}
								printf("\n");
							}
						}
						if (cmd || entities[entity - 1]->getLastCommandID())
						{
							break;
						}
						// Inject delta update
						if (!entities[entity - 1]->injectUpdate(time, msg, time == this->time))
							break;
					}
					//if (time > newesttime)
					//	newesttime = time;
				}
				delete msg;
			}
			// FIXME: We really need proper error handling here
			/*if (newesttime != time)
			{
				// Update from the future
				printf("Warning: Update from the future.\n");
				while (newesttime != time)
				{
					time++;
					for (int i = 0; i < 65535; i++)
					{
						if (entities[i])
						{
							entities[i]->update(msecs);
							entities[i]->save();
						}
					}
				}
			}
			else
			{
				//printf("Update: %d\n", newesttime - time);
			}*/
			// Game logic
			time++;
			for (int i = 0; i < 65535; i++)
			{
				if (entities[i])
				{
					entities[i]->update(msecs);
					entities[i]->save();
				}
			}
			// Write commands to the server
		}
	}
	
	void Game::addEntityType(std::string name, Entity *(*create)(void))
	{
		entitytypes.insert(std::pair<std::string, Entity *(*)(void)>(name, create));
	}
	void Game::removeEntityType(std::string name)
	{
		std::map<std::string, Entity *(*)(void)>::iterator it = entitytypes.find(name);
		if (it != entitytypes.end())
		{
			entitytypes.erase(it);
		}
	}
	
	void Game::sendDataToClients(Buffer *data, bool reliable)
	{
		for (unsigned int i = 0; i < clients.size(); i++)
		{
			clients[i]->conn->sendData(data, reliable);
		}
	}
	void Game::sendServerData(Buffer *data, bool reliable)
	{
		clientconn->sendData(data, reliable);
	}
	
	Game::Game()
	{
		for (int i = 0; i < 65535; i++)
		{
			entities[i] = 0;
		}
		nextentity = 0;
	}
	
	int Game::getNewEntityID(void)
	{
		for (int i = 0; i < 65535; i++)
		{
			if (entities[i] == 0)
				return i + 1;
		}
		return 0;
	}
	
	int Game::lastclientid = 0;
}

