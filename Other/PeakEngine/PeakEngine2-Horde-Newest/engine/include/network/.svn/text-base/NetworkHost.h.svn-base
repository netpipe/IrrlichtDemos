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

#ifndef _NETWORKHOST_H_
#define _NETWORKHOST_H_

#include <enet/enet.h>
#include <queue>
#include <vector>

namespace peak
{
	class Connection;
	class NetworkConnection;
	
	
	/**
	 * \brief Network host which acts as a network server.
	 */
	class NetworkHost
	{
		public:
			NetworkHost();
			~NetworkHost();

			/**
			 * \brief Initializes the host at a given port.
			 * \param port Port used for the host
			 */
			bool init(int port);
			/**
			 * \brief Destroys the host, disconnecting all clients.
			 */
			bool shutdown(void);

			/**
			 * \brief Returns a new connection if there is any.
			 */
			Connection *getNewConnection(void);
			/**
			 * \brief Closes the connection to a client.
			 * \param conn Connection to be closed
			 */
			void closeConnection(Connection *conn);

			/**
			 * \brief Receives incoming data.
			 */
			bool doWork(void);
		private:
			ENetHost *host;

			std::queue<Connection*> newconnections;
			std::vector<Connection*> connections;
	};
}

#endif

