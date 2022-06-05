#include "CInput_catcher.h"

CInput_catcher::CInput_catcher()
{
    //ctor
}

CInput_catcher::~CInput_catcher()
{
    //dtor
}

void CInput_catcher::registerInput(IMouseReceiver* input)
{
    irr::core::list<IMouseReceiver*>::Iterator it = Mouse.begin();
    while (it != Mouse.end())
    {
        if ((*it) == input)
            return;
        ++it;
    }
    Mouse.push_back(input);
}
void CInput_catcher::registerInput(IKeyboardReceiver* input)
{
    irr::core::list<IKeyboardReceiver*>::Iterator it = Keyboard.begin();
    while (it != Keyboard.end())
    {
        if ((*it) == input)
            return;
        ++it;
    }
    Keyboard.push_back(input);
}

void CInput_catcher::unregisterInput(IMouseReceiver* input)
{
    irr::core::list<IMouseReceiver*>::Iterator it = Mouse.begin();
    while (it != Mouse.end())
    {
        if ((*it) == input)
        {
            Mouse.erase(it);
            return;
        }
        ++it;
    }
}
void CInput_catcher::unregisterInput(IKeyboardReceiver* input)
{
    irr::core::list<IKeyboardReceiver*>::Iterator it = Keyboard.begin();
    while (it != Keyboard.end())
    {
        if ((*it) == input)
        {
            Keyboard.erase(it);
            return;
        }
        ++it;
    }
}

bool CInput_catcher::OnEvent(const irr::SEvent& event)
{
    switch (event.EventType)
    {
    case irr::EET_KEY_INPUT_EVENT:
    {
        irr::core::list<IKeyboardReceiver*>::Iterator it = Keyboard.begin();
        while (it != Keyboard.end())
        {
            (*it)->key_event(event.KeyInput);
            ++it;
        }
    }
    break;
    case irr::EET_MOUSE_INPUT_EVENT:
    {
        irr::core::list<IMouseReceiver*>::Iterator it = Mouse.begin();
        while (it != Mouse.end())
        {
            (*it)->mouse_event(event.MouseInput);
            ++it;
        }
    }
    break;
    }
    return false;
}
