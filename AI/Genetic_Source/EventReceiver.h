#include <irrlicht.h>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EventReceiver : public IEventReceiver
{
private:

	bool keys[KEY_KEY_CODES_COUNT];
	bool buttonState[5];//total buttons

	virtual bool OnEvent(const SEvent & event)
	{
		if (event.EventType == EET_KEY_INPUT_EVENT)
		{
				if (event.KeyInput.PressedDown == true)
				{
					keys[event.KeyInput.Key]=true;
				}
				else
				{
                    keys[event.KeyInput.Key]=false;
				}
		}

		if (event.EventType == EET_GUI_EVENT)
        {
            s32 id = event.GUIEvent.Caller->getID();
            switch(event.GUIEvent.EventType)
            {
                case EGET_BUTTON_CLICKED:
                    buttonState[id]=true;
                    break;
            }
        }
		return false;
	}

public:

	bool keyDown(char key)
	{
		return keys[key];
	}

	bool buttonPressed(int id)
	{
        if (buttonState[id]==true)
        {
            buttonState[id]=false;
            return true;
        }
        else
        {
            return false;
        }
	}
};
