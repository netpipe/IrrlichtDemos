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

#include "menu/ComboBox.h"
#include "graphic/GraphicsEngine.h"
#include "core/Util.h"

#ifdef _USE_IRRLICHT_
#include <irrlicht.h>
using namespace irr;
#endif

namespace peak
{
	ComboBox::ComboBox() : ListBox()
	{
		chosen = 0;
	}
	ComboBox::~ComboBox()
	{
	}

	bool ComboBox::init(Menu *menu, MenuElement *parent)
	{
		return ListBox::init(menu, parent);
	}

	bool ComboBox::show(void)
	{
		#ifdef _USE_IRRLICHT_
		gui::IGUIEnvironment *guienv = GraphicsEngine::get()->getGUI();
		element = guienv->addComboBox(core::rect<s32>(10, 10, 100, 100), 0, -1);
		setPosition(position);
		setSize(size);
		setID(id);
		updateItems();
		return true;
		#endif
		return false;
	}
	bool ComboBox::hide(void)
	{
		return ListBox::hide();
	}

	void ComboBox::updateItems(void)
	{
		#ifdef _USE_IRRLICHT_
		if (element)
		{
			((gui::IGUIComboBox*)element)->clear();
			for (unsigned int i = 0; i < items.size(); i++)
			{
				((gui::IGUIComboBox*)element)->addItem(toWString(items[i]).c_str());
			}
		}
		#endif
	}

	EMenuElementType ComboBox::getType(void)
	{
		return EMET_ComboBox;
	}
}

