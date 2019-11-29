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

#include "menu/ListBox.h"
#include "menu/Menu.h"
#include "graphic/GraphicsEngine.h"
#include "core/Util.h"
#include "core/Logger.h"
#include "core/Script.h"

#ifdef _USE_IRRLICHT_
#include <irrlicht.h>
using namespace irr;
#endif

namespace peak
{
	ListBox::ListBox() : MenuElement()
	{
		chosen = -1;
	}
	ListBox::~ListBox()
	{
	}

	bool ListBox::init(Menu *menu, MenuElement *parent)
	{
		this->menu = menu;
		this->parent = parent;
		return true;
	}

	bool ListBox::show(void)
	{
		#ifdef _USE_IRRLICHT_
		gui::IGUIEnvironment *guienv = GraphicsEngine::get()->getGUI();
		element = guienv->addListBox(core::rect<s32>(10, 10, 100, 100), 0, -1, true);
		setPosition(position);
		setSize(size);
		setID(id);
		updateItems();
		return true;
		#endif
		return false;
	}
	bool ListBox::hide(void)
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

	void ListBox::choose(int index)
	{
		chosen = index;
		//((gui::IGUIListBox*)element)->setSelected(index);
		// Trigger action
		if (action != "")
		{
			if (!menu->getScript())
			{
				LERROR("Menu action triggered but no menu script present.\n");
				return;
			}
			if (menu->getScript()->isFunction("menu_action"))
			{
				menu->getScript()->callFunction("menu_action", action, index);
			}
			else
			{
				LERROR("Menu action triggered but no menu handler present.\n");
				return;
			}
		}
	}
	int ListBox::getSelected(void)
	{
		return chosen;
	}

	void ListBox::addItem(std::string label, int index)
	{
		// TODO: index
		items.push_back(label);
		updateItems();
	}
	void ListBox::deleteItem(int index)
	{
		items.erase(items.begin() + index);
		updateItems();
	}
	int ListBox::getItemCount(void)
	{
		return items.size();
	}
	std::string ListBox::getItem(int index)
	{
		return items[index];
	}
	void ListBox::clearItems(void)
	{
		items.clear();
		updateItems();
	}

	void ListBox::updateItems(void)
	{
		#ifdef _USE_IRRLICHT_
		if (element)
		{
			((gui::IGUIListBox*)element)->clear();
			for (unsigned int i = 0; i < items.size(); i++)
			{
				((gui::IGUIListBox*)element)->addItem(toWString(items[i]).c_str());
			}
		}
		#endif
	}

	void ListBox::setAction(std::string action)
	{
		this->action = action;
	}
	std::string ListBox::getAction(void)
	{
		return action;
	}

	EMenuElementType ListBox::getType(void)
	{
		return EMET_ListBox;
	}
}

