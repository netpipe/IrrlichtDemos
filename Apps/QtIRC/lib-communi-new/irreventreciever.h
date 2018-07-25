#ifndef IRREVENTRECIEVER_H
#define IRREVENTRECIEVER_H

#include "irrlicht.h"

using namespace irr;

using namespace gui;

class IrrEventReciever : public IEventReceiver {
public:
    IrrEventReciever();

    ~IrrEventReciever(){}
    virtual bool OnEvent(const SEvent& event){
        return false;
    }
};

#endif // IRREVENTRECIEVER_H
