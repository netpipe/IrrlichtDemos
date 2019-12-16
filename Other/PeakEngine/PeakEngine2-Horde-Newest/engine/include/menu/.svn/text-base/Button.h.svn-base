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

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "menu/MenuElement.h"

#include <string>

//tolua_begin
namespace peak
{
	enum SpecialAction
	{
		ESA_None = 0,
		ESA_QuitGame,
		ESA_CloseMenu
	};
	
	class ButtonListener;
	
	/**
	 * \brief Simple button menu element
	 */
	class Button: public MenuElement
	{
		public:
			Button();
			virtual ~Button();
			
			virtual bool init(Menu *menu, MenuElement *parent = 0);
			
			virtual bool show(void);
			virtual bool hide(void);
			
			/**
			 * \brief Action callback
			 */
			virtual void press(void);
			
			/**
			 * \brief Sets the label of the button
			 */
			virtual void setLabel(std::string label);
			/**
			 * \brief Returns the label of the button
			 */
			virtual std::string getLabel(void);
			
			/**
			 * \brief Connect a script action to the button.
			 *
			 * When the user clicks the button, menu_action(action) in the menu
			 * script is called.
			 */
			virtual void setAction(std::string action);
			/**
			 * \brief Returns the action connected to the button
			 */
			virtual std::string getAction(void);
			
			/**
			 * \brief Connect a menu to the button.
			 *
			 * When the user clicks the button, the menu with the given name is
			 * activated.
			 */
			virtual void setMenu(std::string menu);
			/**
			 * \brief Returns the menu connected to the button
			 */
			virtual std::string getMenu(void);
			
			/**
			 * \brief Connects a special action to the button
			 */
			virtual void setSpecialAction(SpecialAction action);
			/**
			 * \brief Returns a special action connected to the button
			 */
			virtual SpecialAction getSpecialAction(void);
			
			virtual EMenuElementType getType(void);
		private:
			//tolua_end
			std::string action;
			std::string replacemenu;
			SpecialAction specialaction;
			ButtonListener *buttonlistener;
			std::string label;
			//tolua_begin
	};
}
//tolua_end

#endif

