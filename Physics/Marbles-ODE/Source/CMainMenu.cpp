  #include "CMainMenu.h"

CMainMenu::CMainMenu(irr::IrrlichtDevice *pDevice)
{
  m_pDevice=pDevice;
  m_pDevice->setEventReceiver(this);

  m_pDriver=m_pDevice->getVideoDriver();
  m_pSmgr  =m_pDevice->getSceneManager();
}

bool CMainMenu::OnEvent(irr::SEvent event)
{
  return false;
}

int CMainMenu::showMenu()
{
  irr::scene::ICameraSceneNode *pCam=m_pSmgr->addCameraSceneNode(NULL,irr::core::vector3df(0,0,5),irr::core::vector3df(0,0,0));

  m_pSmgr->clear();
  m_pSmgr->loadScene("Menu/menu.irr");
  m_pSmgr->setActiveCamera(pCam);

  while (m_pDevice->run())
  {
    m_pDriver->beginScene(true,true,irr::video::SColor(0,0,0,0));
    m_pSmgr->drawAll();
    m_pGuienv->drawAll();
    m_pDriver->endScene();
  }

  return 0;
}

