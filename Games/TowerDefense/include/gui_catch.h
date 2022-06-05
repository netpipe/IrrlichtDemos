#ifndef GUI_CATCH_H
#define GUI_CATCH_H

#include <Functor.h>

class gui_catch : public irr::gui::IGUIElement
{
public:
    gui_catch(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent)
        : irr::gui::IGUIElement(irr::gui::EGUIET_ELEMENT, environment, parent, -1, irr::core::rect<irr::s32>(0, 0, environment->getVideoDriver()->getScreenSize().Width, environment->getVideoDriver()->getScreenSize().Height))
    {

    }
    bool OnEvent(const irr::SEvent& event)
    {
        if (event.EventType == irr::EET_GUI_EVENT)
        {
            irr::core::list<gcatch>::Iterator it = Catches.begin();
            while(it != Catches.end())
            {
                if ((*it).Element == event.GUIEvent.Caller)
                {
                    (*it).send(event.GUIEvent.EventType);
                }
                ++it;
            }
        }
        return irr::gui::IGUIElement::OnEvent(event);
    }
    template<class T>
    void registerElement(irr::gui::IGUIElement* element, T* c, void (T::*f)(irr::gui::EGUI_EVENT_TYPE eventType, irr::gui::IGUIElement* sender))
    {
        irr::core::list<gcatch>::Iterator it = Catches.begin();
        while(it != Catches.end())
        {
            if ((*it).Element == element)
            {
                (*it).Slots.push_back(new SFunctorReciever2<T, irr::gui::EGUI_EVENT_TYPE,irr::gui::IGUIElement*>(c, f));
            }
            ++it;
        }
        Catches.push_back(gcatch(element));
        (*Catches.getLast()).Slots.push_back(new SFunctorReciever2<T, irr::gui::EGUI_EVENT_TYPE,irr::gui::IGUIElement*>(c, f));
    }
protected:
    struct gcatch
    {
        gcatch(irr::gui::IGUIElement* element)
        {
            Element = element;
        }
        ~gcatch(void)
        {
            irr::core::list<SFunctor2<irr::gui::EGUI_EVENT_TYPE, irr::gui::IGUIElement*>* >::Iterator it = Slots.begin();
            while(it != Slots.end())
            {
                (*it)->drop();
                ++it;
            }
            Slots.clear();
        }
        void send(irr::gui::EGUI_EVENT_TYPE type)
        {
            irr::core::list<SFunctor2<irr::gui::EGUI_EVENT_TYPE, irr::gui::IGUIElement*>* >::Iterator it = Slots.begin();
            while(it != Slots.end())
            {
                (*it)->call(type, Element);
                ++it;
            }
        }
        irr::gui::IGUIElement* Element;
        irr::core::list<SFunctor2<irr::gui::EGUI_EVENT_TYPE, irr::gui::IGUIElement*>* > Slots;
    };
    irr::core::list<gcatch> Catches;
};

#endif // GUI_CATCH_H
