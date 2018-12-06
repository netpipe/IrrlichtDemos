#ifndef CIRRROCKETEVENTRECEIVER_H
#define CIRRROCKETEVENTRECEIVER_H

#include <IEventReceiver.h>

namespace Rocket
{
    namespace Core
    {
        class Context;
    }
}

class CIrrRocketEventReceiver : public irr::IEventReceiver
{
public:
    CIrrRocketEventReceiver(Rocket::Core::Context* context);
    virtual ~CIrrRocketEventReceiver(void);

    bool OnEvent(const irr::SEvent& event);
protected:
    Rocket::Core::Context* Context;
};

#endif // CIRRROCKETEVENTRECEIVER_H
