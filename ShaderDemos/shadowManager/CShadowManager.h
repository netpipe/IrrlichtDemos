#ifndef _C_SHADOW_MANAGER
  #define _C_SHADOW_MANAGER

  #include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CManagedShadow;

class CShadowManager {
  private:
    list <CManagedShadow *> m_lShadows;
    u32 m_iPriority;

  protected:
    CShadowManager();

  public:
    static CShadowManager *getSharedInstance();

    void addShadow(CManagedShadow *pNew);
    void setPriority(u32 iPrio);
};

#endif


