/*
Copyright (c) 2009, Mathias Gottschlag

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

#include "peakengine/network/LocalConnection.hpp"
#include "peakengine/support/ScopedLock.hpp"

#ifdef EMULATE_NETWORK
#include "peakengine/support/OS.hpp"

#include <cstdlib>
#include <iostream>
#endif

namespace peak
{
	LocalConnection::LocalConnection() : other(0), mutex(0)
	{
	}
	LocalConnection::~LocalConnection()
	{
		close();
		if (mutex)
			delete mutex;
	}

	void LocalConnection::create(LocalConnection &a, LocalConnection &b)
	{
		Mutex *mutex = new Mutex();
		a.mutex = mutex;
		b.mutex = mutex;
		a.other = &b;
		b.other = &a;
	}

	bool LocalConnection::isConnected()
	{
		if (!mutex)
			return false;
		ScopedLock lock(*mutex);
		return other != 0;
	}
	void LocalConnection::close()
	{
		if (!mutex)
			return;
		ScopedLock lock(*mutex);
		if (other == 0)
			return;
		other->other = 0;
		other = 0;
		mutex = 0;
	}

	void LocalConnection::send(BufferPointer buffer, bool reliable)
	{
		if (!mutex)
			return;
		mutex->lock();
		#ifndef EMULATE_NETWORK
		if (other)
			other->received.push(new Buffer(*buffer.get()));
		#else
		if (!other)
		{
			mutex->unlock();
			return;
		}
		unsigned int time = OS::get().getTime();
		// Add lag (50-100 ms)
		unsigned int receivetime = time + (50 + rand() % 50) * 1000;
		// Create packet
		ReceivedPacket packet;
		packet.data = new Buffer(*buffer.get());
		packet.time = receivetime;
		packet.sendertime = time;
		if (reliable)
		{
			// Reliable packets always are received in the correct order
			if (other->reliable.size())
			{
				std::list<ReceivedPacket>::reverse_iterator it = other->reliable.rbegin();
				if (packet.time < it->time)
					packet.time = it->time;
			}
			// Insert into packet queue
			other->reliable.push_back(packet);
		}
		else
		{
			// Simulate 5% packet loss
			unsigned int notlost = rand() % 20;
			if (notlost)
			{
				// Insert into packet queue
				std::list<ReceivedPacket>::iterator it = other->unreliable.begin();
				bool inserted = false;
				while (it != other->unreliable.end())
				{
					if (it->time > packet.time)
					{
						it = other->unreliable.insert(it, packet);
						inserted = true;
						// Enet drops packets which arrive out of order
						// We have to do the same
						it++;
						while (it != other->unreliable.end())
						{
							if (it->sendertime < packet.sendertime)
							{
								it = other->unreliable.erase(it);
							}
							else
								break;
						}
						break;
					}
					it++;
				}
				if (!inserted)
					other->unreliable.push_back(packet);
			}
		}
		#endif
		mutex->unlock();
	}
	bool LocalConnection::hasData()
	{
		if (!mutex)
			return false;
		ScopedLock lock(*mutex);
		#ifndef EMULATE_NETWORK
		return received.size() > 0;
		#else
		unsigned int time = OS::get().getTime();
		if (unreliable.size() > 0 && unreliable.begin()->time <= time)
			return true;
		if (reliable.size() > 0 && reliable.begin()->time <= time)
			return true;
		return false;
		#endif
	}
	BufferPointer LocalConnection::receive()
	{
		if (!mutex)
			return 0;
		ScopedLock lock(*mutex);
		#ifndef EMULATE_NETWORK
		if (received.size() == 0)
			return 0;
		BufferPointer data = received.front();
		received.pop();
		return data;
		#else
		unsigned int time = OS::get().getTime();
		if (unreliable.size() > 0 && unreliable.begin()->time <= time)
		{
			BufferPointer data = unreliable.begin()->data;
			unreliable.erase(unreliable.begin());
			return data;
		}
		if (reliable.size() > 0 && reliable.begin()->time <= time)
		{
			BufferPointer data = reliable.begin()->data;
			reliable.erase(reliable.begin());
			return data;
		}
		return 0;
		#endif
	}
}
