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

#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

#include <string>
#include <vector>

namespace gcn
{
	class Gui;
	class Container;
	class ImageFont;
	class Label;
	class OpenGLGraphics;
}

//tolua_begin
namespace peak
{
	class Menu;
	class ImageLoader;
	class GUIInput;
	
	/**
	 * \brief Class which creates and manages menus.
	 */
	class MenuManager
	{
		public:
			/**
			 * \brief Returns pointer to the MenuManager class.
			 *
			 * \return Pointer to the menu manager
			 */
			static MenuManager *get(void);
			~MenuManager();
			
			/**
			 * \brief Loads a menu with the given name.
			 * \param name Name of the menu
			 */
			bool loadMenu(std::string name);
			/**
			 * \brief Destroys a menu.
			 * \param name Name of the menu
			 */
			void unloadMenu(std::string name);
			/**
			 * \brief Destroys a menu.
			 * \param menu Pointer to the menu
			 */
			void unloadMenu(Menu *menu);
			
			/**
			 * \brief Returns the number of loaded menus.
			 */
			int getMenuCount(void);
			/**
			 * \brief Returns the menu with the given index.
			 */
			Menu *getMenu(int index);
			/**
			 * \brief Returns the menu with the name.
			 */
			Menu *getMenu(std::string name);
			/**
			 * \brief Returns the menu which is currently activated.
			 * \return Currently activated menu or 0 if no menu is activated.
			 */
			Menu *getCurrentMenu(void);
			
			/**
			 * \brief Initializes the menu system.
			 */
			bool init(void);
			/**
			 * \brief Destroys the menu system.
			 */
			bool shutdown(void);
			
			/**
			 * \brief Activates the menu with the given name.
			 */
			void activateMenu(std::string name);
			/**
			 * \brief Activates the menu.
			 */
			void activateMenu(Menu *menu);
			
			//tolua_end
			void injectMouseButton(int button, int x, int y, int state);
			void injectMouseMoved(int x, int y);
			void injectMouseWheel(int x, int y, int wheel);
			void injectKey(int key, int state);
			void render(void);
			
			gcn::Container *getRootWidget(void);
		private:
			MenuManager();
			
			std::vector<Menu*> menus;
			Menu *activated;
			bool closemenu;
			Menu *activating;
			
			gcn::OpenGLGraphics *graphics;
			ImageLoader *imageloader;
			GUIInput *input;
			gcn::Gui *gui;
			gcn::Container *top;
			gcn::ImageFont *font;
			gcn::Label *label;

			//tolua_begin
	};
}
//tolua_end

#endif

