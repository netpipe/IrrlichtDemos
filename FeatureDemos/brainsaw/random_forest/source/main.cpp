  #include <stdio.h>
  #include <stdlib.h>

  #include <irrlicht.h>

  #include "CRandomForestNode.h"
  #include "CMeshCombiner.h"

class CRunner : public irr::IEventReceiver {
  protected:
    irr::IrrlichtDevice *m_pDevice;
    irr::scene::ISceneManager *m_pSmgr;
    irr::gui::IGUIEnvironment *m_pGuienv;
    irr::video::IVideoDriver *m_pDrv;

    irr::s32 m_iState,m_iCount;
    irr::gui::IGUIButton *m_pBtnCombine,
                         *m_pBtnNoCombine;

    CRandomForest *m_pForest;
  public:
    virtual bool OnEvent (const SEvent &event) {
      if (m_iState==0) {
        if (event.EventType==irr::EET_GUI_EVENT) {
          if (event.GUIEvent.EventType==irr::gui::EGET_BUTTON_CLICKED) {
            irr::core::array<irr::scene::ISceneNode *> aTrees=m_pForest->getGeneratedTrees();
            m_iCount=aTrees.size();
            if (event.GUIEvent.Caller==m_pBtnCombine) {
              irr::core::array<irr::scene::IMeshSceneNode *> aMeshSceneNodes;

              for (s32 i=0; i<m_iCount; i++) {
                irr::scene::IMeshSceneNode *p=(irr::scene::IMeshSceneNode *)aTrees[i];
                aMeshSceneNodes.push_back(p);
              }

              CMeshCombiner *pCombine=new CMeshCombiner(0.8f);

              irr::scene::IMesh *pCombined=pCombine->combineMeshes(m_pSmgr,m_pDrv,aMeshSceneNodes,"MyCombinedTrees");

              if (pCombined!=NULL) {
                irr::scene::IMeshSceneNode *pNode=m_pSmgr->addMeshSceneNode(pCombined,m_pSmgr->getRootSceneNode());
                for (u32 i=0; i<pNode->getMaterialCount(); i++) {
                  pNode->getMaterial(i).setFlag(irr::video::EMF_LIGHTING,false);
                  pNode->getMaterial(i).setFlag(irr::video::EMF_ZBUFFER,true);
                  pNode->getMaterial(i).setFlag(irr::video::EMF_ZWRITE_ENABLE,true);
                  pNode->getMaterial(i).MaterialType=irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
                }
              }
              m_iState=1;
            }

            if (event.GUIEvent.Caller==m_pBtnNoCombine) {
              m_iState=1;
            }
          }
        }
        if (m_iState==1) {
          irr::scene::ICameraSceneNode *p=m_pSmgr->addCameraSceneNodeFPS();
          m_pGuienv->clear();
          m_pSmgr->setActiveCamera(p);
        }
      }

      return false;
    }

