  #include <geom/CIrrOdeGeom.h>
  #include <IIrrOdeDevice.h>
  #include <CIrrOdeBody.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeManager.h>

namespace irr {
namespace ode {

CIrrOdeGeom::CIrrOdeGeom(ISceneNode *parent,ISceneManager *mgr,s32 id,
                         const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                         CIrrOdeSceneNode(parent, mgr, id, vector3df(0.0f,0.0f,0.0f), rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeom constructor\n");
  #endif

  m_pSpace=NULL;

  m_iGeomId=0;
	m_iCollisionGroup=0;

  m_cCenterOfGravity=vector3df(0.0f,0.0f,0.0f);
  m_cInertia1=vector3df(0.0f,0.0f,0.0f);
  m_cInertia2=vector3df(0.0f,0.0f,0.0f);
  m_cMassTranslate=vector3df(0.0f,0.0f,0.0f);
  m_cOffsetPos=vector3df(0.0f,0.0f,0.0f);
  m_cOffsetRot=vector3df(0.0f,0.0f,0.0f);

  m_iMass=m_pOdeDevice->massCreate();
  m_pOdeDevice->massSetZero(m_iMass);

	m_pBody=reinterpret_cast<CIrrOdeBody *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_BODY_ID));

  CIrrOdeSurfaceParameters *pParams=new CIrrOdeSurfaceParameters();
  CIrrOdeManager::getSharedInstance()->addSurfaceParameter(pParams);
  pParams->setStatic(m_pBody==NULL);
  m_aParams.push_back(pParams);

  m_bCollide=true;

  #ifndef _IRREDIT_PLUGIN
    if (m_pBody) {
      m_pBody->addGeom(this);
      m_pWorld=reinterpret_cast<CIrrOdeWorld *>(m_pBody->getWorld());
    }
    else {
      m_pWorld=reinterpret_cast<CIrrOdeWorld *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID));
      if (m_pWorld) m_pWorld->addGeom(this);
    }
    #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
      printf("CIrrOdeGeom: body=%i\n",m_pBody?m_pBody->getID():-2);
    #endif
  #endif
}

CIrrOdeGeom::~CIrrOdeGeom() {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeom destructor\n");
  #endif

  if (m_pBody) {
    #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
      printf("CIrrOdeGeom destructor: removing geom from body\n");
    #endif
    m_pBody->removeGeom(this);
    m_pOdeDevice->geomSetBody(m_iGeomId,NULL);
  }

  for (u32 i=0; i<m_aParams.size(); i++) {
    m_pOdeManager->removeSurfaceParameter(m_aParams[i]);
    delete m_aParams[i];
  }

  if (m_iGeomId) m_pOdeDevice->geomDestroy(m_iGeomId);
}

u32 CIrrOdeGeom::getGeomId(u32 iNum) {
  return m_iGeomId;
}

CIrrOdeBody *CIrrOdeGeom::getBody() {
  return m_pBody;
}

void CIrrOdeGeom::setBody(CIrrOdeBody *pBody) {
  m_pBody=pBody;
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeGeom::setBody: setting body of geom %i to %i\n",m_iGeomId,(int)(pBody?pBody->getBodyId():0));
  #endif
  if (m_iGeomId) m_pOdeDevice->geomSetBody(m_iGeomId,pBody);
}

u32 CIrrOdeGeom::getSurfaceParametersCount() const {
  return m_aParams.size();
}

CIrrOdeSurfaceParameters *CIrrOdeGeom::getSurfaceParameters(u32 iIdx) {
  return m_aParams[0];
}

void CIrrOdeGeom::setSurfaceParameters(CIrrOdeSurfaceParameters *pParams, u32 iIdx) {
  m_aParams[0]=pParams;
}

CIrrOdeWorld *CIrrOdeGeom::getWorld() {
  return m_pWorld;
}

void CIrrOdeGeom::setMassParameters(f32 fMass, vector3df c, vector3df i1, vector3df i2) {
  m_cCenterOfGravity=c;
  m_cInertia1=i1;
  m_cInertia2=i2;
  m_fMass=fMass;

  m_pOdeDevice->massSetParameters(m_iMass,fMass,c,i1,i2);
}

void CIrrOdeGeom::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeSceneNode::serializeAttributes(out,options);

	if (!m_pBody)
    for (u32 i=0; i<this->getSurfaceParametersCount(); i++) m_aParams[i]->serializeAttributes(out,options,i);
	else
		m_aParams[0]->serializeAttributes(out,options,0);

  out->addFloat("Mass",m_fMass);

  out->addVector3d("CenterOfGravity",m_cCenterOfGravity);
  out->addVector3d("Inertia_1"      ,m_cInertia1       );
  out->addVector3d("Inertia_2"      ,m_cInertia2       );
  out->addVector3d("MassTranslation",m_cMassTranslate  );

  out->addBool("collides",m_bCollide);
	out->addInt("collisionGroup",m_iCollisionGroup);
}

void CIrrOdeGeom::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeSceneNode::deserializeAttributes(in,options);
  for (u32 i=0; i<getSurfaceParametersCount(); i++) {
    m_aParams[i]->deserializeAttributes(in,options,i);
  }

  m_fMass=in->getAttributeAsFloat("Mass");

  m_cCenterOfGravity=in->getAttributeAsVector3d("CenterOfGravity");
  m_cInertia1       =in->getAttributeAsVector3d("Inertia_1"      );
  m_cInertia2       =in->getAttributeAsVector3d("Inertia_2"      );
  m_cMassTranslate  =in->getAttributeAsVector3d("MassTranslation");

  m_bCollide=!in->existsAttribute("collides") || in->getAttributeAsBool("collides");

	m_iCollisionGroup=in->getAttributeAsInt("collisionGroup");
}

