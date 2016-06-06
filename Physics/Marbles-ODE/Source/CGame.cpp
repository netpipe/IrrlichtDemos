  #include <irrlicht.h>
//  #include <irrklang.h>
  #include <irrString.h>

  #include "ode/IIrrOdeEntity.h"
  #include "ode/CIrrOdeManager.h"

  #include "CGame.h"
  #include "CMarble.h"
  #include "CLevelObject.h"

const wchar_t g_cAboutText[]=L"Bulletbyte Marbles\n\n\
It one small step for a marble, one giant leap into madness\n\n\
This game was written in September 2007 by Christian Keimel \
(bulletbyte, http://www.bulletbyte.de) to get used to the \
Irrlicht 3D engine (http://irrlicht.sourceforge.net) and \
ODE (Open Dynamics Engine, http://www.ode.org). This game \
is released as \"giftware\", use it however you want. It is \
not required to but would be nice to drop me an eMail \
(ckeimel@gmx.de) if you like the game or have any suggestions \
for later versions or if you are a 3D designer that has some \
obstacles that I could add to new levels or so.\n\n\
\"I do not accept responsibility for any effects, \
adverse or otherwise, that my homepage or one of \
my programs or whatever may have on you, your \
computer, your sanity, your dog, and anything \
else that you can think of. Use it at your own risk.\"";

const char c_sFinishText[][40]={"","Congratulations,","you","have","finished","the","level.",""};

CGame::CGame(irr::IrrlichtDevice *pDevice)
{
	m_pDriver=pDevice->getVideoDriver   ();
	m_pSmgr  =pDevice->getSceneManager  ();
	m_pGuienv=pDevice->getGUIEnvironment();
	m_pTimer =pDevice->getTimer         ();
  m_pCursor=pDevice->getCursorControl ();

	m_pDevice=pDevice;
//	m_pSound =pSound ;

  m_iSelectedLevel=0;

  m_bLeftKey=false;
  m_bRghtKey=false;
  m_bRespawn=false;

  m_pDevice->setEventReceiver((irr::IEventReceiver *)(this));

  m_pFont=m_pGuienv->getFont("data/bigfont.png");
  m_pSmallFont=m_pGuienv->getFont("data/fontcourier.bmp");

  m_pGuienv->getSkin()->setFont(m_pSmallFont);
  m_pCursor->setVisible(false);
}

void CGame::beginScene() { m_pDriver->beginScene(true, true, irr::video::SColor(0,0,0,0)); }
void CGame::hideCursor() { m_pCursor->setVisible(false); }
void CGame::showCursor() { m_pCursor->setVisible(true ); }

void CGame::fillHiScoreTable()
{
wchar_t s[100],s2[100];

  m_pHiScores->clear();

  for (int i=0; i<10; i++)
    if (m_cLevels.getHiScore(m_iSelectedLevel/10,i)>0)
    {
      memset(s,100,sizeof(wchar_t));
      m_cLevels.getHiScoreName(m_iSelectedLevel/10,i,s);
      swprintf(s2,100,L"%-18s: %4.2f",s,((irr::f32)m_cLevels.getHiScore(m_iSelectedLevel/10,i))/1000);
      m_pHiScores->addItem(s2);
    }
}

