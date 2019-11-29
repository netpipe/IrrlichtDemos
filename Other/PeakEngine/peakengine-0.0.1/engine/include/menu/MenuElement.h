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

#ifndef _MENUELEMENT_H_
#define _MENUELEMENT_H_

#include "core/Vector2D.h"

#include <string>

#ifdef _USE_IRRLICHT_
namespace irr
{
	namespace gui
	{
		class IGUIElement;
	}
}
#endif

//tolua_begin
namespace peak
{
	class Menu;
	
	enum EMenuElementType
	{
		EMET_Frame,
		EMET_Button,
		EMET_Label,
		EMET_ListBox,
		EMET_CheckBox,
		EMET_ComboBox,
		EMET_EditBox,
		EMET_RadioBox
	};
	
	/**
	 * \brief Class containing a menu element like a button, checkbox etc.
	 */
	class MenuElement
	{
		public:
			MenuElement()
			{
				id = 0;
				element = 0;
				menu = 0;
				parent = 0;
			}
			virtual ~MenuElement() {};
			
			/**
			 * \brief Initializes the menu element.
			 * \param menu Menu to place the element into
			 * \param parent Parent element
			 */
			virtual bool init(Menu *menu, MenuElement *parent = 0) = 0;
			virtual bool destroy(void);
			
			virtual void setPosition(Vector2D position);
			virtual Vector2D getPosition(void);
			
			virtual void setSize(Vector2D size);
			virtual Vector2D getSize(void);
			
			virtual void setID(int id);
			virtual int getID(void);
			
			/**
			 * \brief Shows the element.
			 *
			 * Is called by Menu::show().
			 *
			 * For internal use only.
			 */
			virtual bool show(void) = 0;
			/**
			 * \brief Hides the element.
			 *
			 * Is called by Menu::hide().
			 *
			 * For internal use only.
			 */
			virtual bool hide(void) = 0;
			
			virtual void setText(std::string text);
			virtual std::string getText(void);
			
			/**
			 * \brief Returns the type of the element.
			 */
			virtual EMenuElementType getType(void) = 0;
			
			//tolua_end
			#ifdef _USE_IRRLICHT_
			irr::gui::IGUIElement *getElement(void)
			{
				return element;
			}
			#endif
		protected:
			#ifdef _USE_IRRLICHT_
			irr::gui::IGUIElement *element;
			#endif
			std::string text;
			Menu *menu;
			MenuElement *parent;
			Vector2D position;
			Vector2D size;
			int id;
			//tolua_begin
	};
}
//tolua_end

#endif

