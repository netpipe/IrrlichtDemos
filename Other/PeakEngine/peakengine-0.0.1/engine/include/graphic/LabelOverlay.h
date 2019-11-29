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

#include "graphic/Overlay.h"
#include "core/Color.h"

#include <string>

#ifdef _USE_IRRLICHT_
namespace irr
{
	namespace gui
	{
		class IGUIFont;
	}
}
#endif

//tolua_begin
namespace peak
{
	class LabelOverlay : public Overlay
	{
		public:
			LabelOverlay(std::string label = "", Color color = Color(0, 0, 0, 255), std::string font = "", Overlay *parent = 0);
			virtual ~LabelOverlay();
			
			void setLabel(std::string label);
			std::string getLabel(void);
			
			void setColor(Color color);
			Color getColor(void);
			
			virtual bool render(void);
		private:
			//tolua_end
			std::string label;
			Color color;
			#ifdef _USE_IRRLICHT_
			irr::gui::IGUIFont *font;
			#endif
			//tolua_begin
	};
}
//tolua_end

#endif

