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
#include "graphics/GraphicsEngine.h"
#include "core/Util.h"
#include "core/Logger.h"
#include "core/Script.h"
#include "menu/MenuManager.h"
#include "menu/ListModel.h"
#include "menu/ListBoxListener.h"

#include <guichan.hpp>

namespace peak
{
	ListBox::ListBox() : MenuElement()
	{
		chosen = -1;
		model = new ListModel;
		listener = new ListBoxListener(this);
	}
	ListBox::~ListBox()
	{
		delete model;
		delete listener;
	}
	
	bool ListBox::init(Menu *menu, MenuElement *parent)
	{
		this->menu = menu;
		this->parent = parent;
		return true;
	}
	
	bool ListBox::show(void)
	{
		listbox = new gcn::ListBox(model);
		((gcn::ListBox*)listbox)->addSelectionListener(listener);
		widget = new gcn::ScrollArea(listbox);
		MenuManager::get()->getRootWidget()->add(widget);
		widget->setFrameSize(1);
		setPosition(position);
		setSize(size);
		setID(id);
		return true;
	}
	bool ListBox::hide(void)
	{
		if (widget)
		{
			MenuManager::get()->getRootWidget()->remove(widget);
			delete widget;
			if (widget != listbox)
			{
				delete listbox;
			}
			widget = 0;
		}
		return true;
	}
	
	void ListBox::choose(int index)
	{
		if (index == -1)
		{
			chosen = ((gcn::ListBox*)listbox)->getSelected();
		}
		else
		{
			chosen = index;
			((gcn::ListBox*)listbox)->setSelected(chosen);
		}
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
		model->addElement(label);
		updateItems();
	}
	void ListBox::deleteItem(int index)
	{
		model->deleteElementAt(index);
		updateItems();
	}
	int ListBox::getItemCount(void)
	{
		return model->getNumberOfElements();
	}
	std::string ListBox::getItem(int index)
	{
		return model->getElementAt(index);
	}
	void ListBox::clearItems(void)
	{
		model->clear();
		updateItems();
	}
	
	void ListBox::updateItems(void)
	{
		if (widget)
		{
			((gcn::ListBox*)listbox)->setListModel(model);
		}
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

