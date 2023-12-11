#include "TKEventReceiver.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

TKEventReceiver::TKEventReceiver(TKCore* ntkCore)
{
    tkCore=ntkCore;
    for(int i=0;i<KEY_KEY_CODES_COUNT;i++) keys[i]=false;
}

bool TKEventReceiver::OnEvent(const SEvent& event)
{
    if(event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        if(!event.KeyInput.PressedDown)
            tkCore->eventKeyReleased(event.KeyInput.Key);

        if(event.KeyInput.PressedDown)
            tkCore->eventKeyPressed(event.KeyInput.Key);
    }
    if(event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        tkCore->eventMouseClick(event.MouseInput.Event);
    }
    if(event.EventType == irr::EET_GUI_EVENT)
    {
        if(event.GUIEvent.EventType==EGET_BUTTON_CLICKED)
            tkCore->eventGui(event.GUIEvent.Caller->getID());
    }


    return false;
}
