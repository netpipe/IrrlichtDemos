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

#include "menu/Label.h"
#include "graphic/GraphicsEngine.h"
#include "core/Util.h"

#ifdef _USE_IRRLICHT_
#include <irrlicht.h>
using namespace irr;
#endif

namespace peak
{
	Label::Label() : MenuElement()
	{
	}
	Label::~Label()
	{
	}

	bool Label::init(Menu *menu, MenuElement *parent)
	{
		this->menu = menu;
		this->parent = parent;
		return true;
	}

	bool Label::show(void)
	{
		#ifdef _USE_IRRLICHT_
		gui::IGUIEnvironment *guienv = GraphicsEngine::get()->getGUI();
		element = guienv->addStaticText(toWString(text).c_str(), core::rect<s32>(10, 10, 100, 100), false, true, 0, -1, true);
		setPosition(position);
		setSize(size);
		setID(id);
		return true;
		#endif
		return false;
	}
	bool Label::hide(void)
	{
		#ifdef _USE_IRRLICHT_
		if (element)
		{
			element->remove();
			element = 0;
		}
		return true;
		#endif
		return false;
	}

	EMenuElementType Label::getType(void)
	{
		return EMET_Label;
	}
}

