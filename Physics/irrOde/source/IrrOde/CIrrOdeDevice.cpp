#ifndef _USE_ODE_NULL_DEVICE

  #include <CIrrOdeDevice.h>
  #include <CIrrOdeContactParameters.h>
  #include <joints/CIrrOdeJoint.h>
  #include <CIrrOdeManager.h>
  #include <CIrrOdeSpace.h>
  #include <geom/CIrrOdeGeom.h>
  #include <geom/CIrrOdeGeomRay.h>
  #include <geom/CIrrOdeGeomTrimesh.h>
  #include <event/CIrrOdeEventRayHit.h>
  #include <event/CIrrOdeEventBodyMoved.h>
  #include <event/CIrrOdeEventStep.h>
  #include <event/CIrrOdeEventBeforeStep.h>

  #define GETWORLD(i) m_pOdeData[i-1]->m_pWorld
  #define GETJOINT(i) m_pOdeData[i-1]->m_pJoint
  #define GETBODY(i) m_pOdeData[i-1]->m_pBody
  #define GETGEOM(i) m_pOdeData[i-1]->m_pGeom
  #define GETMASS(i) &m_pOdeData[i-1]->m_pMass
  #define GETDATA(i) m_pOdeData[i-1]->m_pData
  #define GETSPACE(i) m_pOdeData[i-1]->m_pSpace
  #define GETJGROUP(i) m_pOdeData[i-1]->m_pJGroup

  #define VERTICES(i) m_pOdeData[i-1]->m_pVertices
  #define INDICES(i) m_pOdeData[i-1]->m_pIndices
  #define VERTEXCOUNT(i) m_pOdeData[i-1]->m_iVertexCount
  #define INDEXCOUNT(i) m_pOdeData[i-1]->m_iIndexCount

  #define GETHFDATA(i) m_pOdeData[i-1]->m_pHFData

  #define GETDEVICEGEOM(i) s_pOdeDevice->m_pOdeData[i-1]->m_pGeom
  #define GETDEVICEBODY(i) s_pOdeDevice->m_pOdeData[i-1]->m_pBody

  #define VEC2IRR(o,v) v.X=o[0]; v.Y=o[1]; v.Z=o[2];
  #define IRR2VEC(v,o) o[0]=v.X; o[1]=v.Y; o[2]=v.Z;

namespace irr {
namespace ode {

COdeDataWrapper::COdeDataWrapper() {
  m_pWorld =NULL;
  m_pJoint =NULL;
  m_pBody  =NULL;
  m_pGeom  =NULL;
  m_pData  =NULL;
  m_pSpace =NULL;
  m_pHFData=NULL;
  m_pJGroup=NULL;

  m_pVertices=NULL;
  m_pIndices=NULL;

  m_iIndexCount=0;
  m_iVertexCount=0;
}

COdeDataWrapper::~COdeDataWrapper() {
  if (m_pVertices) delete m_pVertices;
  if (m_pIndices ) delete m_pIndices ;
}

static CIrrOdeDevice *s_pOdeDevice=NULL;
static list<CFastCollision *> s_lCollisionQueue;

CIrrOdeDevice::CIrrOdeDevice() : IIrrOdeDevice() {
  m_pContactCalculator=new CIrrOdeContactParameters();
  m_fStepTime=0.0;
  m_fStepSize=0.016f;
  m_bOdeInitialized=false;
  m_iStep=0;
  s_pOdeDevice=this;
}

void CIrrOdeDevice::initODE() {
  dInitODE();
  m_bOdeInitialized=true;
  m_iStep=0;
}

void CIrrOdeDevice::clearODE() {
  for (u32 i=0; i<m_pOdeData.size(); i++) delete m_pOdeData[i];
  m_pOdeData.clear();
}

void CIrrOdeDevice::closeODE() {
  for (u32 i=0; i<m_pOdeData.size(); i++) delete m_pOdeData[i];
  m_pOdeData.clear();

  dCleanupODEAllDataForThread();
  dCloseODE();
  m_bOdeInitialized=false;
}

CIrrOdeDevice::~CIrrOdeDevice() {
  delete m_pContactCalculator;
  while (m_pOdeData.size()>0) {
    delete m_pOdeData[0];
    m_pOdeData.erase(0);
  }
}

dWorldID CIrrOdeDevice::getWorldId(int iWorld) {
  return GETWORLD(iWorld);
}

int CIrrOdeDevice::convertToOdeJointParam(u16 iGroup, s32 &iParam) {
  int iOdeParam=-1;
  if (iGroup==0)
    switch (iParam) {
      case CIrrOdeJoint::eHiStop       : iOdeParam=dParamHiStop       ; break;
      case CIrrOdeJoint::eLoStop       : iOdeParam=dParamLoStop       ; break;
      case CIrrOdeJoint::eVel          : iOdeParam=dParamVel          ; break;
      case CIrrOdeJoint::eMaxForce     : iOdeParam=dParamFMax         ; break;
      case CIrrOdeJoint::eFudge        : iOdeParam=dParamFudgeFactor  ; break;
      case CIrrOdeJoint::eBounce       : iOdeParam=dParamBounce       ; break;
      case CIrrOdeJoint::eCFM          : iOdeParam=dParamCFM          ; break;
      case CIrrOdeJoint::eStopERP      : iOdeParam=dParamStopERP      ; break;
      case CIrrOdeJoint::eStopCFM      : iOdeParam=dParamStopCFM      ; break;
      case CIrrOdeJoint::eSuspensionERP: iOdeParam=dParamSuspensionERP; break;
      case CIrrOdeJoint::eSuspensionCFM: iOdeParam=dParamSuspensionCFM; break;
      default: iOdeParam=-1;
  }

  if (iGroup==1)
    switch (iParam) {
      case CIrrOdeJoint::eHiStop       : iOdeParam=dParamHiStop2       ; break;
      case CIrrOdeJoint::eLoStop       : iOdeParam=dParamLoStop2       ; break;
      case CIrrOdeJoint::eVel          : iOdeParam=dParamVel2          ; break;
      case CIrrOdeJoint::eMaxForce     : iOdeParam=dParamFMax2         ; break;
      case CIrrOdeJoint::eFudge        : iOdeParam=dParamFudgeFactor2  ; break;
      case CIrrOdeJoint::eBounce       : iOdeParam=dParamBounce2       ; break;
      case CIrrOdeJoint::eCFM          : iOdeParam=dParamCFM2          ; break;
      case CIrrOdeJoint::eStopERP      : iOdeParam=dParamStopERP2      ; break;
      case CIrrOdeJoint::eStopCFM      : iOdeParam=dParamStopCFM2      ; break;
      case CIrrOdeJoint::eSuspensionERP: iOdeParam=dParamSuspensionERP2; break;
      case CIrrOdeJoint::eSuspensionCFM: iOdeParam=dParamSuspensionCFM2; break;
      default: iOdeParam=-1;
  }

  if (iGroup==2)
    switch (iParam) {
      case CIrrOdeJoint::eHiStop       : iOdeParam=dParamHiStop3       ; break;
      case CIrrOdeJoint::eLoStop       : iOdeParam=dParamLoStop3       ; break;
      case CIrrOdeJoint::eVel          : iOdeParam=dParamVel3          ; break;
      case CIrrOdeJoint::eMaxForce     : iOdeParam=dParamFMax3         ; break;
      case CIrrOdeJoint::eFudge        : iOdeParam=dParamFudgeFactor3  ; break;
      case CIrrOdeJoint::eBounce       : iOdeParam=dParamBounce3       ; break;
      case CIrrOdeJoint::eCFM          : iOdeParam=dParamCFM3          ; break;
      case CIrrOdeJoint::eStopERP      : iOdeParam=dParamStopERP3      ; break;
      case CIrrOdeJoint::eStopCFM      : iOdeParam=dParamStopCFM3      ; break;
      case CIrrOdeJoint::eSuspensionERP: iOdeParam=dParamSuspensionERP3; break;
      case CIrrOdeJoint::eSuspensionCFM: iOdeParam=dParamSuspensionCFM3; break;
      default: iOdeParam=-1;
  }

  return iOdeParam;
}

void CIrrOdeDevice::handleFastCollisions() {
  while (s_lCollisionQueue.getSize()>0) {
    list<CFastCollision *>::Iterator it=s_lCollisionQueue.begin();
    CFastCollision *p=*it;
    s_lCollisionQueue.erase(it);

    p->m_pBody->setPosition(p->m_cPos);
    CIrrOdeGeom *pGeom=p->m_pBody->getFirstGeom();

    if (pGeom!=NULL)
      nearCollisionCallback(p->m_pBody->getWorld(),GETGEOM(pGeom->getGeomId()),GETGEOM(p->m_pGeom->getGeomId()));

    delete p;
  }
}


void CIrrOdeDevice::nearCollisionCallback(void *pData, dGeomID iGeom1, dGeomID iGeom2) {
  s32 iSurfaceIdx[2]={0,0};

  if (dGeomIsSpace(iGeom1) || dGeomIsSpace(iGeom2)) {
    dSpaceCollide2(iGeom1,iGeom2,pData,&CIrrOdeDevice::nearCollisionCallback);

    if (dGeomIsSpace(iGeom1)) dSpaceCollide((dxSpace *)iGeom1,pData,&CIrrOdeDevice::nearCollisionCallback);
    if (dGeomIsSpace(iGeom2)) dSpaceCollide((dxSpace *)iGeom2,pData,&CIrrOdeDevice::nearCollisionCallback);
  }
  else {
    static CIrrOdeDevice *pOdeDevice=NULL;
    if (!pOdeDevice) pOdeDevice=(CIrrOdeDevice *)CIrrOdeManager::getSharedInstance()->getOdeDevice();
    u32 i=0;

    dBodyID b1=dGeomGetBody(iGeom1);
    dBodyID b2=dGeomGetBody(iGeom2);

    CIrrOdeWorld *pWorld=(CIrrOdeWorld *)pData;

    if (b1 &&  b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact)) return;

    CIrrOdeGeom *pGeom1=(CIrrOdeGeom *)dGeomGetData(iGeom1),
                *pGeom2=(CIrrOdeGeom *)dGeomGetData(iGeom2);

    if (pGeom1->getType()==IRR_ODE_GEOM_RAY_ID) {
      handleRayEvent(iGeom1,iGeom2);
      return;
    }

    if (pGeom2->getType()==IRR_ODE_GEOM_RAY_ID) {
      handleRayEvent(iGeom2,iGeom1);
      return;
    }

    if (!b1 && !b2) return;

    dContact contact[MAX_CONTACTS];
    CIrrOdeSurfaceParameters pParam,*p1=pGeom1->getSurfaceParameters(0),*p2=pGeom2->getSurfaceParameters(0);

    if (pGeom1->getType()==IRR_ODE_GEOM_TRIMESH_ID) {
      CIrrOdeGeomTrimesh *tri=(CIrrOdeGeomTrimesh *)pGeom1;
      for (u32 i=0; pOdeDevice->getData(i)!=NULL; i++) {
        if (pOdeDevice->getData(i)->m_pGeom==iGeom1) {
          iSurfaceIdx[0]=tri->getIndexOfGeom(i+1); //search for i+1 because counting the wrapped data starts at 1
          if (iSurfaceIdx[0]!=-1) {
            p1=tri->getSurfaceParameters(iSurfaceIdx[0]);
          }
        }
      }
    }
    else p1=pGeom1->getSurfaceParameters(0);

    if (pGeom2->getType()==IRR_ODE_GEOM_TRIMESH_ID) {
      CIrrOdeGeomTrimesh *tri=(CIrrOdeGeomTrimesh *)pGeom2;
      for (u32 i=0; pOdeDevice->getData(i)!=NULL; i++) {
        if (pOdeDevice->getData(i)->m_pGeom==iGeom2) {
          iSurfaceIdx[1]=tri->getIndexOfGeom(i+1); //search for i+1 because counting the wrapped data starts at 1
          if (iSurfaceIdx[1]!=0) {
            p2=tri->getSurfaceParameters(iSurfaceIdx[1]);
          }
        }
      }
    }
    else p2=pGeom2->getSurfaceParameters(0);

    for(i=0; i<MAX_CONTACTS; i++) {
      pGeom1->getWorld()->getContactCalculator()->calculateContactParameters(p1,p2,&pParam);
      convertToSurfaceParameters(&(contact[i].surface),&pParam);
    }

    u32 numc=dCollide(iGeom1,iGeom2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact));

