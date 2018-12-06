/*
  Copyright (C) 2011 Daniel Sudmann

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Daniel Sudmann suddani@googlemail.com
*/
#include "CIrrRocketEventReceiver.h"
#include <Rocket/Core.h>

CIrrRocketEventReceiver::CIrrRocketEventReceiver(Rocket::Core::Context* context) : Context(context)
{
    //ctor
}

CIrrRocketEventReceiver::~CIrrRocketEventReceiver()
{
    //dtor
}

bool CIrrRocketEventReceiver::OnEvent(const irr::SEvent& event)
{
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        //Rocket::Core::Input::KeyModifier mod;
        int mod = 0;
        if (event.MouseInput.Shift)
            mod &= Rocket::Core::Input::KM_SHIFT;
        if (event.MouseInput.Control)
            mod &= Rocket::Core::Input::KM_CTRL;

        if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED)
        {
            Context->ProcessMouseMove(event.MouseInput.X, event.MouseInput.Y, mod);
        }
        else if (event.MouseInput.Event == irr::EMIE_MOUSE_WHEEL)
        {
            Context->ProcessMouseWheel(-event.MouseInput.Wheel, mod);
        }
        else if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
        {
            Context->ProcessMouseButtonDown(0, mod);
        }
        else if (event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
        {
            Context->ProcessMouseButtonDown(1, mod);
        }
        else if (event.MouseInput.Event == irr::EMIE_MMOUSE_PRESSED_DOWN)
        {
            Context->ProcessMouseButtonDown(2, mod);
        }
        else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP)
        {
            Context->ProcessMouseButtonUp(0, mod);
        }
        else if (event.MouseInput.Event == irr::EMIE_RMOUSE_LEFT_UP)
        {
            Context->ProcessMouseButtonUp(1, mod);
        }
        else if (event.MouseInput.Event == irr::EMIE_MMOUSE_LEFT_UP)
        {
            Context->ProcessMouseButtonUp(2, mod);
        }
    }
    else if (event.EventType  == irr::EET_KEY_INPUT_EVENT)
    {
        int mod = 0;
        if (event.KeyInput.Shift)
            mod &= Rocket::Core::Input::KM_SHIFT;
        if (event.KeyInput.Control)
            mod &= Rocket::Core::Input::KM_CTRL;

        if (event.KeyInput.Char)
            Context->ProcessTextInput(event.KeyInput.Char);
        /*
        // Sends a key down event into this context.
        void ProcessKeyDown(Rocket::Core::Input::KeyIdentifier key_identifier, int key_modifier_state);
        // Sends a key up event into this context.
        void ProcessKeyUp(Rocket::Core::Input::KeyIdentifier key_identifier, int key_modifier_state);
        */
    }
    return false;
}
