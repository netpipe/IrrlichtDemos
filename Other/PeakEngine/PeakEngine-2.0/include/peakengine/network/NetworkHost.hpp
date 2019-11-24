/*
Copyright (c) 2009, Mathias Gottschlag, Lukas Kropatschek

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

#ifndef _PEAKENGINE_NETWORK_NETWORKHOST_HPP_
#define _PEAKENGINE_NETWORK_NETWORKHOST_HPP_

#include <enet/enet.h>
#include <queue>

namespace peak
{
	class Connection;
	class NetworkConnection; 
	
	class NetworkHost
	{

		public:
			/**
			 * Constructor.
			 */
			NetworkHost();
			/**
			 * Destructor.
			 */
			~NetworkHost();

			/**
			 * Creates a network host at the given port.
			 * @return False if there have been any errors.
			 */
			bool init(unsigned int port, unsigned int maxclients = 32, unsigned int maxin = 200000, unsigned int maxout = 20000);
			/**
			 * Closes the network socket.
			 * @return False if there have been any errors.
			 */
			bool shutdown();

			/**
			 * Returns a new connection if there is one and removes it from the
			 * list of new connections.
			 * @return Connection or 0 if there is no new connection.
			 */
			Connection *getNewConnection();

			/**
			 * Sends outgoing packets, receives data and injects it into the
			 * NetworkConnection instances and accepts new connections.
			 * @return False if there have been any errors.
			 */
			bool update();

		private:
			ENetHost* host;
			std::queue<NetworkConnection*> newconnections;
	};
}

#endif
