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

#include "peakengine/network/NetworkClient.hpp"
#include "peakengine/support/Buffer.hpp"
#include "peakengine/network/NetworkConnection.hpp"

#include <iostream>

namespace peak
{
	NetworkClient::NetworkClient()
	{
	}
	
	NetworkClient::~NetworkClient()
	{
	}
	
	bool NetworkClient::init(unsigned int maxdown, unsigned int maxup)
	{
		client = enet_host_create(0, 1, maxdown, maxup);
		if (!client)
		{
			return false;
		}
		return true;
	}

	Connection *NetworkClient::connect(std::string address, unsigned int port, unsigned int ms)
	{
		ENetAddress enetaddress;
		ENetEvent event;
		ENetPeer *peer;
		enet_address_set_host(&enetaddress, address.c_str());
		enetaddress.port = port;

		peer = enet_host_connect(client, &enetaddress, 1);

		if (peer == 0)
		{
			//TODO
			// log/print: "No available peers for initiating an ENet connection."
			return 0;
		}

		if (enet_host_service (client, &event, ms) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			NetworkConnection *connection = new NetworkConnection(peer);
			peer->data = connection;
			// log/print: "Connection succeeded."
			return connection;
		}
		else
		{
			// log/print "Connection failed."
			enet_peer_reset(peer);
			return 0;
		}
	}
	
	bool NetworkClient::shutdown()
	{
		if (client)
			enet_host_destroy(client);
		client = 0;
		return true;
	}
	
	bool NetworkClient::update()
	{
		if (!client)
			return false;

		// Receive data
		ENetEvent event;
		while (enet_host_service(client, &event, 0) > 0)
		{
			switch (event.type)
			{
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
