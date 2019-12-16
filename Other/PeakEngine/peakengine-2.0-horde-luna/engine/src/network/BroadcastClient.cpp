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

#include "network/BroadcastClient.h"
#include "core/Script.h"
#include "core/Logger.h"

#include <errno.h>

namespace peak
{
	BroadcastClient::BroadcastClient()
	{
		port = 0;
		bcclients.push_back(this);
		//cbscript = 0;
	}
	BroadcastClient::~BroadcastClient()
	{
		for (unsigned int i = 0; i < bcclients.size(); i++)
		{
			if (bcclients[i] == this)
			{
				bcclients.erase(bcclients.begin() + i);
				return;
			}
		}
	}

	void BroadcastClient::start(int port)
	{
		// Create raw socket
		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = ENET_PORT_ANY;
///		socket = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM, &address);
socket = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM);
		// enet 1.2 needs ENET_SOCKOPT_NONBLOCK, enet 1.1 does not provide it.
		#ifdef _ENET_1_2_
		enet_socket_set_option(socket, ENET_SOCKOPT_NONBLOCK, 1);
		#endif
		/*if (enet_socket_set_option(socket, ENET_SOCKOPT_BROADCAST, 1) == -1)
		{
			perror("ENET_SOCKOPT_BROADCAST");
		}*/
		this->port = port;
		updatetime = 1000;
		LERROR("Started broadcast client (%d).\n", port);
	}
	void BroadcastClient::stop(void)
	{
		enet_socket_destroy(socket);
		port = 0;
	}

	void BroadcastClient::clearList(void)
	{
		serverinfo.clear();
		serveraddr.clear();
	}

	int BroadcastClient::getServerCount(void)
	{
		return serverinfo.size();
	}
	std::string BroadcastClient::getServerInfo(int index)
	{
		return serverinfo[index];
	}
	Address BroadcastClient::getServerAddress(int index)
	{
		return serveraddr[index];
	}

	void BroadcastClient::setCallback(Script *script, std::string function)
	{
		cbscript = script;
		cbfunc = function;
	}

	void BroadcastClient::doWork(float msecs)
	{
		if (!port)
			return;


		//printf("BroadcastClient: doWork().\n");

		ENetBuffer buffer;
		buffer.data = malloc(1024);
		buffer.dataLength = 1024;
		ENetAddress remoteaddr;
		// Broadcast address
		ENetAddress bcaddr;
		bcaddr.host = ENET_HOST_BROADCAST;
		bcaddr.port = 14141;
		enet_address_set_host(&bcaddr, "127.0.0.1");

		// Message to be sent to all hosts
		char msg[] = "PING";
		ENetBuffer msgbuffer;
		msgbuffer.data = msg;
		msgbuffer.dataLength = 4;


		// Broadcast
		updatetime += msecs;
		if (updatetime > 250)
		{
			LERROR("Sending.\n");
			if (enet_socket_send(socket, &bcaddr, &msgbuffer, 1) == -1)
			{
				perror("Sending failed");
			}
			updatetime = 0;
		}


		// Receive answers
		bool listchanged = false;
		int length;
		while ((length = enet_socket_receive(socket, &remoteaddr, &buffer, 1) > 0)
				!= 0)
		{
			if (length < 0)
			{
				LERROR("Socket error: %d\n", errno);
				break;
			}
			if (length > 0)
			{
				LDEBUG("Got broadcast response.\n");


				// Get address
				char addrstr[16];
				enet_address_get_host_ip(&remoteaddr, addrstr, 16);
				Address addr(addrstr);
				// FIXME: We need the proper port here!
				addr.setPort(0);


				// Look if server is already known
				bool newaddr = true;
				for (unsigned int i = 0; i < serveraddr.size(); i++)
				{
					if (serveraddr[i] == addr)
					{
						newaddr = false;
						if (serverinfo[i] != (char*)buffer.data)
						{
							listchanged = true;
							serverinfo[i] = (char*)buffer.data;
						}
						break;
					}
				}


				// Add server to list
				if (newaddr)
				{
					serveraddr.push_back(addr);
					serverinfo.push_back((char*)buffer.data);
					listchanged = true;
				}

			}
		}
		// Call calback function
		if (listchanged && cbscript && (cbfunc != ""))
		{
			cbscript->callFunction(cbfunc);
		}

		free(buffer.data);
	}

	void BroadcastClient::doAllWork(float msecs)
	{
		for (unsigned int i = 0; i < bcclients.size(); i++)
		{
			bcclients[i]->doWork(msecs);
		}
	}

	std::vector<BroadcastClient*> BroadcastClient::bcclients;
}

