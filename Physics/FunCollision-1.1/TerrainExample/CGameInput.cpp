#include <irrlicht.h>
using namespace irr;

#include "CGameInput.h"


//! constructor
CGameInput::CGameInput(IrrlichtDevice* Device)
{
    // instance
    device = Device;

    // by the start, none of the keys are pressed
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
    {
        keys[i] = false;
        keysPrev[i] = false;
    }
    shift = control = shiftPrev = controlPrev = false;

    // nor the mouse
    LMB = MMB = RMB = LMBPrev = MMBPrev = RMBPrev = false;
    wheel = 0.0f;
    mouseX = mouseY = 0;
    if (device)
    {
        core::dimension2d<u32> dc = device->getVideoDriver()->getScreenSize() / 2;
        center.X = dc.Width;
        center.Y = dc.Height;
        mouseX = center.X;
        mouseY = center.Y;
    }
    relMouseX = relMouseY = 0;
    lockMouse = locked = false;
}

//! OnEvent
bool CGameInput::OnEvent(const SEvent& event)
{
    // which key is pressed
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        // is it pressed or released?
        keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
        shift = event.KeyInput.Shift;
        control = event.KeyInput.Control;
    }

    // mouse events
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        switch (event.MouseInput.Event)
        {
            case irr::EMIE_LMOUSE_PRESSED_DOWN  : LMB = true; break;
            case irr::EMIE_LMOUSE_LEFT_UP       : LMB = false; break;
            case irr::EMIE_MMOUSE_PRESSED_DOWN  : MMB = true; break;
            case irr::EMIE_MMOUSE_LEFT_UP       : MMB = false; break;
            case irr::EMIE_RMOUSE_PRESSED_DOWN  : RMB = true; break;
            case irr::EMIE_RMOUSE_LEFT_UP       : RMB = false; break;

            case irr::EMIE_MOUSE_WHEEL : wheel = event.MouseInput.Wheel; break;
            case irr::EMIE_MOUSE_MOVED:
                if (locked)
                {
                    locked = false;
                    mouseX = center.X;
                    mouseY = center.Y;
                }
                else
                {
                    relMouseX = event.MouseInput.X - mouseX;
                    relMouseY = event.MouseInput.Y - mouseY;
                    mouseX = event.MouseInput.X;
                    mouseY = event.MouseInput.Y;

                    if (device && lockMouse)
                    {
                        locked = true;
                        device->getCursorControl()->setPosition(center);
                    }
                }
                break;
            default : break;
        }
    }

    return false;
}

//! update
void CGameInput::update()
{
    // keys
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i) keysPrev[i] = keys[i];

    shiftPrev = shift;
    controlPrev = control;

    // mouse state
    LMBPrev = LMB;
    MMBPrev = MMB;
    RMBPrev = RMB;

    wheel = 0.0f;

    relMouseX = relMouseY = 0;
}