    void run() {
      m_pDevice=irr::createDevice(irr::video::EDT_OPENGL);
      m_pSmgr=m_pDevice->getSceneManager();
      m_pGuienv=m_pDevice->getGUIEnvironment();
      m_pDrv=m_pDevice->getVideoDriver();

      m_pDevice->setEventReceiver(this);
      m_iCount=-1;

      /*CRandomForest *theForest=new CRandomForest(NULL,m_pSmgr);

      irr::core::array<irr::u32> aInit;
      aInit.push_back(123);
      aInit.push_back(234);
      aInit.push_back(345);
      aInit.push_back(456);

      theForest->setSeed(aInit);

      theForest->addToPolygon(irr::core::position2di(  50,  50));
      theForest->addToPolygon(irr::core::position2di( 300, 400));
      theForest->addToPolygon(irr::core::position2di( 610, 150));
      theForest->addToPolygon(irr::core::position2di(1010, 600));
      theForest->addToPolygon(irr::core::position2di(1240,1000));
      theForest->addToPolygon(irr::core::position2di( 650,1600));
      theForest->addToPolygon(irr::core::position2di(  35, 300));

      theForest->setStepSize(75,75);
      theForest->setThreshold(500);
      theForest->setPosition(irr::core::vector3df(500.0f,0.0f,500.0f));

      irr::scene::ITerrainSceneNode *terrain=m_pSmgr->addTerrainSceneNode(
                    "Resources/terrain-heightmap.bmp",0,-1,
                    irr::core::vector3df(0.f, 0.f, 0.f),
                    irr::core::vector3df(0.f, 0.f, 0.f),
                    irr::core::vector3df(40.f, 4.4f, 40.f),
                    irr::video::SColor ( 255, 255, 255, 255 ),
                    5,irr::scene::ETPS_17,4);

      terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
      terrain->setMaterialTexture(0,m_pDrv->getTexture("Resources/terrain-texture.jpg"));
      terrain->setMaterialTexture(1,m_pDrv->getTexture("Resources/detailmap3.jpg"));
      terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);
      terrain->scaleTexture(1.0f, 20.0f);
      terrain->setName("terrain");

      irr::scene::ISceneNode *pTemplate=m_pSmgr->addEmptySceneNode(m_pSmgr->getRootSceneNode());
      pTemplate->setName("tree_template");

      irr::scene::IMesh *tm1=m_pSmgr->getMesh("Resources/birchA.obj"),
                        *tm2=m_pSmgr->getMesh("Resources/bigtreeB.obj"),
                        *tm3=m_pSmgr->getMesh("Resources/fruittreelow.obj"),
                        *tm4=m_pSmgr->getMesh("Resources/lindgreentreelow.obj");

      irr::scene::IMeshSceneNode *tn1=m_pSmgr->addMeshSceneNode(tm1,pTemplate,-1,irr::core::vector3df(-5,0,0)),
                                 *tn2=m_pSmgr->addMeshSceneNode(tm2,pTemplate,-1,irr::core::vector3df( 0,0,0)),
                                 *tn3=m_pSmgr->addMeshSceneNode(tm3,pTemplate,-1,irr::core::vector3df( 5,0,0)),
                                 *tn4=m_pSmgr->addMeshSceneNode(tm4,pTemplate,-1,irr::core::vector3df(10,0,0));

      tn1->setScale(irr::core::vector3df(2.5f,2.5f,2.5f));
      tn2->setScale(irr::core::vector3df(2.5f,2.5f,2.5f));
      tn3->setScale(irr::core::vector3df(2.5f,2.5f,2.5f));
      tn4->setScale(irr::core::vector3df(2.5f,2.5f,2.5f));

      pTemplate->addChild(tn1); tn1->setParent(pTemplate);
      pTemplate->addChild(tn2); tn2->setParent(pTemplate);
      pTemplate->addChild(tn3); tn3->setParent(pTemplate);
      pTemplate->addChild(tn4); tn4->setParent(pTemplate);

      theForest->setResize(0.15f);
      theForest->setTemplateNodeName("tree_template");
      theForest->setTerrainNodeName("terrain");

      theForest->plantTrees();*/

      m_pBtnCombine  =m_pGuienv->addButton(irr::core::rect<s32>(5, 5,125,25),NULL,-1,L"Combine Trees");
      m_pBtnNoCombine=m_pGuienv->addButton(irr::core::rect<s32>(5,30,125,50),NULL,-1,L"Don't Combine Trees");

      CRandomForestFactory *pFactory=new CRandomForestFactory(m_pSmgr);
      m_pSmgr->registerSceneNodeFactory(pFactory);
      pFactory->drop();

      m_pSmgr->loadScene("resources/random_forest.xml");

      m_pForest=(CRandomForest *)m_pSmgr->getSceneNodeFromName("the_forest");

      m_pSmgr->addCameraSceneNode();
      s32 lastFPS=-1;
      m_iState=0;

      while (m_pDevice->run()) {
        m_pDrv->beginScene(true,true,irr::video::SColor(255,128,128,128));
        m_pSmgr->drawAll();
        m_pGuienv->drawAll();
        m_pDrv->endScene();

        s32 fps=m_pDrv->getFPS();

        if (lastFPS != fps) {
          core::stringw str = L"RandomForestNode Plugin Demo [";
          str+=m_pDrv->getName();
          str+="] FPS:";
          str+=fps;
          if (m_iCount!=-1) {
            str+=", Number of Trees: ";
            str+=m_iCount;
          }

          m_pDevice->setWindowCaption(str.c_str());
          lastFPS=fps;
        }
      }

      m_pDevice->drop();
    }
};

int main() {
  CRunner theRunner;
  theRunner.run();
  return 0;
}
