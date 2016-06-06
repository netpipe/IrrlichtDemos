/*
Copyright (c) 2008 Nils Daumann

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include "IrrEventsClass.h"

IrrEvents::IrrEvents()
{
	on_mouse_left = NULL;
	mouse_left = false;

	on_key_esc = NULL;
	key_esc = false;

	on_key_space = NULL;
	key_space = false;

	on_key_f6 = NULL;
	key_f6 = false;
}

bool IrrEvents::OnEvent(const SEvent& event)
{
	switch(event.EventType)
	{
		//if it is a mouse event
		case EET_MOUSE_INPUT_EVENT:
		{
			switch(event.MouseInput.Event)
			{
				case EMIE_LMOUSE_PRESSED_DOWN:
				{
					if(on_mouse_left != NULL && mouse_left == false)
						on_mouse_left();
					mouse_left = true;
					break;
				}

				case EMIE_LMOUSE_LEFT_UP:
				{
					mouse_left = false;
					break;
				}

				default:
					break;
			}
		}

		//if it is a keyboard event
		case EET_KEY_INPUT_EVENT:
		{
			switch(event.KeyInput.PressedDown)
			{
				//if a key was pressed
				case true:
				{
					switch(event.KeyInput.Key)
					{
						case KEY_ESCAPE:
						{
							if(on_key_esc != NULL && key_esc == false)
								on_key_esc();
							key_esc = true;
							break;
						}

						case KEY_SPACE:
						{
							if(on_key_space != NULL && key_space == false)
								on_key_space();
							key_space = true;
							break;
						}

						case KEY_KEY_W:
						{
							break;
						}

						case KEY_F6:
						{
							if(on_key_f6 != NULL && key_f6 == false)
								on_key_f6();
							key_f6 = true;
							break;
						}

						default:
							break;
					}
				}

				//if a key was released
				case false:
				{
					switch(event.KeyInput.Key)
					{
						case KEY_KEY_W:
						{
							break;
						}

						case KEY_F6:
						{
							key_f6 = false;
							break;
						}

						default:
							break;
					}
				}
			}

		}

		default:
			return false;
	}

	return true;
}
