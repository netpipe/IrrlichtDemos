#ifndef KEYLISTENER_H
#define KEYLISTENER_H
#include "irrlicht.h"

using namespace irr;
using namespace scene;
using namespace core;



class KeyListener : public IEventReceiver
{
public:
	struct mouseCoords
	{
		int X;
		int Y;
		float wheel;
	};
	struct mouseCoords mouse;
	bool mouseLButton;
	bool mouseRButton;
	
        // This is the one method that we have to implement
        virtual bool OnEvent(const SEvent& event)
        {
                // Remember whether each key is down or up
                if (event.EventType == irr::EET_KEY_INPUT_EVENT)
                        KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

				if(event.EventType==irr::EET_MOUSE_INPUT_EVENT)
				{
					if(event.MouseInput.Event==irr::EMIE_MOUSE_MOVED)
					{
						// check the mouse coords
						mouse.X = event.MouseInput.X;
						mouse.Y = event.MouseInput.Y;
					}
					if(event.MouseInput.Event==irr::EMIE_MOUSE_WHEEL)
					{
						// how do i make it work?
						mouse.wheel += event.MouseInput.Wheel;
					}

					if(event.MouseInput.Event==irr::EMIE_LMOUSE_PRESSED_DOWN)
					{
						if(mouseLButton!=true)
						{
							mouseLButton=true;
						}
					}
					if(event.MouseInput.Event==irr::EMIE_LMOUSE_LEFT_UP)
					{
						if(mouseLButton!=false)
						{
							mouseLButton=false;
						}
					}
					if(event.MouseInput.Event==irr::EMIE_RMOUSE_PRESSED_DOWN)
					{
						if(mouseRButton!=true)
						{
							mouseRButton=true;
						}
					}
					if(event.MouseInput.Event==irr::EMIE_RMOUSE_LEFT_UP)
					{
						if(mouseRButton!=false)
						{
							mouseRButton=false;
						}
					}

				}

                return false;
        }

        // This is used to check whether a key is being held down
        virtual bool IsKeyDown(EKEY_CODE keyCode) const
        {
                return KeyIsDown[keyCode];
        }
		float mouseWheel()
		{
			return mouse.wheel;
		}
        int mouseX()
		{
			return mouse.X;
		}
		int mouseY()
		{
			return mouse.Y;
		}
		bool mouseLButtonDown()
		{
			if(mouseLButton==true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool mouseRButtonDown()
		{
			if(mouseRButton)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
        KeyListener()
        {
                for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
                        KeyIsDown[i] = false;
				mouse.X=0;
				mouse.Y=0;
				mouse.wheel = 0;
        }

private:
        // We use this array to store the current state of each key
        bool KeyIsDown[KEY_KEY_CODES_COUNT];

};


#endif