void CIrrOdeGeom::setMassTranslation(vector3df pos) {
  m_cMassTranslate=pos;
  if (m_iGeomId) m_pOdeDevice->massTranslate(m_iMass,pos);
}

void CIrrOdeGeom::initPhysics() {
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeGeom::initPhysics\n");
  #endif

  if (m_pBody!=NULL) {
    if (m_cCenterOfGravity.getLength()!=0.0f || m_cInertia1.getLength()!=0.0f || m_cInertia2.getLength()!=0.0f)
      m_pOdeDevice->massSetParameters(m_iMass,m_fMass,m_cCenterOfGravity,m_cInertia1,m_cInertia2);

    if (m_cMassTranslate.getLength()!=0.0f) m_pOdeDevice->massTranslate(m_iMass,m_cMassTranslate);

    m_cOffsetPos=getParent()->getPosition();
    if (m_cOffsetPos!=vector3df(0.0f,0.0f,0.0f)) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeGeom::initPhysics: offset position of geom set to (%.2f, %.2f, %.2f)\n",m_cOffsetPos.X,m_cOffsetPos.Y,m_cOffsetPos.Z);
      #endif
      m_pOdeDevice->geomSetOffsetPosition(m_iGeomId,m_cOffsetPos);
      m_pOdeDevice->massTranslate(m_iMass,m_cOffsetPos);
    }

    m_cOffsetRot=getParent()->getRotation();
    if (m_cOffsetRot!=vector3df(0.0f,0.0f,0.0f)) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeGeom::initPhysics: offset rotation of geom set to (%.2f, %.2f, %.2f)\n",m_cOffsetRot.X,m_cOffsetRot.Y,m_cOffsetRot.Z);
      #endif
      m_pOdeDevice->geomSetOffsetQuaternion(m_iGeomId,m_cOffsetRot);
    }
  }
  else {
    vector3df rot=getParent()->getRotation();
    m_pOdeDevice->geomSetRotation(m_iGeomId,rot);
  }

  for (u32 i=0; i<m_aParams.size(); i++) m_aParams[i]->initPhysics();
  CIrrOdeSceneNode::initPhysics();
}

void CIrrOdeGeom::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeSceneNode::copyParams(pDest);
  pDest->setName(getName());

  CIrrOdeGeom *pDst=(CIrrOdeGeom *)pDest;
  pDst->setCollide(doesCollide());

  for (u32 i=0; i<getSurfaceParametersCount() && i<pDst->getSurfaceParametersCount(); i++)
    m_aParams[i]->copy(pDst->getSurfaceParameters(i));

  if (getSurfaceParametersCount()<pDst->getSurfaceParametersCount())
    for (u32 i=getSurfaceParametersCount(); i<pDst->getSurfaceParametersCount(); i++)
      m_aParams[0]->copy(pDst->getSurfaceParameters(i));

  if (m_cInertia1.getLength()>0.0f)
    pDst->setMassParameters(m_fMass,m_cCenterOfGravity,m_cInertia1,m_cInertia2);
  else
    pDst->setMassTotal(m_fMass);

  pDst->setMassTranslation(m_cMassTranslate);
}

void CIrrOdeGeom::removeFromPhysics() {
  CIrrOdeSceneNode::removeFromPhysics();
  if (m_iGeomId) {
    m_pOdeDevice->geomDestroy(m_iGeomId);
    m_iGeomId=0;
    if (m_pBody)
      m_pBody->removeGeom(this);
    else
      if (m_pWorld)
        m_pWorld->removeGeom(this);
  }
  else
    if (!m_bPhysicsInitialized) {
      if (m_pBody) m_pBody->removeGeom(this); else m_pWorld->removeGeom(this);
    }
}

void CIrrOdeGeom::setOffsetPosition(vector3df pPos) {
  m_cOffsetPos=pPos;
  if (m_iGeomId) {
    #ifdef _TRACE_INIT_PHYSICS
      printf("CIrrOdeGeom::setOffsetPosition: setting offset position to (%.2f, %.2f, %.2f)\n",pPos.X,pPos.Y,pPos.Z);
    #endif
    m_pOdeDevice->geomSetOffsetPosition(m_iGeomId,m_cOffsetPos);
  }
}

void CIrrOdeGeom::setOffsetQuaternion(vector3df pRot) {
  m_cOffsetRot=pRot;
  if (m_iGeomId) {
    #ifdef _TRACE_INIT_PHYSICS
      printf("CIrrOdeGeom::setOffsetQuaternion: setting offset rotation to (%.2f, %.2f, %.2f)\n",pRot.X,pRot.Y,pRot.Z);
    #endif
    m_pOdeDevice->geomSetOffsetQuaternion(m_iGeomId,m_cOffsetRot);
  }
}

void CIrrOdeGeom::setPosition(const vector3df &newpos) {
	ISceneNode::setPosition(vector3df(0.0f,0.0f,0.0f));
}

void CIrrOdeGeom::setCollide(bool b) {
  m_bCollide=b;
}

bool CIrrOdeGeom::doesCollide() {
  return m_bCollide;
}

} //namespace ode
} //namespace irr
