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

#include <guichan.hpp>
#include <guichan/opengl/openglgraphics.hpp>
#include <GL/glfw.h>

#include "menu/MenuManager.h"
#include "graphics/GraphicsEngine.h"
#include "menu/Menu.h"
#include "core/InputManager.h"
#include "core/Logger.h"
#include "gui/ImageLoader.h"
#include "gui/GUIInput.h"
#include "core/SettingsManager.h"
#include "core/FileSystem.h"

namespace peak
{
	MenuManager *MenuManager::get(void)
	{
		static MenuManager menumgr;
		return &menumgr;
	}
	MenuManager::MenuManager()
	{
		activated = 0;
		activating = 0;
		closemenu = false;
	}
	MenuManager::~MenuManager()
	{
	}

	bool MenuManager::init(void)
	{
		try
		{
			Vector2DI size = SettingsManager::get()->getVector2DI("window.resolution");
			// Setup guichan
			imageloader = new ImageLoader();
			gcn::Image::setImageLoader(imageloader);
			graphics = new gcn::OpenGLGraphics(size.x, size.y);
			input = new GUIInput();
			font = new gcn::ImageFont(FileSystem::get()->getFileExists("menus/fixedfont.png"), " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
			gcn::Widget::setGlobalFont(font);

			gui = new gcn::Gui();
			gui->setGraphics(graphics);
			gui->setInput(input);

			top = new gcn::Container();
			top->setOpaque(false);
			top->setDimension(gcn::Rectangle(0, 0, size.x, size.y));
			gui->setTop(top);
		}
		catch (gcn::Exception e)
		{
			std::cout << e.getMessage() << std::endl;
		}


		return true;
	}
	bool MenuManager::shutdown(void)
	{
		while (menus.size() > 0)
		{
			unloadMenu(menus[0]);
		}
		delete top;
		delete gui;
		delete font;
		delete input;
		delete graphics;
		delete imageloader;
		return true;
	}

	bool MenuManager::loadMenu(std::string name)
	{
		Menu *newmenu = new Menu;
		menus.push_back(newmenu);
		if (!newmenu->init(name))
		{
			menus.erase(menus.begin() + (menus.size() - 1));
			delete newmenu;
			return false;
		}
		return true;
	}
	void MenuManager::unloadMenu(std::string name)
	{
		for (unsigned int i = 0; i < menus.size(); i++)
		{
			if (menus[i]->getName() == name)
			{
				menus[i]->destroy();
				delete menus[i];
				menus.erase(menus.begin() + i);
				return;
			}
		}
	}
	void MenuManager::unloadMenu(Menu *menu)
	{
		for (unsigned int i = 0; i < menus.size(); i++)
		{
			if (menus[i] == menu)
			{
				menus[i]->destroy();
				delete menus[i];
				menus.erase(menus.begin() + i);
				return;
			}
		}
	}

	int MenuManager::getMenuCount(void)
	{
		return menus.size();
	}
	Menu *MenuManager::getMenu(int index)
	{
		return menus[index];
	}
	Menu *MenuManager::getMenu(std::string name)
	{
		for (unsigned int i = 0; i < menus.size(); i++)
		{
			if (menus[i]->getName() == name)
			{
				return menus[i];
			}
		}
		LERROR("Did not find menu \"%s\".\n", name.c_str());
		return 0;
	}
	Menu *MenuManager::getCurrentMenu(void)
	{
		return activated;
	}

	void MenuManager::activateMenu(std::string name)
	{
		if (name == "") closemenu = true;
		// Look for menu
		for (unsigned int i = 0; i < menus.size(); i++)
		{
			if (menus[i]->getName() == name)
			{
				activating = menus[i];
				return;
			}
		}
		// Menu not found - load it.
		if (name != "")
		{
			if (loadMenu(name))
			{
				activateMenu(name);
			}
			else
			{
				LERROR("Could not activate menu \"%s\".\n", name.c_str());
			}
		}
	}
	void MenuManager::activateMenu(Menu *menu)
	{
		if (!menu) closemenu = true;
		activating = menu;
	}

	void MenuManager::injectMouseButton(int button, int x, int y, int state)
	{
		if (state)
			input->pushMouseButtonPressed(x, y, button + 1);
		else
			input->pushMouseButtonReleased(x, y, button + 1);
	}
	void MenuManager::injectMouseMoved(int x, int y)
	{
		input->pushMouseMoved(x, y);
	}
	void MenuManager::injectMouseWheel(int x, int y, int wheel)
	{
		if (wheel == -1)
			input->pushMouseWheelMovedUp(x, y);
		if (wheel == 1)
			input->pushMouseWheelMovedDown(x, y);
	}
	void MenuManager::injectKey(int key, int state)
	{
		if (state)
			input->pushKeyPressed(key);
		else
			input->pushKeyReleased(key);
	}

	void MenuManager::render(void)
	{
		// Switch menu
		if (activating)
		{
			if (activated)
			{
				activated->hide();
			}
			activated = activating;
			activated->show();
			glfwEnable(GLFW_KEY_REPEAT);
			activating = 0;
			InputManager::get()->setInputEnabled(false);
		}
		// Close menu
		if (closemenu)
		{
			if (activated)
			{
				activated->hide();
				activated = 0;
				glfwDisable(GLFW_KEY_REPEAT);
				InputManager::get()->setInputEnabled(true);
			}
			closemenu = false;
		}
		// Update and render menu
		gui->logic();
		gui->draw();
	}

	gcn::Container *MenuManager::getRootWidget(void)
	{
		return top;
	}
}

