#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include "Globals.hpp"

class MyEventReceiver : public IEventReceiver
{

  public:

    MyEventReceiver();

    virtual bool OnEvent(const SEvent &event);

    bool getKeyState(EKEY_CODE key);

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }

  private:

    bool keys[KEY_KEY_CODES_COUNT];

    IrrlichtDevice *irrDevice;

};

#endif
