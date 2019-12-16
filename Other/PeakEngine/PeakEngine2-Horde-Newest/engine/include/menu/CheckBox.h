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

#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include "menu/MenuElement.h"

#include <string>

//tolua_begin
namespace peak
{
	/**
	 * \brief Check box menu element
	 */
	class CheckBox: public MenuElement
	{
		public:
			CheckBox();
			virtual ~CheckBox();
			
			virtual bool init(Menu *menu, MenuElement *parent = 0);
			
			virtual bool show(void);
			virtual bool hide(void);
			
			/**
			 * \brief Sets the checkbox to a new value
			 */
			virtual void set(bool value);
			
			/**
			 * \brief Sets the label of the check box
			 */
			virtual void setLabel(std::string label);
			/**
			 * \brief Returns the label of the check box
			 */
			virtual std::string getLabel(void);

			/**
			 * \brief Connect a script action to the checkbox.
			 *
			 * When the user toggles the checkbox, menu_action(action) in the menu
			 * script is called.
			 */
			virtual void setAction(std::string action);
			/**
			 * \brief Returns the action connected to the checkbox
			 */
			virtual std::string getAction(void);
			
			/**
			 * \brief Connects a setting to the checkbox
			 *
			 * Whenever the checkbox is changed, the setting is updated to the
			 * value of the checkbox
			 */
			virtual void setSetting(std::string setting);
			/**
			 * \brief Returns the setting connected to the checkbox
			 */
			virtual std::string getSetting(void);
			
			virtual EMenuElementType getType(void);
		private:
			//tolua_end
			std::string action;
			std::string setting;
			
			bool checked;
			std::string label;
			//tolua_begin
	};
}
//tolua_end

#endif
 
