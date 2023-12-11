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

#include "peakengine/core/Server.hpp"
#include "peakengine/support/Thread.hpp"
#include "peakengine/network/NetworkData.hpp"
#include "peakengine/network/Connection.hpp"
#include "peakengine/network/NetworkHost.hpp"
#include "peakengine/support/OS.hpp"
#include "peakengine/entity/ServerEntity.hpp"

#include <iostream>
#include <cstdlib>

namespace peak
{
	unsigned int ClientInfo::lastid = 0;

	Server::Server(Engine *engine) : EntityManager(engine), thread(0),
		localconnection(0), host(0)
	{
	}
	Server::~Server()
	{
		if (thread)
			delete thread;
	}

	bool Server::init(BufferPointer serverdata, unsigned int port)
	{
		if (port)
		{
			// Create network host
			host = new NetworkHost();
			if (!host->init(port))
			{
				delete host;
				return false;
			}
		}
		// Load server data
		time = 0;
		if (!load(serverdata))
			return false;
		stopping = false;
		return true;
	}
	bool Server::shutdown()
	{
		// Exit thread
		stopping = true;
		if (thread)
		{
			thread->wait();
			delete thread;
		}
		if (host && !host->shutdown())
			return false;
		return true;
	}

	void Server::addEntity(Entity *entity)
	{
		entity->setID(0);
		EntityManager::addEntity(entity);
		// Save owner of the entity
		ClientInfo *owner = 0;
		unsigned int ownerid = ((ServerEntity*)entity)->getOwner();
		// Send the entity to all clients
		BufferPointer buffer = new Buffer();
		buffer->write8(EPT_EntityCreated);
		buffer->write16(entity->getID());
		buffer->writeString(entity->getType());
		// Owner bit unset
		buffer->writeUnsignedInt(0, 1);
		entity->getState(buffer.get());
		for (unsigned int i = 0; i < clients.size(); i++)
		{
			if (clients[i].id != ownerid)
				clients[i].connection->send(buffer, true);
			else
				owner = &clients[i];
		}
		// Send the entity to the owner
		if (!owner)
			return;
		buffer = new Buffer();
		buffer->write8(EPT_EntityCreated);
		buffer->write16(entity->getID());
		buffer->writeString(entity->getType());
		// Set owner bit this time
		buffer->writeUnsignedInt(1, 1);
		entity->getState(buffer.get());
		owner->connection->send(buffer, true);
	}
	void Server::removeEntity(Entity *entity)
	{
		EntityManager::removeEntity(entity);
		// Remove the entity from all clients
		BufferPointer buffer = new Buffer();
		buffer->write8(EPT_EntityDeleted);
		buffer->write16(entity->getID());
		for (unsigned int i = 0; i < clients.size(); i++)
			clients[i].connection->send(buffer, true);
	}

	void Server::addClient(Connection *connection)
	{
		localconnection = connection;
	}

	void Server::sendEntityMessage(Entity *entity, BufferPointer data,
		bool reliable)
	{
		BufferPointer msg = new Buffer();
		msg->write8(EPT_EntityMessage);
		msg->write16(entity->getID() - 1);
		*msg.get() += *data.get();
		for (unsigned int i = 0; i < clients.size(); i++)
			clients[i].connection->send(msg, reliable);
	}

	unsigned int Server::getTime()
	{
		return time;
	}

	void Server::startThread()
	{
		// Create server thread
		thread = new Thread();
		thread->create(new ClassFunctor<Server>(this, &Server::runThread));
	}