    if(numc>0) {
      CIrrOdeGeom *pGeom1=(CIrrOdeGeom *)dGeomGetData(iGeom1),*pGeom2=(CIrrOdeGeom *)dGeomGetData(iGeom2);

      if (b1) {
        CIrrOdeBody *pBody1=(CIrrOdeBody *)dBodyGetData(b1);
        pBody1->setCollision(true);
        pBody1->setIsTouching(pGeom2);
        pBody1->setCollisionMaterial(iSurfaceIdx[1]);

        vector3df point=vector3df(contact[0].geom.pos[0],contact[0].geom.pos[1],contact[0].geom.pos[2]);
        pBody1->setCollisionPoint(point);

        if (!b2 && p2->dampsObjects()) {
          vector3df oldVL=pBody1->getLinearVelocity() ,
                    oldVA=pBody1->getAngularVelocity();

          oldVL*=(1-p2->getLinearDamping ());
          oldVA*=(1-p2->getAngularDamping());

          if (p2->getMaxLinearVelocity()>0.0f && oldVL.getLength()>p2->getMaxLinearVelocity()) {
            f32 fact=1.0f-p2->getMaxLinearVelocityDamping(),v=(oldVL.getLength())*fact;
            if (v>=p2->getMaxLinearVelocity())
              oldVL=oldVL*fact;
            else {
              oldVL=oldVL.normalize()*p2->getMaxLinearVelocity();
            }
          }

          if (p2->getMaxAngularVelocity()>0.0f && oldVA.getLength()>p2->getMaxAngularVelocity()) {
            f32 fact=1.0f-p2->getMaxAngularVelocityDamping(),v=(oldVA.getLength())*fact;
            if (v>=p2->getMaxAngularVelocity())
              oldVA=oldVA*fact;
            else {
              oldVA=oldVA.normalize()*p2->getMaxAngularVelocity();
            }
          }

          pBody1->setLinearVelocity (oldVL);
          pBody1->setAngularVelocity(oldVA);
        }
      }

      if (b2) {
        CIrrOdeBody *pBody2=(CIrrOdeBody *)dBodyGetData(b2);
        pBody2->setCollision(true);
        pBody2->setIsTouching(pGeom1);
        pBody2->setCollisionMaterial(iSurfaceIdx[0]);

        vector3df point=vector3df(contact[0].geom.pos[0],contact[0].geom.pos[1],contact[0].geom.pos[2]);
        pBody2->setCollisionPoint(point);

        if (!b1 && p1->dampsObjects()) {
          vector3df oldVL=pBody2->getLinearVelocity() ,
                    oldVA=pBody2->getAngularVelocity();

          oldVL*=(1-p1->getLinearDamping ());
          oldVA*=(1-p1->getAngularDamping());

          if (p1->getMaxLinearVelocity()>0.0f && oldVL.getLength()>p1->getMaxLinearVelocity()) {
            f32 fact=1.0f-p1->getMaxLinearVelocityDamping(),v=(oldVL.getLength())*fact;
            if (v>=p1->getMaxLinearVelocity())
              oldVL=oldVL*fact;
            else {
              oldVL=oldVL.normalize()*p1->getMaxLinearVelocity();
            }
          }

          if (p1->getMaxAngularVelocity()>0.0f && oldVA.getLength()>p1->getMaxAngularVelocity()) {
            f32 fact=1.0f-p1->getMaxAngularVelocityDamping(),v=(oldVA.getLength())*fact;
            if (v>=p1->getMaxAngularVelocity())
              oldVA=oldVA*fact;
            else {
              oldVA=oldVA.normalize()*p1->getMaxAngularVelocity();
            }
          }

          pBody2->setLinearVelocity (oldVL);
          pBody2->setAngularVelocity(oldVA);
        }
      }

      dJointID pJoints[MAX_CONTACTS];

      CIrrOdeDevice *pDevice=(CIrrOdeDevice *)CIrrOdeManager::getSharedInstance()->getOdeDevice();

      for(i=0; i<numc; i++) {
        pJoints[i]=dJointCreateContact(pDevice->getWorldId(pWorld->getWorldId()),pOdeDevice->getJGroupId(pWorld->getJointGroupId()),&contact[i]);
        dJointAttach(pJoints[i],b1,b2);
      }
    }
  }
}

void CIrrOdeDevice::handleRayEvent(dGeomID theRay, dGeomID theGeom) {
  dContactGeom contact;
  if (dCollide(theRay,theGeom,1,&contact,sizeof(dContactGeom))) {
    CIrrOdeBody *pBody=dGeomGetBody(theGeom)?(CIrrOdeBody *)dBodyGetData(dGeomGetBody(theGeom)):NULL;
    CIrrOdeGeom *pGeom=(CIrrOdeGeom *)dGeomGetData(theGeom);
    CIrrOdeGeomRay *pRay=(CIrrOdeGeomRay *)dGeomGetData(theRay);
    vector3df pos(contact.pos[0],contact.pos[1],contact.pos[2]);

    if (pBody!=pRay->getAttachedBody() && pBody!=NULL && pRay->getAttachedBody()!=NULL) {
      CIrrOdeBody *pTouch=pBody->getTouched()?pBody->getTouched()->getBody():NULL;
      if (pTouch!=pRay->getAttachedBody()) {
        CFastCollision *pCol=new CFastCollision;
        pCol->m_pBody=pRay->getAttachedBody();
        pCol->m_pGeom=pGeom;
        pCol->m_cPos=pos;
        s_lCollisionQueue.push_back(pCol);
      }
    }

    CIrrOdeEventRayHit cEvent(pBody,pGeom,pRay,pos,contact.depth);
    CIrrOdeManager::getSharedInstance()->postEvent(&cEvent);
  }
}

dGeomID CIrrOdeDevice::getGeomId(u32 iGeom) {
  return GETGEOM(iGeom);
}

dJointGroupID CIrrOdeDevice::getJGroupId(u32 iJGroup) {
  return GETJGROUP(iJGroup);
}

/**
 * The body moved callback. This method is called by ODE every time the body moved and is used to update
 * the body's Irrlicht position and rotation to the values calculated by ODE.
 * @param iMovedBody the ODE body ID of the moved body
 */
