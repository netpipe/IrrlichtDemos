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

#include "menu/Button.h"
#include "menu/Menu.h"
#include "menu/MenuManager.h"
#include "graphic/GraphicsEngine.h"
#include "core/Util.h"
#include "core/Logger.h"
#include "core/Script.h"
#include "core/GameEngine.h"

#ifdef _USE_IRRLICHT_
#include <irrlicht.h>
using namespace irr;
#endif

namespace peak
{
	Button::Button() : MenuElement()
	{
	}
	Button::~Button()
	{
	}

	bool Button::init(Menu *menu, MenuElement *parent)
	{
		this->menu = menu;
		this->parent = parent;
		specialaction = ESA_None;
		return true;
	}

	bool Button::show(void)
	{
		#ifdef _USE_IRRLICHT_
		gui::IGUIEnvironment *guienv = GraphicsEngine::get()->getGUI();
		element = guienv->addButton(core::rect<s32>(10, 10, 100, 100), 0, -1, L"");
		element->setText(toWString(text).c_str());
		setPosition(position);
		setSize(size);
		setID(id);
		return true;
		#endif
		return false;
	}
	bool Button::hide(void)
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

	void Button::press(void)
	{
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
		// Special action
		if (specialaction != ESA_None)
		{
			if (specialaction == ESA_QuitGame)
			{
				GameEngine::get()->stopGame();
			}
			else if (specialaction == ESA_CloseMenu)
			{
				MenuManager::get()->activateMenu(0);
				return;
			}
		}
		// Replace the menu if needed
		// WARNING: The button gets deleted at this point, you MUST NOT access any member variables afterwards.
		if (replacemenu != "")
		{
			MenuManager::get()->activateMenu(replacemenu);
		}
	}

	void Button::setAction(std::string action)
	{
		this->action = action;
	}
	std::string Button::getAction(void)
	{
		return action;
	}

	void Button::setMenu(std::string menu)
	{
		replacemenu = menu;
	}
	std::string Button::getMenu(void)
	{
		return replacemenu;
	}

	void Button::setSpecialAction(SpecialAction action)
	{
		specialaction = action;
	}
	SpecialAction Button::getSpecialAction(void)
	{
		return specialaction;
	}

	EMenuElementType Button::getType(void)
	{
		return EMET_Button;
	}
}

