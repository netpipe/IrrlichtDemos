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

#ifndef _LABELOVERLAY_H_
#define _LABELOVERLAY_H_

#include "graphics/Overlay.h"

#include <string>

//tolua_begin
namespace peak
{
	/**
	 * \brief Overlay displaying some text
	 */
	class LabelOverlay : public Overlay
	{
		public:
			LabelOverlay(std::string label = "", std::string font = "", Overlay *parent = 0);
			virtual ~LabelOverlay();
			
			/**
			 * \brief Sets the label of the overlay
			 */
			void setLabel(std::string label);
			/**
			 * \brief Returns the label of the overlay
			 */
			std::string getLabel(void);
			
			virtual bool render(void);
		private:
			//tolua_end
			std::string label;
			int material;
			//tolua_begin
	};
}
//tolua_end

#endif

