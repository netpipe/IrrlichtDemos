  #include <CProjectile.h>
  #include <geom/CIrrOdeGeom.h>

CProjectile::CProjectile(irr::ode::CIrrOdeBody *pBody, irr::s32 iTtl, irr::core::vector3df vAcc) {
  m_pBody=pBody;
  m_iTtl=iTtl;
  m_vAcc=vAcc;

  CProjectileManager::getSharedInstance()->addShot(this);
}

irr::ode::CIrrOdeBody *CProjectile::getBody() {
  return m_pBody;
}

irr::s32 CProjectile::getTtl() {
  return m_iTtl;
}

void CProjectile::step() {
  m_iTtl--;

  if (m_iTtl<=0) {
    irr::ode::CIrrOdeGeom *pGeom=m_pBody->getFirstGeom();
    if (pGeom) pGeom->removeFromPhysics();
    m_pBody->removeFromPhysics();
    m_pBody->remove();
  }
  else m_pBody->addForce(m_pBody->getRotation().rotationToDirection(m_vAcc));
}

void CProjectile::collide() {
  m_iTtl=0;
}

CProjectileManager::CProjectileManager() {
  m_iShots=0;
  m_iHits=0;
  m_pLast=NULL;

  irr::ode::CIrrOdeManager::getSharedInstance()->addEventListener(this);
}

CProjectileManager *CProjectileManager::getSharedInstance() {
  static CProjectileManager theManager;
  return &theManager;
}

void CProjectileManager::addShot(CProjectile *p) {
  m_lShots.push_back(p);
  m_pLast=p;
  m_iShots++;
}

bool CProjectileManager::onEvent(irr::ode::IIrrOdeEvent *pEvent) {
  if (pEvent->getType()==irr::ode::eIrrOdeEventStep) {
    irr::core::list<CProjectile *>::Iterator it;
    for (it=m_lShots.begin(); it!=m_lShots.end(); it++) {
      CProjectile *p=*it;
      p->step();
      if (p->getTtl()<=0) {
        if (m_pLast==p) m_pLast=NULL;
        m_lShots.erase(it);
        delete p;
        return false;
      }
    }
  }

  if (pEvent->getType()==irr::ode::eIrrOdeEventBodyMoved) {
    irr::ode::CIrrOdeEventBodyMoved *pMove=(irr::ode::CIrrOdeEventBodyMoved *)pEvent;
    irr::core::list<CProjectile *>::Iterator it;
    for (it=m_lShots.begin(); it!=m_lShots.end(); it++) {
      CProjectile *p=*it;
      if (p->getBody()==pMove->getBody() && pMove->getTouched()!=NULL) {
        irr::ode::CIrrOdeGeom *g=pMove->getTouched();
        if (g->getBody()!=NULL) m_iHits++;
        p->collide();
      }
    }
  }

  return false;
}

bool CProjectileManager::handlesEvent(irr::ode::IIrrOdeEvent *pEvent) {
  return pEvent->getType()==irr::ode::eIrrOdeEventBodyMoved || pEvent->getType()==irr::ode::eIrrOdeEventStep;
}

CProjectile *CProjectileManager::getLast() {
  return m_pLast;
}
