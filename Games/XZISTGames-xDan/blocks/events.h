
#ifndef __EVENTS_H
#define __EVENTS_H

#include <string.h>

class PlayerEventReceiver : public IEventReceiver
{
public:

    PlayerEventReceiver()
    {
        Reset();
    }

    void Reset()
    {
        magnetMode = false;
        mouseLeft = false;
        mouseRight = false;
        mouseWheel = 0;
        jump = false;
        memset(keyCode, 0, sizeof(c8)*256);
    }

    Info *info;
    void setInfo(Info *info)
    {
        this->info = info;
    }

    bool magnetMode;
    bool mouseLeft;
    bool mouseRight;
    bool jump;
    s32 mouseDX,mouseDY;
    f32 mouseWheel;

    f32 getMouseWheel()
    {
        f32 ret = mouseWheel;
        mouseWheel = 0;
        return ret;
    }

    // prob. replace all above bools with a reference to this array
    c8 keyCode[256];

    virtual bool OnEvent(const SEvent& e)
    {
        if (e.EventType == EET_KEY_INPUT_EVENT)
        {
            keyCode[e.KeyInput.Key] = e.KeyInput.PressedDown;

            // maybe remove switch
            switch (e.KeyInput.Key)
            {
            case KEY_SPACE:
                jump = e.KeyInput.PressedDown;
                return true;
            case KEY_KEY_E:
                if (e.KeyInput.PressedDown)
                {
                    magnetMode ^= 1;
                }
                return true;
            case KEY_ESCAPE:
                if (e.KeyInput.PressedDown)
                {
                    info->device->closeDevice();
                }
                return true;
            }
        }

        if (e.EventType == EET_MOUSE_INPUT_EVENT)
        {
            mouseDX = e.MouseInput.X-400;
            mouseDY = e.MouseInput.Y-400;
            if (e.MouseInput.Event == EMIE_MOUSE_WHEEL)
            {
                mouseWheel = e.MouseInput.Wheel;
            }
            else if (e.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
            {
                mouseLeft = true;
            }
            else if (e.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
            {
                mouseLeft = false;
            }
            else if (e.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
            {
                mouseRight = true;
            }
            else if (e.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
            {
                mouseRight = false;
            }
        }
        return false;
    }
};

#endif
