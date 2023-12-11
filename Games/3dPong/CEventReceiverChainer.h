#ifndef _CEVENTRECEIVERCHAINER_H_
#define _CEVENTRECEIVERCHAINER_H_
#include "IEventReceiver.h"
#include "irrArray.h"
namespace irr{
namespace game{
class CEventReceiverChainer: public IEventReceiver{
    public:
        virtual void addReceiver(IEventReceiver* receiver);
        virtual void removeReceiver(IEventReceiver* receiver);
        virtual void clearReceiver();
        virtual bool OnEvent(const SEvent& event);
    protected:
        core::array<IEventReceiver*> Receivers;
};
}}//irr::game
#endif
