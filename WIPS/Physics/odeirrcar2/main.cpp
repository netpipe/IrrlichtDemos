
#ifdef _MSC_VER

#pragma comment(lib, "Irrlicht.lib")
#endif
#ifdef _IRR_WINDOWS_

#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#include "PlayerCar.h"
#include "NPCCar.h"
#include <vector>
#include "GUI.h"
#include "Cube.h"








int main()
{

	video::E_DRIVER_TYPE driverType = driverChoiceConsole();

	MyEventReceiver receiver;

	IrrlichtDevice* device = createDevice(driverType,
			core::dimension2d<u32>(1024, 768), 32, false, false, false, &receiver);
	receiver.setDevice(device);

	//  dimension2d cSize=driver->getScreenSize();

	if (device == 0)
		return; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	// инициализация меню
	 GUI::getInstance()->setDevice(device);
	GUI::getInstance()->setMenuState(0);









	device->getFileSystem()->addFolderFileArchive("data/");



  //ODE
  CIrrOdeSceneNodeFactory cFactory(smgr);
  smgr->registerSceneNodeFactory(&cFactory);
  //set the scene manager
  CIrrOdeManager::getSharedInstance()->install(device);
  //init the ODE
  CIrrOdeManager::getSharedInstance()->initODE();


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //fadd a worldNode
  ISceneNode *pNode1=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_WORLD_NAME),smgr->getRootSceneNode());
  CIrrOdeWorld *worldNode=reinterpret_cast<CIrrOdeWorld *>(pNode1);
  worldNode->setGravity(vector3df(0,-10.0f,0));
  worldNode->setName("world");
  //worldNode->setMaxAngularSpeed(1);

  scene::IAnimatedMesh *mesh=smgr->getMesh("map.bsp");

  if (mesh) {
    //for optimization we use an octree scene node...
    scene::IMeshSceneNode *node=smgr->addOctreeSceneNode(mesh,worldNode); //->addAnimatedMeshSceneNode(mesh,worldNode);
    node->setName("level");
    //node->setScale(vector3df(300.0f,10.0f,300.0f));
    if (node) {
      node->setPosition(core::vector3df(0,-50,0));

      //for (u32 x=0; x<node->getMaterialCount(); x++) node->getMaterial(x).Wireframe=true;
      //... and for detailed collision detection we use a trimesh.
      CIrrOdeGeomTrimesh *pTri=reinterpret_cast<CIrrOdeGeomTrimesh *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_TRIMESH_NAME),node));
      pTri->setName("trimesh");

      //One important thing: a static trimesh has one surface for every meshbuffer (aka "material") of the parent scene node
      for (u32 j=0; j<pTri->getSurfaceParametersCount(); j++) {
        pTri->getSurfaceParameters(j)->setMu(1000.000000);
      }

      pTri->drop();
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		vector <vector3df> way1;
		way1.push_back(vector3df(-300,10,-300));
		way1.push_back(vector3df(500,10,500));
		way1.push_back(vector3df(-500,10,300));
		way1.push_back(vector3df(-500,10,-300));
		way1.push_back(vector3df(50,10,-500));


		f32 start = device->getTimer()->getTime();


   PlayerCar* playerCar = new PlayerCar(device,worldNode, "models/car.obj","models/cartext.psd",&receiver);
   NPCCar* npc1 = NULL;


		npc1 = new NPCCar(device,worldNode, way1, "models/car.obj","models/cartext.psd",&receiver);
		 //playerCar = new PlayerCar(device,worldNode, "models/car.obj","models/cartext.psd",&receiver);

			smgr->addCameraSceneNodeFPS();
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  smgr->saveScene("test.xml");
  worldNode->initPhysics();

  ICameraSceneNode *cam=smgr->addCameraSceneNodeFPS();
  //cam=smgr->addCameraSceneNodeMaya();
  cam->setPosition(core::vector3df(-20.0f, 15.0f, -20.0f));
  device->getCursorControl()->setVisible(false);
  cam->setFarValue(50000.0f);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//--------------------CREATING ROAD------------------------------------
