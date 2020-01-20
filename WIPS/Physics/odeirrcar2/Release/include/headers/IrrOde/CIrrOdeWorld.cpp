  #include <CIrrOdeWorld.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeManager.h>
  #include <geom/CIrrOdeGeomRay.h>
  #include <event/CIrrOdeEventRayHit.h>
  #include <event/CIrrOdeEventBodyMoved.h>
  #include <CIrrOdeContactParameters.h>
  #include <geom/CIrrOdeGeom.h>
  #include <CIrrOdeBody.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeWorld::CIrrOdeWorld(ISceneNode *parent,ISceneManager *mgr,s32 id,
                           const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                           CIrrOdeDampable(parent, mgr, id, position, rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeWorld constructor\n");
  #endif

  m_iWorldId=0;
  m_iJointGroupId=0;
  m_sSurfaceFile=stringw("");

  m_pWorldSpace=NULL;

	m_cGravity=vector3df(0.0f,-10.0f,0.0f);
	m_pOdeManager->addWorld(this);
	m_bDrawEditorMesh=true;
  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeWorld.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

CIrrOdeWorld::~CIrrOdeWorld() {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeWorld destructor\n");
  #endif

  if (m_iJointGroupId) {
    m_pOdeDevice->jointGroupDestroy(m_iJointGroupId);
    m_iJointGroupId=0;
  }

  if (m_iWorldId) {
    m_pOdeDevice->worldDestroy(m_iWorldId);
    m_iWorldId=0;
  }

  m_pOdeManager->removeWorld(this);
}

void CIrrOdeWorld::initPhysics() {
  if (m_bPhysicsInitialized) return;
  CIrrOdeSceneNode::initPhysics();

  m_iWorldId=m_pOdeDevice->worldCreate();
  m_iJointGroupId=m_pOdeDevice->jointGroupCreate(0);

  if (!m_pWorldSpace) m_pWorldSpace=new CIrrOdeSpace(this,m_pSceneManager);
  if (m_pWorldSpace!=NULL) m_pWorldSpace->initPhysics();

  setLinearDamping          (m_fDampingLinear          );
  setAngularDamping         (m_fDampingAngular         );
  setLinearDampingThreshold (m_fDampingLinearThreshold );
  setAngularDampingThreshold(m_fDampingAngularThreshold);

  setAutoDisableFlag            (m_iAutoDisableFlag            );
  setAutoDisableLinearThreshold (m_fAutoDisableLinearThreshold );
  setAutoDisableAngularThreshold(m_fAutoDisableAngularThreshold);
  setAutoDisableSteps           (m_iAutoDisableSteps           );
  setAutoDisableTime            (m_fAutoDisableTime            );

  setGravity(m_cGravity);

  if (m_fMaxAngularSpeed!=_DEFAULT_MAX_ANGULAR_SPEED) setMaxAngularSpeed(m_fMaxAngularSpeed);

  #ifdef _TRACE_INIT_PHYSICS
    printf("world created .. id=%i\n",(int)m_iWorldId);
  #endif

  list<CIrrOdeSpace *>::Iterator s;
  for (s=m_pSpaces.begin(); s!=m_pSpaces.end(); s++) {
    #ifdef _TRACE_INIT_PHYSICS
      printf("CIrrOdeWorld::initPhysics: init space\n");
    #endif
    CIrrOdeSpace *pSpace=(*s);
    pSpace->initPhysics();
  }

  list<CIrrOdeGeom *>::Iterator i;

  for (i=m_pGeoms.begin(); i!=m_pGeoms.end(); i++) {
    #ifdef _TRACE_INIT_PHYSICS
      printf("CIrrOdeWorld::initPhysics: init geom\n");
    #endif
    CIrrOdeGeom *pGeom=(*i);
    pGeom->initPhysics();
  }

  list<CIrrOdeBody *>::Iterator b,ib2;
  for (b=m_pBodies.begin(); b!=m_pBodies.end(); b++) {
    #ifdef _TRACE_INIT_PHYSICS
      printf("CIrrOdeWorld::initPhysics: init body\n");
    #endif
    CIrrOdeBody *b1=*b;
    b1->initPhysics();

    if (b1->isParamMaster()) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeWorld::initPhysics: \"%s\" parameter master of ODE body class \"%s\"\n",b1->getName(),b1->getOdeClassname().c_str());
      #endif
      for (ib2=m_pBodies.begin(); ib2!=m_pBodies.end(); ib2++) {
        CIrrOdeBody *b2=*ib2;
        if (b1->getOdeClassname()==b2->getOdeClassname()) {
          #ifdef _TRACE_INIT_PHYSICS
            printf("CIrrOdeWorld::initPhysics: setting body parameters of \"%s\"\n",b2->getName());
          #endif
          b1->copyParams(b2,false);
        }
      }
    }
  }
}

