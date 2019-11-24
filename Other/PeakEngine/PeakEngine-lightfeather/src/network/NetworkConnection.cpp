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

#include "peakengine/network/NetworkConnection.hpp"

namespace peak
{
	NetworkConnection::NetworkConnection(ENetPeer* peer)
	{
		this->peer = peer;
	}
	NetworkConnection::~NetworkConnection()
	{
	}

	void NetworkConnection::injectData(BufferPointer buffer)
	{
		received.push(buffer);
	}

	bool NetworkConnection::isConnected()
	{
		if(peer != 0)
			return true;
		else
			return false;
	}
	
	void NetworkConnection::close()
	{
		enet_peer_disconnect(peer, 0);
		peer->data = 0;
		peer = 0;
	}

	void NetworkConnection::send(BufferPointer buffer, bool reliable)
	{
		ENetPacket * packet = enet_packet_create (buffer->getData(),
		                                          buffer->getSize(),
		                                          reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

		// Send the packet to the peer over channel id 0. 
		enet_peer_send(peer, 0, packet);
		enet_host_flush(peer->host);
	}
	
	bool NetworkConnection::hasData()
	{
		return received.size() > 0;
	}
	
	BufferPointer NetworkConnection::receive()
	{
		if (received.size() == 0)
			return 0;
		BufferPointer data = received.front();
		received.pop();
		return data;
	}
}