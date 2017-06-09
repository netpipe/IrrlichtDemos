#include "CCore.h"

#include "shader.h"

namespace island
{
	CCore::CCore()
	{
		setup();
	}

	CCore::~CCore()
	{
		stop();
	}

	void CCore::render()
	{
		while(device->run())
		{
			//run controller
			this->controller->inclinedPlane();
			this->controller->difficulty();

			//update camera
			this->cam->update();

			//render
			getVideoDriver()->beginScene(true, true, irr::video::SColor(255,100,101,140));

			getSceneManager()->drawAll();
			getGUIEnvironment()->drawAll();

			getVideoDriver()->endScene();
		}
	}

	/*
	 * set the objects and default data
	 */
	void CCore::setup()
	{

#ifdef _IRR_WINDOWS_
		device = irr::createDevice(irr::video::EDT_DIRECT3D9,irr::core::dimension2d<irr::s32>(1024,768),32,false,true,true);
#else
		device = irr::createDevice(irr::video::EDT_OPENGL,irr::core::dimension2du(1024,768),32,false,true,true);
#endif

		device->setWindowCaption(L"TheIsland");
		//Add a camera node
		getSceneManager()->addCameraSceneNode();

		irr::core::dimension2du screen = getVideoDriver()->getScreenSize();
		getGUIEnvironment()->addStaticText(L"Strong99 (Rex van Dorp) and Miintz (Maarten van Hees)",irr::core::rect<irr::s32>(screen.Width - 128,screen.Height - 64,screen.Width-2,screen.Height-32));
		getGUIEnvironment()->addStaticText(L"Irrlicht motion (christmass) competition - 2008-2009",irr::core::rect<irr::s32>(screen.Width - 128,screen.Height - 32,screen.Width-2,screen.Height-2));

		//Add skydome if texture is found
		irr::video::ITexture* skytexture = getVideoDriver()->getTexture("textures/panorama.jpg");
		if (skytexture)
			getSceneManager()->addSkyDomeSceneNode(skytexture,20,12,1,2.0f,0);

		//Creates the sun
		irr::scene::ILightSceneNode* sun = getSceneManager()->addLightSceneNode(0,irr::core::vector3df(900,2000,0),irr::video::SColorf(1,1,1),3500);

		//create the island
		island = 0;
		irr::scene::IAnimatedMesh* islandMesh = getSceneManager()->getMesh("models/island_test.x");
		irr::scene::IAnimatedMesh* islandDecorMesh = getSceneManager()->getMesh("models/island_decor.x");
		if (islandMesh) {
			//getSceneManager()->getMeshManipulator()->makePlanarTextureMapping(islandMesh,2);
			//create island
			island = getSceneManager()->addAnimatedMeshSceneNode(islandMesh);
			irr::scene::ISceneNode* islandDecor = getSceneManager()->addAnimatedMeshSceneNode(islandDecorMesh,island);

			irr::video::IGPUProgrammingServices* gpu = getVideoDriver()->getGPUProgrammingServices();
			irr::s32 newMaterialType = 0;

			if (gpu)
			{
				CGroundShader* mc = new CGroundShader(irr::core::vector3df(900,2000,0),true);

				// create the shaders depending on if the user wanted high level
				// or low level shaders:
					// create material from high level shaders (hlsl or glsl)

				if (getVideoDriver()->getDriverType() == irr::video::EDT_OPENGL)
				{
					newMaterialType = gpu->addHighLevelShaderMaterialFromFiles(
						"shaders/terrain.vert", "main", irr::video::EVST_VS_1_1,
						"shaders/terrain.frag", "main", irr::video::EPST_PS_1_1,
						mc, irr::video::EMT_SOLID);
				}
				else
				{
					newMaterialType = gpu->addHighLevelShaderMaterialFromFiles(
						"shaders/terrain_vs.hlsl", "vs_main", irr::video::EVST_VS_2_0,
						"shaders/terrain_vs.hlsl", "ps_main", irr::video::EPST_PS_2_0,
						mc, irr::video::EMT_SOLID);
				}
				island->setMaterialType((irr::video::E_MATERIAL_TYPE)newMaterialType);
				island->setMaterialFlag(irr::video::EMF_LIGHTING,false);
				island->setMaterialTexture(0,getVideoDriver()->getTexture("textures/rock.jpg"));
				island->setMaterialTexture(1,getVideoDriver()->getTexture("textures/grass1.jpg"));

				mc->drop();
			}

			irr::scene::ITriangleSelector* selectorIsland = getSceneManager()->createTriangleSelector(islandMesh,island);
			island->setTriangleSelector(selectorIsland);
			selectorIsland->drop();
		}

		//add gui elements
		irr::video::ITexture* barTexture = getVideoDriver()->getTexture("textures/bar.png");
		irr::gui::IGUIImage* imageBar1_points  = getGUIEnvironment()->addImage(barTexture,irr::core::position2d<irr::s32>(20,20),true,0,101);
		irr::gui::IGUIImage* imageBar2_ballons = getGUIEnvironment()->addImage(barTexture,irr::core::position2d<irr::s32>(20,40),true,0,102);
		irr::gui::IGUIImage* imageBar3_wagons  = getGUIEnvironment()->addImage(barTexture,irr::core::position2d<irr::s32>(20,60),true,0,103);
		irr::gui::IGUIImage* imageBar4_angle   = getGUIEnvironment()->addImage(barTexture,irr::core::position2d<irr::s32>(20,80),true,0,104);

		irr::gui::IGUIStaticText* bar1_points  = getGUIEnvironment()->addStaticText(L"Points",irr::core::rect<irr::s32>(22,22,100,34),false,false,0,111,false);
		irr::gui::IGUIStaticText* bar2_ballons = getGUIEnvironment()->addStaticText(L"Ballons",irr::core::rect<irr::s32>(22,42,100,54),false,false,0,112,false);
		irr::gui::IGUIStaticText* bar3_wagons  = getGUIEnvironment()->addStaticText(L"Wagons",irr::core::rect<irr::s32>(22,62,100,74),false,false,0,113,false);
		irr::gui::IGUIStaticText* bar4_angle   = getGUIEnvironment()->addStaticText(L"ESC to quit",irr::core::rect<irr::s32>(22,82,100,94),false,false,0,114,false);

		getGUIEnvironment()->getSkin()->setColor(irr::gui::EGDC_BUTTON_TEXT,irr::video::SColor(255,255,255,255));

		//Create the player and island controller
		controller = new CController(this,island);
		device->setEventReceiver(controller);
	}

	/*
	 * stops everything and clears the buffers
	 */
	void CCore::stop()
	{
		device->closeDevice();
	}

	void CCore::restart()
	{
		delete controller;
		//controller = 0;

		//Create the player and island controller
		controller = new CController(this,island);
		device->setEventReceiver(controller);
	}

	////////////////////////////////////////////////////
	//////// return irrlicht default pointers //////////
	////////////////////////////////////////////////////
	irr::gui::ICursorControl* CCore::getCursor()
	{
		return device->getCursorControl();
	}

	irr::scene::ISceneManager* CCore::getSceneManager()
	{
		return device->getSceneManager();
	}

	irr::gui::IGUIEnvironment* CCore::getGUIEnvironment()
	{
		return device->getGUIEnvironment();
	}

	irr::video::IVideoDriver* CCore::getVideoDriver()
	{
		return device->getVideoDriver();
	}

	irr::s32 CCore::getRealTime()
	{
		return device->getTimer()->getRealTime();
	}
}