	void Server::runThread()
	{
		uint64_t lastframe = OS::getSystemTime();
		while (!stopping)
		{
			if (host)
			{
				host->update();
			}
			// Incoming local connections
			if (localconnection != 0)
			{
				insertNewConnection(localconnection);
				localconnection = 0;
			}
			// Incoming network connections
			if (host)
			{
				Connection *newconnection = host->getNewConnection();
				while (newconnection)
				{
					std::cout << "Client connected!" << std::endl;
					insertNewConnection(newconnection);
					// Next connection
					newconnection = host->getNewConnection();
				}
			}
			// Update clients
			for (unsigned int i = 0; i < clients.size(); i++)
			{
				// Increase client time. Might get overridden by any incoming
				// packet
				clients[i].clienttime++;
				// Receive data
				while (clients[i].connection->hasData())
				{
					BufferPointer data = clients[i].connection->receive();
					PacketType type = (PacketType)data->read8();
					switch (type)
					{
						case EPT_Update:
						{
							// Read last received packet
							clients[i].lastreceived = data->read32();
							unsigned int updatetime = data->read32();
							clients[i].clienttime = updatetime;
							// Read entity updates
							bool updatevalid = true;
							while (data->getPosition() + 16 <= data->getSize() * 8)
							{
								unsigned int id = data->read16() + 1;
								ServerEntity *entity = (ServerEntity*)getEntity(id);
								// Ignore invalid updates
								if (!entity)
								{
									updatevalid = false;
									break;
								}
								// Apply update
								entity->applyClientUpdate(data.get(), updatetime);
								entity->onUpdate();
							}
							// Only tell the client that we have received this
							// update if it was valid
							if (updatevalid)
								clients[i].lastupdate = updatetime;
							break;
						}
						case EPT_EntityMessage:
						{
							unsigned int id = data->read16() + 1;
							Entity *entity = getEntity(id);
							entity->receiveMessage(data.get());
							break;
						}
						default:
							break;
					}
				}
			}
			// Update entities
			time++;
			update();
			// Send updates
			for (unsigned int i = 0; i < clients.size(); i++)
			{
				// Create update buffer
				BufferPointer update = new Buffer();
				update->write8(EPT_Update);
				update->write32(time);
				update->write32(clients[i].clienttime);
				update->write32(clients[i].lastupdate);
				// Fill buffer with updates
				EntityMap::Iterator it(entities);
				for (Entity *entity = it.next();entity != 0; entity = it.next())
				{
					if (entity->hasChanged(clients[i].lastreceived))
					{
						update->write16(entity->getID() - 1);
						entity->getUpdate(update.get(), clients[i].lastreceived);
					}
				}
				// Send update
				clients[i].connection->send(update);
			}
			// 20 ms per frame
			lastframe = lastframe + 20000;
			uint64_t currenttime = OS::getSystemTime();
			if (currenttime < lastframe)
			{
				float percentage = (float)(lastframe - currenttime) / 200;
				//std::cout << "Server: " << lastframe - currenttime << std::endl;
				std::cout << std::fixed;
				std::streamsize prec = std::cout.precision(1);
				std::cout << "Server: " << percentage << "%" << std::endl;
				std::cout.precision(prec);
				OS::sleep(lastframe - currenttime);
			}
			else
				std::cout << "Warning: Too much CPU usage." << std::endl;
		}
	}

	void Server::insertNewConnection(Connection *connection)
	{
		BufferPointer serverdata = onNewConnection(connection);
		if (serverdata)
		{
			clients.push_back(ClientInfo(connection));
			// Write server data
			BufferPointer msg = new Buffer();
			msg->write8(EPT_InitialData);
			*msg.get() += *serverdata.get();
			connection->send(msg, true);
			// Write entities
			// TODO: Inefficient, we should use much larger packets
			EntityMap::Iterator it(entities);
			for (Entity *entity = it.next();entity != 0; entity = it.next())
			{
				BufferPointer buffer = new Buffer();
				buffer->write8(EPT_EntityCreated);
				buffer->write16(entity->getID());
				buffer->writeString(entity->getType());
				entity->getState(buffer.get());
				connection->send(buffer, true);
			}
			onConnectionAccepted(clients[clients.size() - 1]);
		}
	}
}
