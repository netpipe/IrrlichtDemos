#ifndef CGUIEVENTDISPATCHER_H
#define CGUIEVENTDISPATCHER_H

#include <IEventReceiver.h>
#include <irrArray.h>
#include <irrList.h>

class CGUIEventDispatcher : public irr::IEventReceiver
{
    public:
        /** Default constructor */
        CGUIEventDispatcher();
        /** Default destructor */
        virtual ~CGUIEventDispatcher();

        virtual bool OnEvent(const irr::SEvent& event);

        bool bind(irr::gui::IGUIElement* element, bool (*func)(const irr::SEvent::SGUIEvent&), irr::gui::EGUI_EVENT_TYPE type = irr::gui::EGET_COUNT);
        template<class T>
        bool bind(irr::gui::IGUIElement* element, T* c, bool (T::*func)(const irr::SEvent::SGUIEvent&), irr::gui::EGUI_EVENT_TYPE type = irr::gui::EGET_COUNT)
        {
            Storage::Iterator it = Bindings.begin();
            while(it != Bindings.end())
            {
                if ((*it) == element && (*it).checkClass(c))
                    return false;
                else if ((*it) == element)
                {
                    (*it).Data.push_back(new CMethodHandle<T>(c, func, type));
                    return true;
                }
                ++it;
            }
            Pair p;
            p.Key = element;
            p.Data.push_back(new CMethodHandle<T>(c, func, type));
            Bindings.push_back(p);
            return true;
        }

        void unbind(irr::gui::IGUIElement* element);
        void unbind(irr::gui::IGUIElement* element, bool (*func)(const irr::SEvent::SGUIEvent&));
        template<class T>
        void unbind(irr::gui::IGUIElement* element, T* c)
        {
            Storage::Iterator it = Bindings.begin();
            while(it != Bindings.end())
            {
                if ((*it) == element)
                {
                    bool r = (*it).releaseClass(c);
                    return;
                }
                ++it;
            }
        }
        template<class T>
        void unbind(T* c)
        {
            Storage::Iterator it = Bindings.begin();
            while(it != Bindings.end())
            {
                bool r = (*it).releaseClass(c);
                ++it;
            }
        }

    protected:
        typedef bool (*FuncHandle)(const irr::SEvent::SGUIEvent&);
        struct IHandle
        {
            IHandle(irr::gui::EGUI_EVENT_TYPE type = irr::gui::EGET_COUNT)
            {
                EventType = type;
            }
            virtual ~IHandle(void){};
            virtual bool call(const irr::SEvent::SGUIEvent& event) = 0;
            virtual bool checkFunc(FuncHandle f) const = 0;
            virtual bool checkClass(void* c) const = 0;
            virtual const irr::gui::EGUI_EVENT_TYPE& getType(void)const{return EventType;}
            irr::gui::EGUI_EVENT_TYPE EventType;
        };
        struct CFunctionHandle : IHandle
        {
            CFunctionHandle(FuncHandle f, irr::gui::EGUI_EVENT_TYPE type): IHandle(type), Func(f)
            {}
            bool call(const irr::SEvent::SGUIEvent& event)
            {
                if (getType() == irr::gui::EGET_COUNT || getType() == event.EventType)
                    return Func(event);
                return false;
            }
            bool checkFunc(FuncHandle f) const
            {
                return f == Func;
            }
            bool checkClass(void* c) const
            {
                return false;
            }
            FuncHandle Func;
        };
        template<class T>
        struct CMethodHandle : IHandle
        {
            typedef bool (T::*MethodHandle)(const irr::SEvent::SGUIEvent&);
            CMethodHandle(T* c, MethodHandle f, irr::gui::EGUI_EVENT_TYPE type): IHandle(type),Func(f), Class(c)
            {
            }
            bool call(const irr::SEvent::SGUIEvent& event)
            {
                if (getType() == irr::gui::EGET_COUNT || getType() == event.EventType)
                    return (Class->*Func)(event);
                return false;
            }
            bool checkFunc(FuncHandle f) const
            {
                return false;
            }
            bool checkClass(void* c) const
            {
                return Class == c;
            }
            MethodHandle Func;
            T* Class;
        };
        typedef irr::core::array<IHandle*> Link;
        struct Pair
        {
            Pair(void) : Key(0){}
            Pair (const Pair& other)
            {
                *this = other;
            }
            Pair& operator= (const Pair& other)
            {
                Key = other.Key;
                Data = other.Data;
                return *this;
            }
            bool operator==(const irr::gui::IGUIElement* key)
            {
                return Key == key;
            }
            bool call(const irr::SEvent::SGUIEvent& event)
            {
                for (irr::u32 i=0;i<Data.size();++i)
                {
                    if (Data[i]->call(event))
                        return true;
                }
                return false;
            }
            bool checkFunc(FuncHandle f)
            {
                for (irr::u32 i=0;i<Data.size();++i)
                {
                    if (Data[i]->checkFunc(f))
                        return true;
                }
                return false;
            }
            bool checkClass(void* c)
            {
                for (irr::u32 i=0;i<Data.size();++i)
                {
                    if (Data[i]->checkClass(c))
                        return true;
                }
                return false;
            }
            bool releaseFunc(FuncHandle f)
            {
                for (irr::u32 i=0;i<Data.size();++i)
                {
                    if (Data[i]->checkFunc(f))
                    {
                        delete Data[i];
                        Data.erase(i);
                        return true;
                    }
                }
                return false;
            }
            bool releaseClass(void* c)
            {
                for (irr::u32 i=0;i<Data.size();++i)
                {
                    if (Data[i]->checkClass(c))
                    {
                        delete Data[i];
                        Data.erase(i);
                        return true;
                    }
                }
                return false;
            }
            void releaseHandle(void)
            {
                for (irr::u32 i=0;i<Data.size();++i)
                {
                    delete Data[i];
                }
                Data.clear();
            }
            irr::gui::IGUIElement* Key;
            Link Data;
        };
        typedef irr::core::list<Pair> Storage;
        Storage Bindings;
    private:
};

#endif // CGUIEVENTDISPATCHER_H
