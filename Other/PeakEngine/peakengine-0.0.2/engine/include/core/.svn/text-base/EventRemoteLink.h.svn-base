
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

#ifndef _EVENTREMOTELINK_H_
#define _EVENTREMOTELINK_H_

#include "core/Link.h"

#include <string>

//tolua_begin
namespace peak
{
	class Event;
	
	/**
	 * \brief Link which calls an EventSciptLink on the server
	 */
	class EventRemoteLink : public Link
	{
		public:
			EventRemoteLink();
			virtual ~EventRemoteLink();
			
			/**
			 * \brief Creates the link
			 * \param source Source event
			 * \param entityid ID of the target entity
			 * \param link ID of the target link
			 * \param filter Filter to apply to the event data
			 */
			bool create(Event *source, int entityid, int link, std::string filter);
			virtual bool destroy(void);
			
			virtual void update(void);
			//tolua_end
		private:
			Event *source;
			int entityid;
			int link;
			std::string filter;
			//tolua_begin
	};
}
//tolua_end

#endif

