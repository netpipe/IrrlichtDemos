  #include <event/CIrrOdeEventBodyMoved.h>

namespace irr {
namespace ode {

class CIrrOdeBody;

CIrrOdeEventBodyMoved::CIrrOdeEventBodyMoved(CIrrOdeBody *pBody, vector3df &vPos, vector3df &vRot, vector3df &vLinVel, vector3df &vAngVel) : IIrrOdeEvent(){
  m_pBody=pBody;
  m_pTouch=pBody->getTouched();

  m_iBodyId=pBody?pBody->getID():0;

  m_iFlags=0;
  if (m_pTouch!=NULL) m_iTouchId=m_pTouch->getID(); else m_iTouchId=0;

  if (vPos!=pBody->getAbsolutePosition()) m_iFlags+=siIrrOdeEventPosition;
  if (vRot!=pBody->getRotation()) m_iFlags+=siIrrOdeEventRotation;

  if (vLinVel!=pBody->getNodeLinearVelocity ()) m_iFlags+=siIrrOdeEventLinearVel ;
  if (vAngVel!=pBody->getNodeAngularVelocity()) m_iFlags+=siIrrOdeEventAngularVel;

  if (pBody->dampingChanged()) m_iFlags+=siIrrOdeEventDamping;

  vector3df v=vLinVel-pBody->getLinearVelocity();

  if (m_pTouch!=NULL) m_iFlags+=siIrrOdeEventBodyTouching;

  m_vNewPos=vPos;
  m_vNewRot=vRot;
  m_vNewLinVel=vLinVel;
  m_vNewAngVel=vAngVel;

  m_iAngularDamping=(u16)(pBody->getAngularDamping()*65000.0f);
  m_iLinearDamping =(u16)(pBody->getLinearDamping ()*65000.0f);

  m_iColMaterial=pBody->getCollisionMaterial();
  m_vColPoint=pBody->getCollisionPoint();
}

CIrrOdeEventBodyMoved::CIrrOdeEventBodyMoved(CSerializer *pData, ISceneManager *pSmgr) : IIrrOdeEvent() {
  pData->resetBufferPos();
  u16 iCode=pData->getU16();
  if (iCode==eIrrOdeEventBodyMoved) {
    m_iBodyId=pData->getU32();
    m_pBody=reinterpret_cast<CIrrOdeBody *>(pSmgr->getSceneNodeFromId(m_iBodyId));

    m_iFlags=pData->getU8();

    if (m_iFlags&siIrrOdeEventPosition    ) pData->getVector3df(m_vNewPos);
    if (m_iFlags&siIrrOdeEventRotation    ) pData->getVector3df(m_vNewRot);
    if (m_iFlags&siIrrOdeEventLinearVel   ) pData->getVector3df(m_vNewLinVel);
    if (m_iFlags&siIrrOdeEventAngularVel  ) pData->getVector3df(m_vNewAngVel);
    if (m_iFlags&siIrrOdeEventBodyTouching) {
      m_iTouchId=pData->getU32();
      m_iColMaterial=pData->getS32();
      pData->getVector3df(m_vColPoint);
    }

    if (m_iFlags&siIrrOdeEventDamping) {
      m_iAngularDamping=pData->getU16();
      m_iLinearDamping=pData->getU16();
    }
  }
}

CIrrOdeEventBodyMoved::CIrrOdeEventBodyMoved(IIrrOdeEvent *pIn) : IIrrOdeEvent() {
  if (pIn->getType()==eIrrOdeEventBodyMoved) {
    CIrrOdeEventBodyMoved *p=(CIrrOdeEventBodyMoved *)pIn;
    m_iFlags=p->getFlags();
    m_pBody=p->getBody();
    m_iBodyId=p->getBodyId();
    m_vNewPos=p->getNewPosition();
    m_vNewRot=p->getNewRotation();
    m_vNewLinVel=p->getNewLinearVelocity();
    m_vNewAngVel=p->getNewAngularVelocity();
    m_iTouchId=p->getTouchId();
    m_pTouch=p->getTouched();
  }
}

u16 CIrrOdeEventBodyMoved::getType() {
  return eIrrOdeEventBodyMoved;
}

CIrrOdeBody *CIrrOdeEventBodyMoved::getBody() {
  return m_pBody;
}

void CIrrOdeEventBodyMoved::setBody(CIrrOdeBody *pBody) {
  m_pBody=pBody;
}

CSerializer *CIrrOdeEventBodyMoved::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventBodyMoved);
    m_pSerializer->addU32(m_iBodyId);

