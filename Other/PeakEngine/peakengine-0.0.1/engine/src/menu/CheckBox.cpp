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
#include "graphic/GraphicsEngine.h"
#include "core/Util.h"
#include "core/Logger.h"
#include "core/Script.h"
#include "core/GameEngine.h"
#include "core/SettingsManager.h"

#ifdef _USE_IRRLICHT_
#include <irrlicht.h>
using namespace irr;
#endif

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
		#ifdef _USE_IRRLICHT_
		gui::IGUIEnvironment *guienv = GraphicsEngine::get()->getGUI();
		element = guienv->addCheckBox(true, core::rect<s32>(10, 10, 100, 100));
		element->setText(toWString(text).c_str());
		setPosition(position);
		setSize(size);
		setID(id);
		set(checked);
		return true;
		#endif
		return false;
	}
	bool CheckBox::hide(void)
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

	void CheckBox::set(bool state)
	{
		#ifdef _USE_IRRLICHT_
		if (element)
		{
			((irr::gui::IGUICheckBox*)element)->setChecked(state);
		}
		#endif
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

