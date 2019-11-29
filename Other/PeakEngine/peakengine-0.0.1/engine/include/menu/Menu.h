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

#ifndef _MENU_H_
#define _MENU_H_

#include <string>
#include <list>

//tolua_begin
namespace peak
{
	class Script;
	class MenuElement;
	class Button;
	class Label;
	class ListBox;
	
	class Menu
	{
		public:
			Menu();
			~Menu();
			
			/**
			 * \brief Initialize menu with the given name.
			 */
			bool init(std::string name);
			/**
			 * \brief Destroys the menu and all its elements.
			 */
			bool destroy(void);
			
			/**
			 * \brief Returns the name of the menu.
			 */
			std::string getName(void);
			
			/**
			 * \brief Shows all the menu elements.
			 */
			bool show(void);
			/**
			 * \brief Hides all the menu elements.
			 */
			bool hide(void);
			
			bool pressButton(int id);
			bool setCheckBox(int id, bool value);
			bool setListBox(int id, int selected);
			
			ListBox *getListBox(int id);
			Button *getButton(int id);
			Label *getLabel(int id);
			
			MenuElement *getElement(int id);
			
			/**
			 * \brief Returns the script defining the window.
			 */
			Script *getScript(void);
		private:
			//tolua_end
			Script *script;
			std::string name;
			std::list<MenuElement*> elements;
			//tolua_begin
	};
}
//tolua_end

#endif

