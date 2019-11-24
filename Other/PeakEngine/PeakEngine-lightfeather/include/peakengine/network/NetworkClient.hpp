/*
Copyright (c) 2009, Lukas Kropatschek

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

#ifndef _PEAKENGINE_NETWORK_NETWORKCLIENT_HPP_
#define _PEAKENGINE_NETWORK_NETWORKCLIENT_HPP_

#include <enet/enet.h>
#include <string>
#include <queue>

namespace peak
{
	class Connection;
	
	class NetworkClient
	{

		public:
			/**
			 * Constructor.
			 */
			NetworkClient();
			/**
			 * Destructor.
			 */
			~NetworkClient();

			/**
			 * Creates a network client with given up-/downstream limits.
			 * @return False if there have been any errors.
			 */
			bool init(unsigned int maxdown = 0, unsigned int maxup = 0);

			/**
			 * Connects the client to a foreign Host at given address, port and given timeout.
			 * @param ms Milliseconds to wait to establish a connection.
			 * @return False if there have been problems establishing the connection.
			 */
			Connection *connect(std::string address, unsigned int port, unsigned int ms = 5000);
			
			/**
			 * Closes the network socket.
			 * @return False if there have been any errors.
			 */
			bool shutdown();

			/**
			 * Sends outgoing packets, receives data and injects it into the
			 * NetworkConnection instances and accepts new connections.
			 * @return False if there have been any errors.
			 */
			bool update();

		private:
			ENetHost* client;
	};
}

#endif
