/*-----------------------------------------------------------------------------*
| headerfile MInput.h                                                          |
|                                                                              |
| version 1.10                                                                 |
| date: (29.04.2007)                                                           |
|                                                                              |
| author:  Michal Švantner                                                     |
|                                                                              |
| for Irrlicht engine                                                          |
| store keyboard and mouse input in structures "key" and "mouse"               |
*-----------------------------------------------------------------------------*/

#ifndef MINPUT_H
#define MINPUT_H

#include <irrlicht.h>
using namespace irr;

struct
{
    bool pressed(EKEY_CODE keycode)
    {
        return code[keycode];
    }

    bool pressed_once(EKEY_CODE keycode)
    {
        if(code[keycode])
        {
            code[keycode] = false;
            return true;
        }
        return false;
    }

    void reset()
    {
         for(s32 i=0; i<KEY_KEY_CODES_COUNT; i++) code[i] = false;
    }

    bool code[KEY_KEY_CODES_COUNT];

}key;

struct
{
    bool left_pressed()
    {
        return left;
    }

    bool middle_pressed()
    {
        return middle;
    }

    bool right_pressed()
    {
        return right;
    }

    bool left_pressed_once()
    {
        if(left)
        {
            left = false;
            return true;
        }
        return false;
    }

    bool middle_pressed_once()
    {
        if(middle)
        {
            middle = false;
            return true;
        }
        return false;
    }

    bool right_pressed_once()
    {
        if(right)
        {
            right = false;
            return true;
        }
        return false;
    }

    void reset()
    {
        X = 0;
        Y = 0;
        wheel = 0.0f;
        left = false;
        middle = false;
        right = false;
    }

    s32 X;
    s32 Y;
    f32 wheel;
    bool left;
    bool middle;
    bool right;

}mouse;

class MyEventReceiver : public IEventReceiver
{
public:
    MyEventReceiver()
    {
        for(s32 i=0; i<KEY_KEY_CODES_COUNT; i++)
            key.code[i] = false;

        mouse.X = 0;
        mouse.Y = 0;
        mouse.wheel = 0.0f;
        mouse.left = false;
        mouse.middle = false;
        mouse.right = false;
    }

	virtual bool OnEvent(const SEvent& event)
	{
        bool value = false;

		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			key.code[event.KeyInput.Key] = event.KeyInput.PressedDown;
			value = true;
		}

		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
            switch(event.MouseInput.Event)
            {
                case EMIE_LMOUSE_PRESSED_DOWN: mouse.left = true; break;
                case EMIE_RMOUSE_PRESSED_DOWN: mouse.right = true; break;
                case EMIE_MMOUSE_PRESSED_DOWN: mouse.middle = true; break;
                case EMIE_LMOUSE_LEFT_UP: mouse.left = false; break;
                case EMIE_RMOUSE_LEFT_UP: mouse.right = false; break;
                case EMIE_MMOUSE_LEFT_UP: mouse.middle = false; break;
                case EMIE_MOUSE_MOVED: mouse.X = event.MouseInput.X; mouse.Y = event.MouseInput.Y; break;
                case EMIE_MOUSE_WHEEL: mouse.wheel = mouse.wheel + event.MouseInput.Wheel; break;
            }
			value = true;
		}

		return value;
	}
};

#endif
