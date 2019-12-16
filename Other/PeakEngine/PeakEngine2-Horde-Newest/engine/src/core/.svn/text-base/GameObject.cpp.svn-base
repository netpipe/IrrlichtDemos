
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

#include <core/GameObject.h>
#include <core/Property.h>
#include <core/Event.h>

namespace peak
{
	GameObject::GameObject()
	{
	}
	GameObject::~GameObject()
	{
		std::map<std::string, Property*>::iterator it = properties.begin();
		while (it != properties.end())
		{
			delete it->second;
			it++;
		}
	}
	
	Property *GameObject::getProperty(std::string name)
	{
		if (properties.find(name) != properties.end())
		{
			return properties[name];
		}
		if (aliases.find(name) != aliases.end())
		{
			return aliases[name];
		}
		return 0;
	}
	Event *GameObject::getEvent(std::string name)
	{
		if (events.find(name) != events.end())
		{
			return events[name];
		}
		return 0;
	}
	
	void GameObject::onPropertyChanged(std::string name)
	{
		// Trigger events
	}

	void GameObject::addProperty(std::string name, Property *property)
	{
		properties.insert(std::pair<std::string, Property*>(name, property));
		property->setGameObject(this);
	}
	void GameObject::deleteProperty(std::string name)
	{
	}
	
	void GameObject::addAlias(std::string name, Property *property)
	{
		aliases.insert(std::pair<std::string, Property*>(name, property));
	}
	void GameObject::deleteAlias(std::string name)
	{
		std::map<std::string, Property*>::iterator it = properties.begin();
		while (it != properties.end())
		{
			if (it->first == name)
			{
				delete it->second;
				properties.erase(it);
				return;
			}
			it++;
		}
	}
	
	void GameObject::addEvent(std::string name, Event *event)
	{
		events.insert(std::pair<std::string, Event*>(name, event));
	}
	void GameObject::deleteEvent(std::string name)
	{
		std::map<std::string, Event*>::iterator it = events.begin();
		while (it != events.end())
		{
			if (it->first == name)
			{
				delete it->second;
				events.erase(it);
				return;
			}
			it++;
		}
	}
}
