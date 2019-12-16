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

#include "menu/CheckBox.h"
#include "menu/Menu.h"
#include "menu/MenuManager.h"
#include "graphics/GraphicsEngine.h"
#include "core/Util.h"
#include "core/Logger.h"
#include "core/Script.h"
#include "core/GameEngine.h"
#include "core/SettingsManager.h"

#include <guichan.hpp>

namespace peak
{
	CheckBox::CheckBox() : MenuElement()
	{
	}
	CheckBox::~CheckBox()
	{
	}
	
	bool CheckBox::init(Menu *menu, MenuElement *parent)
	{
		this->menu = menu;
		this->parent = parent;
		checked = false;
		return true;
	}
	
	bool CheckBox::show(void)
	{
		widget = new gcn::CheckBox();
		MenuManager::get()->getRootWidget()->add(widget);
		setPosition(position);
		setSize(size);
		setID(id);
		setLabel(label);
		return true;
	}
	bool CheckBox::hide(void)
	{
		if (widget)
		{
			MenuManager::get()->getRootWidget()->remove(widget);
			delete widget;
			widget = 0;
		}
		return true;
	}
	
	void CheckBox::setLabel(std::string label)
	{
		this->label = label;
		if (widget)
		{
			((gcn::CheckBox*)widget)->setCaption(label);
		}
	}
	std::string CheckBox::getLabel(void)
	{
		return label;
	}
	
	void CheckBox::set(bool state)
	{
		// Change setting
		if (setting != "")
		{
			checked = state;
			SettingsManager::get()->setBool(setting, state);
			printf("Setting \"%s\" set to %d.\n", setting.c_str(), state);
		}
		// Script action
		if (action != "")
		{
			if (!menu->getScript())
			{
				LERROR("Menu action triggered but no menu script present.\n");
				return;
			}
			if (menu->getScript()->isFunction("menu_action"))
			{
				menu->getScript()->callFunction("menu_action", action);
			}
			else
			{
				LERROR("Menu action triggered but no menu handler present.\n");
				return;
			}
		}
	}
	
	void CheckBox::setAction(std::string action)
	{
		this->action = action;
	}
	std::string CheckBox::getAction(void)
	{
		return action;
	}
	
	void CheckBox::setSetting(std::string setting)
	{
		this->setting = setting;
		if (setting != "")
		{
			set(SettingsManager::get()->getBool(setting));
		}
	}
	std::string CheckBox::getSetting(void)
	{
		return setting;
	}
	
	EMenuElementType CheckBox::getType(void)
	{
		return EMET_CheckBox;
	}
}
 