void CIrrOdeDevice::bodyMovedCallback(dBodyID iMovedBody) {
  static CIrrOdeDevice *pOdeDevice=NULL;
  if (!pOdeDevice) pOdeDevice=(CIrrOdeDevice *)CIrrOdeManager::getSharedInstance()->getOdeDevice();

  if (iMovedBody) {
    CIrrOdeBody *theBody=(CIrrOdeBody *)dBodyGetData(iMovedBody);

    irr::core::vector3df pos;
    irr::core::vector3df rot;

    CIrrOdeGeom *theGeom=theBody->getFirstGeom();

    while (theGeom) {
      if(theGeom!=0){
        // get the new position of the ODE geom
        dReal* ode_pos=(dReal*)dGeomGetPosition(pOdeDevice->getGeomId(theGeom->getGeomId()));
        // set the position of the scenenode
        pos.set((irr::f32)ode_pos[0],(irr::f32)ode_pos[1],(irr::f32)ode_pos[2]);

        // get the rotation quaternion
        dQuaternion result;
        dGeomGetQuaternion(pOdeDevice->getGeomId(theGeom->getGeomId()), result);
        // convert it to eulerangles
        quaternionToEuler(result,rot);
      }
      theGeom=theBody->getNextGeom(theGeom);
    }

    theBody->updateAbsolutePosition();
    theBody->bodyMoved(pos);

    const dReal *vLin=dBodyGetLinearVel (iMovedBody);
    const dReal *vAng=dBodyGetAngularVel(iMovedBody);

    vector3df vl=vector3df(vLin[0],vLin[1],vLin[2]);
    vector3df va=vector3df(vAng[0],vAng[1],vAng[2]);

    CIrrOdeEventBodyMoved cEvent(theBody,pos,rot,vl,va);
    CIrrOdeManager::getSharedInstance()->postEvent(&cEvent);
  }
}

/**
 * Convert quaternion to euler angles
 * @param quaternion the quaternion to be converted
 * @param euler the resulting euler angle vector
 */
void CIrrOdeDevice::quaternionToEuler(const dQuaternion quaternion, vector3df &euler){
  dReal w,x,y,z;
  w=quaternion[0];
  x=quaternion[1];
  y=quaternion[2];
  z=quaternion[3];
  double sqw=w*w;
  double sqx=x*x;
  double sqy=y*y;
  double sqz=z*z;

  euler.Z=(irr::f32)(atan2( 2.0*(x*y+z*w),( sqx-sqy-sqz+sqw))*GRAD_PI);
  euler.X=(irr::f32)(atan2( 2.0*(y*z+x*w),(-sqx-sqy+sqz+sqw))*GRAD_PI);
  euler.Y=(irr::f32)(asin (-2.0*(x*z-y*w))*GRAD_PI);
}

/**
 * Convert euler angles to quaternion
 * @param v the euler angles vector to be converted
 * @param q the resulting quaternion
 */
void CIrrOdeDevice::eulerToQuaternion (const vector3df v, dQuaternion q) {
  float heading=v.Z*GRAD_PI2/2.0f;
  float attitude=v.Y*GRAD_PI2/2.0f;
  float bank=v.X*GRAD_PI2/2.0f;
  float c1=cos(heading);
  float s1=sin(heading);
  float c2=cos(attitude);
  float s2=sin(attitude);
  float c3=cos(bank);
  float s3=sin(bank);

  q[0]=(dReal)(c1*c2*c3+s1*s2*s3); //w
  q[1]=(dReal)(c1*c2*s3-s1*s2*c3); //x
  q[2]=(dReal)(c1*s2*c3+s1*c2*s3); //y
  q[3]=(dReal)(s1*c2*c3-c1*s2*s3); //z
}

void CIrrOdeDevice::setStepSize(f32 f) {
  m_fStepSize=f;
}

f32 CIrrOdeDevice::getStepSize() {
  return m_fStepSize;
}

void CIrrOdeDevice::step(f32 fTime, CIrrOdeWorld *pWorld, u32 iJointGroupId) {
  m_fStepTime+=fTime;
  while (m_fStepTime>0.0f) {
    CIrrOdeEventBeforeStep cBeforeStepEvent;
    CIrrOdeManager::getSharedInstance()->postEvent(&cBeforeStepEvent);
    dSpaceCollide(GETSPACE(pWorld->getSpace()->getSpaceId()),pWorld,&CIrrOdeDevice::nearCollisionCallback);
    dWorldStep(GETWORLD(pWorld->getWorldId()),m_fStepSize);
    dJointGroupEmpty(GETJGROUP(iJointGroupId));
    handleFastCollisions();
    m_fStepTime-=m_fStepSize;
    CIrrOdeEventStep cStepEvent(m_iStep++);
    cStepEvent.setFrameNo(CIrrOdeManager::getSharedInstance()->getFrameNo());
    CIrrOdeManager::getSharedInstance()->postEvent(&cStepEvent);
    pWorld->frameUpdate();
  }
}

void CIrrOdeDevice::quickStep(f32 fTime, CIrrOdeWorld *pWorld, u32 iJointGroupId) {
  m_fStepSize+=fTime;
  while (m_fStepTime>0.0f) {
    CIrrOdeEventBeforeStep cBeforeStepEvent;
    CIrrOdeManager::getSharedInstance()->postEvent(&cBeforeStepEvent);
    dSpaceCollide(GETSPACE(pWorld->getSpace()->getSpaceId()),pWorld,&CIrrOdeDevice::nearCollisionCallback);
    dWorldQuickStep(GETWORLD(pWorld->getWorldId()),m_fStepSize);
    dJointGroupEmpty(GETJGROUP(iJointGroupId));
    m_fStepTime-=m_fStepSize;
    CIrrOdeEventStep cStepEvent(m_iStep++);
    cStepEvent.setFrameNo(CIrrOdeManager::getSharedInstance()->getFrameNo());
    CIrrOdeManager::getSharedInstance()->postEvent(&cStepEvent);
    pWorld->frameUpdate();
  }
}

void CIrrOdeDevice::setContactCalculator(IIrrOdeContactParameters *pCalculator) {
  if (m_pContactCalculator) delete m_pContactCalculator;
  m_pContactCalculator=pCalculator;
}

IIrrOdeContactParameters *CIrrOdeDevice::getContactCalculator() {
  return m_pContactCalculator;
}

