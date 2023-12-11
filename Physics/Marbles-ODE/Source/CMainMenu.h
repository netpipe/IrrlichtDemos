#ifndef _MAINMENU
  #define _MAINMENU

  #include <irrlicht.h>

class CMainMenu : public irr::IEventReceiver
{
  private:
    irr::IrrlichtDevice        *m_pDevice;
    irr::video::IVideoDriver   *m_pDriver;
    irr::gui::IGUIEnvironment  *m_pGuienv;
    irr::scene::IMeshSceneNode *m_pMenuItems[6];
    irr::scene::ISceneManager  *m_pSmgr;

  public:
    CMainMenu(irr::IrrlichtDevice *pDevice);

    virtual bool OnEvent(irr::SEvent event);
    int showMenu();
};

#endif