    m_pSerializer->addU8(m_iFlags);

    if (m_iFlags&siIrrOdeEventPosition    ) m_pSerializer->addVector3df(m_vNewPos);
    if (m_iFlags&siIrrOdeEventRotation    ) m_pSerializer->addVector3df(m_vNewRot);
    if (m_iFlags&siIrrOdeEventLinearVel   ) m_pSerializer->addVector3df(m_vNewLinVel);
    if (m_iFlags&siIrrOdeEventAngularVel  ) m_pSerializer->addVector3df(m_vNewAngVel);
    if (m_iFlags&siIrrOdeEventBodyTouching) {
      m_pSerializer->addU32(m_iTouchId);
      m_pSerializer->addS32(m_iColMaterial);
      m_pSerializer->addVector3df(m_vColPoint);
    }
    if (m_iFlags&siIrrOdeEventDamping) {
      m_pSerializer->addU16(m_iAngularDamping);
      m_pSerializer->addU16(m_iLinearDamping);
    }
  }

  return m_pSerializer;
}

CIrrOdeGeom *CIrrOdeEventBodyMoved::getTouched() {
  return m_pTouch;
}

void CIrrOdeEventBodyMoved::setTouched(CIrrOdeGeom *pGeom) {
  m_pTouch=pGeom;
}

void CIrrOdeEventBodyMoved::merge(CIrrOdeEventBodyMoved *pOther) {
  if (pOther->positionChanged()) m_vNewPos=pOther->getNewPosition();
  if (pOther->rotationChanged()) m_vNewRot=pOther->getNewRotation();
  if (pOther->linearVelChanged()) m_vNewLinVel=pOther->getNewLinearVelocity();
  if (pOther->angularVelChanged()) m_vNewAngVel=pOther->getNewAngularVelocity();
  if (pOther->isTouching()) {
    m_pTouch=pOther->getTouched();
    m_iTouchId=pOther->getTouchId();
    m_vColPoint=pOther->getCollisionPoint();
    m_iColMaterial=pOther->getCollisionMaterial();
  }

  m_iFlags|=pOther->getFlags();

  if (m_pSerializer) {
    delete m_pSerializer;
    m_pSerializer=NULL;
  }
}

const c8 *CIrrOdeEventBodyMoved::toString() {
  sprintf(m_sString,"CIrrOdeEventBodyMoved: body=%i, flags=%i",m_iBodyId,m_iFlags);

  if (positionChanged())
    sprintf(m_sString,"%s, new position=(%.2f, %.2f, %.2f)",m_sString,m_vNewPos.X,m_vNewPos.Y,m_vNewPos.Z);

  if (rotationChanged())
    sprintf(m_sString,"%s, new rotation=(%.2f, %.2f, %.2f)",m_sString,m_vNewRot.X,m_vNewRot.Y,m_vNewRot.Z);

  if (linearVelChanged())
    sprintf(m_sString,"%s, new linear velocity=(%.2f, %.2f, %.2f)",m_sString,m_vNewLinVel.X,m_vNewLinVel.Y,m_vNewLinVel.Z);

  if (angularVelChanged())
    sprintf(m_sString,"%s, new angular velocity=(%.2f, %.2f, %.2f)",m_sString,m_vNewAngVel.X,m_vNewAngVel.Y,m_vNewAngVel.Z);

  if (isTouching())
    sprintf(m_sString,"%s, touching body: %i, mat: %i, pos: (%.2f, %.2f, %.2f)",m_sString,m_iTouchId,m_iColMaterial,m_vColPoint.X,m_vColPoint.Y,m_vColPoint.Z);

  return m_sString;
}

f32 CIrrOdeEventBodyMoved::getLinearDamping() {
  return ((f32)m_iLinearDamping)/65000.0f;
}

f32 CIrrOdeEventBodyMoved::getAngularDamping() {
  return ((f32)m_iAngularDamping)/65000.0f;
}

u16 CIrrOdeEventBodyMoved::getLinearDampingInt() {
  return m_iLinearDamping;
}

u16 CIrrOdeEventBodyMoved::getAngularDampingInt() {
  return m_iAngularDamping;
}

void CIrrOdeEventBodyMoved::setLinearDamping(f32 f) {
  m_iLinearDamping =(u16)(f*65000.0f);
}

void CIrrOdeEventBodyMoved::setAngularDamping(f32 f) {
  m_iAngularDamping=(u16)(f*65000.0f);
}

} //namespace ode
} //namespace irr
