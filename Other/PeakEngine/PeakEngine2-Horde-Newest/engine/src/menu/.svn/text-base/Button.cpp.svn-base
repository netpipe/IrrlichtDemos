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
#include "graphics/GraphicsEngine.h"
#include "core/Util.h"
#include "core/Logger.h"
#include "core/Script.h"
#include "core/GameEngine.h"

#include <guichan.hpp>

namespace peak
{
	class ButtonListener : public gcn::ActionListener
	{
		public:
			ButtonListener(Button *button)
			{
				this->button = button;
			}
			
			virtual void action (const gcn::ActionEvent &actionEvent)
			{
				button->press();
			}
		private:
			Button *button;
	};
	
	Button::Button() : MenuElement()
	{
		buttonlistener = new ButtonListener(this);
	}
	Button::~Button()
	{
		delete buttonlistener;
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
		/*#ifdef _USE_IRRLICHT_
		gui::IGUIEnvironment *guienv = GraphicsEngine::get()->getGUI();
		element = guienv->addButton(core::rect<s32>(10, 10, 100, 100), 0, -1, L"");
		element->setText(toWString(text).c_str());*/
		widget = new gcn::Button();
		widget->addActionListener(buttonlistener);
		MenuManager::get()->getRootWidget()->add(widget);
		setPosition(position);
		setSize(size);
		setID(id);
		setLabel(label);
		return true;
	}
	bool Button::hide(void)
	{
		if (widget)
		{
			widget->removeActionListener(buttonlistener);
			MenuManager::get()->getRootWidget()->remove(widget);
			delete widget;
			widget = 0;
		}
		return true;
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
		// Note: This is not done at once, but rather after the next frame
		if (replacemenu != "")
		{
			MenuManager::get()->activateMenu(replacemenu);
		}
	}
	
	void Button::setLabel(std::string label)
	{
		this->label = label;
		if (widget)
		{
			((gcn::Button*)widget)->setCaption(label);
		}
	}
	std::string Button::getLabel(void)
	{
		return label;
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

