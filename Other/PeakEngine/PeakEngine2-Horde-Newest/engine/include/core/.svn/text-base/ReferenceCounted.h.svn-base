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

#ifndef _REFERENCECOUNTED_H_
#define _REFERENCECOUNTED_H_

#include "core/Logger.h"

namespace peak
{
	/**
	 * \brief Class which  automatically deletes an object when it isn't used
	 * any more
	 *
	 * This is achieved using a use counter, when the use counter becomes 0 the
	 * class deletes itself (the use counter is 0 when the class is created, of
	 * course at this moment it is not deleted).
	 */
	class ReferenceCounted
	{
		public:
			ReferenceCounted()
			{
				usecount = 0;
			}
			virtual ~ReferenceCounted()
			{
			}
			
			/**
			 * \brief Grabs the object
			 *
			 * This increases the use counter.
			 */
			void grab(void)
			{
				usecount++;
			}
			/**
			 * \brief Drops the object
			 *
			 * This decreases the use counter.
			 */
			bool drop(void)
			{
				usecount--;
				if (usecount == 0)
				{
					delete this;
					return false;
				}
				if (usecount < 0)
				{
					LERROR("ReferenceCounted: usecount < 0 (%p).\n", this);
					delete this;
					return false;
				}
				return true;
			}
			
		private:
			int usecount;
	};
}

#endif

