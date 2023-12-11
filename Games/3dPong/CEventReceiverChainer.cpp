#include "CEventReceiverChainer.h"
namespace irr{
namespace game{
void CEventReceiverChainer::addReceiver(IEventReceiver* receiver){
    for(u32 i=0;i<Receivers.size();++i){
        if(Receivers[i]==receiver)
            return;
    }
    Receivers.push_back(receiver);
}
void CEventReceiverChainer::removeReceiver(IEventReceiver* receiver){
    s32 matched=-1;
    for(u32 i=0;i<Receivers.size();++i){
        if(Receivers[i]==receiver){
            matched = i;
            break;
        }
    }
    if(matched!=-1){
        for(u32 i=matched;i+1<Receivers.size();i++){
            Receivers[i] = Receivers[i+1];
        }
        Receivers.erase(Receivers.size()-1,1);
    }
}
void CEventReceiverChainer::clearReceiver(){
    Receivers.clear();
}
bool CEventReceiverChainer::OnEvent(const SEvent& event){
    for(u32 i=0;i<Receivers.size();++i){
        if(Receivers[i]->OnEvent(event))
            return true;
    }
    return false;
}
}}//
