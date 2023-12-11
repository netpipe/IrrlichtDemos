  #include <irrlicht.h>
  #include <CShadowManager.h>
  #include <CManagedShadow.h>

CShadowManager::CShadowManager() {
}

CShadowManager *CShadowManager::getSharedInstance() {
  static CShadowManager theManager;
  return &theManager;
}

void CShadowManager::addShadow(CManagedShadow *pNew) {
  m_lShadows.push_back(pNew);
}

void CShadowManager::setPriority(u32 iPrio) {
  if (iPrio!=m_iPriority) {
    m_iPriority=iPrio;
    list<CManagedShadow *>::Iterator it;
    for (it=m_lShadows.begin(); it!=m_lShadows.end(); it++) {
      (*it)->update(m_iPriority);
    }
  }
}
