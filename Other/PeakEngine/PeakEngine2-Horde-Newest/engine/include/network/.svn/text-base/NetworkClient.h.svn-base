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

#ifndef _NETWORKCLIENT_H_
#define _NETWORKCLIENT_H_

#include "core/Address.h"
#include <enet/enet.h>

namespace peak
{
	class Connection;
	
	
	/**
	 * \brief Network client which can connect to a NetworkServer.
	 */
	class NetworkClient
	{
		public:
			NetworkClient();
			~NetworkClient();

			/**
			 * \brief Connects to a local server.
			 * \return Connection to the server, 0 if no connection could be created
			 */
			Connection *init(void);
			/**
			 * \brief Connects to a remote server.
			 * \return Connection to the server, 0 if no connection could be created
			 */
			Connection *init(Address addr);
			/**
			 * \brief Disconnects from the server.
			 */
			void shutdown(void);

			/**
			 * \brief Returns whether the client is connected to a server.
			 */
			bool isConnected(void);

			/**
			 * \brief Returns the connection to the server.
			 */
			Connection *getConnection(void);

			/**
			 * \brief Receives incoming data.
			 */
			bool doWork(void);
		private:
			ENetHost *host;
			ENetPeer *peer;
			Connection *conn;
	};
}

#endif

