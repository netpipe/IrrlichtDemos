// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#include "CEventReceiver.h"

CEventReceiver::CEventReceiver()
{
    FarValue = 1800;
}

bool CEventReceiver::OnEvent(const SEvent& event)
{
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        if(event.KeyInput.PressedDown)
        {
            if(event.KeyInput.Key == KEY_KEY_A)
            {
                FarValue += 50;
                return true;
            }
            else if(event.KeyInput.Key == KEY_KEY_Y)
            {
                FarValue -= 50;
                return true;
            }
        }
    }

    return false;
}

int CEventReceiver::getFarValue()
{
    return FarValue;
}
