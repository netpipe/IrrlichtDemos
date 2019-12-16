
/*
Copyright (C) 2008   Mathias Gottschlag, Christian Reiser

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

#include "core/Event.h"
#include "core/Link.h"
#include "core/Script.h"
#include "core/Buffer.h"

namespace peak
{
	EventData::EventData(std::string name)
	{
		this->name = name;
	}
	EventData::~EventData()
	{
	}
	
	std::string EventData::getName(void)
	{
		return name;
	}
	
	void EventData::set(std::string name, float value)
	{
		values[name] = value;
	}
	void EventData::set(Buffer *buffer)
	{
	}
	void EventData::apply(Script *script)
	{
		std::map<std::string, float>::iterator it = values.begin();
		while (it != values.end())
		{
			// Set script variable
			script->registerVariable(it->second, it->first);
			
			it++;
		}
	}
	void EventData::apply(Buffer *buffer)
	{
		// Write value count
		buffer->writeInt(values.size());
		// Write values
		std::map<std::string, float>::iterator it = values.begin();
		while (it != values.end())
		{
			buffer->writeString(it->first);
			buffer->writeFloat(it->second);
			it++;
		}
	}

	Event::Event()
	{
		data = 0;
	}
	Event::~Event()
	{
	}
	
	void Event::trigger(EventData *data)
	{
		this->data = data;
		// Update links
		std::list<Link*>::iterator it = links.begin();
		while (it != links.end())
		{
			(*it)->update();
			it++;
		}
	}
	
	EventData *Event::getData(void)
	{
		return data;
	}
	
	void Event::addLink(Link *link)
	{
		links.push_back(link);
	}
	void Event::removeLink(Link *link)
	{
		std::list<Link*>::iterator it = links.begin();
		while (it != links.end())
		{
			if (*it == link)
			{
				links.erase(it);
				return;
			}
			it++;
		}
	}
}