u32 CIrrOdeWorld::getWorldId() {
  return m_iWorldId;
}

void CIrrOdeWorld::addBody(CIrrOdeBody *pBody) {
  m_pBodies.push_back(pBody);
}

void CIrrOdeWorld::addGeom(CIrrOdeGeom *pGeom) {
  m_pGeoms.push_back(pGeom);
}

void CIrrOdeWorld::removeBody(CIrrOdeBody *pBody) {
  list<CIrrOdeBody *>::Iterator i;
  for (i=m_pBodies.begin(); i!=m_pBodies.end(); i++)
    if ((*i)==pBody) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeWorld::removeBody: removing body from world\n");
      #endif
      m_pBodies.erase(i);
      return;
    }
}

void CIrrOdeWorld::removeGeom(CIrrOdeGeom *pGeom) {
  list<CIrrOdeGeom *>::Iterator i;
  for (i=m_pGeoms.begin(); i!=m_pGeoms.end(); i++)
    if ((*i)==pGeom) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeWorld::removeGeom: removing geom from world\n");
      #endif
      m_pGeoms.erase(i);
      return;
    }
}

u32 CIrrOdeWorld::getBodyCount() {
  return m_pBodies.getSize();
}

u32 CIrrOdeWorld::getGeomCount() {
  return m_pGeoms.getSize();
}

void CIrrOdeWorld::setLinearDamping(f32 fScale) {
  m_fDampingLinear=fScale;
  if (m_iWorldId) m_pOdeDevice->worldSetLinearDamping(m_iWorldId,fScale);
  #ifdef _TRACE_CHANGED_PROPERTIES
    printf("world %i: linear damping: %.5f\n",(s32)m_iWorldId,getLinearDamping());
  #endif
}

f32 CIrrOdeWorld::getLinearDamping() {
  return m_iWorldId?m_pOdeDevice->worldGetLinearDamping(m_iWorldId):m_fDampingLinear;
}

void CIrrOdeWorld::setAngularDamping(f32 fScale) {
  m_fDampingAngular=fScale;
  if (m_iWorldId) m_pOdeDevice->worldSetAngularDamping(m_iWorldId,fScale);
  #ifdef _TRACE_CHANGED_PROPERTIES
    printf("world %i: angular damping: %.5f\n",(s32)m_iWorldId,getAngularDamping());
  #endif
}

f32 CIrrOdeWorld::getAngularDamping() {
  return m_iWorldId?m_pOdeDevice->worldGetAngularDamping(m_iWorldId):m_fDampingAngular;
}

void CIrrOdeWorld::setLinearDampingThreshold(f32 fThreshold) {
  m_fDampingLinearThreshold=fThreshold;
  if (m_iWorldId) m_pOdeDevice->worldSetLinearDampingThreshold(m_iWorldId,fThreshold);
  #ifdef _TRACE_CHANGED_PROPERTIES
    printf("world %i: linear damping threshold: %.5f\n",(s32)m_iWorldId,getLinearDampingThreshold());
  #endif
}

f32 CIrrOdeWorld::getLinearDampingThreshold() {
  return m_iWorldId?m_pOdeDevice->worldGetLinearDampingThreshold(m_iWorldId):m_fDampingLinearThreshold;
}

void CIrrOdeWorld::setAngularDampingThreshold(f32 fThreshold) {
  m_fDampingAngularThreshold=fThreshold;
  if (m_iWorldId) m_pOdeDevice->worldSetAngularDampingThreshold(m_iWorldId,fThreshold);
  #ifdef _TRACE_CHANGED_PROPERTIES
    printf("world %i: angular damping threshhold: %.5f\n",(s32)m_iWorldId,getAngularDampingThreshold());
  #endif
}