//	/*
//	CIrrOdeSceneNodeFactory cFactory(smgr);
//	smgr->registerSceneNodeFactory(&cFactory);
//
//	CIrrOdeManager::getSharedInstance()->install(device);
//
// CIrrOdeManager::getSharedInstance()->initODE();
//
//
// /* IMeshSceneNode* planeNode;
//  planeNode = smgr->addSceneNode(
//		CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_WORLD_NAME),
//		smgr->getRootSceneNode());
//	worldNode = reinterpret_cast<CIrrOdeWorld *>(planeNode);
//		CIrrOdeWorld *worldNode;
//	worldNode->setGravity(vector3df(0,-10,0));
//	//planeNode - Сцена представляющая дорогу
//	irr::video::SMaterial* roadMaterial = new SMaterial();
//	roadMaterial->setTexture(0,driver->getTexture("models/road.psd"));
//	roadMaterial->Lighting = false;
//	IMesh* fterr;
//
//	fterr = smgr->addHillPlaneMesh("",dimension2d<f32>(100,100),dimension2d<u32>(25,25),roadMaterial,1,dimension2d<f32>(200,200),dimension2d<f32>(3,3));
//	planeNode = smgr->addMeshSceneNode(fterr);
//	*/
//
//
//	/*
//  ISceneNode *pNode1 = smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_WORLD_NAME),smgr->getRootSceneNode());
//
//    CIrrOdeWorld *worldNode=reinterpret_cast<CIrrOdeWorld *>(pNode1);
//    worldNode->setGravity(vector3df(0,-98.1,0));
//    worldNode->setName("world");
//
//	*/
//	    ISceneNode *pNode=smgr->addSceneNode(
//          CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_WORLD_NAME),
//          smgr->getRootSceneNode());
//    CIrrOdeWorld *worldNode=reinterpret_cast<CIrrOdeWorld *>(pNode);
//	worldNode->setGravity(vector3df(0,-10.0f,0));
//
//	/*
  /*
	 scene::IQ3LevelMesh* map_mesh;
	  device->getFileSystem()->addZipFileArchive("models/map.pk3");
     map_mesh = (scene::IQ3LevelMesh*)smgr->getMesh("20kdm2.bsp");

	 if (map_mesh)
    {
		cout<<"YES!!!!!"<<endl;
        //for optimization we use an octree scene node...
        scene::IMeshSceneNode *node=smgr->addOctreeSceneNode(map_mesh,worldNode);
        node->setName("level");
        if (node)
        {
            node->setPosition(vector3df(-2400,-2530,-850));
			node->setScale(node->getScale()*2);
            //for (u32 x=0; x<node->getMaterialCount(); x++) node->getMaterial(x).Wireframe=true;
            //... and for detailed collision detection we use a trimesh.
            CIrrOdeGeomTrimesh *pTri=reinterpret_cast<CIrrOdeGeomTrimesh *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_TRIMESH_NAME),node));
          //  pTri->setName("trimesh");

            //One important thing: a static trimesh has one surface for every meshbuffer (aka "material") of the parent scene node
            for (u32 j=0; j<pTri->getSurfaceParametersCount(); j++)
            {
                pTri->getSurfaceParameters(j)->setMu(1000);
                pTri->getSurfaceParameters(j)->setMu2(0);
            }

            pTri->drop();
        }
    }
	*/



//



//
//	//------------массив путей движения машины NPC---------------------------------------
//
//





/*
while(device->run())
{
	driver->beginScene(true, true, SColor(0,200,200,200));

    smgr->drawAll();

    device->getGUIEnvironment()->drawAll();
    driver->endScene();
    CIrrOdeManager::getSharedInstance()->step();
}

  device->drop();
  */
	while(device->run())
	{

		switch(GUI::getInstance()->getMenuState())
		{
			case 0:// игра
				{
						device->getCursorControl()->setVisible(false);
					if( device->getTimer()->getTime() - start > 10 && receiver.WasKeyDown() )
					{
						if(receiver.IsKeyDown(irr::KEY_KEY_W))
							playerCar->PRESSED_KEY_W();
						else if(receiver.IsKeyDown(irr::KEY_KEY_S))
							playerCar->PRESSED_KEY_S();
						if(receiver.IsKeyDown(irr::KEY_KEY_A))
							playerCar->PRESSED_KEY_A();
						else if(receiver.IsKeyDown(irr::KEY_KEY_D))
						playerCar->PRESSED_KEY_D();
						if(receiver.IsKeyDown(irr::KEY_SPACE))
							playerCar->PRESSED_KEY_SPACE();
						if(receiver.IsKeyDown(irr::KEY_KEY_H))
							playerCar->ChangeCameraMode();
						start = device->getTimer()->getTime();
						if(receiver.IsKeyDown(irr::KEY_ESCAPE))
							GUI::getInstance()->setMenuState(2);
					}




						driver->beginScene(true, true, video::SColor(255,113,113,133));
						playerCar->Update();
						npc1->Update();

						smgr->drawAll();

						driver->endScene();
						 CIrrOdeManager::getSharedInstance()->step();
					}

				break;
			default: // меню паузы
				{
					driver->beginScene(true, true, video::SColor(255,113,113,133));


					device->getGUIEnvironment()->drawAll();
					driver->endScene();
				}
				break;

		case -1:
			{


				driver->beginScene(true, true, video::SColor(255,113,113,133));
				if(npc1 != NULL)
				delete npc1;
					npc1 = new NPCCar(device,worldNode, way1, "models/car.obj","models/cartext.psd",&receiver);
				if(playerCar != NULL)
					delete playerCar;
				 playerCar = new PlayerCar(device,worldNode, "models/car.obj","models/cartext.psd",&receiver);
				 worldNode->initPhysics();
						GUI::getInstance()->setMenuState(0);

				smgr->addCameraSceneNodeFPS();


				//device->getGUIEnvironment()->drawAll();
					driver->endScene();
			}
			break;
		}

	}
	device->drop();


}

