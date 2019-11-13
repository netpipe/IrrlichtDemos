#ifndef _I_VEHICLE
  #define _I_VEHICLE

  #include <irrlicht.h>
  #include <IrrOde.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace ode;

class CVehicle {
  protected:
    bool m_bSwitchToMenu,m_bHelp;

    IrrlichtDevice *m_pDevice;
    ISceneManager *m_pSmgr;
    IGUIEnvironment *m_pGuiEnv;

    IGUIStaticText *m_pHelp;

  public:
    CVehicle(IrrlichtDevice *pDevice, wchar_t *sVehicleName, c8 *sHelpFile);

    virtual ~CVehicle() { }

    virtual void activate()=0;
    virtual void deactivate()=0;
    virtual u32 update();

    virtual bool OnEvent(const SEvent &event);
};

#endif
