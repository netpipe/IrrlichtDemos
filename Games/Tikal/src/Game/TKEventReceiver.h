#ifndef __TK_EVENT_RECEIVER__
#define __TK_EVENT_RECEIVER__

#include "TKCore.h"
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class TKEventReceiver : public IEventReceiver
{
private:
    TKCore* tkCore;
    bool keys[KEY_KEY_CODES_COUNT];
public:
    TKEventReceiver(TKCore* ntkCore);

    bool OnEvent(const SEvent& event);
};

#endif
