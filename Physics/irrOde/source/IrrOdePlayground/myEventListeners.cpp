#include "myEventListeners.h"
#include <math.h>

myBodyEventListener::myBodyEventListener(IVideoDriver *pDriver, IGUIStaticText *pActiveBodies, f32 fMinX, f32 fMinZ, f32 fMaxX, f32 fMaxZ) {
  m_pDriver=pDriver;
  m_pActiveBodies=pActiveBodies;
  m_iActive=0;
  m_iTotal=0;

  m_fMinX=fMinX;
  m_fMinZ=fMinZ;
  m_fMaxX=fMaxX;
  m_fMaxZ=fMaxZ;

  char sSuffix[4][10]={"_bw","0","1"};
  for (s32 i=0; i<3; i++) {
    char sDummy[0xFF];
    sprintf(sDummy,"../../data/box%s.jpg"     ,sSuffix[i]); m_pBoxTextures   [i]=m_pDriver->getTexture(sDummy);
    sprintf(sDummy,"../../data/sphere%s.jpg"  ,sSuffix[i]); m_pSphereTextures[i]=m_pDriver->getTexture(sDummy);
    sprintf(sDummy,"../../data/cylinder%s.jpg",sSuffix[i]); m_pCylTexture    [i]=m_pDriver->getTexture(sDummy);
    sprintf(sDummy,"../../data/trimesh%s.jpg" ,sSuffix[i]); m_pTriTexture    [i]=m_pDriver->getTexture(sDummy);
  }
}

bool myBodyEventListener::onEvent(IIrrOdeEvent *pEvent) {
  if (pEvent->getType()==irr::ode::eIrrOdeEventActivationChanged) {
    CIrrOdeEventActivationChanged *pActEvt=(CIrrOdeEventActivationChanged *)pEvent;

    m_iActive+=pActEvt->isActive()?1:-1;
    list<ISceneNode *> children=pActEvt->getBody()->getChildren();
    list<ISceneNode *>::Iterator i;

    for (i=children.begin(); i!=children.end(); i++) {
      ISceneNode *child=*i;
      ITexture *pTexture=NULL;

      list<ISceneNode *> grandChildren=child->getChildren();
      list<ISceneNode *>::Iterator i2;

      for (i2=grandChildren.begin(); i2!=grandChildren.end(); i2++)
        switch ((*i2)->getType()) {
          case IRR_ODE_GEOM_SPHERE_ID  : pTexture=m_pSphereTextures[pActEvt->isActive()?1:0]; break;
          case IRR_ODE_GEOM_BOX_ID     : pTexture=m_pBoxTextures   [pActEvt->isActive()?1:0]; break;
          case IRR_ODE_GEOM_CYLINDER_ID: pTexture=m_pCylTexture    [pActEvt->isActive()?1:0]; break;
          case IRR_ODE_GEOM_TRIMESH_ID : pTexture=m_pTriTexture    [pActEvt->isActive()?1:0]; break;
          default: pTexture=NULL;
        }

      if (pTexture) (*i)->setMaterialTexture(0,pTexture);
    }
    updateBodyText();
  }

  if (pEvent->getType()==irr::ode::eIrrOdeEventBodyCreated) {
    m_iTotal++;
    updateBodyText();
  }

  if (pEvent->getType()==irr::ode::eIrrOdeEventBodyMoved) {
    CIrrOdeEventBodyMoved *pEvt=(CIrrOdeEventBodyMoved *)pEvent;
    if (pEvt->getBody()->getBodyId()) {
      vector3df pos=pEvt->getBody()->getPosition(),vel=pEvt->getBody()->getLinearVelocity();

      if (pos.X<m_fMinX) {
        pEvt->getBody()->setLinearVelocity(vector3df(-vel.X,vel.Y,vel.Z)/2);
        pEvt->getBody()->setPosition(vector3df(m_fMinX+1,pos.Y,pos.Z));
      }

      if (pos.X>m_fMaxX) {
        pEvt->getBody()->setLinearVelocity(vector3df(-vel.X,vel.Y,vel.Z)/2);
        pEvt->getBody()->setPosition(vector3df(m_fMaxX-1,pos.Y,pos.Z));
      }

      if (pos.Z<m_fMinZ) {
        pEvt->getBody()->setLinearVelocity(vector3df(vel.X,vel.Y,-vel.Z)/2);
        pEvt->getBody()->setPosition(vector3df(pos.X,pos.Y,m_fMinZ+1 ));
      }

      if (pos.Z>m_fMaxZ) {
        pEvt->getBody()->setLinearVelocity(vector3df(vel.X,vel.Y,-vel.Z)/2);
        pEvt->getBody()->setPosition(vector3df(pos.X,pos.Y,m_fMaxZ-1));
      }

      if (pEvt->getTouched()) {
        static int cnt=0;

        if (pEvt->getTouched()->getBody()!=NULL) {
          cnt?cnt=0:cnt=1;
          list<ISceneNode *> children=pEvt->getBody()->getChildren();
          list<ISceneNode *>::Iterator i;

          for (i=children.begin(); i!=children.end(); i++) {
            ISceneNode *child=*i;
            ITexture *pTexture=NULL;

            list<ISceneNode *> grandChildren=child->getChildren();
            list<ISceneNode *>::Iterator i2;

            for (i2=grandChildren.begin(); i2!=grandChildren.end(); i2++)
              switch ((*i2)->getType()) {
                case IRR_ODE_GEOM_SPHERE_ID  : pTexture=m_pSphereTextures[cnt+1]; break;
                case IRR_ODE_GEOM_BOX_ID     : pTexture=m_pBoxTextures   [cnt+1]; break;
                case IRR_ODE_GEOM_CYLINDER_ID: pTexture=m_pCylTexture    [cnt+1]; break;
                case IRR_ODE_GEOM_TRIMESH_ID : pTexture=m_pTriTexture    [cnt+1]; break;
                default: pTexture=NULL;
              }

            if (pTexture) (*i)->setMaterialTexture(0,pTexture);
          }
        }
      }
    }
  }

  return false;
}

