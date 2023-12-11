#include <irrlicht.h>

using namespace irr;

class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
			KeyState[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}

		//If the event is a mouse input then change the Keystate
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			switch(event.MouseInput.Event)
			{
			case EMIE_LMOUSE_PRESSED_DOWN: 
				KeyIsDown[KEY_LBUTTON]=true; 
				KeyState[KEY_LBUTTON]=true;
				break;
			case EMIE_LMOUSE_LEFT_UP:      
				LastKeyState[KEY_LBUTTON]=false;
				KeyState[KEY_LBUTTON]=false;
				break;
			}
		}

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode)
	{
		return KeyIsDown[keyCode];
	}

	//If the keystate is true and the last key state is not true,
	//then the key must have been pressed
	virtual bool HasKeyBeenPressed(EKEY_CODE keyCode)
	{
		if (KeyState[keyCode] == true && LastKeyState[keyCode] == false)
		{
			LastKeyState[keyCode] = true;

			return true;
		}

		if (KeyState[keyCode] != LastKeyState[keyCode])
		{
			LastKeyState[keyCode] = KeyState[keyCode];
		}

		return false;
	}



MyEventReceiver()
{
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
	{
		KeyIsDown[i] = false;
		LastKeyState[i] = false;
		KeyState[i] = false;
	}
}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];

	//This is used to check for button presses
	bool LastKeyState[KEY_KEY_CODES_COUNT];
	bool KeyState[KEY_KEY_CODES_COUNT];
};