f32 CIrrOdeWorld::getAngularDampingThreshold() {
  return m_iWorldId?m_pOdeDevice->worldGetAngularDampingThreshold(m_iWorldId):m_fDampingAngularThreshold;
}

void CIrrOdeWorld::setMaxAngularSpeed(f32 fMaxSpeed) {
  m_fMaxAngularSpeed=fMaxSpeed;
  if (m_iWorldId) m_pOdeDevice->worldSetMaxAngularSpeed(m_iWorldId,fMaxSpeed);
}

f32 CIrrOdeWorld::getMaxAngularSpeed() {
  return m_iWorldId?m_pOdeDevice->worldGetMaxAngularSpeed(m_iWorldId):m_fMaxAngularSpeed;
}

void CIrrOdeWorld::setAutoDisableFlag(int iFlag) {
  m_iAutoDisableFlag=iFlag;
  if (m_iWorldId) m_pOdeDevice->worldSetAutoDisableFlag(m_iWorldId,iFlag);
}

int CIrrOdeWorld::getAutoDisableFlag() {
  return m_iWorldId?m_pOdeDevice->worldGetAutoDisableFlag(m_iWorldId):m_iAutoDisableFlag;
}

void CIrrOdeWorld::setAutoDisableLinearThreshold(f32 fThreshold) {
  m_fAutoDisableLinearThreshold=fThreshold;
  if (m_iWorldId) m_pOdeDevice->worldSetAutoDisableLinearThreshold(m_iWorldId,fThreshold);
}

f32 CIrrOdeWorld::getAutoDisableLinearThreshold() {
  return m_iWorldId?m_pOdeDevice->worldGetAutoDisableLinearThreshold(m_iWorldId):m_fAutoDisableLinearThreshold;
}

void CIrrOdeWorld::setAutoDisableAngularThreshold(f32 fThreshold) {
  m_fAutoDisableAngularThreshold=fThreshold;
  if (m_iWorldId) m_pOdeDevice->worldSetAutoDisableAngularThreshold(m_iWorldId,fThreshold);
}

f32 CIrrOdeWorld::getAutoDisableAngularThreshold() {
  return m_iWorldId?m_pOdeDevice->worldGetAutoDisableAngularThreshold(m_iWorldId):m_fAutoDisableAngularThreshold;
}

void CIrrOdeWorld::setAutoDisableSteps(int iSteps) {
  m_iAutoDisableSteps=iSteps;
  if (m_iWorldId) m_pOdeDevice->worldSetAutoDisableSteps(m_iWorldId,iSteps);
}

int CIrrOdeWorld::getAutoDisableSteps() {
  return m_iWorldId?m_pOdeDevice->worldGetAutoDisableSteps(m_iWorldId):m_iAutoDisableSteps;
}

void CIrrOdeWorld::setAutoDisableTime(f32 fTime) {
  m_fAutoDisableTime=fTime;
  if (m_iWorldId) m_pOdeDevice->worldSetAutoDisableTime(m_iWorldId,fTime);
}

f32 CIrrOdeWorld::getAutoDisableTime() {
  return m_iWorldId?m_pOdeDevice->worldGetAutoDisableTime(m_iWorldId):m_fAutoDisableTime;
}

s32 CIrrOdeWorld::getID() {
  return ID;
}

void CIrrOdeWorld::setStepSize(f32 fStepSize) {
  m_fStepSize=fStepSize;
  m_pOdeDevice->setStepSize(fStepSize);
}

f32 CIrrOdeWorld::getStepSize() {
  return m_fStepSize;
}

void CIrrOdeWorld::setQuickStepNumIterations(s32 iIterations) {
  if (m_iWorldId) m_pOdeDevice->worldSetQuickStepNumIterations(m_iWorldId,iIterations);
}

s32 CIrrOdeWorld::getQuickStepNumIterations() {
  return m_iWorldId?m_pOdeDevice->worldGetQuickStepNumIterations(m_iWorldId):-1;
}

void CIrrOdeWorld::setQuickStepSOR(f32 fSOR) {
  if (m_iWorldId) m_pOdeDevice->worldSetQuickStepW(m_iWorldId,fSOR);
}

f32 CIrrOdeWorld::getQuickStepSOR() {
  return m_iWorldId?m_pOdeDevice->worldGetQuickStepW(m_iWorldId):0.0f;
}

