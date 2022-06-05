#ifndef CEVENTDISPATCHER_H
#define CEVENTDISPATCHER_H

#include <irrlicht.h>
#include <Functor.h>
#include <Property.h>

class CEntity;
class CEventDispatcher
{
public:
    CEventDispatcher();
    virtual ~CEventDispatcher();
    template<typename T>
    void FireEvent(const irr::core::stringc& name, const T& data, CEntity* sender)
    {
        if (Callbacks.find(name) != NULL)
        {
            Property<T>* p = new Property<T>(name);
            p->getValue() = data;
            Callbacks.find(name)->getValue()->Call(p, sender);
            delete p;
        }
    }
    template<typename T>
    void registerCallback(const irr::core::stringc& name, T* Class, void (T::*func)(Base_Property* data, CEntity* sender))
    {
        if (Callbacks.find(name) != NULL)
        {
            Callbacks.find(name)->getValue()->addCallback(Class, func);
            return;
        }

        Callbacks[name] = new SFunctorContainer2<Base_Property*, CEntity*>();

        Callbacks.find(name)->getValue()->addCallback(Class, func);
    }
    template<typename T>
    void unregisterCallback(const irr::core::stringc& name, T* Class)
    {
        if (Callbacks.find(name) != NULL)
            Callbacks.find(name)->getValue()->removeCallback(Class);
    }
protected:
    irr::core::map<irr::core::stringc, SFunctorContainer2<Base_Property*, CEntity*>* > Callbacks;
private:
};

#endif // CEVENTDISPATCHER_H
