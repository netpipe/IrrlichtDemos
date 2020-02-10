
#ifndef __EVENT_H
#define __EVENT_H

#include <irrlicht.h>
#include <irrInfo.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// should probably have actual game state modification in here. but who cares.
// will need to create new one at each level start so values are reset.
// will have separate event class for menu system, between levels etc.
class PlayerEventReceiver : public IEventReceiver
{
public:

    PlayerEventReceiver()
    {
        Reset();
    }

    void Reset()
    {
        goForwards = false;
        goLeft = false;
        goRight = false;
        mouseWheel = 0;
    }

    irrInfo *info;
    void setIrrInfo(irrInfo *info)
    {
        this->info = info;
    }

    bool goForwards;
    bool goLeft;
    bool goRight;
    s32 mouseDX,mouseDY;
    f32 mouseWheel;

    f32 getMouseWheel()
    {
        f32 ret = mouseWheel;
        mouseWheel = 0;
        return ret;
    }

    virtual bool OnEvent(const SEvent& e)
    {
        if (e.EventType == EET_KEY_INPUT_EVENT)
        {
            switch (e.KeyInput.Key)
            {
            case KEY_KEY_W:
                goForwards = e.KeyInput.PressedDown;
                return true;
            case KEY_KEY_A:
                goLeft = e.KeyInput.PressedDown;
                return true;
            case KEY_KEY_D:
                goRight = e.KeyInput.PressedDown;
                return true;
            case KEY_ESCAPE:
                if (e.KeyInput.PressedDown)
                {
                    info->timer->stop();
                    info->gamePaused = true;
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
        }
        return false;
    }
};

#endif
