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
#include "CGUIEventDispatcher.h"

CGUIEventDispatcher::CGUIEventDispatcher()
{
    //ctor
}

CGUIEventDispatcher::~CGUIEventDispatcher()
{
    //dtor
}

bool CGUIEventDispatcher::OnEvent(const irr::SEvent& event)
{
    if (event.EventType == irr::EET_GUI_EVENT)
    {
        irr::gui::IGUIElement* caller = event.GUIEvent.Caller;
        Storage::Iterator it = Bindings.begin();
        while(it != Bindings.end())
        {
            if ((*it) == caller)
                return (*it).call(event.GUIEvent);
            ++it;
        }
    }
    return false;
}

bool CGUIEventDispatcher::bind(irr::gui::IGUIElement* element, bool (*func)(const irr::SEvent::SGUIEvent&), irr::gui::EGUI_EVENT_TYPE type)
{
    Storage::Iterator it = Bindings.begin();
    while(it != Bindings.end())
    {
        if ((*it) == element && (*it).checkFunc(func))
            return false;
        else if ((*it) == element)
        {
            (*it).Data.push_back(new CFunctionHandle(func, type));
            return true;
        }
        ++it;
    }
    Pair p;
    p.Key = element;
    p.Data.push_back(new CFunctionHandle(func, type));
    Bindings.push_back(p);
    return true;
}

void CGUIEventDispatcher::unbind(irr::gui::IGUIElement* element)
{
    Storage::Iterator it = Bindings.begin();
    while(it != Bindings.end())
    {
        if ((*it) == element)
        {
            (*it).releaseHandle();
            Bindings.erase(it);
            return;
        }
        ++it;
    }
}

void CGUIEventDispatcher::unbind(irr::gui::IGUIElement* element, bool (*func)(const irr::SEvent::SGUIEvent&))
{
    Storage::Iterator it = Bindings.begin();
    while(it != Bindings.end())
    {
        if ((*it) == element)
        {
            bool r = (*it).releaseFunc(func);
            return;
        }
        ++it;
    }
}