int CGame::play(char *sLevelFileName, int iPlayMode)
{
int     i,x,y,iLastChar=0,iFinishTextNum=-1,iBounceCnt=0,iNextBeep=5000;
wchar_t s[100];
const   dReal *pVelocity;
bool    bHit=false,bFinishSound=false,bFinished=false,bGameOver=false;
char    sFinishText[100]="";

CLevelData *pLvl;

irr::scene::ISceneNode             *pGameOver;
irr::scene::ICameraSceneNode       *pCam;
irr::gui  ::IGUIImage              *pImages[3];

CMarble *pMarble;

irr::ode::CIrrOdeManager *pManager;

irr::f32 fCamRot=0, fCamHi=0, fGameOverRot=0;
irr::f32 fCamX, fCamY, fCamZ, fCamRad;
irr::f32 fMoveX, fMoveY;
irr::u32 iOldTime, iNewTime;
irr::u32 iChlTime;
irr::u32 iStartTime, iEndTime;
irr::f32 fTimeDiff, iImgTime;
irr::f32 fMoveMult;

irr::ode::IIrrOdeEntity *pEntity;

irr::gui::IGUIWindow     *pHiScoreWindow;
irr::gui::IGUIStaticText *pHiScoreText,*pHiScoreText2;

  m_fCamDist=15;
  m_bPlaying=true;

  m_bEnterHiScore=true;

  pManager=new irr::ode::CIrrOdeManager(m_pDevice);

//  pManager->setGravity(irr::core::vector3df(0,-5,0));

  pMarble=new CMarble(pManager,m_pDevice);
//  pMarble->setSoundEngine(m_pSound);

  pLvl=CLevelData::loadLevelData(getSelectedFile(),m_pDevice,pManager,pMarble);
  if (pLvl==NULL) return -1;

  pGameOver=m_pSmgr->getSceneNodeFromName(pLvl->m_sGameOver);
  if (pGameOver) pGameOver->setVisible(false);

  pHiScoreWindow=m_pGuienv->addWindow(irr::core::rect<irr::s32>(75,100,600,400),false,L"HiScore");
  m_pHiScores=m_pGuienv->addListBox(irr::core::rect<irr::s32>(5,25,250,300),pHiScoreWindow,-1,true);
  pHiScoreText =m_pGuienv->addStaticText(L"You have failed to challange\nthe best level time!",irr::core::rect<irr::s32>(260, 25,595,130),false,true,pHiScoreWindow);
  pHiScoreText2=m_pGuienv->addStaticText(L"HiScore: you made it!",irr::core::rect<irr::s32>(260,135,595,150),false,true,pHiScoreWindow);
  m_pHiScoreName=m_pGuienv->addEditBox(L"",irr::core::rect<irr::s32>(260,160,500,175),true,pHiScoreWindow);

  m_pHiScoreName->setMax(20);
  fillHiScoreTable();

  pHiScoreWindow->setVisible(false);

  x=m_pDriver->getScreenSize().Width;
  y=m_pDriver->getScreenSize().Height;

  pImages[0]=m_pGuienv->addImage(m_pDriver->getTexture("data/irrlichtlogo.bmp"),irr::core::position2d<irr::s32>(x- 93,y-36),false);
  pImages[1]=m_pGuienv->addImage(m_pDriver->getTexture("data/odelogo.bmp"     ),irr::core::position2d<irr::s32>(x-107,y-34),false);
  pImages[2]=m_pGuienv->addImage(m_pDriver->getTexture("data/logo_b2.bmp"     ),irr::core::position2d<irr::s32>(x-110,y-57),false);

  pCam=m_pSmgr->addCameraSceneNode(NULL,irr::core::vector3df(-5,5,5),irr::core::vector3df(5,5,-5));

  m_pSmgr->setShadowColor(irr::video::SColor(128,0,0,0));

  m_pCursor->setPosition(320,200);
  m_pTimer->start();
  iOldTime=m_pTimer->getRealTime();
  iStartTime=iOldTime;
  iEndTime=iOldTime;
  iImgTime=(irr::f32)iStartTime;
  for (i=0; i<3; i++) pImages[i]->setVisible(i==0);

  if (m_cLevels.getHiScore(m_iSelectedLevel/10,0)<pLvl->m_iChallange && m_cLevels.getHiScore(m_iSelectedLevel/10,0)!=0.0f) pLvl->m_iChallange=m_cLevels.getHiScore(m_iSelectedLevel/10,0);

  iChlTime=iStartTime+pLvl->m_iChallange;

  m_pDevice->run();
	while(m_pDevice->run() && (!bFinished || !m_bLeftKey || !bGameOver) && m_bPlaying)
	{
	  bFinished=pMarble->getNextCheckPoint()>=pLvl->m_iCheckCnt;

	  iNewTime=m_pTimer->getRealTime();
	  fTimeDiff=((irr::f32)(iNewTime-iOldTime))/500;

    for (i=0; i<pManager->getObjectCount(); i++)
      if (!strcmp(pManager->getObject(i)->getName(),"LevelObject"))
      {
        pEntity=pManager->getObject(i);
        ((CLevelObject *)pEntity)->addTime(iNewTime-iOldTime);
      }

    if (iPlayMode==1 && (int)(iChlTime)-(int)(iNewTime)<iNextBeep && iNextBeep>=0 && !bFinished && !bGameOver)
    {
//      m_pSound->play2D("sounds/timeout.ogg",false,false);
      iNextBeep-=1000;

      if (iNextBeep<0) bGameOver=true;
    }

	  iOldTime=iNewTime;

	  if (!fTimeDiff) fTimeDiff=(irr::f32)0.001;

	  if (fTimeDiff)
	    fMoveMult=2;
    else
      fMoveMult=2;

  	beginScene();

		if ((irr::f32)(iNewTime)-(irr::f32)(iImgTime)>10000.0f)
		{
		  iImgTime=(irr::f32)iNewTime;
		  x++;
		  if (x>2) x=0;
		  for (i=0; i<3; i++) pImages[i]->setVisible(i==x);
		}

    if (!bFinished && !bGameOver)
    {
      iEndTime=iNewTime;

      if (pMarble->getNextCheckPoint()<pLvl->m_iCheckCnt)
      {
        if (iPlayMode==0) swprintf(s,100,L"%.2f",((double)(iNewTime-iStartTime))/1000);
        if (iPlayMode==1) swprintf(s,100,L"%.2f",((double)(iChlTime-iNewTime  ))/1000);

        pLvl->m_pChecks[pMarble->getNextCheckPoint()]->pEdTime->setText(s);
      }

      pManager->worldStep(fTimeDiff);

      pMarble->frameCallback();

      if (pMarble->respawn())
      {
        irr::scene::ISceneNode *pCheckNode;

        if (pMarble->getNextCheckPoint()>0)
          pCheckNode=m_pSmgr->getSceneNodeFromName(pLvl->m_pChecks[pMarble->getNextCheckPoint()-1]->pObject->getObjName());
        else
          pCheckNode=NULL;

        if (pCheckNode)
          pMarble->setPosition(pCheckNode->getPosition()+irr::core::vector3df((irr::f32)pLvl->m_pChecks[pMarble->getNextCheckPoint()-1]->iRespawnX,(irr::f32)pLvl->m_pChecks[pMarble->getNextCheckPoint()-1]->iRespawnY,(irr::f32)pLvl->m_pChecks[pMarble->getNextCheckPoint()-1]->iRespawnZ));
       // else
          pMarble->setPosition(irr::core::vector3df((irr::f32)pLvl->m_iStartX,(irr::f32)pLvl->m_iStartY,(irr::f32)pLvl->m_iStartZ));
      }

      if (m_pCursor)
      {
        if (m_bRghtKey)
        {
          fCamRot=fCamRot+(320-m_pCursor->getPosition().X);
          fCamHi =fCamHi +(200-m_pCursor->getPosition().Y);

          if (fCamHi> 80) fCamHi= 80;
          if (fCamHi<-80) fCamHi=-80;
        }
        else
          if (m_bLeftKey)
            dBodySetTorque(pMarble->getBody(),0,0,0);
          else
          {
            fMoveX=(irr::f32)(m_pCursor->getPosition().X-320)/fMoveMult;
            fMoveY=(irr::f32)(m_pCursor->getPosition().Y-200)/fMoveMult;

            dBodyAddTorque(pMarble->getBody(),(dReal)(fMoveX*sin(fCamRot*irr::ode::GRAD_PI2)+fMoveY*cos(fCamRot*irr::ode::GRAD_PI2)),0,
                                              (dReal)(fMoveX*cos(fCamRot*irr::ode::GRAD_PI2)-fMoveY*sin(fCamRot*irr::ode::GRAD_PI2)));
          }

        m_pCursor->setPosition(320,200);
      }

      if (m_bLeftKey && pMarble->getNextCheckPoint()<pLvl->m_iCheckCnt)
      {
        irr::scene::ISceneNode *pNextCheck=m_pSmgr->getSceneNodeFromName(pLvl->m_pChecks[pMarble->getNextCheckPoint()]->pObject->getObjName());
        irr::core::vector3df    pCamTarget=pNextCheck->getPosition();

        pCam->setPosition(pMarble->getSceneNode()->getPosition()-8*(pCamTarget-pMarble->getSceneNode()->getPosition()).normalize()+irr::core::vector3df(0,4,0));
        pCam->setTarget(pCamTarget);
      }
      else
      {
        pCam->setPosition(irr::core::vector3df(pMarble->getSceneNode()->getPosition().X+fCamX,pMarble->getSceneNode()->getPosition().Y+fCamY,pMarble->getSceneNode()->getPosition().Z+fCamZ));
        pCam->setTarget(pMarble->getSceneNode()->getPosition());
      }
    }
    else
    {
      pMarble->stopRollingSound();

      if (!bFinishSound)
      {
        bFinishSound=true;
  //      if (bFinished)
//          m_pSound->play2D("sounds/applause.ogg",false,false);
//        else
 //         m_pSound->play2D("sounds/gameover.ogg",false,false);
      }


      pHiScoreWindow->setVisible(true);

      if (m_bEnterHiScore && bFinished)
      {
        m_iLevelTime=(int)(iEndTime-iStartTime);

        fillHiScoreTable();

        if (m_cLevels.getHiScorePos(m_iSelectedLevel/10,m_iLevelTime)<10)
        {
          swprintf(s,100,L"Congratulations.\n\nYou have entered the hi scores.\n\nYour Leveltime: %.2f sec\nYour position : %i.",((float)m_iLevelTime)/1000,m_cLevels.getHiScorePos(m_iSelectedLevel/10,m_iLevelTime)+1);
          m_pGuienv->setFocus(m_pHiScoreName);
          pHiScoreText->setText(s);
          swprintf(s,100,L"Please enter your name:");
          pHiScoreText2->setText(s);
        }
        else
        {
          swprintf(s,100,L"You have finished the level.");
          m_pHiScoreName->setVisible(false);
          pHiScoreText2->setVisible(false);
          m_bEnterHiScore=false;
        }
      }
      else
      {
        m_pHiScoreName->setVisible(false);
        pHiScoreText2->setVisible(false);
      }

      if (pGameOver)
      {
        pGameOver->setVisible(true);
        fGameOverRot-=(irr::f32)(20.0f*fTimeDiff);

        pGameOver->setRotation(irr::core::vector3df(0,fGameOverRot,0));
        pGameOver->setPosition(pMarble->getSceneNode()->getPosition()+irr::core::vector3df(0,2,0));
      }

      fCamRot+=(irr::f32)(10.0f*fTimeDiff);

      if (m_fCamDist<35) m_fCamDist+=(irr::f32)0.1;
      if (  fCamHi  <45)   fCamHi  +=(irr::f32)0.1;
      if (  fCamHi  >45)   fCamHi  -=(irr::f32)0.1;

      if ((int)iNewTime>iLastChar+1000 && iFinishTextNum<7)
      {
        iLastChar=iNewTime;
        iFinishTextNum++;
      }

      pCam->setPosition(irr::core::vector3df(pMarble->getSceneNode()->getPosition().X+fCamX,pMarble->getSceneNode()->getPosition().Y+fCamY,pMarble->getSceneNode()->getPosition().Z+fCamZ));
    }

    fCamRad=(irr::f32)(m_fCamDist*cos(fCamHi*irr::ode::GRAD_PI2));

    fCamX=(irr::f32)(m_fCamDist*sin(fCamRot*irr::ode::GRAD_PI2)*cos(fCamHi*irr::ode::GRAD_PI2));
    fCamY=(irr::f32)(m_fCamDist*sin(fCamHi*irr::ode::GRAD_PI2));
    fCamZ=(irr::f32)(m_fCamDist*cos(fCamRot*irr::ode::GRAD_PI2)*cos(fCamHi*irr::ode::GRAD_PI2));

    m_pSmgr->drawAll();

    pVelocity=dBodyGetLinearVel(pMarble->getBody());

//    swprintf(s,L"Velocity: %.2f",irr::core::vector3df(pVelocity[0],pVelocity[1],pVelocity[2]).getLength());
    m_pFont->draw(s,irr::core::rect<irr::s32>(10,m_pDriver->getScreenSize().Height-40,400,m_pDriver->getScreenSize().Height),irr::video::SColor(128,255,0,0));

//    if (iPlayMode==0) swprintf(s,L"Time: %.2f",(((irr::f32)iEndTime)-((irr::f32)iStartTime))/1000);
   // if (iPlayMode==1) swprintf(s,L"Time: %.2f",(((irr::f32)iChlTime)-((irr::f32)iEndTime  ))/1000);

    m_pFont->draw(s,irr::core::rect<irr::s32>(200,10,400,30),irr::video::SColor(128,255,0,0));

    if (bFinished && iPlayMode==1)
    {
      strcpy(sFinishText,c_sFinishText[iFinishTextNum]);
      m_pFont->draw(irr::core::stringw(sFinishText).c_str(),irr::core::rect<irr::s32>(200,70,400,90),irr::video::SColor(128,255,0,0));
    }

    if (bGameOver) m_pFont->draw(L"Game Over",irr::core::rect<irr::s32>(200,40,400,50),irr::video::SColor(128,255,0,0));

  	m_pGuienv->drawAll();
    m_pDriver->endScene();
  }

  delete pManager;

  m_pTimer->stop();
  m_pSmgr->clear();
  m_pGuienv->clear();

//  m_pSound->stopAllSounds();

  m_bLeftKey=false;

  free(pLvl);

  if (bFinished) return (int)(iEndTime-iStartTime); else return -1;
}

