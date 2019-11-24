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

#ifndef _PEAKENGINE_NETWORK_NETWORKCONNECTION_HPP_
#define _PEAKENGINE_NETWORK_NETWORKCONNECTION_HPP_

#include "Connection.hpp"
#include <enet/enet.h>
#include <queue>

namespace peak
{
	class NetworkConnection : public Connection
	{
		public:
			NetworkConnection(ENetPeer* peer);
			virtual ~NetworkConnection();

			void injectData(BufferPointer buffer);
			
			virtual bool isConnected();
			virtual void close();

			virtual void send(BufferPointer buffer, bool reliable = false);
			virtual bool hasData();
			virtual BufferPointer receive();
		private:
			std::queue<BufferPointer> received;
			ENetPeer* peer;
	};
}

#endif
