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

#include "network/Connection.h"
#include "core/Timer.h"
//#include <unistd.h>
#include <stdio.h>
namespace peak
{
	Connection::Connection(ENetPeer *peer)
	{
		this->peer = peer;
		speedtimer = new Timer;
		speedtimer->start();
		sent = 0;
		relsent = 0;
	}
	Connection::~Connection()
	{
	}

	void Connection::destroy(void)
	{
		// TODO: delete received data
		if (peer)
		{
			enet_peer_reset(peer);
			peer = 0;
		}
	}

	void Connection::disconnect(void)
	{
		enet_peer_disconnect(peer, 0);
		peer = 0;
	}
	bool Connection::isConnected(void)
	{
		return (peer != 0);
	}

	bool Connection::hasNewData(void)
	{
		return !received.empty();
	}
	Buffer *Connection::readData(void)
	{
		if (received.empty())
		{
			return 0;
		}

		Buffer *data = received.front();
		received.pop();
		return data;
	}
	bool Connection::sendData(Buffer *data, bool reliable)
	{
		//printf("Sending %d, %d\n", data->getDataSize(), data->readByte());
		ENetPacket *packet = enet_packet_create(data->getData(),
				data->getDataSize(), reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
		enet_peer_send(peer, 0, packet);
		enet_host_flush(peer->host);
		sent += data->getDataSize();
		if (reliable)
			relsent += data->getDataSize();
		if (speedtimer->getTime() >= 1)
		{
			printf("Connection: %d bytes per second, %d% reliable.\n", (int)((float)sent / speedtimer->getTime()), (int)((float)relsent / sent * 100));
			sent = 0;
			relsent = 0;
			speedtimer->reset();
		}
		return true;
	}

	void Connection::injectData(Buffer *data)
	{
		received.push(data);
	}
	ENetPeer *Connection::getPeer(void)
	{
		return peer;
	}
}

