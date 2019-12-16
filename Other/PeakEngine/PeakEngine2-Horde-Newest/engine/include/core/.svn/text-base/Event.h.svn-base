
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

#ifndef _EVENT_H_
#define _EVENT_H_

#include <map>
#include <list>
#include <string>

//tolua_begin
namespace peak
{
	class Link;
	class Script;
	class Buffer;
	
	/**
	 * \brief Data passed to a script by an Event
	 */
	class EventData
	{
		public:
			EventData(std::string name);
			~EventData();
			
			std::string getName(void);
			
			// TODO: Other data types
			// TODO: Networking
			void set(std::string name, float value);
			void set(Buffer *buffer);
			void apply(Script *script);
			void apply(Buffer *buffer);
			//tolua_end
		private:
			std::map<std::string, float> values;
			std::string name;
			//tolua_begin
	};
	/**
	 * \brief Base class for all events
	 */
	class Event
	{
		public:
			Event();
			~Event();
			
			/**
			 * \brief Triggers the event and executes all links attached to it
			 * \brief data Data passed to the event
			 */
			void trigger(EventData *data = 0);
			
			/**
			 * \brief Returns the event data
			 */
			EventData *getData(void);
			
			/**
			 * \brief Adds a link to the event
			 */
			void addLink(Link *link);
			/**
			 * \brief Removes a link from the event
			 */
			void removeLink(Link *link);
			//tolua_end
		private:
			EventData *data;
			std::list<Link*> links;
			//tolua_begin
	};
}
//tolua_end

#endif
