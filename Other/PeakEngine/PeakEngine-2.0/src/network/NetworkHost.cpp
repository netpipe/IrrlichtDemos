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

#include "peakengine/network/NetworkHost.hpp"
#include "peakengine/support/Buffer.hpp"
#include "peakengine/network/NetworkConnection.hpp"
#include "peakengine/network/Connection.hpp"

namespace peak
{
	NetworkHost::NetworkHost()
	{
	}
	
	NetworkHost::~NetworkHost()
	{
	}
	
	bool NetworkHost::init(unsigned int port, unsigned int maxclients, unsigned int maxin, unsigned int maxout)
	{
		// Create network host
		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = port;
		host = enet_host_create(&address, maxclients, maxin, maxout);
		if (!host)
		{
			return false;
		}
		return true;
	}
	
	bool NetworkHost::shutdown()
	{
		if (host)
		enet_host_destroy(host);
		host = 0;
		return true;
	}
	
	Connection* NetworkHost::getNewConnection()
	{
		if (newconnections.empty())
		{
			return 0;
		}
		
		Connection *con = newconnections.front();
		newconnections.pop();
		return con;
	}
	
	bool NetworkHost::update()
	{
		if (!host)
			return false;
		
		ENetEvent event;
		while (enet_host_service(host, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				NetworkConnection *newconn = new NetworkConnection(event.peer);
				event.peer->data = newconn;
				newconnections.push(newconn);
				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
			{
				// Push received data into connection
				Buffer *buffer = new Buffer(event.packet->data, event.packet->dataLength, true);
				NetworkConnection* con = ((NetworkConnection*) event.peer->data);
				con->injectData((BufferPointer) buffer);
				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
				((NetworkConnection*) event.peer->data)->close();
				break;
			default:
				break;
			}
		}
		return true;
	}
	
}
