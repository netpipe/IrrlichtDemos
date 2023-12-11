/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakgraphics/menu/Menu.hpp"
#include "peakgraphics/menu/MenuInputListener.hpp"
#include "peakgraphics/Graphics.hpp"
#include <peakengine/support/ScopedLock.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

namespace peak
{
	Menu::Menu(Graphics *graphics, std::string themename)
		: Loadable(), graphics(graphics), themename(themename), guimgr(0),
		theme(0), changed(false), clear(false), clearcolor(0)
	{
		listener = new MenuInputListener(this);
		graphics->registerLoading(this);
	}
	Menu::~Menu()
	{
		delete listener;
	}

	bool Menu::load()
	{
		// Load theme
		theme = CResourceManager::getInstancePtr()->getGUITheme(themename.c_str());
		if (!theme)
			return false;
		// Create render layer
		// TODO: Custom background color
		guimgr = new gui::CGUIManager(core::CColorI(0, 0, 0, 255), false);
		guimgr->setActiveTheme(theme);
		return true;
	}
	bool Menu::destroy()
	{
		if (!guimgr)
			return false;
		guimgr->drop();
		return true;
	}

	void Menu::show()
	{
		graphics->getWindow()->addGUIManager(guimgr);
	}
	void Menu::hide()
	{
		graphics->getWindow()->removeGUIManager(guimgr);
	}

	std::string Menu::getThemeName()
	{
		return themename;
	}

	void Menu::onAction(MenuElement *element)
	{
	}

	void Menu::registerMenu(Menu *menu, std::string name)
	{
		menus.insert(std::pair<std::string, Menu*>(name, menu));
	}
	void Menu::deregisterMenu(std::string name)
	{
		std::map<std::string, MenuPointer>::iterator it = menus.find(name);
		if (it == menus.end())
			return;
		menus.erase(it);
	}
	void Menu::deregisterMenu(Menu *menu)
	{
		deregisterMenu(menu->getName());
	}
	Menu *Menu::getMenu(std::string name)
	{
		std::map<std::string, MenuPointer>::iterator it = menus.find(name);
		if (it == menus.end())
			return 0;
		return it->second.get();
	}

	void Menu::setClear(bool clear)
	{
		if (this->clear == clear)
			return;
		this->clear = clear;
		changed = true;
	}
	bool Menu::getClear()
	{
		return clear;
	}
	void Menu::setClearColor(unsigned int color)
	{
		if (clearcolor == color)
			return;
		clearcolor = color;
		changed = true;
	}
	unsigned int Menu::getClearColor()
	{
		return clearcolor;
	}

	void Menu::setActive(bool active)
	{
		if (active)
			activemenu = this;
		else if (activemenu == this)
			activemenu = 0;
	}
	bool Menu::isActive()
	{
		return activemenu == this;
	}
	Menu *Menu::getActiveMenu()
	{
		return activemenu;
	}

	void Menu::addRootElement(MenuElement *element)
	{
		ScopedLock lock(mutex);
		rootelements.push_back(element);
	}
	void Menu::removeRootElement(MenuElement *element)
	{
		ScopedLock lock(mutex);
		for (unsigned int i = 0; i < rootelements.size(); i++)
		{
			if (rootelements[i] == element)
			{
				rootelements.erase(rootelements.begin() + i);
				break;
			}
		}
	}

	void Menu::registerInput(MenuElement *element)
	{
		listener->registerInput(element);
	}
	void Menu::deregisterInput(MenuElement *element)
	{
		listener->deregisterInput(element);
	}

	void Menu::registerParentChange(MenuElement *element)
	{
		parentchange.push(element);
	}
	void Menu::update()
	{
		if (changed && guimgr)
		{
			// Set background color
			guimgr->setClearBuffers(clear);
			guimgr->setClearColor(core::CColorI(clearcolor, core::ECO_ARGB));
			changed = false;
		}
		// Update elements
		mutex.lock();
		for (unsigned int i = 0; i < rootelements.size(); i++)
		{
			rootelements[i]->update();
		}
		mutex.unlock();
	}
	void Menu::updateAll()
	{
		// Set active menu
		if (activemenu != shownmenu)
		{
			if (shownmenu)
				shownmenu->hide();
			if (activemenu)
				activemenu->show();
			shownmenu = activemenu;
		}
		// Update menu elements
		while (parentchange.size() > 0)
		{
			MenuElementPointer element = parentchange.front();
			parentchange.pop();
			element->updateParent();
		}
		// Update active menu
		if (activemenu)
			activemenu->update();
	}

	Graphics *Menu::getGraphics()
	{
		return graphics;
	}
	lf::gui::CGUIManager *Menu::getGUIManager()
	{
		return guimgr;
	}
	MenuInputListener *Menu::getInputListener()
	{
		return listener;
	}

	std::map<std::string, SharedPointer<Menu> > Menu::menus;
	Menu *Menu::activemenu = 0;
	Menu *Menu::shownmenu = 0;

	Mutex Menu::parentchangemutex;
	std::queue<MenuElementPointer> Menu::parentchange;
}
