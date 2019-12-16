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

#include "network/NetworkHost.h"
#include "network/Connection.h"
#include "core/Logger.h"

#include <stdio.h>

namespace peak
{
	NetworkHost::NetworkHost()
	{
		host = 0;
	}
	NetworkHost::~NetworkHost()
	{
	}

	bool NetworkHost::init(int port)
	{
		// Create network host
		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = port;
		host = enet_host_create(&address, 32, 200000, 20000,0);
		if (!host)
		{
			LERROR("Could not create network host.\n");
			return false;
		}
		LINFO("Created network host.\n");
		return true;
	}
	bool NetworkHost::shutdown(void)
	{
		for (unsigned int i = 0; i < connections.size(); i++)
		{
			Connection *conn = connections[i];
			conn->destroy();
			delete connections[i];
		}
		connections.clear();
		if (host)
			enet_host_destroy(host);
		host = 0;
		return true;
	}

	Connection *NetworkHost::getNewConnection(void)
	{
		if (newconnections.empty())
		{
			return 0;
		}

		Connection *conn = newconnections.front();
		newconnections.pop();
		return conn;
	}
	void NetworkHost::closeConnection(Connection *conn)
	{
		for (unsigned int i = 0; i < connections.size(); i++)
		{
			if (connections[i] == conn)
			{
				connections[i]->destroy();
				delete connections[i];
				connections.erase(connections.begin() + i);
				return;
			}
		}
	}

	bool NetworkHost::doWork(void)
	{
		if (!host)
			return false;


		// Receive data
		ENetEvent event;
		while (enet_host_service(host, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				LINFO("Host: Someone connected.\n");
				Connection *newconn = new Connection(event.peer);
				connections.push_back(newconn);
				newconnections.push(newconn);
				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
				//printf("Host: Received data.\n");
				// Push received data into connection
				for (unsigned int i = 0; i < connections.size(); i++)
				{
					if (event.peer == connections[i]->getPeer())
					{
						Buffer *buffer = new Buffer;
						buffer->writeData(event.packet->data,
								event.packet->dataLength, true);
						connections[i]->injectData(buffer);
					}
				}
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				LINFO("Host: Someone disconnected.\n");
				for (unsigned int i = 0; i < connections.size(); i++)
				{
					if (event.peer == connections[i]->getPeer())
					{
						connections[i]->destroy();
					}
				}
				break;
			default:
				break;
			}
		}
		// Send data

		return true;
	}
}