bool CGame::OnEvent(const irr::SEvent &event)
{
  if (event.EventType==irr::EET_MOUSE_INPUT_EVENT)
  {
    switch (event.MouseInput.Event)
    {
      case irr::EMIE_LMOUSE_PRESSED_DOWN : m_bLeftKey=true ; return true; break;
      case irr::EMIE_LMOUSE_LEFT_UP      : m_bLeftKey=false; return true; break;
      case irr::EMIE_RMOUSE_PRESSED_DOWN : m_bRghtKey=true ; return true; break;
      case irr::EMIE_RMOUSE_LEFT_UP      : m_bRghtKey=false; return true; break;
      case irr::EMIE_MOUSE_WHEEL :
        m_fCamDist=m_fCamDist+event.MouseInput.Wheel;
        if (m_fCamDist<15) m_fCamDist=15;
        if (m_fCamDist>35) m_fCamDist=35;
        return true;
        break;
    }
  }

  if (event.EventType==irr::EET_KEY_INPUT_EVENT)
  {
    if (event.KeyInput.Key==irr::KEY_ESCAPE)
    {
      m_bPlaying=false;
      return true;
    }

    if (m_bEnterHiScore && event.KeyInput.Key==irr::KEY_RETURN)
    {
      m_cLevels.addHiScore(m_iSelectedLevel/10,m_iLevelTime,m_pHiScoreName->getText());
      fillHiScoreTable();
      m_bEnterHiScore=false;
    }
  }

  return false;
}

