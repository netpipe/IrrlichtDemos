  #include "CLevelData.h"
//  #include <irrXml.h>

CLevelData::CLevelData()
{
  int m_pObjects=0;
  m_iObjectCnt=0;
}

CLevelData::~CLevelData()
{
}

CLevelData *CLevelData::loadLevelData(const char *sFileName, irr::IrrlichtDevice *pDevice, irr::ode::CIrrOdeManager *pManager, CMarble *pMarble)
{
int iObjCnt=0,iChkCnt=0,i,x,y;
CLevelData *lvl=new CLevelData();
CLevelObject *pLvlObj=NULL;
irr::io::IrrXMLReader *pXml=irr::io::createIrrXMLReader(sFileName);
irr::gui::IGUIEnvironment *pGuiEnv=pDevice->getGUIEnvironment();
irr::video::IVideoDriver  *pDriver=pDevice->getVideoDriver();
irr::scene::ISceneNode             *levelNode;
irr::scene::IMeshSceneNode         *levelMesh;

  lvl->m_cObjectClasses.loadObjectData(pDevice);

  lvl->m_iCheckCnt =0;
  lvl->m_iObjectCnt=0;

  while (pXml && pXml->read())
  {
    if (!strcmp(pXml->getNodeName(),"level"))
    {
      strcpy(lvl->m_sLevelName,pXml->getAttributeValue("name"  ));
      strcpy(lvl->m_sGameOver ,pXml->getAttributeValue("gameover"));

      pDevice->getSceneManager()->loadScene(lvl->m_sLevelName);

      lvl->m_fRespawn=pXml->getAttributeValueAsFloat("respawn");
      lvl->m_iChallange=pXml->getAttributeValueAsInt("challange")*1000;

      pMarble->setRespawn(lvl->m_fRespawn);
    }

    if (!strcmp(pXml->getNodeName(),"start"))
    {
      lvl->m_iStartX=pXml->getAttributeValueAsInt("x");
      lvl->m_iStartY=pXml->getAttributeValueAsInt("y");
      lvl->m_iStartZ=pXml->getAttributeValueAsInt("z");

//      pMarble->setPosition(irr::core::vector3df((irr::f32)lvl->m_iStartX,(irr::f32)lvl->m_iStartY,(irr::f32)lvl->m_iStartZ));
    }

    if (!strcmp(pXml->getNodeName(),"object"))
    {
      pLvlObj=new CLevelObject(pManager,pDevice);
      pLvlObj->setMarble(pMarble);
//      pLvlObj->setSoundEngine(pSound);

      pLvlObj->setObjName((irr::c8 *)pXml->getAttributeValue("name" ));

      levelNode=pDevice->getSceneManager()->getSceneNodeFromName(pLvlObj->getObjName());
      levelMesh=(irr::scene::IMeshSceneNode *)levelNode;

      pLvlObj->setMesh     (levelMesh->getMesh());
      pLvlObj->setSceneNode(levelNode);
      pLvlObj->setGeomData();

      if (pXml->getAttributeValue("class"))
        pLvlObj->setClass((irr::c8 *)pXml->getAttributeValue("class"));
      else
        pLvlObj->setClass("");

      pLvlObj->setMass(pXml->getAttributeValueAsFloat("mass"));

      for (i=0; i<lvl->m_cObjectClasses.m_iObjCnt; i++)
        if (!strcmp(lvl->m_cObjectClasses.m_pObjectData[i]->m_sName,pLvlObj->getClass()))
          pLvlObj->setObjectData(lvl->m_cObjectClasses.m_pObjectData[i]);

      iObjCnt++;
      lvl->m_iObjectCnt++;
      lvl->m_pObjects.push_back(pLvlObj);
      pLvlObj->updateMass();
    }

    if (!strcmp(pXml->getNodeName(),"check"))
    {
      lvl->m_pChecks.push_back((_CHECK *)malloc(sizeof(_CHECK)));
      lvl->m_pChecks[iChkCnt]->iNum=pXml->getAttributeValueAsInt("num");

      lvl->m_pChecks[iChkCnt]->iRespawnX=pXml->getAttributeValueAsInt("x");
      lvl->m_pChecks[iChkCnt]->iRespawnY=pXml->getAttributeValueAsInt("y");
      lvl->m_pChecks[iChkCnt]->iRespawnZ=pXml->getAttributeValueAsInt("z");

      strcpy(lvl->m_pChecks[iChkCnt]->sImg  ,pXml->getAttributeValue("img"   ));
      strcpy(lvl->m_pChecks[iChkCnt]->sImgOk,pXml->getAttributeValue("img_ok"));
      strcpy(lvl->m_pChecks[iChkCnt]->sLight,pXml->getAttributeValue("light" ));

      lvl->m_pChecks[iChkCnt]->pImgTexture  =pDriver->getTexture(lvl->m_pChecks[iChkCnt]->sImg  );
      lvl->m_pChecks[iChkCnt]->pImgTextureOk=pDriver->getTexture(lvl->m_pChecks[iChkCnt]->sImgOk);

      x=10;
      y=10+25*iChkCnt;

      lvl->m_pChecks[iChkCnt]->pImg  =pGuiEnv->addImage(lvl->m_pChecks[iChkCnt]->pImgTexture  ,irr::core::position2d<irr::s32>(x,y),false);
      lvl->m_pChecks[iChkCnt]->pImgOk=pGuiEnv->addImage(lvl->m_pChecks[iChkCnt]->pImgTextureOk,irr::core::position2d<irr::s32>(x,y),false);
      lvl->m_pChecks[iChkCnt]->pEdTime=pGuiEnv->addEditBox(L" time",irr::core::rect<irr::s32>(x+80,y,x+150,y+22));

      lvl->m_pChecks[iChkCnt]->pImgOk->setVisible(false);

      for (i=0; i<lvl->m_iObjectCnt; i++)
        if (!strcmp(lvl->m_pObjects[i]->getObjName(),pXml->getAttributeValue("name")))
        {
          lvl->m_pObjects[i]->setCheckPoint(iChkCnt);
          lvl->m_pObjects[i]->setCheckLight(pDevice->getSceneManager()->getSceneNodeFromName(lvl->m_pChecks[iChkCnt]->sLight));
          lvl->m_pObjects[i]->setImages(lvl->m_pChecks[iChkCnt]->pImg,lvl->m_pChecks[iChkCnt]->pImgOk);
          lvl->m_pChecks[iChkCnt]->pObject=lvl->m_pObjects[i];
          iChkCnt++;
          i=lvl->m_iObjectCnt;
        }

      lvl->m_iCheckCnt++;
    }
  }

  delete(pXml);
  return lvl;
}