void CIrrOdeWorld::frameUpdate() {
  list<CIrrOdeBody *>::Iterator i;
  for (i=m_pBodies.begin(); i!=m_pBodies.end(); i++) (*i)->frameUpdate();
}

void CIrrOdeWorld::step(f32 fTime) {
  m_pOdeDevice->step(fTime,this);
}

void CIrrOdeWorld::quickStep(f32 fTime) {
  m_pOdeDevice->quickStep(fTime,this);
}

void CIrrOdeWorld::resetCollisionAttributes() {
  list<CIrrOdeBody *>::Iterator it;
  for (it=m_pBodies.begin(); it!=m_pBodies.end(); it++) {
    (*it)->setCollision(false);
    (*it)->setIsTouching(NULL);
    (*it)->setCollisionPoint(vector3df(0.0f,0.0f,0.0f));
    (*it)->setCollisionMaterial(0);
  }
}

void CIrrOdeWorld::render() {
  CIrrOdeSceneNode::render();
}

void CIrrOdeWorld::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

ESCENE_NODE_TYPE CIrrOdeWorld::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID;
}

void CIrrOdeWorld::setGravity(vector3df cGravity) {
  m_cGravity=cGravity;
  if (m_iWorldId) m_pOdeDevice->worldSetGravity(m_iWorldId,m_cGravity);
}

vector3df &CIrrOdeWorld::getGravity() {
  return m_cGravity;
}

u32 CIrrOdeWorld::getJointGroupId() {
  return m_iJointGroupId;
}

list<CIrrOdeBody *> &CIrrOdeWorld::getBodyList() {
  return m_pBodies;
}

const wchar_t *CIrrOdeWorld::getTypeName() {
  return IRR_ODE_WORLD_NAME;
}

CIrrOdeSpace *CIrrOdeWorld::getSpace() {
  return m_pWorldSpace;
}

void CIrrOdeWorld::setSpace(CIrrOdeSpace *pWorldSpace) {
  m_pWorldSpace=pWorldSpace;
}

void CIrrOdeWorld::setContactCalculator(IIrrOdeContactParameters *pCalculator) {
  m_pOdeDevice->setContactCalculator(pCalculator);
}

IIrrOdeContactParameters *CIrrOdeWorld::getContactCalculator() {
  return m_pOdeDevice->getContactCalculator();
}

void CIrrOdeWorld::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeDampable::serializeAttributes(out,options);

  out->addFloat("StepSize",m_pOdeDevice->getStepSize());
  out->addVector3d("Gravity",m_cGravity);
  out->addString("surfaceDefinitions",m_sSurfaceFile.c_str());
  out->addBool("DrawEditorMesh",m_bDrawEditorMesh);
}

void CIrrOdeWorld::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  #ifdef _IRREDIT_PLUGIN
    bool b=m_bDrawEditorMesh;
  #endif
  CIrrOdeDampable::deserializeAttributes(in,options);

  m_fStepSize=in->getAttributeAsFloat("StepSize");
  m_pOdeDevice->setStepSize(m_fStepSize);
  m_cGravity=in->getAttributeAsVector3d("Gravity");
  m_sSurfaceFile=in->getAttributeAsString("surfaceDefinitions");
  if (stringw("")!=m_sSurfaceFile) loadFromFile(m_sSurfaceFile.c_str());
  m_bDrawEditorMesh=in->getAttributeAsBool("DrawEditorMesh");

  #ifdef _IRREDIT_PLUGIN
    if (b!=m_bDrawEditorMesh) {
      list<CIrrOdeSceneNode *>::Iterator it;
      for (it=m_pChildNodes.begin(); it!=m_pChildNodes.end(); it++)
        (*it)->setDrawEditorMesh(m_bDrawEditorMesh);
    }
  #endif
}

ISceneNode *CIrrOdeWorld::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeWorld *pWorld=new CIrrOdeWorld(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pWorld); 
  CIrrOdeSceneNode::cloneChildren(pWorld,newManager);
  return pWorld;
}

void CIrrOdeWorld::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeDampable::copyParams(pDest);
  CIrrOdeWorld *pDst=(CIrrOdeWorld *)pDest;
  m_iWorldId=0;
  m_iJointGroupId=0;
  pDst->setSpace(m_pWorldSpace);
  pDst->setGravity(m_cGravity);

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeWorld.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