int CGame::showMenu()
{
wchar_t sMenuItems[][15]={L"Level",L"Play",L"Challange",L"HiScore",L"About",L"Quit"},s[100],s2[100];
int iCurItem=0,iOldItem=0,iState=0,i,iSelTrk1=0,iSelTrk2=0;

irr::scene::ICameraSceneNode *pCam=m_pSmgr->addCameraSceneNode(NULL,irr::core::vector3df(6,0,-10),irr::core::vector3df(6,0,0));
irr::scene::ISceneNode       *pMenu=NULL;
irr::       f32               fRot=0.0f;
irr::gui  ::IGUIListBox      *pLevels,*pHiScore;
irr::gui  ::IGUIStaticText   *pLevelText,*pHiScoreText;
irr::gui  ::IGUIStaticText   *pAbout;

  m_pSmgr->loadScene("levels/menu.irr");

  pMenu=m_pSmgr->getSceneNodeFromName("menu");

  m_pDevice->run();

  fRot=pMenu->getRotation().X;

  pLevels=m_pGuienv->addListBox(irr::core::rect<irr::s32>(100,100,600,400),NULL,-1,true);
  pLevels->setVisible(false);
  pLevelText=m_pGuienv->addStaticText(L"Select Level...",irr::core::rect<irr::s32>(105,80,600,100));
  pLevelText->setVisible(false);

  pHiScore=m_pGuienv->addListBox(irr::core::rect<irr::s32>(100,100,600,400),NULL,-1,true);
  pHiScore->setVisible(false);
  pHiScoreText=m_pGuienv->addStaticText(L"HiScore",irr::core::rect<irr::s32>(105,80,600,100));
  pHiScoreText->setVisible(false);

  pAbout=m_pGuienv->addStaticText(g_cAboutText,irr::core::rect<irr::s32>(100,100,600,400),true,true,NULL,-1,true);
  pAbout->setVisible(false);

  for (i=0; i<m_cLevels.m_iLevels; i++) pLevels->addItem(irr::core::stringw(m_cLevels.m_pLevels[i].sName).c_str());

  if (pLevels->getItemCount()>0) pLevels->setSelected(0);

	while(m_pDevice->run() && iState!=2 && iState!=3 && iState!=6)
	{
  	beginScene();
    m_pSmgr->drawAll();
  	m_pGuienv->drawAll();


    switch (iState)
    {
      case 0:
   //     if (m_bLeftKey && iCurItem!=5) m_pSound->play2D("sounds/MouseClick.ogg",false,false);
        fRot+=m_pCursor->getPosition().Y-200;

        while (fRot>=360) fRot-=360;
        while (fRot<   0) fRot+=360;

        iOldItem=iCurItem;
        iCurItem=(int)((fRot+30)/60);
        while (iCurItem>=6) iCurItem-=6;

//        if (iCurItem!=iOldItem) m_pSound->play2D("sounds/MenuChange.ogg",false,false);

        if (m_bLeftKey) { iState=iCurItem+1; m_bLeftKey=false; }

        switch (iState)
        {
          case 1: pLevels->setVisible(true); pLevelText->setVisible(true); break;
          case 4:
            pHiScore->clear();
            swprintf(s,100,L"HiScore of level \"%s\"",irr::core::stringw(m_cLevels.m_pLevels[m_iSelectedLevel/10].sName).c_str());
            pHiScoreText->setText(s);

            for (i=0; i<10; i++)
              if (m_cLevels.getHiScore(m_iSelectedLevel/10,i)>0)
              {
                m_cLevels.getHiScoreName(m_iSelectedLevel/10,i,s2);
                swprintf(s,100,L"%s: %.3f",s2,((float)(m_cLevels.getHiScore(m_iSelectedLevel/10,i)))/1000);
                pHiScore->addItem(s);
              }

            pHiScore->setVisible(true);
            pHiScoreText->setVisible(true);

            break;
          case 5:
            pAbout->setVisible(true);
        }
        break;

      case 1:
        if (m_bLeftKey)
        {
          m_bLeftKey=false;
          iState=0;
          pLevels->setVisible(false);
          pLevelText->setVisible(false);
//          m_pSound->play2D("sounds/MenuBack.ogg",false,false);
        }

        m_iSelectedLevel+=m_pCursor->getPosition().Y-200;

        while (m_iSelectedLevel>10*(pLevels->getItemCount())) m_iSelectedLevel=10*pLevels->getItemCount()-1;
        while (m_iSelectedLevel<                           0) m_iSelectedLevel=0;

        iSelTrk1=m_iSelectedLevel/10;
//        if (iSelTrk1!=iSelTrk2) m_pSound->play2D("sounds/MenuChange.ogg",false,false);
        iSelTrk2=iSelTrk1;

        pLevels->setSelected(m_iSelectedLevel/10);
        break;

      case 4:
        if (m_bLeftKey)
        {
          m_bLeftKey=false;
          iState=0;
          pHiScore->setVisible(false);
          pHiScoreText->setVisible(false);
//          m_pSound->play2D("sounds/MenuBack.ogg",false,false);
        }
        break;

      case 5:
        if (m_bLeftKey)
        {
          m_bLeftKey=false;
          iState=0;
          pAbout->setVisible(false);
  //        m_pSound->play2D("sounds/MenuBack.ogg",false,false);
        }
        break;
    }

    if (iState==0)
      swprintf(s,100,L"Level: %s",irr::core::stringw(m_cLevels.m_pLevels[m_iSelectedLevel/10].sName).c_str());
    else
      swprintf(s,100,L"Mouseclick to return to menu");

    m_pFont->draw(s,irr::core::rect<irr::s32>(10,m_pDriver->getScreenSize().Height-40,400,m_pDriver->getScreenSize().Height),irr::video::SColor(128,255,0,0));
    m_pFont->draw(sMenuItems[iCurItem],irr::core::rect<irr::s32>(10,10,110,30),irr::video::SColor(128,255,0,0));

  	pMenu->setRotation(irr::core::vector3df(fRot,0,0));

  	m_pCursor->setPosition(320,200);
    m_pDriver->endScene();
  }

  m_pGuienv->clear();
  m_pSmgr->clear();
  return iState;
}
