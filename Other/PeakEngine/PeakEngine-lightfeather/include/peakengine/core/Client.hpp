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

#ifndef _PEAKENGINE_CORE_CLIENT_HPP_
#define _PEAKENGINE_CORE_CLIENT_HPP_

#include "../entity/EntityManager.hpp"
#include "../support/Buffer.hpp"

#include <queue>

namespace peak
{
	class Server;
	class Connection;
	class NetworkClient;
	class Thread;

	struct EntityMessage
	{
		EntityMessage(Entity *entity, BufferPointer data) : entity(entity),
			data(data)
		{
		}
		Entity *entity;
		BufferPointer data;
	};

	/**
	 * Base class for game clients.
	 */
	class Client : public EntityManager
	{
		public:
			/**
			 * Constructor.
			 */
			Client(Engine *engine);
			/**
			 * Destructor.
			 */
			virtual ~Client();

			/**
			 * Connects the client to the given address, port and timeout. This calls load() after
			 * the client has received the server data.
			 * @param ms Milliseconds to wait to establish a connection, default = 5000.
			 */
			bool init(std::string address, unsigned int port, unsigned int ms = 5000);
			/**
			 * Connects the client to a local server. This calls load() after
			 * the client has received the server data.
			 */
			bool initLocally(Server *server);
			/**
			 * Closes the client. Can be implemented by the user.
			 */
			virtual bool shutdown();
			/**
			 * Creates the client. Has to be implemented by the user.
			 */
			virtual bool load(BufferPointer serverdata) = 0;

			/**
			 * Sends a message to a certain entity. This usually is not called
			 * directly but rather through ClientEntity::sendMessage().
			 */
			void sendEntityMessage(Entity *entity, BufferPointer data,
				bool reliable = false);

			/**
			 * Returns the game time of the client. The game time is incremented
			 * every 20 ms.
			 */
			virtual unsigned int getTime();

			/**
			 * Starts a thread which calls runThread().
			 */
			void startThread();
			/**
			 * Main client game loop.
			 */
			void runThread();
		private:
			Connection *connection;
			NetworkClient *client;
			volatile bool stopping;
			unsigned int time;
			unsigned int lastupdate;
			unsigned int lastacked;
			std::queue<EntityMessage> entitymessages;
			Thread *thread;
	};
}

#endif
