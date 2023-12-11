#ifndef _CSETUPMENU
  #define _CSETUPMENU

  #include <irrlicht.h>
  #include "CLevels.h"

class CMenu : public irr::IEventReceiver
{
  private:
    irr::video::E_DRIVER_TYPE   m_iDriver;
    irr::       s32             m_iResX, m_iResY;
    irr::       IrrlichtDevice *m_pDevice;
    irr::gui::  IGUIComboBox   *m_pRes,*m_pVid;
    irr::gui::  IGUICheckBox   *m_pFs;

    int m_iStartedLevel;
    int m_iResult;

    CLevels m_cLevelInfo;

    bool m_bRefreshHiScore;
    bool m_bFullscreen;

    void saveSetup();
    void loadSetup();

    void fillHiScore(CLevelTop10 *top10, irr::gui::IGUIListBox *pList);
    void saveHiScore(CLevelInfo *pLvl);

  public:
    char m_sLevelFileName[255];

     CMenu();
    ~CMenu();

    irr::s32 getResX() { return m_iResX; }
    irr::s32 getResY() { return m_iResY; }

    irr::video::E_DRIVER_TYPE getDriver() { return m_iDriver; }

    bool getFullscreen() { return m_bFullscreen; }

    int showMenu(int iLevelTime);

    virtual bool OnEvent(const irr::SEvent &event);
};

#endif
