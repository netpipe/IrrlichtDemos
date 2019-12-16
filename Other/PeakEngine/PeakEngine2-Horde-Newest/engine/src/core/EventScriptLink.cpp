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

#include "core/EventScriptLink.h"
#include "core/Script.h"
#include "core/Event.h"
#include "core/Logger.h"

namespace peak
{
	EventScriptLink::EventScriptLink()
	{
	}
	EventScriptLink::~EventScriptLink()
	{
	}
	
	bool EventScriptLink::create(Event *source, Script *script, std::string function, std::string filter)
	{
		if (source)
			source->addLink(this);
		this->source = source;
		this->script = script;
		this->function = function;
		this->filter = filter;
		return true;
	}
	bool EventScriptLink::destroy(void)
	{
		if (source)
			source->removeLink(this);
		return true;
	}
	
	void EventScriptLink::update(EventData *data)
	{
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
			// Apply event data
			data->apply(script);
		}
		if (!script->isFunction(function))
		{
			LERROR("Function \"%s\" does not exist.\n", function.c_str());
		}
		else
		{
			script->callFunction(function);
		}
	}
	void EventScriptLink::update(void)
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
			// Apply event data
			data->apply(script);
		}
		if (!script->isFunction(function))
		{
			LERROR("Function \"%s\" does not exist.\n", function.c_str());
		}
		else
		{
			script->callFunction(function);
		}
	}
}

