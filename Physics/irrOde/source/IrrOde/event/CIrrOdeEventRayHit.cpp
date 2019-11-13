  #include <event/CIrrOdeEventRayHit.h>

namespace irr {
namespace ode {

CIrrOdeEventRayHit::CIrrOdeEventRayHit(CIrrOdeBody *pBody, CIrrOdeGeom *pGeom, CIrrOdeGeomRay *pRay, vector3df pos, f32 fDistance, s32 ID) : IIrrOdeEvent() {
  m_pBody=pBody;
  m_pGeom=pGeom;
  m_pRay=pRay;
  m_cPos=pos;
  m_fDistance=fDistance;

  this->ID=ID;
}

CIrrOdeEventRayHit::CIrrOdeEventRayHit(CSerializer *pData, ISceneManager *pSmgr, s32 ID) : IIrrOdeEvent() {
  this->ID=ID;
  pData->resetBufferPos();
  if (pData->getU16()==eIrrOdeEventRayHit) {
    u32 iRayId=pData->getU32(),iBodyId=pData->getU32(),iGeomId=pData->getU32();
    pData->getVector3df(m_cPos);
    m_fDistance=pData->getF32();

    m_pRay =reinterpret_cast<CIrrOdeGeomRay *>(pSmgr->getSceneNodeFromId(iRayId ));
    m_pBody=reinterpret_cast<CIrrOdeBody    *>(pSmgr->getSceneNodeFromId(iBodyId));
    m_pGeom=reinterpret_cast<CIrrOdeGeom    *>(pSmgr->getSceneNodeFromId(iGeomId));
  }
}

CIrrOdeEventRayHit::CIrrOdeEventRayHit(IIrrOdeEvent *pIn) : IIrrOdeEvent() {
  this->ID=ID;
  CIrrOdeEventRayHit *p=(CIrrOdeEventRayHit *)pIn;
  m_pBody=p->getBody();
  m_pGeom=p->getGeom();
  m_pRay=p->getRay();
  m_cPos=p->getPosition();
  m_fDistance=p->getDistance();
}

s32 CIrrOdeEventRayHit::getID() {
  return ID;
}

u16 CIrrOdeEventRayHit::getType() {
  return eIrrOdeEventRayHit;
}

CIrrOdeGeom *CIrrOdeEventRayHit::getGeom() {
  return m_pGeom;
}

CIrrOdeBody *CIrrOdeEventRayHit::getBody() {
  return m_pBody;
}

CIrrOdeGeomRay *CIrrOdeEventRayHit::getRay() {
  return m_pRay;
}

vector3df CIrrOdeEventRayHit::getPosition() {
  return m_cPos;
}

f32 CIrrOdeEventRayHit::getDistance() {
  return m_fDistance;
}

CSerializer *CIrrOdeEventRayHit::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventRayHit);
    m_pSerializer->addU32(m_pRay->getID());
    m_pSerializer->addU32(m_pBody->getID());
    m_pSerializer->addU32(m_pGeom->getID());
    m_pSerializer->addVector3df(m_cPos);
    m_pSerializer->addF32(m_fDistance);
  }

  return m_pSerializer;
}

const c8 *CIrrOdeEventRayHit::toString() {
  sprintf(m_sString,"CIrrOdeEventRayHit: ray=%i, body=%i, geom=%i, pos=(%.2f, %.2f, %.2f), distance=%.2f",
          m_pRay?m_pRay->getID():-2,m_pBody?m_pBody->getID():-2,m_pGeom?m_pGeom->getID():-2,
          m_cPos.X,m_cPos.Y,m_cPos.Z,m_fDistance);

  return m_sString;
}

} //namespace ode
} //namespace irr
