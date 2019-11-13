  #include "CVehicle.h"

CVehicle::CVehicle(IrrlichtDevice *pDevice, wchar_t *sVehicleName, c8 *sHelpFile) {
  m_pDevice=pDevice;
  m_pSmgr=m_pDevice->getSceneManager();
  m_pGuiEnv=m_pDevice->getGUIEnvironment();

  m_pHelp=pDevice->getGUIEnvironment()->addStaticText(L"Help",rect<s32>(5,5,635,475),true);
  m_pHelp->setDrawBackground(true);
  m_pHelp->setBackgroundColor(SColor(0x80,0xFF,0xFF,0xFF));
  m_pHelp->setVisible(false);

  char sHelp[10000];
  FILE *f=fopen(sHelpFile,"rt");
  if (f) {
    fread(sHelp,1,10000,f);
    fclose(f);
  }
  else sprintf(sHelp,"Can't open \"%s\".",sHelpFile);

  memset(sHelp,10000,0);
  stringw sw(sHelp);
  m_pHelp->setText(sw.c_str());

  m_bHelp=false;
  m_bSwitchToMenu=false;
}

bool CVehicle::OnEvent(const SEvent &event) {
  if (event.EventType==EET_KEY_INPUT_EVENT) {
    if (event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_F1:
          m_bHelp=!m_bHelp;
          return true;
          break;

        case KEY_TAB:
          m_bSwitchToMenu=!m_bHelp;
          return true;
          break;

        default:
          break;
      }
    }
  }

  return false;
}

u32 CVehicle::update() {
  int iRet=0;

  if (m_bSwitchToMenu) {
    iRet=1;
    m_bSwitchToMenu=true;
  }

  if (m_bHelp) {
    deactivate();
    m_pHelp->setVisible(true);
  }
  else
    if (m_pHelp->isVisible()) {
      m_pHelp->setVisible(false);
      activate();
    }

  return iRet;
}
