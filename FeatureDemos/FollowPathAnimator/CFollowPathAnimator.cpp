  #include <CFollowPathAnimator.h>

CFollowPathAnimator::CFollowPathAnimator(IrrlichtDevice *pDevice, c8 *sPathRootName, f32 iAnimSpeed, f32 iCamTargetSpeed) {
  m_fAnimSpeed=iAnimSpeed;
  m_fCamTargetSpeed=iCamTargetSpeed;

  m_iLastTime=0;

  m_bAnimRunning=true;
  m_bStopNextHit=false;
  m_bLoopAnim=true;

  ISceneNode *path=pDevice->getSceneManager()->getSceneNodeFromName(sPathRootName);
  if (path) {
    list<ISceneNode *> children=path->getChildren();
    list<ISceneNode *>::Iterator it;
    for (it=children.begin(); it!=children.end(); it++) {
      m_cPathPoints.push_back((*it)->getAbsolutePosition());
    }
  }

  m_cIter=m_cPathPoints.begin();
  m_cCamPos=*m_cIter; m_cIter++;
  m_cCamTgt=*m_cIter; m_cIter++;
  m_cCamDir=(m_cCamTgt-m_cCamPos).normalize();
  m_iState=0;
}

void CFollowPathAnimator::animateNode(ISceneNode *pNode, u32 timeMs) {
  f32 fTimeDiff=(f32)(timeMs-m_iLastTime);
  if (timeMs==m_iLastTime) return;
  m_iLastTime=timeMs;
  ICameraSceneNode *pCam=reinterpret_cast<ICameraSceneNode *>(pNode);
  vector3df move;
  f32 fLength_old,f1,f2,fLength_new;
  static int iCnt=0;
  if (m_bAnimRunning) {
    switch (m_iState) {
      case 0:
        move=fTimeDiff*(m_cCamTgt-m_cCamPos).normalize()/m_fAnimSpeed;
        fLength_old=(m_cCamTgt-m_cCamPos).getLength();
        pCam->setPosition(pNode->getPosition()+move);
        pCam->setTarget(pCam->getPosition()+m_cCamDir);

        m_cCamPos=pCam->getPosition();
        fLength_new=(m_cCamTgt-m_cCamPos).getLength();

        if (fLength_new-fLength_old>=0)
          if (m_bStopNextHit)
            m_iState=4;
          else
            m_iState=1;
        break;

      case 1:
        m_cCamNDr=m_cCamDir.normalize();

        m_cCamPos=m_cCamTgt;
        m_cCamTgt=*m_cIter;

        m_cCamDir=(m_cCamTgt-m_cCamPos).normalize();
        m_cCamTrn=(m_cCamDir-m_cCamNDr)/m_fCamTargetSpeed;

        m_cIter++;
        if (m_cIter==m_cPathPoints.end()) m_bStopNextHit=true;
        m_iState=5;
        m_iNextState=2;
        m_iDelayTime=0;
        iCnt=0;
        break;

      case 2:
        if (iCnt++<150) fTimeDiff=0.0f;

        f1=(m_cCamNDr*100-m_cCamDir*100).getLength();
        m_cCamNDr+=(m_cCamTrn*fTimeDiff);
        f2=(m_cCamNDr*100-m_cCamDir*100).getLength();
        pCam->setTarget(pCam->getPosition()+m_cCamNDr);

        if (f1-f2<0) {
          m_iState=5;
          m_iNextState=3;
          m_iDelayTime=0;
        }
        break;

      case 3:
        m_cCamDir=m_cCamTgt-pCam->getPosition();
        pCam->setTarget(m_cCamTgt);
        m_iState=0;
        break;

      case 4:
        animatedEnded();
        if (m_bLoopAnim) {
          m_cIter=m_cPathPoints.begin();
          m_cCamPos=*m_cIter; m_cIter++;
          m_cCamTgt=*m_cIter; m_cIter++;

          pCam->setPosition(m_cCamPos);
          pCam->setTarget(m_cCamTgt);
          m_cCamDir=(m_cCamTgt-m_cCamPos).normalize();
          m_bStopNextHit=false;
          m_iState=0;
        }
        else m_bAnimRunning=false;

      case 5:
        m_iDelayTime+=(u32)fTimeDiff;
        if (m_iDelayTime>100) m_iState=m_iNextState;
        break;
    }
  }
}

ISceneNodeAnimator *CFollowPathAnimator::createClone(ISceneNode *node, ISceneManager *pNewMngr) {
  return NULL;
}

ESCENE_NODE_ANIMATOR_TYPE CFollowPathAnimator::getType() const {
  return (ESCENE_NODE_ANIMATOR_TYPE)2305;
}

bool CFollowPathAnimator::isEventReceiverEnabled() const {
  return false;
}

CFollowPathAnimator::~CFollowPathAnimator() { }

void CFollowPathAnimator::animatedEnded() {
}

void CFollowPathAnimator::setCamParameter(ICameraSceneNode *pCam) {
  pCam->setPosition(m_cCamPos);
  pCam->setTarget(m_cCamTgt);
}
