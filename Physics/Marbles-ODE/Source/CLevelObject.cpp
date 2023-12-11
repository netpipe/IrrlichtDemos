  #include "CLevelObject.h"

CLevelObject::CLevelObject(irr::ode::CIrrOdeManager *pManager, irr::IrrlichtDevice *pDevice) : irr::ode::IIrrOdeEntity(pManager->getWorld(),pManager->getSpace(),NULL,NULL)
{
  pManager->addEntity(this);
  setName("LevelObject");
  m_pMarble=NULL;
//  m_pSound=NULL;
  m_iCheckPoint=-1;
  m_iDelay=0;

  m_pImg=NULL;
  m_pImgOk=NULL;
}

void CLevelObject::collisionCallback(IIrrOdeEntity *pOpponent, int iContactCnt, dJointFeedback *pFeedback)
{
irr::ode::CObjData *dat=getData();

  if (pOpponent==m_pMarble && m_iCheckPoint>-1)
  {
    m_pMarble->checkPointHit(m_iCheckPoint);
    if (m_pCheckLight) m_pCheckLight->setVisible(false);

    if (m_pImg  ) m_pImg  ->setVisible(false);
    if (m_pImgOk) m_pImgOk->setVisible(true );
  }

  if (dat && m_iDelay<=0 && !strcmp(pOpponent->getName(),"Marble"))
  {
    const dReal *pVel;

    pVel=dBodyGetLinearVel(m_pMarble->getBody());
    dBodySetLinearVel(m_pMarble->getBody(),pVel[0]+dat->m_ax,pVel[1]+dat->m_ay,pVel[2]+dat->m_az);

//    if (m_pSound && strcmp(dat->m_sSound,"")) m_pSound->play2D(dat->m_sSound);

    startDelay();
  }
}

void CLevelObject::setMarble(CMarble *pMarble)
{
  m_pMarble=pMarble;
}

void CLevelObject::setCheckPoint(int i)
{
  m_iCheckPoint=i;
}

int CLevelObject::getCheckPointNo()
{
  return m_iCheckPoint;
}

void CLevelObject::setCheckLight(irr::scene::ISceneNode *pCheckLight)
{
  m_pCheckLight=pCheckLight;
}

void CLevelObject::setImages(irr::gui::IGUIImage *pImg, irr::gui::IGUIImage *pImgOk)
{
  m_pImg=pImg;
  m_pImgOk=pImgOk;
}
