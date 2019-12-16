
/*
Copyright (C) 2008   Mathias Gottschlag

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

#include "core/EventRemoteLink.h"
#include "core/Script.h"
#include "core/Event.h"
#include "core/Script.h"
#include "core/Logger.h"
#include "core/Game.h"
#include "core/NetworkData.h"
#include "network/Connection.h"

namespace peak
{
	EventRemoteLink::EventRemoteLink()
	{
	}
	EventRemoteLink::~EventRemoteLink()
	{
	}
	
	bool EventRemoteLink::create(Event *source, int entityid, int link, std::string filter, bool reliable)
	{
		if (source)
			source->addLink(this);
		this->source = source;
		this->entityid = entityid;
		this->link = link;
		this->filter = filter;
		this->reliable = reliable;
		return true;
	}
	bool EventRemoteLink::destroy(void)
	{
		if (source)
			source->removeLink(this);
		return true;
	}
	
	void EventRemoteLink::update(void)
	{
		EventData *data = source->getData();
		if (data)
		{
			// Apply filter
			if (filter != "")
			{
				if (filter.find(data->getName()) == filter.npos)
				{
					return;
				}
			}
		}
		// Call remote event
		Connection *server = Game::get()->getServerConnection();
		Buffer *msg = new Buffer;
		msg->writeByte(ENM_Link);
		msg->writeInt(entityid);
		msg->writeInt(link);
		if (data)
		{
			data->apply(msg);
		}
		else
		{
			msg->writeInt(0);
		}
		server->sendData(msg, reliable);
		delete msg;
	}
}

