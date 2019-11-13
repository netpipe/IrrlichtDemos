  #include "CMenu.h"

CMenu::CMenu(IrrlichtDevice *pDevice) : CVehicle(pDevice,L"Vehicle Select Menu","../../data/irrOdeVehicleHelp.txt") {
  IGUIButton *b=NULL;

  b=m_pGuiEnv->addButton(rect<s32>(5, 5,100, 25),0,2,L"Select Car"       ); m_aButtons.push_back(b);
  b=m_pGuiEnv->addButton(rect<s32>(5,30,100, 50),0,3,L"Select Tank"      ); m_aButtons.push_back(b);
  b=m_pGuiEnv->addButton(rect<s32>(5,55,100, 75),0,4,L"Select Helicopter"); m_aButtons.push_back(b);
  b=m_pGuiEnv->addButton(rect<s32>(5,80,100,100),0,5,L"Select Plane"     ); m_aButtons.push_back(b);

  m_pText=m_pGuiEnv->addStaticText(m_pHelp->getText(),rect<s32>(105,5,275,50));
  m_pText->setDrawBackground(true);
  m_pText->setBackgroundColor(SColor(0x80,0xFF,0xFF,0xFF));
}

CMenu::~CMenu() {
}

void CMenu::activate() {
  static bool bFirstCall=true;
  m_pDevice->setEventReceiver(this);

  if (bFirstCall) {
    m_pSmgr->getActiveCamera()->setPosition(vector3df(3502.5f,1000.0f,2490.0f));
    m_pSmgr->getActiveCamera()->setTarget(vector3df(3500.0f,1000.0f,2500.0f));
    bFirstCall=false;
  }

  m_pDevice->getCursorControl()->setVisible(true);
  m_iMenuSelect=0;

  m_pText->setVisible(true);

  list<IGUIButton *>::Iterator i;
  for (i=m_aButtons.begin(); i!=m_aButtons.end(); i++) (*i)->setVisible(true);
}

void CMenu::deactivate() {
  m_pText->setVisible(false);

  list<IGUIButton *>::Iterator i;
  for (i=m_aButtons.begin(); i!=m_aButtons.end(); i++) (*i)->setVisible(false);
}

u32 CMenu::update() {
  if (!m_bHelp && m_pHelp->isVisible()) {
    m_pText->setVisible(true);

    list<IGUIButton *>::Iterator i;
    for (i=m_aButtons.begin(); i!=m_aButtons.end(); i++) (*i)->setVisible(true);
  }

  if (m_bHelp && !m_pHelp->isVisible()) {
    m_pText->setVisible(false);

    list<IGUIButton *>::Iterator i;
    for (i=m_aButtons.begin(); i!=m_aButtons.end(); i++) (*i)->setVisible(false);
  }

  CVehicle::update();

  u32 iRet=m_iMenuSelect;
  m_iMenuSelect=0;

  return iRet;
}

bool CMenu::OnEvent(const SEvent &event) {
  if (CVehicle::OnEvent(event)) return true;

  if (event.EventType==EET_GUI_EVENT) {
    if (event.GUIEvent.EventType==EGET_BUTTON_CLICKED) {
      m_iMenuSelect=event.GUIEvent.Caller->getID();
    }
  }

  return false;
}