u32 CIrrOdeDevice::worldCreate() {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pWorld=dWorldCreate();
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

void CIrrOdeDevice::worldDestroy(u32 iIdx) {
  dWorldDestroy(GETWORLD(iIdx));
  GETWORLD(iIdx)=0;
}

void CIrrOdeDevice::worldSetLinearDamping(u32 iWorld, f32 fScale) {
  dWorldSetLinearDamping(GETWORLD(iWorld),fScale);
}

f32 CIrrOdeDevice::worldGetLinearDamping(u32 iWorld) {
  return dWorldGetLinearDamping(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetAngularDamping(u32 iWorld, f32 fScale) {
  dWorldSetAngularDamping(GETWORLD(iWorld),fScale);
}

f32 CIrrOdeDevice::worldGetAngularDamping(u32 iWorld) {
  return dWorldGetAngularDamping(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetLinearDampingThreshold(u32 iWorld, f32 fScale) {
  dWorldSetLinearDampingThreshold(GETWORLD(iWorld),fScale);
}

f32 CIrrOdeDevice::worldGetLinearDampingThreshold(u32 iWorld) {
  return dWorldGetLinearDampingThreshold(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetAngularDampingThreshold(u32 iWorld, f32 fScale) {
  dWorldSetAngularDampingThreshold(GETWORLD(iWorld),fScale);
}

f32 CIrrOdeDevice::worldGetAngularDampingThreshold(u32 iWorld) {
  return dWorldGetAngularDampingThreshold(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetMaxAngularSpeed(u32 iWorld, f32 fMax) {
  dWorldSetMaxAngularSpeed(GETWORLD(iWorld),fMax);
}

f32 CIrrOdeDevice::worldGetMaxAngularSpeed(u32 iWorld) {
  return dWorldGetMaxAngularSpeed(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetAutoDisableFlag(u32 iWorld, s32 iFlag) {
  dWorldSetAutoDisableFlag(GETWORLD(iWorld),iFlag);
}

s32 CIrrOdeDevice::worldGetAutoDisableFlag(u32 iWorld) {
  return dWorldGetAutoDisableFlag(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetAutoDisableLinearThreshold(u32 iWorld, f32 f) {
  dWorldSetAutoDisableLinearThreshold(GETWORLD(iWorld),f);
}

f32 CIrrOdeDevice::worldGetAutoDisableLinearThreshold(u32 iWorld) {
  return dWorldGetAutoDisableLinearThreshold(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetAutoDisableAngularThreshold(u32 iWorld, f32 f) {
  dWorldSetAutoDisableAngularThreshold(GETWORLD(iWorld),f);
}

f32 CIrrOdeDevice::worldGetAutoDisableAngularThreshold(u32 iWorld) {
  return dWorldGetAutoDisableAngularThreshold(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetAutoDisableSteps(u32 iWorld, s32 i) {
  dWorldSetAutoDisableSteps(GETWORLD(iWorld),i);
}

s32 CIrrOdeDevice::worldGetAutoDisableSteps(u32 iWorld) {
  return dWorldGetAutoDisableSteps(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetAutoDisableTime(u32 iWorld, f32 f) {
  dWorldSetAutoDisableTime(GETWORLD(iWorld),f);
}

f32 CIrrOdeDevice::worldGetAutoDisableTime(u32 iWorld) {
  return dWorldGetAutoDisableTime(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetQuickStepNumIterations(u32 iWorld, s32 i) {
  dWorldSetQuickStepNumIterations(GETWORLD(iWorld),i);
}

s32 CIrrOdeDevice::worldGetQuickStepNumIterations(u32 iWorld) {
  return dWorldGetQuickStepNumIterations(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetQuickStepW(u32 iWorld, f32 f) {
  dWorldSetQuickStepW(GETWORLD(iWorld),f);
}

f32 CIrrOdeDevice::worldGetQuickStepW(u32 iWorld) {
  return dWorldGetQuickStepW(GETWORLD(iWorld));
}

void CIrrOdeDevice::worldSetGravity(u32 iWorld, vector3df cGravity) {
  dWorldSetGravity(GETWORLD(iWorld),cGravity.X,cGravity.Y,cGravity.Z);
}

u32 CIrrOdeDevice::jointCreateBallSocket(u32 iWorld) {
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pJoint=dJointCreateBall(GETWORLD(iWorld),0);
  m_pOdeData.push_back(wrap);

  return m_pOdeData.size();
}

u32 CIrrOdeDevice::jointCreateFixed(u32 iWorld) {
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pJoint=dJointCreateFixed(GETWORLD(iWorld),0);
  m_pOdeData.push_back(wrap);

  return m_pOdeData.size();
}

u32 CIrrOdeDevice::jointCreateHinge(u32 iWorld) {
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pJoint=dJointCreateHinge(GETWORLD(iWorld),0);
  m_pOdeData.push_back(wrap);

  return m_pOdeData.size();
}

u32 CIrrOdeDevice::jointCreateHinge2(u32 iWorld) {
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pJoint=dJointCreateHinge2(GETWORLD(iWorld),0);
  m_pOdeData.push_back(wrap);

  return m_pOdeData.size();
}

u32 CIrrOdeDevice::jointCreatePiston(u32 iWorld) {
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pJoint=dJointCreatePiston(GETWORLD(iWorld),0);
  m_pOdeData.push_back(wrap);

  return m_pOdeData.size();
}

u32 CIrrOdeDevice::jointCreateSlider(u32 iWorld) {
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pJoint=dJointCreateSlider(GETWORLD(iWorld),0);
  m_pOdeData.push_back(wrap);

  return m_pOdeData.size();
}

u32 CIrrOdeDevice::jointCreateUniversal(u32 iWorld) {
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pJoint=dJointCreateUniversal(GETWORLD(iWorld),0);
  m_pOdeData.push_back(wrap);

  return m_pOdeData.size();
}

void CIrrOdeDevice::jointAttach(u32 iJoint, CIrrOdeBody *b1, CIrrOdeBody *b2) {
  dBodyID id1=b1?GETBODY(b1->getBodyId()):0,id2=b2?GETBODY(b2->getBodyId()):0;
  dJointAttach(GETJOINT(iJoint),id1,id2);
}

void CIrrOdeDevice::jointSetData(u32 iJoint, void *pData) {
  dJointSetData(GETJOINT(iJoint),pData);
}

void CIrrOdeDevice::jointDestroy(u32 iJoint) {
  dJointDestroy(GETJOINT(iJoint));
  GETJOINT(iJoint)=0;
}

void CIrrOdeDevice::jointSetBallAnchor(u32 iJoint, vector3df pos) {
  dJointSetBallAnchor(GETJOINT(iJoint),pos.X,pos.Y,pos.Z);
}

vector3df CIrrOdeDevice::jointGetBallAnchor(u32 iJoint) {
  static vector3df v;
  dVector3 result;
  dJointGetBallAnchor(GETJOINT(iJoint),result);
  v.X=result[0];
  v.Y=result[1];
  v.Z=result[2];
  return v;
}

vector3df CIrrOdeDevice::jointGetBallAnchor2(u32 iJoint) {
  static vector3df v;
  dVector3 result;
  dJointGetBallAnchor2(GETJOINT(iJoint),result);
  v.X=result[0];
  v.Y=result[1];
  v.Z=result[2];
  return v;
}

void CIrrOdeDevice::jointSetFixed(u32 iJoint) {
  dJointSetFixed(GETJOINT(iJoint));
}

void CIrrOdeDevice::jointSetHingeAnchor(u32 iJoint, vector3df p) {
  dJointSetHingeAnchor(GETJOINT(iJoint),p.X,p.Y,p.Z);
}

void CIrrOdeDevice::jointSetHingeAxis(u32 iJoint, vector3df a) {
  dJointSetHingeAxis(GETJOINT(iJoint),a.X,a.Y,a.Z);
}

vector3df CIrrOdeDevice::jointGetHingeAxis(u32 iJoint) {
  static vector3df v;
  dVector3 result;
  dJointGetHingeAxis(GETJOINT(iJoint),result);
  VEC2IRR(result,v);
  return v;
}

vector3df CIrrOdeDevice::jointGetHingeAnchor1(u32 iJoint) {
  static vector3df v;
  dVector3 result;
  dJointGetHingeAnchor(GETJOINT(iJoint),result);
  VEC2IRR(result,v);
  return v;
}

vector3df CIrrOdeDevice::jointGetHingeAnchor2(u32 iJoint) {
  static vector3df v;
  dVector3 result;
  dJointGetHingeAnchor2(GETJOINT(iJoint),result);
  VEC2IRR(result,v);
  return v;
}

f32 CIrrOdeDevice::jointGetHingeAngle(u32 iJoint) {
  return dJointGetHingeAngle(GETJOINT(iJoint));
}

f32 CIrrOdeDevice::jointGetHingeAngleRate(u32 iJoint) {
  return dJointGetHingeAngleRate(GETJOINT(iJoint));
}

void CIrrOdeDevice::jointSetHingeParam(u32 iJoint, s32 iParam, f32 iValue) {
  dJointSetHingeParam(GETJOINT(iJoint),convertToOdeJointParam(0,iParam),iValue);
}

f32 CIrrOdeDevice::jointGetHingeParam(u32 iJoint, s32 iParam) {
  return dJointGetHingeParam(GETJOINT(iJoint),convertToOdeJointParam(0,iParam));
}

void CIrrOdeDevice::jointSetHinge2Anchor(u32 iJoint, vector3df a) {
  dJointSetHinge2Anchor(GETJOINT(iJoint),a.X,a.Y,a.Z);
}

void CIrrOdeDevice::jointSetHinge2Axis1(u32 iJoint, vector3df a) {
  dJointSetHinge2Axis1(GETJOINT(iJoint),a.X,a.Y,a.Z);
}

void CIrrOdeDevice::jointSetHinge2Axis2(u32 iJoint, vector3df a) {
  dJointSetHinge2Axis2(GETJOINT(iJoint),a.X,a.Y,a.Z);
}

vector3df CIrrOdeDevice::jointGetHinge2Axis1(u32 iJoint) {
  static vector3df result;
  dVector3 v;
  dJointGetHinge2Axis1(GETJOINT(iJoint),v);
  VEC2IRR(v,result);
  return result;
}

vector3df CIrrOdeDevice::jointGetHinge2Axis2(u32 iJoint) {
  static vector3df result;
  dVector3 v;
  dJointGetHinge2Axis2(GETJOINT(iJoint),v);
  VEC2IRR(v,result);
  return result;
}

vector3df CIrrOdeDevice::jointGetHinge2Anchor(u32 iJoint) {
  static vector3df result;
  dVector3 v;
  dJointGetHinge2Anchor(GETJOINT(iJoint),v);
  VEC2IRR(v,result);
  return result;
}

vector3df CIrrOdeDevice::jointGetHinge2Anchor2(u32 iJoint) {
  static vector3df result;
  dVector3 v;
  dJointGetHinge2Anchor2(GETJOINT(iJoint),v);
  VEC2IRR(v,result);
  return result;
}

f32 CIrrOdeDevice::jointGetHinge2Angle1(u32 iJoint) {
  return dJointGetHinge2Angle1(GETJOINT(iJoint));
}

f32 CIrrOdeDevice::jointGetHinge2Angle1Rate(u32 iJoint) {
  return dJointGetHinge2Angle1Rate(GETJOINT(iJoint));
}

f32 CIrrOdeDevice::jointGetHinge2Angle2Rate(u32 iJoint) {
  return dJointGetHinge2Angle2Rate(GETJOINT(iJoint));
}

void CIrrOdeDevice::jointSetHinge2Param(u32 iJoint, u32 iGroup, s32 iParam, f32 iValue) {
  dJointSetHinge2Param(GETJOINT(iJoint),convertToOdeJointParam(iGroup,iParam),iValue);
}

f32 CIrrOdeDevice::jointGetHinge2Param(u32 iJoint, u32 iGroup, s32 iParam) {
  return dJointGetHinge2Param(GETJOINT(iJoint),convertToOdeJointParam(iGroup,iParam));
}

void CIrrOdeDevice::jointSetPistonAnchor(u32 iJoint, vector3df a) {
  dJointSetPistonAnchor(GETJOINT(iJoint),a.X,a.Y,a.Z);
}

void CIrrOdeDevice::jointSetPistonAxis(u32 iJoint, vector3df a) {
  dJointSetPistonAxis(GETJOINT(iJoint),a.X,a.Y,a.Z);
}

f32 CIrrOdeDevice::jointGetPistonPosition(u32 iJoint) {
  return dJointGetPistonPosition(GETJOINT(iJoint));
}

f32 CIrrOdeDevice::jointGetPistonPositionRate(u32 iJoint) {
  return dJointGetPistonPositionRate(GETJOINT(iJoint));
}

void CIrrOdeDevice::jointSetPistonParam(u32 iJoint, u32 iGroup, s32 iParam, f32 fValue) {
  dJointSetPistonParam(GETJOINT(iJoint),convertToOdeJointParam(iGroup,iParam),fValue);
}

f32 CIrrOdeDevice::jointGetPistonParam(u32 iJoint, u32 iGroup, s32 iParam) {
  return dJointGetPistonParam(GETJOINT(iJoint),convertToOdeJointParam(iGroup,iParam));
}

vector3df CIrrOdeDevice::jointGetPistonAnchor(u32 iJoint) {
  static vector3df result;
  dVector3 v;
  dJointGetPistonAnchor(GETJOINT(iJoint),v);
  VEC2IRR(v,result);
  return result;
}

vector3df CIrrOdeDevice::jointGetPistonAxis(u32 iJoint) {
  static vector3df result;
  dVector3 v;
  dJointGetPistonAxis(GETJOINT(iJoint),v);
  VEC2IRR(v,result);
  return result;
}

void CIrrOdeDevice::jointSetSliderAxis(u32 iJoint, vector3df a) {
  dJointSetSliderAxis(GETJOINT(iJoint),a.X,a.Y,a.Z);
}

vector3df CIrrOdeDevice::jointGetSliderAxis(u32 iJoint) {
  static vector3df result;
  dVector3 v;
  dJointGetSliderAxis(GETJOINT(iJoint),v);
  VEC2IRR(v,result);
  return result;
}

f32 CIrrOdeDevice::jointGetSliderPosition(u32 iJoint) {
  return dJointGetSliderPosition(GETJOINT(iJoint));
}

f32 CIrrOdeDevice::jointGetSliderPositionRate(u32 iJoint) {
  return dJointGetSliderPositionRate(GETJOINT(iJoint));
}

void CIrrOdeDevice::jointSetSliderParam(u32 iJoint, s32 iParam, f32 fValue) {
  dJointSetSliderParam(GETJOINT(iJoint),convertToOdeJointParam(0,iParam),fValue);
}

f32 CIrrOdeDevice::jointGetSliderParam(u32 iJoint, s32 iParam) {
  return dJointGetSliderParam(GETJOINT(iJoint),convertToOdeJointParam(0,iParam));
}

void CIrrOdeDevice::jointSetUniversalAnchor(u32 iJoint, vector3df a) {
  dJointSetUniversalAnchor(GETJOINT(iJoint),a.X,a.Y,a.Z);
}

void CIrrOdeDevice::jointSetUniversalAxis1(u32 iJoint, vector3df a) {
  dJointSetUniversalAxis1(GETJOINT(iJoint),a.X,a.Y,a.Z);
}

void CIrrOdeDevice::jointSetUniversalAxis2(u32 iJoint, vector3df a) {
  dJointSetUniversalAxis2(GETJOINT(iJoint),a.X,a.Y,a.Z);
}

vector3df CIrrOdeDevice::jointGetUniversalAnchor(u32 iJoint) {
  static vector3df result;
  dVector3 v;
  dJointGetUniversalAnchor(GETJOINT(iJoint),v);
  VEC2IRR(v,result);
  return result;
}

vector3df CIrrOdeDevice::jointGetUniversalAnchor2(u32 iJoint) {
  static vector3df result;
  dVector3 v;
  dJointGetUniversalAnchor2(GETJOINT(iJoint),v);
  VEC2IRR(v,result);
  return result;
}

vector3df CIrrOdeDevice::jointGetUniversalAxis1(u32 iJoint) {
  static vector3df result;
  dVector3 v;
  dJointGetUniversalAxis1(GETJOINT(iJoint),v);
  VEC2IRR(v,result);
  return result;
}

vector3df CIrrOdeDevice::jointGetUniversalAxis2(u32 iJoint) {
  static vector3df result;
  dVector3 v;
  dJointGetUniversalAxis2(GETJOINT(iJoint),v);
  VEC2IRR(v,result);
  return result;
}

f32 CIrrOdeDevice::jointGetUniversalAngle1(u32 iJoint) {
  return dJointGetUniversalAngle1(GETJOINT(iJoint));
}

f32 CIrrOdeDevice::jointGetUniversalAngle2(u32 iJoint) {
  return dJointGetUniversalAngle2(GETJOINT(iJoint));
}

f32 CIrrOdeDevice::jointGetUniversalAngle1Rate(u32 iJoint) {
  return dJointGetUniversalAngle1Rate(GETJOINT(iJoint));
}

f32 CIrrOdeDevice::jointGetUniversalAngle2Rate(u32 iJoint) {
  return dJointGetUniversalAngle2Rate(GETJOINT(iJoint));
}

void CIrrOdeDevice::jointSetUniversalParam(u32 iJoint, u32 iGroup, s32 iParam, f32 fValue) {
  dJointSetUniversalParam(GETJOINT(iJoint),convertToOdeJointParam(iGroup,iParam),fValue);
}

f32 CIrrOdeDevice::jointGetUniversalParam(u32 iJoint, u32 iGroup, s32 iParam) {
  return dJointGetUniversalParam(GETJOINT(iJoint),convertToOdeJointParam(iGroup,iParam));
}

u32 CIrrOdeDevice::bodyCreate(u32 iWorld) {
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pBody=dBodyCreate(GETWORLD(iWorld));
  m_pOdeData.push_back(wrap);

  dBodySetMovedCallback(wrap->m_pBody,CIrrOdeDevice::bodyMovedCallback);

  return m_pOdeData.size();
}

void CIrrOdeDevice::bodyDestroy(u32 iBody) {
  list<CFastCollision *>::Iterator it;
  for (it=s_lCollisionQueue.begin(); it!=s_lCollisionQueue.end(); it++) {
    CFastCollision *p=*it;
    if (p->m_pBody->getBodyId()==iBody) {
      s_lCollisionQueue.erase(it);
      it=s_lCollisionQueue.begin();
      delete p;
    }
  }
  dBodyDestroy(GETBODY(iBody));
  GETBODY(iBody)=0;
}

void CIrrOdeDevice::bodySetPosition(u32 iBody, vector3df pos) {
  if (iBody) dBodySetPosition(GETBODY(iBody),pos.X,pos.Y,pos.Z);
}

void CIrrOdeDevice::bodySetQuaternion(u32 iBody, vector3df rot) {
  if (iBody) {
    dQuaternion q;
    eulerToQuaternion(rot,q);
    dBodySetQuaternion(GETBODY(iBody),q);
  }
}

void CIrrOdeDevice::bodySetData(u32 iBody, void *pData) {
  if (iBody) dBodySetData(GETBODY(iBody),pData);
}

void CIrrOdeDevice::bodySetLinearDamping(u32 iBody, f32 f) {
  if (iBody) dBodySetLinearDamping(GETBODY(iBody),f);
}

f32 CIrrOdeDevice::bodyGetLinearDamping(u32 iBody) {
  return iBody && m_bOdeInitialized ?dBodyGetLinearDamping(GETBODY(iBody)):0.0f;
}

void CIrrOdeDevice::bodySetAngularDamping(u32 iBody, f32 f) {
  if (iBody) dBodySetAngularDamping(GETBODY(iBody),f);
}

f32 CIrrOdeDevice::bodyGetAngularDamping(u32 iBody) {
  return iBody && m_bOdeInitialized && m_bOdeInitialized ?dBodyGetAngularDamping(GETBODY(iBody)):0.0f;
}

void CIrrOdeDevice::bodySetLinearDampingThreshold(u32 iBody, f32 f) {
  if (iBody) dBodySetLinearDampingThreshold(GETBODY(iBody),f);
}

f32 CIrrOdeDevice::bodyGetLinearDampingThreshold(u32 iBody) {
  return iBody && m_bOdeInitialized ?dBodyGetLinearDampingThreshold(GETBODY(iBody)):0.0f;
}

void CIrrOdeDevice::bodySetAngularDampingThreshold(u32 iBody, f32 f) {
  if (iBody) dBodySetAngularDampingThreshold(GETBODY(iBody),f);
}

f32 CIrrOdeDevice::bodyGetAngularDampingThreshold(u32 iBody) {
  return iBody && m_bOdeInitialized ?dBodyGetAngularDampingThreshold(GETBODY(iBody)):0.0f;
}

void CIrrOdeDevice::bodySetMaxAngularSpeed(u32 iBody, f32 f) {
  if (iBody) dBodySetMaxAngularSpeed(GETBODY(iBody),f);
}

f32 CIrrOdeDevice::bodyGetMaxAngularSpeed(u32 iBody) {
  return iBody && m_bOdeInitialized ?dBodyGetMaxAngularSpeed(GETBODY(iBody)):0.0f;
}

void CIrrOdeDevice::bodySetAutoDisableFlag(u32 iBody, s32 i) {
  if (iBody) dBodySetAutoDisableFlag(GETBODY(iBody),i);
}

s32 CIrrOdeDevice::bodyGetAutoDisableFlag(u32 iBody) {
  return iBody && m_bOdeInitialized ?dBodyGetAutoDisableFlag(GETBODY(iBody)):0;
}

 void CIrrOdeDevice::bodySetLinearAutoDisableThreshold(u32 iBody, f32 f) {
   if (iBody) dBodySetAutoDisableLinearThreshold(GETBODY(iBody),f);
 }

 f32 CIrrOdeDevice::bodyGetLinearAutoDisableThreshold(u32 iBody) {
   return iBody && m_bOdeInitialized ?dBodyGetAutoDisableLinearThreshold(GETBODY(iBody)):0.0f;
 }

 void CIrrOdeDevice::bodySetAngularAutoDisableThreshold(u32 iBody, f32 f) {
   if (iBody) dBodySetAutoDisableAngularThreshold(GETBODY(iBody),f);
 }

 f32 CIrrOdeDevice::bodyGetAngularAutoDisableThreshold(u32 iBody) {
   return iBody && m_bOdeInitialized ?dBodyGetAutoDisableAngularThreshold(GETBODY(iBody)):0.0f;
 }

void CIrrOdeDevice::bodySetAutoDisableSteps(u32 iBody, s32 i) {
  if (iBody) dBodySetAutoDisableSteps(GETBODY(iBody),i);
}

s32 CIrrOdeDevice::bodyGetAutoDisableSteps(u32 iBody) {
  return iBody && m_bOdeInitialized ?dBodyGetAutoDisableSteps(GETBODY(iBody)):0;
}

void CIrrOdeDevice::bodySetAutoDisableTime(u32 iBody, f32 f) {
  if (iBody) dBodySetAutoDisableTime(GETBODY(iBody),f);
}

f32 CIrrOdeDevice::bodyGetAutoDisableTime(u32 iBody) {
  return iBody && m_bOdeInitialized ?dBodyGetAutoDisableTime(GETBODY(iBody)):0.0f;
}

void CIrrOdeDevice::bodySetFiniteRotationMode(u32 iBody, s32 i) {
  if (iBody) dBodySetFiniteRotationMode(GETBODY(iBody),i);
}

s32 CIrrOdeDevice::bodyGetFiniteRotationMode(u32 iBody) {
  return iBody && m_bOdeInitialized ?dBodyGetFiniteRotationMode(GETBODY(iBody)):0;
}

void CIrrOdeDevice::bodySetFiniteRotationAxis(u32 iBody, vector3df v) {
  if (iBody) dBodySetFiniteRotationAxis(GETBODY(iBody),v.X,v.Y,v.Z);
}

const vector3df CIrrOdeDevice::bodyGetFiniteRotationAxis(u32 iBody) {
  static vector3df pResultVector(0,0,0);

  if (iBody && m_bOdeInitialized) {
    dVector3 pResult;
    dBodyGetFiniteRotationAxis(GETBODY(iBody),pResult);
    pResultVector.X=pResult[0];
    pResultVector.Y=pResult[1];
    pResultVector.Z=pResult[2];
  }
  else {
    pResultVector.X=0;
    pResultVector.Y=0;
    pResultVector.Z=0;
  }

  return pResultVector;
}

void CIrrOdeDevice::bodySetGravityMode(u32 iBody, s32 i) {
  if (iBody) dBodySetGravityMode(GETBODY(iBody),i);
}

s32 CIrrOdeDevice::bodyGetGravityMode(u32 iBody) {
  return iBody && m_bOdeInitialized ?dBodyGetGravityMode(GETBODY(iBody)):0;
}

CIrrOdeGeom *CIrrOdeDevice::bodyGetFirstGeom(u32 iBody) {
  if (!iBody) return NULL;
  dGeomID iGeom=dBodyGetFirstGeom(GETBODY(iBody));
  return iGeom?(CIrrOdeGeom *)dGeomGetData(iGeom):NULL;
}

CIrrOdeGeom *CIrrOdeDevice::bodyGetNextGeom(CIrrOdeGeom *pPrev) {
  dGeomID iGeom=dBodyGetNextGeom(GETGEOM(pPrev->getGeomId()));
  return iGeom?(CIrrOdeGeom *)dGeomGetData(iGeom):NULL;
}

bool CIrrOdeDevice::bodyIsEnabled(u32 iBody) {
  return dBodyIsEnabled(GETBODY(iBody));
}

void CIrrOdeDevice::bodyAddForce(u32 iBody, vector3df f) {
  dBodyAddForce(GETBODY(iBody),f.X,f.Y,f.Z);
}

void CIrrOdeDevice::bodyAddForceAtPosition(u32 iBody, vector3df f, vector3df p) {
  dBodyAddForceAtPos(GETBODY(iBody),f.X,f.Y,f.Z,p.X,p.Y,p.Z);
}

void CIrrOdeDevice::bodyAddTorque(u32 iBody, vector3df t) {
  dBodyAddTorque(GETBODY(iBody),t.X,t.Y,t.Z);
}

void CIrrOdeDevice::bodySetTorque(u32 iBody, vector3df t) {
  if (iBody) dBodySetTorque(GETBODY(iBody),t.X,t.Y,t.Z);
}

void CIrrOdeDevice::bodyEnable(u32 iBody) {
  dBodyEnable(GETBODY(iBody));
}

void CIrrOdeDevice::bodyDisable(u32 iBody) {
  dBodyDisable(GETBODY(iBody));
}

void CIrrOdeDevice::bodySetLinearVelocity(u32 iBody, vector3df v) {
  if (iBody) dBodySetLinearVel(GETBODY(iBody),v.X,v.Y,v.Z);
}

vector3df CIrrOdeDevice::bodyGetLinearVelocity(u32 iBody) {

  static vector3df vLinear;

  if (!iBody || !m_bOdeInitialized ) {
    vLinear.X=0.0f;
    vLinear.Y=0.0f;
    vLinear.Z=0.0f;
  }
  else {
    const dReal *v=dBodyGetLinearVel(GETBODY(iBody));

    vLinear.X=v[0];
    vLinear.Y=v[1];
    vLinear.Z=v[2];
  }

  return vLinear;
}

void CIrrOdeDevice::bodySetAngularVelocity(u32 iBody, vector3df v) {
  if (iBody) dBodySetAngularVel(GETBODY(iBody),v.X,v.Y,v.Z);
}

vector3df CIrrOdeDevice::bodyGetAngularVelocity(u32 iBody) {
  static vector3df vAngular;

  if (!iBody || !m_bOdeInitialized) {
    vAngular.X=0.0f;
    vAngular.Y=0.0f;
    vAngular.Z=0.0f;
    return vAngular;
  }
  const dReal *v=dBodyGetAngularVel(GETBODY(iBody));

  vAngular.X=v[0];
  vAngular.Y=v[1];
  vAngular.Z=v[2];

  return vAngular;
}

vector3df CIrrOdeDevice::bodyGetPointVel(u32 iBody, vector3df vPos) {
  static vector3df vRelPointPos=vector3df(0,0,0);
  if (iBody) {
    dVector3 v,p;
    IRR2VEC(vPos,p);
    dBodyGetRelPointPos(GETBODY(iBody),p[0],p[1],p[2],v);
    VEC2IRR(v,vRelPointPos);
  }
  return vRelPointPos;
}

vector3df CIrrOdeDevice::bodyGetRelPointVel(u32 iBody, vector3df vPos) {
  static vector3df vRelPointVel=vector3df(0,0,0);
  if (iBody) {
    dVector3 v;
    dBodyGetRelPointVel(GETBODY(iBody),vPos.X,vPos.Y,vPos.Z,v);
    VEC2IRR(v,vRelPointVel);
  }
  return vRelPointVel;
}

vector3df CIrrOdeDevice::bodyGetRelPointPos(u32 iBody, vector3df vPos) {
  static vector3df vPointVel=vector3df(0,0,0);
  if (iBody) {
    dVector3 v;
    dBodyGetPointVel(GETBODY(iBody),vPos.X,vPos.Y,vPos.Z,v);
    VEC2IRR(v,vPointVel);
  }
  return vPointVel;
}

void CIrrOdeDevice::bodySetMass(u32 iBody, u32 iMass) {
  if (iBody) dBodySetMass(GETBODY(iBody),GETMASS(iMass));
}

void CIrrOdeDevice::geomSetBody(u32 iGeom, CIrrOdeBody *pBody) {
  dGeomSetBody(iGeom?GETGEOM(iGeom):0,pBody?GETBODY(pBody->getBodyId()):0);
}

void CIrrOdeDevice::geomDestroy(u32 iGeom) {
  dGeomDestroy(GETGEOM(iGeom));
  GETGEOM(iGeom)=0;
}

void CIrrOdeDevice::geomSetPosition(u32 iGeom, vector3df pos) {
  dGeomSetPosition(GETGEOM(iGeom),pos.X,pos.Y,pos.Z);
}

void CIrrOdeDevice::geomSetRotation(u32 iGeom, vector3df rot) {
  dQuaternion q;
  CIrrOdeDevice::eulerToQuaternion(rot,q);
  #ifdef _TRACE_INIT_PHYSICS
    printf("rotation: %.2f, %.2f, %.2f\n",rot.X,rot.Y,rot.Z);
  #endif
  if (iGeom) dGeomSetQuaternion(GETGEOM(iGeom),q);
}

void CIrrOdeDevice::geomSetData(u32 iGeom, void *pData) {
  if (iGeom) dGeomSetData(GETGEOM(iGeom),pData);
}

void CIrrOdeDevice::geomRaySetData(u32 iGeom, vector3df pos, vector3df dir, f32 fLength) {
  dGeomRaySet(GETGEOM(iGeom),pos.X,pos.Y,pos.Z,dir.X,dir.Y,dir.Z);
  dGeomRaySetLength(GETGEOM(iGeom),fLength);
}

void CIrrOdeDevice::geomSetBackfaceCull(u32 iGeom, bool bBackfaceCull) {
  dGeomRaySetParams(GETGEOM(iGeom),0,bBackfaceCull?1:0);
}

void CIrrOdeDevice::geomSetOffsetPosition(u32 iGeom, vector3df offset) {
  dGeomSetOffsetPosition(GETGEOM(iGeom),offset.X,offset.Y,offset.Z);
}

void CIrrOdeDevice::geomSetOffsetQuaternion(u32 iGeom, vector3df rot) {
  dQuaternion q;
  eulerToQuaternion(rot,q);
  dGeomSetOffsetQuaternion(GETGEOM(iGeom),q);
}

u32 CIrrOdeDevice::geomCreateBox(u32 iSpace, f32 fWidth, f32 fHeight, f32 fDepth) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pGeom=dCreateBox(GETSPACE(iSpace),fWidth,fHeight,fDepth);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

u32 CIrrOdeDevice::geomCreateCapsule(u32 iSpace, f32 fRadius, f32 fLength) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pGeom=dCreateCapsule(GETSPACE(iSpace),fRadius,fLength);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

u32 CIrrOdeDevice::geomCreateCylinder(u32 iSpace, f32 fRadius, f32 fLength) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pGeom=dCreateCylinder(GETSPACE(iSpace),fRadius,fLength);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

u32 CIrrOdeDevice::geomCreateHeightfield(u32 iSpace, u32 iData) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pGeom=dCreateHeightfield(GETSPACE(iSpace),GETHFDATA(iData),1);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

u32 CIrrOdeDevice::geomCreatePlane(u32 iSpace, f32 A, f32 B, f32 C, f32 D) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pGeom=dCreatePlane(GETSPACE(iSpace),A,B,C,D);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

u32 CIrrOdeDevice::geomCreateRay(u32 iSpace, f32 fLength) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pGeom=dCreateRay(GETSPACE(iSpace),fLength);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

u32 CIrrOdeDevice::geomCreateSphere(u32 iSpace, f32 fRadius) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pGeom=dCreateSphere(GETSPACE(iSpace),fRadius);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

u32 CIrrOdeDevice::geomCreateTrimesh(u32 iSpace, u32 iData) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pGeom=dCreateTriMesh(GETSPACE(iSpace),GETDATA(iData),0,0,0);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

u32 CIrrOdeDevice::geomHeightfieldDataCreate() {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pHFData=dGeomHeightfieldDataCreate();
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

void CIrrOdeDevice::geomHeightfieldSetBounds(u32 iHFData, f32 fMinHeight, f32 fMaxHeight) {
  dGeomHeightfieldDataSetBounds(GETHFDATA(iHFData),fMinHeight,fMaxHeight);
}

void CIrrOdeDevice::geomHeightfieldDataCallback(u32 iHFData, void *pData, void *pCallback, f32 fWidth, f32 fDepth, s32 iWidthSamples, s32 iDepthSamples, f32 fScale, f32 fOffset, f32 fThickness, s32 bWrap) {
  dGeomHeightfieldDataBuildCallback(GETHFDATA(iHFData),pData,(dHeightfieldGetHeight *)pCallback,fWidth,fDepth,iWidthSamples,iDepthSamples,fScale,fOffset,fThickness,bWrap);
}

u32 CIrrOdeDevice::massCreate() {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

void CIrrOdeDevice::massAdd(u32 iDst, u32 iSrc) {
  dMassAdd(GETMASS(iDst),GETMASS(iSrc));
}

void CIrrOdeDevice::massSetZero(u32 iMass) {
  dMassSetZero(GETMASS(iMass));
}

void CIrrOdeDevice::massSetParameters(u32 iMass, f32 fMass, vector3df c, vector3df i1, vector3df i2) {
  dMassSetParameters(GETMASS(iMass),fMass,c.X,c.Y,c.Z,i1.X,i1.Y,i1.Z,i2.X,i2.Y,i2.Z);
}

void CIrrOdeDevice::massTranslate(u32 iMass, vector3df pos) {
  dMassTranslate(GETMASS(iMass),pos.X,pos.Y,pos.Z);
}

void CIrrOdeDevice::massSetBoxTotal(u32 iMass, f32 fMass, f32 lx, f32 ly, f32 lz) {
  dMassSetBoxTotal(GETMASS(iMass),fMass,lx,ly,lz);
}

void CIrrOdeDevice::massSetCapsuleTotal(u32 iMass, f32 fMass, s32 iDirection, f32 fRadius, f32 fLength) {
  dMassSetCapsuleTotal(GETMASS(iMass),fMass,iDirection,fRadius,fLength);
}

void CIrrOdeDevice::massSetCylinderTotal(u32 iMass, f32 fMass, s32 iDirection, f32 fRadius, f32 fLength) {
  dMassSetCylinderTotal(GETMASS(iMass),fMass,iDirection,fRadius,fLength);
}

void CIrrOdeDevice::massSetSphereTotal(u32 iMass, f32 fMass, f32 fRadius) {
  dMassSetSphereTotal(GETMASS(iMass),fMass,fRadius);
}

u32 CIrrOdeDevice::geomTrimeshDataCreate() {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pData=dGeomTriMeshDataCreate();
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

void CIrrOdeDevice::geomTrimeshDataBuildFromMeshBuffer(u32 iData, ISceneNode *pNode, u32 iBuffer) {
  IMesh *pMesh=NULL;
  CDynamicMeshBuffer cDynMeshBuffer(EVT_2TCOORDS,EIT_16BIT);
  if (pNode->getType()==ESNT_TERRAIN)
    (reinterpret_cast<ITerrainSceneNode *>(pNode))->getMeshBufferForLOD(cDynMeshBuffer,0);

  switch (pNode->getType()) {
    case ESNT_TERRAIN:
      pMesh=(reinterpret_cast<ITerrainSceneNode *>(pNode))->getMesh();
      break;

    case ESNT_OCTREE:
    case ESNT_MESH :
      pMesh=(reinterpret_cast<IMeshSceneNode *>(pNode))->getMesh();
      break;

    case ESNT_ANIMATED_MESH:
      pMesh=(reinterpret_cast<IAnimatedMeshSceneNode *>(pNode))->getMesh();
      break;

    default:
      printf("Non supported scene node type detected!\n");
      return;
  }

  f32 sx=pNode->getScale().X,sy=pNode->getScale().Y,sz=pNode->getScale().Z;
  u32 j,ci,cif,cv;

  if (pMesh==NULL) {
    printf("Mesh==NULL!\n");
    return;
  }

	IMeshBuffer *pBuffer=NULL;
  if (pNode->getType()==ESNT_TERRAIN) {
    INDEXCOUNT(iData)+=cDynMeshBuffer.getIndexCount();
    VERTEXCOUNT(iData)+=cDynMeshBuffer.getVertexCount();
    pBuffer=pMesh->getMeshBuffer(iBuffer);
  }
  else {
    pBuffer=pMesh->getMeshBuffer(iBuffer);
    if (pBuffer!=NULL) {
      INDEXCOUNT(iData)+=pBuffer->getIndexCount();
      VERTEXCOUNT(iData)+=pBuffer->getVertexCount();
    }
    else {
      IAnimatedMesh *pAnim=(reinterpret_cast<IAnimatedMeshSceneNode *>(pNode))->getMesh();
      if (pAnim && pAnim->getFrameCount()>0) {
        if (pMesh) {
          pBuffer=pAnim->getMesh(0)->getMeshBuffer(iBuffer);
          if (pBuffer) {
            INDEXCOUNT(iData)+=pBuffer->getIndexCount();
            VERTEXCOUNT(iData)+=pBuffer->getVertexCount();
          }
          else {
            printf("pBuffer==NULL!\n");
            return;
          }
        }
        else {
          printf("frame mesh buffer==NULL!\n");
          return;
        }
      }
      else {
        printf("no meshbuffer found!\n");
        return;
      }
    }
  }

  VERTICES(iData)=new dVector3[VERTEXCOUNT(iData)];
  INDICES(iData)=new u32[INDEXCOUNT(iData)];

  ci=0;  // current index in the indices array
  cif=0; // offset of the irrlicht-vertex-index in the vetices array
  cv=0;  // current index in the vertices array
	u16 *mb_indices;

  if (pBuffer==NULL) pBuffer=pMesh->getMeshBuffer(iBuffer);
  if (pNode->getType()==ESNT_TERRAIN) {
    mb_indices=cDynMeshBuffer.getIndices();
    for (j=0; j<cDynMeshBuffer.getIndexCount(); j++) {
      INDICES(iData)[ci]=cif+mb_indices[j];
      ci++;
    }
  }
  else {
    // fill indices
    mb_indices=pBuffer->getIndices();
    for(j=0; j<pBuffer->getIndexCount(); j++) {
      // scale the indices from multiple meshbuffers to single index array
      INDICES(iData)[ci]=cif+mb_indices[j];
      ci++;
    }
  }
  // update the offset for the next meshbuffer
  cif=cif+pBuffer->getVertexCount();
  // fill vertices
  if(pBuffer->getVertexType()==irr::video::EVT_STANDARD) {
    irr::video::S3DVertex *mb_vertices=(S3DVertex *)pBuffer->getVertices();
    for(j=0; j<pBuffer->getVertexCount(); j++) {
      VERTICES(iData)[cv][0]=sx*mb_vertices[j].Pos.X;
      VERTICES(iData)[cv][1]=sy*mb_vertices[j].Pos.Y;
      VERTICES(iData)[cv][2]=sz*mb_vertices[j].Pos.Z;
      cv++;
    }
  }
  else
    if(pBuffer->getVertexType()==irr::video::EVT_2TCOORDS) {
      irr::video::S3DVertex2TCoords *mb_vertices=(S3DVertex2TCoords*)pBuffer->getVertices();
      for(j=0; j<pBuffer->getVertexCount(); j++) {
        VERTICES(iData)[cv][0]=sx*mb_vertices[j].Pos.X;
        VERTICES(iData)[cv][1]=sy*mb_vertices[j].Pos.Y;
        VERTICES(iData)[cv][2]=sz*mb_vertices[j].Pos.Z;
        cv++;
      }
    }

  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeGeomTrimesh::initPhysics: %i vertices, %i indices\n",VERTEXCOUNT(iData),INDEXCOUNT(iData));
  #endif

  dGeomTriMeshDataBuildSimple(GETDATA(iData),(dReal *)VERTICES(iData),VERTEXCOUNT(iData),INDICES(iData),INDEXCOUNT(iData));
}

void CIrrOdeDevice::geomTrimeshDataBuildSimple(u32 iData, ISceneNode *pNode) {
  IMesh *pMesh=NULL;
  CDynamicMeshBuffer cDynMeshBuffer(EVT_2TCOORDS,EIT_16BIT);
  if (pNode->getType()==ESNT_TERRAIN)
    (reinterpret_cast<ITerrainSceneNode *>(pNode))->getMeshBufferForLOD(cDynMeshBuffer,0);

  switch (pNode->getType()) {
    case ESNT_TERRAIN:
      pMesh=(reinterpret_cast<ITerrainSceneNode *>(pNode))->getMesh();
      break;

    //case ESNT_OCT_TREE:
    case ESNT_MESH :
      pMesh=(reinterpret_cast<IMeshSceneNode *>(pNode))->getMesh();
      break;

    case ESNT_ANIMATED_MESH:
      pMesh=(reinterpret_cast<IAnimatedMeshSceneNode *>(pNode))->getMesh();
      break;

    default:
      printf("Non supported scene node type detected!\n");
      return;
  }

  f32 sx=pNode->getScale().X,sy=pNode->getScale().Y,sz=pNode->getScale().Z;

  u32 i,j,ci,cif,cv;

  if (pMesh==NULL) {
    printf("Mesh==NULL!\n");
    return;
  }

	IMeshBuffer *pBuffer;
  for(u32 i=0; i<pMesh->getMeshBufferCount(); i++) {
    if (pNode->getType()==ESNT_TERRAIN) {
      INDEXCOUNT(iData)+=cDynMeshBuffer.getIndexCount();
      VERTEXCOUNT(iData)+=cDynMeshBuffer.getVertexCount();
    }
    else {
      pBuffer=pMesh->getMeshBuffer(i);
      INDEXCOUNT(iData)+=pBuffer->getIndexCount();
      VERTEXCOUNT(iData)+=pBuffer->getVertexCount();
    }
  }
  #ifdef _TRACE_INIT_PHYSICS
    printf("ic: %i, vc: %i, iData: %i\n",INDEXCOUNT(iData),VERTEXCOUNT(iData),iData);
  #endif

  VERTICES(iData)=new dVector3[VERTEXCOUNT(iData)];
  INDICES(iData)=new u32[INDEXCOUNT(iData)];

  ci=0;  // current index in the indices array
  cif=0; // offset of the irrlicht-vertex-index in the vetices array
  cv=0;  // current index in the vertices array
	u16 *mb_indices;
  for(i=0; i<pMesh->getMeshBufferCount(); i++) {
    pBuffer=pMesh->getMeshBuffer(i);
    if (pNode->getType()==ESNT_TERRAIN) {
      mb_indices=cDynMeshBuffer.getIndices();
      for (j=0; j<cDynMeshBuffer.getIndexCount(); j++) {
        INDICES(iData)[ci]=cif+mb_indices[j];
        ci++;
      }
    }
    else {
      // fill indices
      mb_indices=pBuffer->getIndices();
      for(j=0; j<pBuffer->getIndexCount(); j++) {
        // scale the indices from multiple meshbuffers to single index array
        INDICES(iData)[ci]=cif+mb_indices[j];
        ci++;
      }
    }
    // update the offset for the next meshbuffer
    cif=cif+pBuffer->getVertexCount();
    // fill vertices
    if(pBuffer->getVertexType()==irr::video::EVT_STANDARD) {
      irr::video::S3DVertex *mb_vertices=(S3DVertex *)pBuffer->getVertices();
      for(j=0; j<pBuffer->getVertexCount(); j++) {
        VERTICES(iData)[cv][0]=sx*mb_vertices[j].Pos.X;
        VERTICES(iData)[cv][1]=sy*mb_vertices[j].Pos.Y;
        VERTICES(iData)[cv][2]=sz*mb_vertices[j].Pos.Z;
        cv++;
      }
    }
    else
      if(pBuffer->getVertexType()==irr::video::EVT_2TCOORDS) {
        irr::video::S3DVertex2TCoords *mb_vertices=(S3DVertex2TCoords*)pBuffer->getVertices();
        for(j=0; j<pBuffer->getVertexCount(); j++) {
          VERTICES(iData)[cv][0]=sx*mb_vertices[j].Pos.X;
          VERTICES(iData)[cv][1]=sy*mb_vertices[j].Pos.Y;
          VERTICES(iData)[cv][2]=sz*mb_vertices[j].Pos.Z;
          cv++;
        }
      }
  }

  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeGeomTrimesh::initPhysics: %i vertices, %i indices\n",VERTEXCOUNT(iData),INDEXCOUNT(iData));
  #endif

  dGeomTriMeshDataBuildSimple(GETDATA(iData),(dReal *)VERTICES(iData),VERTEXCOUNT(iData),INDICES(iData),INDEXCOUNT(iData));
}

u32 CIrrOdeDevice::spaceCreateSimple(u32 iParent) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pSpace=dSimpleSpaceCreate(iParent?GETSPACE(iParent):0);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

u32 CIrrOdeDevice::spaceCreateHash(u32 iParent) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pSpace=dHashSpaceCreate(iParent?GETSPACE(iParent):0);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

u32 CIrrOdeDevice::spaceCreateQuadTree(u32 iParent, vector3df center, vector3df extends, s32 iDepth) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  dVector3 ctr; ctr[0]=center.X; ctr[1]=center.Y; ctr[2]=center.Z;
  dVector3 ext; ext[0]=extends.X; ext[1]=extends.Y; ext[2]=extends.Z;
  wrap->m_pSpace=dQuadTreeSpaceCreate(iParent?GETSPACE(iParent):0,ctr,ext,iDepth);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

void CIrrOdeDevice::spaceDestroy(u32 iSpace) {
  dSpaceDestroy(GETSPACE(iSpace));
  GETSPACE(iSpace)=0;
}

u32 CIrrOdeDevice::jointGroupCreate(s32 iMaxSize) {
  u32 iRet;
  COdeDataWrapper *wrap=new COdeDataWrapper();
  wrap->m_pJGroup=dJointGroupCreate(iMaxSize);
  m_pOdeData.push_back(wrap);
  iRet=m_pOdeData.size();
  return iRet;
}

void CIrrOdeDevice::jointGroupDestroy(u32 iJointGroup) {
  dJointGroupDestroy(GETJGROUP(iJointGroup));
  GETJGROUP(iJointGroup)=0;
}

void CIrrOdeDevice::jointGroupEmtpy(u32 iJointGroup) {
  dJointGroupEmpty(GETJGROUP(iJointGroup));
}

void CIrrOdeDevice::convertToSurfaceParameters(dSurfaceParameters *pOde, CIrrOdeSurfaceParameters *pIrr) {
  pOde->mode      =pIrr->getMode();
  pOde->mu        =pIrr->getMu();
  pOde->mu2       =pIrr->getMu2();
  pOde->bounce    =pIrr->getBounce();
  pOde->bounce_vel=pIrr->getBounceVel();
  pOde->soft_erp  =pIrr->getSoftErp();
  pOde->soft_cfm  =pIrr->getSoftCfm();
  pOde->motion1   =pIrr->getMotion1();
  pOde->motion2   =pIrr->getMotion2();
  pOde->motionN   =pIrr->getMotionN();
  pOde->slip1     =pIrr->getSlip1();
  pOde->slip2     =pIrr->getSlip2();
}

void CIrrOdeDevice::trimeshDrawDebug(u32 iData, IVideoDriver *pDrv) {
  pDrv->setTransform(ETS_WORLD,CMatrix4<f32>());
  for (u32 i=0; i<INDEXCOUNT(iData); i+=3) {
    const dReal *p1=VERTICES(iData)[INDICES(iData)[i  ]],
                *p2=VERTICES(iData)[INDICES(iData)[i+1]],
                *p3=VERTICES(iData)[INDICES(iData)[i+2]];

    pDrv->draw3DLine(vector3df(p1[0],p1[1],p1[2]),vector3df(p2[0],p2[1],p2[2]),SColor(0x80,0xFF,0xFF,0xFF));
    pDrv->draw3DLine(vector3df(p3[0],p3[1],p3[2]),vector3df(p2[0],p2[1],p2[2]),SColor(0x80,0xFF,0xFF,0xFF));
    pDrv->draw3DLine(vector3df(p1[0],p1[1],p1[2]),vector3df(p3[0],p3[1],p3[2]),SColor(0x80,0xFF,0xFF,0xFF));
  }
}

COdeDataWrapper *CIrrOdeDevice::getData(u32 iIdx) {
  return iIdx<m_pOdeData.size()?m_pOdeData[iIdx]:NULL;
}

void CIrrOdeDevice::rayCollide(CIrrOdeWorld *pWorld) {
}

} //namespace ode
} //namespace irr
#endif