bool myBodyEventListener::handlesEvent(IIrrOdeEvent *pEvent) {
  return pEvent->getType()==irr::ode::eIrrOdeEventBodyCreated ||
         pEvent->getType()==irr::ode::eIrrOdeEventActivationChanged ||
         pEvent->getType()==irr::ode::eIrrOdeEventBodyMoved;
}

myRayListener::myRayListener(myEventReceiver *pInput, ICameraSceneNode *pCam, IGUIStaticText *pInfo, IGUIStaticText *pJoints) {
  m_pInput=pInput;
  m_pJoints=pJoints;
  m_pInfo=pInfo;
  m_pCam=pCam;
  m_bRayHit=false;
}

bool myRayListener::onEvent(IIrrOdeEvent *pEvent) {
  CIrrOdeEventRayHit *pEvtRay=(CIrrOdeEventRayHit *)pEvent;
  CIrrOdeBody *pBody=pEvtRay->getBody();
  CIrrOdeGeom *pGeom=pEvtRay->getGeom();
  if (pBody==NULL) {
    wchar_t dummy[2048];
    swprintf(dummy,2048,L"distance to terrain: %.2f",pEvtRay->getDistance());
    m_pInfo->setText(dummy);
  }
  else {
    vector3df pos=pBody->getAbsolutePosition(),
              rot=pBody->getRotation(),
              vl=pBody->getLinearVelocity(),
              va=pBody->getAngularVelocity();

    m_bRayHit=true;
    m_iRayCnt=50;

    wchar_t dummy[2048];
    swprintf(dummy,2048,cInfoText,pBody->getID(),pGeom->getTypeName(),pEvtRay->getDistance(),pos.X,pos.Y,pos.Z,rot.X,rot.Y,rot.Z,vl.getLength(),vl.X,vl.Y,vl.Z,va.getLength(),va.X,va.Y,va.Z,pBody->isEnabled()?L"active":L"inactive");

    m_pInfo->setText(dummy);
    m_pInfo->setBackgroundColor(pBody->isEnabled()?SColor(0x50,0x80,0xFF,0x80):SColor(0x50,0xFF,0x80,0x80));

    list<CIrrOdeJoint *> joints=pBody->getJoints();

    swprintf(dummy,2048,L"# of attached joints: %i\n",joints.getSize());

    list<CIrrOdeJoint *>::Iterator i;

    int iCnt=0;
    for (i=joints.begin(); i!=joints.end(); i++) {
      CIrrOdeJoint *joint=*i;

      swprintf(dummy,2048,L"%s\n\njoint %i (%i): \"%s\"\n",dummy,iCnt++,joint->getID(),joint->getTypeName());

      CIrrOdeBody *b1=joint->getBody1(),*b2=joint->getBody2();
      swprintf(dummy,2048,L"%sBody 1 ID: %i\nBody 2 ID: %i\n",dummy,b1?b1->getID():-23,b2?b2->getID():-23);


      vector3df a1,a2;

    CIrrOdeJointHinge *hinge=NULL;
    CIrrOdeJointPiston *piston=NULL;
    CIrrOdeJointSlider *slider=NULL;
    CIrrOdeJointBallSocket *ball=NULL;

    f32 a=0.0f,ar=0.0f;

      switch (joint->getType()) {
        case IRR_ODE_JOINT_HINGE_ID:
          hinge=reinterpret_cast<CIrrOdeJointHinge *>(joint);
          a1=hinge->getAnchor1(),a2=hinge->getAnchor2();
          swprintf(dummy,2048,L"%s\nanchor 1: (%.0f, %.0f, %.0f)\nanchor 2: (%.0f, %.0f, %.0f)\n",dummy,a1.X,a1.Y,a1.Z,a2.X,a2.Y,a2.Z);

          a=hinge->getHingeAngle()*180/M_PI;
          ar=hinge->getHingeAngleRate()*180/M_PI;
          swprintf(dummy,2048,L"%s\nhinge angle: %.2f\nhinge angle rate: %.2f\n",dummy,a,ar);
          break;

        case IRR_ODE_JOINT_PISTON_ID:
          piston=reinterpret_cast<CIrrOdeJointPiston *>(joint);
          swprintf(dummy,2048,L"%s\nposition: %.2f\nrate: %.2f\n",dummy,piston->getPistonPosition(),piston->getPistonPositionRate());
          break;

        case IRR_ODE_JOINT_SLIDER_ID:
          slider=reinterpret_cast<CIrrOdeJointSlider *>(joint);
          swprintf(dummy,2048,L"%s\nposition: %.2f\nrate: %.2f\n",dummy,slider->getSliderPosition(),slider->getSliderPositionRate());
          break;

        case IRR_ODE_JOINT_BALL_SOCKET_ID:
          ball=reinterpret_cast<CIrrOdeJointBallSocket *>(joint);
          a1=ball->getAnchor1(),a2=ball->getAnchor2();
          swprintf(dummy,2048,L"%s\nanchor1: (%.0f, %.0f, %.0f)\nanchor2: (%.0f, %.0f, %.0f)\n",dummy,a1.X,a1.Y,a1.Z,a2.X,a2.Y,a2.Z);
          break;

        default:
          break;
      }

      swprintf(dummy,2048,L"%s\n%i parameter groups:\n",dummy,joint->numParamGroups());

      for (u16 x=0; x<joint->numParamGroups(); x++) {
        for (u16 y=0; y<(int)CIrrOdeJoint::eParamEnd; y++) {
          swprintf(dummy,2048,L"%s\n%s: %.2f",dummy,IRR_ODE_PARAM_NAMES[y],joint->getParameter(x,(CIrrOdeJoint::eJointParameter)y));
        }
      }
    }

    swprintf(dummy,2048,L"%s\n",dummy);
    m_pJoints->setText(dummy);

    if (m_pInput->leftBtn()) {
      pos=pEvtRay->getPosition();
      vector3df force=-5*(m_pCam->getAbsolutePosition()-pos);

      pBody->addForceAtPosition(pos,force);
    }

    if (m_pInput->rghtBtn()) {
      vector3df force=-50*(m_pCam->getAbsolutePosition()-pos);
      pBody->addForce(force);
    }

    if (m_pInput->midlBtn()) {
      pBody->setLinearVelocity(vector3df(0.0f,0.0f,0.0f));
      pBody->setAngularVelocity(vector3df(0.0f,0.0f,0.0f));
    }
  }

  return false;
}

bool myRayListener::handlesEvent(IIrrOdeEvent *pEvent) {
  return pEvent->getType()==irr::ode::eIrrOdeEventRayHit;
}

bool myRayListener::rayHit() {
  bool bRet=m_bRayHit;
  m_bRayHit=false;
  if (m_iRayCnt>0) {
    m_iRayCnt--;
    bRet=true;
  }
  return bRet;
}
