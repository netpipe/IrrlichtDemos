
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

#ifndef _DIRECTLINK_H_
#define _DIRECTLINK_H_

#include "core/Link.h"

//tolua_begin
namespace peak
{
	class Property;

	/**
	 * \brief Link which simply copies the value of a property to another whenever
	 * it is changed
	 */
	class DirectLink : public Link
	{
		public:
			DirectLink();
			virtual ~DirectLink();
			
			/**
			 * \brief Creates the link
			 * \param source Source property
			 * \param target Target property
			 */
			virtual bool create(Property *source, Property *target);
			virtual bool destroy(void);
			
			virtual void update(void);
			//tolua_end
		private:
			Property *source;
			Property *target;
			//tolua_begin
	};
}
//tolua_end

#endif
