  #include <event/CIrrOdeEventLoadScene.h>

namespace irr {
namespace ode {

CIrrOdeEventLoadScene::CIrrOdeEventLoadScene(const c8 *sScene) : IIrrOdeEvent() {
  m_sScene=stringc(sScene);
}

CIrrOdeEventLoadScene::CIrrOdeEventLoadScene(IIrrOdeEvent *pIn) : IIrrOdeEvent() {
  if (pIn->getType()==eIrrOdeEventLoadScene) {
    m_sScene=stringc(((CIrrOdeEventLoadScene *)pIn)->getScene());
  }
}

CIrrOdeEventLoadScene::CIrrOdeEventLoadScene(CSerializer *pData) : IIrrOdeEvent() {
  pData->resetBufferPos();
  if (pData->getU16()==eIrrOdeEventLoadScene) {
    c8 s[1024];
    pData->getString(s,1023);
    m_sScene=stringc(s);
  }
}

u16 CIrrOdeEventLoadScene::getType() {
  return eIrrOdeEventLoadScene;
}

CSerializer *CIrrOdeEventLoadScene::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventLoadScene);
    m_pSerializer->addString(m_sScene.c_str());
  }
  return m_pSerializer;
}

const c8 *CIrrOdeEventLoadScene::toString() {
  sprintf(m_sString,"CIrrOdeEventLoadScene: \"%s\"",m_sScene.c_str());
  return m_sString;
}

const c8 *CIrrOdeEventLoadScene::getScene() {
  return m_sScene.c_str();
}

} //namespace ode
} //namespace irr

