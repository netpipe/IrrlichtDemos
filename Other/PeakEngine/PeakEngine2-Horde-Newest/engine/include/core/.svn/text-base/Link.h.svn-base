
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

#ifndef _LINK_H_
#define _LINK_H_

//tolua_begin
namespace peak
{
	/**
	 * \brief Class which creates an automatic link
	 *
	 * Links can be created between two properties or from a Property or an Event
	 * to a script function.
	 */
	class Link
	{
		public:
			Link();
			virtual ~Link();
			
			virtual bool create();
			/**
			 * \brief Destroys the link and removes it from both
			 * source and target.
			 */
			virtual bool destroy(void);
			
			/**
			 * \brief Sets whether the link is active
			 */
			virtual void setActive(bool active);
			/**
			 * \brief Returns whether the link is active
			 */
			virtual bool isActive(void);
			
			//tolua_end
			/**
			 * \brief Executes the link
			 */
			virtual void update(void);
		private:
			bool active;
			//tolua_begin
	};
}
//tolua_end

#endif