void CIrrOdeWorld::addSpace(CIrrOdeSpace *pSpace) {
  #ifdef _TRACE_INIT_PHYSICS
    printf("add space to world\n");
  #endif
  m_pSpaces.push_back(pSpace);
}

void CIrrOdeWorld::removeSpace(CIrrOdeSpace *pSpace) {
}

void CIrrOdeWorld::removeFromPhysics() {
  CIrrOdeSceneNode::removeFromPhysics();
  if (m_iWorldId) {
    m_pOdeDevice->worldDestroy(m_iWorldId);
    m_iWorldId=0;
  }
}

void CIrrOdeWorld::loadParameter(IXMLReader *pReader) {
  while (pReader->read()) {
    switch (pReader->getNodeType()) {
      case io::EXN_ELEMENT_END:
        if (stringw("attributes")==pReader->getNodeName()) {
          return;
        }
        break;

      case io::EXN_ELEMENT:
        if (stringw("attributes")==pReader->getNodeName()) {
          io::IAttributes *pAttr=m_pSceneManager->getGUIEnvironment()->getFileSystem()->createEmptyAttributes(NULL);
          pAttr->read(pReader);
          CIrrOdeSurfaceParameters *p=new CIrrOdeSurfaceParameters();
          CIrrOdeManager::getSharedInstance()->addSurfaceParameter(p);
          p->setStatic(true);
          p->deserializeAttributes(pAttr,NULL,0);
        }
        break;

      default: break;
    }
  }
}

u32 CIrrOdeWorld::loadFromFile(const wchar_t *sName) {
  u32 iRet=0;

  printf("load from file!\n");
  io::IReadFile *pRead=m_pSceneManager->getGUIEnvironment()->getFileSystem()->createAndOpenFile(stringc(stringw(sName)).c_str());

  if (!pRead) {
    printf("unable to open \"%s\"\n",stringc(stringw(sName)).c_str());
    return 0;
  }

  io::IXMLReader *pReader=m_pSceneManager->getGUIEnvironment()->getFileSystem()->createXMLReader(pRead);

  if (!pReader) {
    printf("\"%s\" is an invalid XML file\n",stringc(stringw(sName)).c_str());
    return 0;
  }

  while (pReader->read()) {
    switch (pReader->getNodeType()) {
      case io::EXN_ELEMENT_END:
        break;

      case io::EXN_ELEMENT:
        if (stringw("parameters")==pReader->getNodeName()) {
          printf("parameter!\n");
          iRet++;
          loadParameter(pReader);
        }
        break;

      default: break;
    }
  }

  pReader->drop();
  pRead->drop();

  printf("%i surface parameter sets loaded from \"%s\"\n",iRet,stringc(stringw(sName)).c_str());
  return iRet;
}

u32 CIrrOdeWorld::setSurfaceXml(const wchar_t *sName) {
	m_sSurfaceFile=stringw(sName);
	return loadFromFile(sName);
}

#ifdef _IRREDIT_PLUGIN
  void CIrrOdeWorld::addOdeChildNode(CIrrOdeSceneNode *pNode) {
    list<CIrrOdeSceneNode *>::Iterator it;
    for (it=m_pChildNodes.begin(); it!=m_pChildNodes.end(); it++) if (*it==pNode) return;
    m_pChildNodes.push_back(pNode);
  }

  void CIrrOdeWorld::removeOdeChildNode(CIrrOdeSceneNode *pNode) {
    list<CIrrOdeSceneNode *>::Iterator it;
    for (it=m_pChildNodes.begin(); it!=m_pChildNodes.end(); it++)
      if (*it==pNode) {
        m_pChildNodes.erase(it);
        return;
      }
  }
#endif

void CIrrOdeWorld::setCFM(f32 fCfm) {
  m_pOdeDevice->worldSetCFM(m_iWorldId,fCfm);
}

void CIrrOdeWorld::setERP(f32 fErp) {
  m_pOdeDevice->worldSetERP(m_iWorldId,fErp);
}

f32 CIrrOdeWorld::getCFM() {
  return m_pOdeDevice->worldGetCFM(m_iWorldId);
}

f32 CIrrOdeWorld::getERP() {
  return m_pOdeDevice->worldGetERP(m_iWorldId);
}

} //namespace ode
} //namespace irr
