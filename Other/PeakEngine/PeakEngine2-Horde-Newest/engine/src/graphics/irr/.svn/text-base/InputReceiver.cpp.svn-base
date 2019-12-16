/*
Copyright (C) 2008  Lukas Kropatschek, Mathias Gottschlag

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

#include "graphic/irr/InputReceiver.h"
#include "core/InputManager.h"
#include "menu/MenuManager.h"
#include "menu/Menu.h"

namespace peak
{
	InputReceiver *InputReceiver::get(void)
	{
		static InputReceiver receiver;
		return &receiver;
	}

	InputReceiver::InputReceiver()
	{
	}

	bool InputReceiver::OnEvent(const irr::SEvent &event)
	{
		InputManager * inputmgr = InputManager::get();
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			if (event.KeyInput.PressedDown)
			{	
				if(!pressed[event.KeyInput.Key])
				{
					inputmgr->keyboardEvent((Keycode) event.KeyInput.Key, true);
					pressed[event.KeyInput.Key] = true;
				}
			}

			else
			{
				if(pressed[event.KeyInput.Key])
				{
					inputmgr->keyboardEvent((Keycode) event.KeyInput.Key, false);
					pressed[event.KeyInput.Key] = false;
				}
			}
		}
		else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			if(event.MouseInput.Event == irr::EMIE_MOUSE_MOVED)
			{
				float y = event.MouseInput.Y;
				float x = event.MouseInput.X;
				inputmgr->mouseMoved(x, y);
			}

			if(event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
			{
				if(!leftmousepressed)
				{
					inputmgr->keyboardEvent(EK_LBUTTON, true);
					leftmousepressed = true;
				}
			}

			if(event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP)
			{
				if(leftmousepressed)
				{
					inputmgr->keyboardEvent(EK_LBUTTON, false);
					leftmousepressed = false;
				}
			}

			if(event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
			{
				if(!rightmousepressed)
				{
					inputmgr->keyboardEvent(EK_RBUTTON, true);
					rightmousepressed = true;
				}
			}

			if(event.MouseInput.Event == irr::EMIE_RMOUSE_LEFT_UP)
			{
				if(rightmousepressed)
				{
					inputmgr->keyboardEvent(EK_RBUTTON, false);
					rightmousepressed = false;
				}
			}

			if(event.MouseInput.Event == irr::EMIE_MMOUSE_PRESSED_DOWN)
			{
				if(!middlemousepressed)
				{
					inputmgr->keyboardEvent(EK_MBUTTON, true);
					middlemousepressed = true;
				}
			}

			if(event.MouseInput.Event == irr::EMIE_MMOUSE_LEFT_UP)
			{
				if(middlemousepressed)
				{
					inputmgr->keyboardEvent(EK_MBUTTON, true);
					middlemousepressed = false;
				}
			}
		}
		else if (event.EventType == irr::EET_GUI_EVENT)
		{
			Menu *menu = MenuManager::get()->getCurrentMenu();
			if (menu)
			{
				int id = event.GUIEvent.Caller->getID();
				switch(event.GUIEvent.EventType)
				{
					case irr::gui::EGET_BUTTON_CLICKED:
						menu->pressButton(id);
						break;
					case irr::gui::EGET_CHECKBOX_CHANGED:
						menu->setCheckBox(id,
							((irr::gui::IGUICheckBox*)event.GUIEvent.Caller)->isChecked());
						break;
					case irr::gui::EGET_LISTBOX_CHANGED:
						menu->setListBox(id, ((irr::gui::IGUIListBox*)event.GUIEvent.Caller)->getSelected());
						break;
					case irr::gui::EGET_COMBO_BOX_CHANGED:
						menu->setListBox(id, ((irr::gui::IGUIComboBox*)event.GUIEvent.Caller)->getSelected());
						break;
					default:
						break;
				}
			}
		}
		return false;
	}

}
