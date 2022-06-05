#ifndef CINPUT_CATCHER_H
#define CINPUT_CATCHER_H

#include <irrlicht.h>

class IMouseReceiver
{
public:
    virtual void mouse_event(const irr::SEvent::SMouseInput& mouse) = 0;
};

class IKeyboardReceiver
{
public:
    virtual void key_event(const irr::SEvent::SKeyInput& key) = 0;
};

class CInput_catcher : public irr::IEventReceiver
{
    public:
        /** Default constructor */
        CInput_catcher();
        /** Default destructor */
        virtual ~CInput_catcher();

        bool OnEvent(const irr::SEvent& event);

        void registerInput(IMouseReceiver* input);
        void registerInput(IKeyboardReceiver* input);

        void unregisterInput(IMouseReceiver* input);
        void unregisterInput(IKeyboardReceiver* input);
    protected:
        irr::core::list<IMouseReceiver*> Mouse;
        irr::core::list<IKeyboardReceiver*> Keyboard;
    private:
};

#endif // CINPUT_CATCHER_H