void CObjects::loadObjectData(irr::IrrlichtDevice *pDevice)
{
irr::ode::CObjData *pObjData;
irr::io::IrrXMLReader *pXml=irr::io::createIrrXMLReader("Levels/objects.xml");

  m_iObjCnt=0;
  while (pXml && pXml->read())
    if (!strcmp(pXml->getNodeName(),"object"))
    {
      pObjData=new irr::ode::CObjData();

      pObjData->m_ax=pXml->getAttributeValueAsFloat("ax");
      pObjData->m_ay=pXml->getAttributeValueAsFloat("ay");
      pObjData->m_az=pXml->getAttributeValueAsFloat("az");

      pObjData->m_fSlip1=pXml->getAttributeValueAsFloat("slip1"); if (pObjData->m_fSlip1==0.0f) pObjData->m_fSlip1=1.0f;
      pObjData->m_fSlip2=pXml->getAttributeValueAsFloat("slip2"); if (pObjData->m_fSlip2==0.0f) pObjData->m_fSlip2=1.0f;

      pObjData->m_iDelay=pXml->getAttributeValueAsInt("delay");

      strcpy(pObjData->m_sName ,pXml->getAttributeValue("name" ));

      if (pXml->getAttributeValue("sound")) strcpy(pObjData->m_sSound,pXml->getAttributeValue("sound"));

      m_pObjectData.push_back(pObjData);

      m_iObjCnt++;
    }

  if (pXml) delete pXml;
}
