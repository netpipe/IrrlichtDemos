#ifndef _C_MENU
  #define _C_MENU

  #include <irrlicht.h>
  #include <IrrOde.h>

  #include "CVehicle.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CMenu : public CVehicle, public IEventReceiver {
  protected:
    list<IGUIButton *> m_aButtons;
    IGUIStaticText *m_pText;
    u32 m_iMenuSelect;

  public:
    CMenu(IrrlichtDevice *pDevice);
    virtual ~CMenu();

    virtual void activate();
    virtual void deactivate();
    virtual u32 update();

    virtual bool OnEvent(const SEvent &event);
};

#endif
