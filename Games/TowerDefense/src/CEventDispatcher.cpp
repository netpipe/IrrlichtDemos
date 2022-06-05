#include "CEventDispatcher.h"

CEventDispatcher::CEventDispatcher()
{
    //ctor
}

CEventDispatcher::~CEventDispatcher()
{
    irr::core::map<irr::core::stringc, SFunctorContainer2<Base_Property*, CEntity*>* >::Iterator it = Callbacks.getIterator();
    while (!it.atEnd())
    {
        SFunctorContainer2<Base_Property*, CEntity*>* value = it.getNode()->getValue();
        delete value;
        it++;
    }
    Callbacks.clear();
}
