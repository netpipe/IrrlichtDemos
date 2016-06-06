  #include "CMarble.h"

CMarble::CMarble(irr::ode::CIrrOdeManager *pManager, irr::IrrlichtDevice *pDevice) : irr::ode::IIrrOdeEntity(pManager->getWorld(),pManager->getSpace(),NULL,NULL)
{
  irr::scene::IAnimatedMesh          *pMarble=pDevice->getSceneManager()->getMesh("levels/marble.3ds");
  irr::scene::IAnimatedMeshSceneNode *pNode  =pDevice->getSceneManager()->addAnimatedMeshSceneNode(pMarble);

  pNode->addShadowVolumeSceneNode();

  setMesh(pMarble->getMesh(0));
  setSceneNode(pNode);

  pManager->addEntity(this);
  setSphereGeomData();

  setName("Marble");

//  m_pSndEngine=NULL;
//  m_pBounce=NULL;
//  m_pRolling=NULL;

  m_iJointCnt=0;
  m_iNextCheckPoint=0;
}

CMarble::~CMarble()
{
}

void CMarble::collisionCallback(IIrrOdeEntity *pOpponent, int iContactCnt, dJointFeedback *pFeedback)
{
  irr::f32 fCollisionForce=0.0f;
  irr::core::vector3df vFeedback;

  vFeedback.X=pFeedback[0].f1[0]+pFeedback[0].f2[0]+pFeedback[0].t1[0]+pFeedback[0].t2[0];
  vFeedback.Y=pFeedback[0].f1[1]+pFeedback[0].f2[1]+pFeedback[0].t1[1]+pFeedback[0].t2[1];
  vFeedback.Z=pFeedback[0].f1[2]+pFeedback[0].f2[2]+pFeedback[0].t1[2]+pFeedback[0].t2[2];

  fCollisionForce=vFeedback.getLength()/500;

//  if (fCollisionForce>0.1f && m_pSndEngine && (m_pBounce==NULL || m_pBounce->isFinished()))
 // {
   // m_pBounce=m_pSndEngine->play2D("sounds/bounce.ogg",false,true);
   // m_pBounce->setVolume(5*fCollisionForce);
   // m_pBounce->setIsPaused(false);
 //}

  m_iJointCnt=iContactCnt;
}


void CMarble::frameCallback()
{


  if (m_iJointCnt)
  {}
}

void CMarble::checkPointHit(int iNr)
{
  if (iNr==m_iNextCheckPoint)
  {
    m_iNextCheckPoint++;
    m_bCheckPointHit=true;
//    m_pSndEngine->play2D("sounds/checkpoint.ogg");
  }
}

bool CMarble::checkPointHit()
{
bool b=m_bCheckPointHit;

  m_bCheckPointHit=false;
  return b;
}

int CMarble::getNextCheckPoint()
{
  return m_iNextCheckPoint;
}

void CMarble::stopRollingSound()
{
//  m_pRolling->setIsPaused(true);
}

void CMarble::setRespawn(irr::f32 fRespawn)
{
  m_fRespawn=fRespawn;
}

bool CMarble::respawn()
{
const dReal *pPosition;
bool         b;

  pPosition=dGeomGetPosition(m_iGeom);
  b=pPosition[1]<m_fRespawn;

  if (b)
  {
    setForce          (irr::core::vector3df(0,0,0));
    setTorque         (irr::core::vector3df(0,0,0));
    setAngularVelocity(irr::core::vector3df(0,0,0));
    setVelocity       (irr::core::vector3df(0,0,0));

//    m_pSndEngine->play2D("sounds/respawn.ogg");
  }

  return b;
}
