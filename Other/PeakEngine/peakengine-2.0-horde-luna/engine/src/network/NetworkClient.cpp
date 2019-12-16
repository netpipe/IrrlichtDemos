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

#include "network/NetworkClient.h"
#include "network/Connection.h"
#include "core/Logger.h"

#include <stdio.h>

namespace peak
{
	NetworkClient::NetworkClient()
	{
		host = 0;
	}
	NetworkClient::~NetworkClient()
	{
	}

	Connection *NetworkClient::init(void)
	{
		return 0;
	}
	Connection *NetworkClient::init(Address addr)
	{
		// Create network host
		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = ENET_PORT_ANY;
		host = enet_host_create(NULL, 1, 0, 0,0);
		if (!host)
		{
			return 0;
		}


		// Create network peer
		//ENetAddress address;
		enet_address_set_host(&address, addr.getAddressString(false).c_str());
		address.port = addr.getPort();
		LINFO("Client: Address: %X:%d\n", address.host, address.port);
		peer = enet_host_connect(host, &address, 2,0);
		if (!peer)
		{
			enet_host_destroy(host);
			return 0;
		}
		conn = 0;
		while (!conn)
		{
			doWork();
		}
		LINFO("Connected.\n");
		return conn;
	}
	void NetworkClient::shutdown(void)
	{
		// Close connection
		if (conn)
		{
			conn->destroy();
			delete conn;
			conn = 0;
		}
		// Delete network host
		if (host)
		{
			enet_host_destroy(host);
			host = 0;
		}
	}

	bool NetworkClient::isConnected(void)
	{
		return (conn != 0);
	}
	Connection *NetworkClient::getConnection(void)
	{
		return conn;
	}

	bool NetworkClient::doWork(void)
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
				conn = new Connection(peer);
				LDEBUG("Client: Connected.\n");
				break;
			case ENET_EVENT_TYPE_RECEIVE:
			{
				//printf("Client: Received data.\n");
				Buffer *buffer = new Buffer;
				buffer->writeData(event.packet->data, event.packet->dataLength,
						true);
				conn->injectData(buffer);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
				LDEBUG("Client: Disconnected.\n");
				conn->destroy();
				delete conn;
				conn = 0;
				return false;
				break;
			default:
				break;
			}
		}

		return true;
	}
}

