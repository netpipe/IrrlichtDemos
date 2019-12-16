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

#include "network/BroadcastHost.h"
#include "core/Logger.h"

#include <errno.h>
#include <cstring>

namespace peak
{
	BroadcastHost::BroadcastHost()
	{
	}
	BroadcastHost::~BroadcastHost()
	{
	}

	bool BroadcastHost::init(int port)
	{
		// Create raw socket
		ENetAddress bcaddr;
		bcaddr.host = ENET_HOST_ANY;
		bcaddr.port = 14141;
		bcastsocket = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM, &bcaddr);
		// FIXME: enet 1.2 needs ENET_SOCKOPT_NONBLOCK, enet 1.1 does not provide it.
		#ifdef _ENET_1_2_
		enet_socket_set_option(bcastsocket, ENET_SOCKOPT_NONBLOCK, 1);
		#endif
		printf("Listening (%d).\n", port);
		return true;
	}
	bool BroadcastHost::shutdown(void)
	{
		enet_socket_destroy(bcastsocket);
		info = "";
		return true;
	}

	void BroadcastHost::setInfo(std::string info)
	{
		this->info = info;
	}
	std::string BroadcastHost::getInfo(void)
	{
		return info;
	}

	void BroadcastHost::doWork(void)
	{
		int length;
		ENetAddress addr;
		ENetBuffer bcbuffer;
		bcbuffer.data = malloc(1024);
		bcbuffer.dataLength = 1024;
		//printf("BroadcastHost::doWork(void).\n");
		// Look for broadcast messages
		while ((length = enet_socket_receive(bcastsocket, &addr, &bcbuffer, 1))
				!= 0)
		{
			printf("Listening.\n");
			if (length < 0)
			{
				LERROR("Socket error: %d\n", errno);
				break;
			}
			if (length > 0)
			{
				LDEBUG("Got broadcast message.\n");
				// Send answer
				memcpy(bcbuffer.data, info.c_str(), info.size() + 1);
				bcbuffer.dataLength = info.size() + 1;
				enet_socket_send(bcastsocket, &addr, &bcbuffer, 1);
			}
		}
		free(bcbuffer.data);
	}
}

