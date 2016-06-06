#ifndef EVENT_RECEIVER_H
#define EVENT_RECEIVER_H

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
//using namespace irrklang;

// Event receiver
class cEventReceiver : public IEventReceiver
{
	public:
		// Keyboard
		bool keys[KEY_KEY_CODES_COUNT];

 		// Mouse
		bool mouseLeft, mouseRight, mouseMiddle;
		s32 mouseX, mouseY, mouseDeltaX, mouseDeltaY;
		f32 mouseWheel;

		// Constructors
		cEventReceiver()
		{
			// Zero out the keyboard key array
			for(int x = 0; x < KEY_KEY_CODES_COUNT; x++)
			keys[x] = false;
		}

		// Event handler
		bool OnEvent(const SEvent& event)
		{
			// GUI events
			if(event.EventType == EET_GUI_EVENT)
			{
				// Get the caller id
				s32 callerID = event.GUIEvent.Caller->getID();

				switch(event.GUIEvent.EventType)
				{
					case EGET_SCROLL_BAR_CHANGED:
						break;
					case EGET_CHECKBOX_CHANGED:
						break;
					case EGET_ELEMENT_HOVERED:
      					break;
					case EGET_EDITBOX_ENTER:
						break;
					case EGET_BUTTON_CLICKED:
						break;
				}
			}

			// Mouse events
			if(event.EventType == EET_MOUSE_INPUT_EVENT)
			{
				if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
					 mouseLeft = true;
				if(event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
					 mouseRight = true;
				if(event.MouseInput.Event == EMIE_MMOUSE_PRESSED_DOWN)
					 mouseMiddle = true;

				if(event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
					 mouseLeft = false;
				if(event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
					 mouseRight = false;
				if(event.MouseInput.Event == EMIE_MMOUSE_LEFT_UP)
					 mouseMiddle = false;

				if(event.MouseInput.Event == EMIE_MOUSE_WHEEL)
					 mouseWheel += event.MouseInput.Wheel;

				// Get the mouse pos and change in pos and 3d pos
				if(event.MouseInput.Event == EMIE_MOUSE_MOVED)
				{
					mouseDeltaX = event.MouseInput.X - mouseX;
					mouseDeltaY = event.MouseInput.Y - mouseY;
					mouseX = event.MouseInput.X;
					mouseY = event.MouseInput.Y;
				}
			}

			if(event.EventType == EET_KEY_INPUT_EVENT)
				keys[event.KeyInput.Key] = event.KeyInput.PressedDown;

			return false;
		}
};

#endif
