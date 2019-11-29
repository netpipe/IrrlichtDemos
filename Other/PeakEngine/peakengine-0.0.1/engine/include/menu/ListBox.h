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

#ifndef _LISTBOX_H_
#define _LISTBOX_H_

#include "menu/MenuElement.h"

#include <string>
#include <vector>

//tolua_begin
namespace peak
{
	class ListBox: public MenuElement
	{
		public:
			ListBox();
			virtual ~ListBox();
			
			virtual bool init(Menu *menu, MenuElement *parent = 0);
			
			virtual bool show(void);
			virtual bool hide(void);
			
			virtual void choose(int index);
			virtual int getSelected(void);
			
			virtual void addItem(std::string label, int index = -1);
			virtual void deleteItem(int index);
			virtual int getItemCount(void);
			virtual std::string getItem(int index);
			virtual void clearItems(void);
			
			virtual void updateItems(void);
			
			/**
			 * \brief Connect a script action to the list box.
			 *
			 * When the user selects an item, menu_action(action) in the menu
			 * script is called.
			 */
			virtual void setAction(std::string action);
			virtual std::string getAction(void);
			
			virtual EMenuElementType getType(void);
		protected:
			//tolua_end
			std::string action;
			std::vector<std::string> items;
			int chosen;
			//tolua_begin
	};
}
//tolua_end

#endif

