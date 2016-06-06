#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;
using namespace io;

#pragma comment(lib, "Irrlicht.lib")


class MyEventReceiver : public IEventReceiver
{
public:

	MyEventReceiver(scene::ISceneNode* terrain)
	{
		// store pointer to terrain so we can change its drawing mode
		Terrain = terrain;
	}

	bool OnEvent(const SEvent& event)
	{
		// check if user presses the key 'W' or 'D'
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_KEY_W: // switch wire frame mode
				Terrain->setMaterialFlag(video::EMF_WIREFRAME, !Terrain->getMaterial(0).Wireframe);
				Terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
				return true;
			case irr::KEY_KEY_P: // switch wire frame mode
				Terrain->setMaterialFlag(video::EMF_POINTCLOUD, !Terrain->getMaterial(0).PointCloud);
				Terrain->setMaterialFlag(video::EMF_WIREFRAME, false);
				return true;
			case irr::KEY_KEY_D: // toggle detail map
				Terrain->setMaterialType(
					Terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
					video::EMT_DETAIL_MAP : video::EMT_SOLID);
				return true;
			}
		}

		return false;
	}

private:
	scene::ISceneNode* Terrain;
};


int main()
{
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	printf("\n Select terrain method:\n"\
		" (1) Disabled Backface culling with inverted heightmap. \n"\
		" (2) Mirrored heightmap with flipped terrain. \n"\
		" (3) Inverted Y-Axis scale on second terrain. (Only requires one heightmap) \n"\
		" (4) Third way with Dynamic Lighting. (Best Choice!) \n\n");

	char i;
	std::cin >> i;

	u8 testtype = 0;

	switch(i)
	{
		case '1': testtype = 1;break;
		case '2': testtype = 2;break;
		case '3': testtype = 3;break;
		case '4': testtype = 4;break;
		default: return 1;
	}


	// create device

	IrrlichtDevice* device = createDevice(driverType, core::dimension2d<s32>(640,480));

	if (device == 0)
		return 1; // could not create selected driver.




	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);


	// add camera
	scene::ICameraSceneNode* camera =
		smgr->addCameraSceneNodeFPS(0,100.0f,.50f);

	camera->setPosition(core::vector3df(4000,600,4000));
	camera->setTarget(core::vector3df(2397*2,343*2,2700*2));
	camera->setFarValue(1200.0f);

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	scene::ITerrainSceneNode* terrain;
	scene::ITerrainSceneNode* terrain2;

	if(testtype == 1)
	{
	// add terrain scene node
	terrain = smgr->addTerrainSceneNode(
		"media/cave1.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 0, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(200.f, 5.0f, 200.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		2					// smoothFactor
		);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	//terrain->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 20.0f);
	//terrain->setDebugDataVisible ( true );


	terrain2 = smgr->addTerrainSceneNode(
		"media/cave12.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 0.f, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(200.f, 5.0f, 200.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		2										// smoothFactor
		);

	terrain2->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain2->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	terrain2->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
	terrain2->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));

	terrain2->setMaterialType(video::EMT_DETAIL_MAP);

	terrain2->scaleTexture(1.0f, 20.0f);
	//terrain2->setVisible(false);

	}


	if(testtype == 2)
	{
	// add terrain scene node
	terrain = smgr->addTerrainSceneNode(
		"media/cave1.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 0, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(200.f, 5.0f, 200.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		2					// smoothFactor
		);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	//terrain->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));
	//terrain->setMaterialFlag(video::EMF_NORMALIZE_NORMALS,true);

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 20.0f);
	//terrain->setDebugDataVisible ( true );


	terrain2 = smgr->addTerrainSceneNode(
		"media/cave22.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(-200.f, 200.f, 0.f),			// position
		core::vector3df(0.f, 0.f, 180.f),			// rotation
		core::vector3df(200.f, 5.0f, 200.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		2										// smoothFactor
		);

	terrain2->setMaterialFlag(video::EMF_LIGHTING, false);
	//terrain2->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	terrain2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS,true);
	terrain2->setMaterialTexture(0, driver->getTexture("media/terrain-texture2.jpg"));
	terrain2->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));

	terrain2->setMaterialType(video::EMT_DETAIL_MAP);

	terrain2->scaleTexture(1.0f, 20.0f);

	//terrain2->setVisible(false);
	}



	if(testtype == 3)
	{
	// add terrain scene node
	terrain = smgr->addTerrainSceneNode(
		"media/cave1.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 0, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(200.f, 5.0f, 200.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		2					// smoothFactor
		);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	//terrain->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 20.0f);
	//terrain->setDebugDataVisible ( true );


	terrain2 = smgr->addTerrainSceneNode(
		"media/cave1.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 1500.f, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(200.f, -5.0f, 200.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		2										// smoothFactor
		);

	terrain2->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain2->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	terrain2->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
	terrain2->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));

	terrain2->setMaterialType(video::EMT_DETAIL_MAP);

	terrain2->scaleTexture(1.0f, 20.0f);
	//terrain2->setVisible(false);

	}



	if(testtype == 4)
	{
	// add terrain scene node
	terrain = smgr->addTerrainSceneNode(
		"media/cave1.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 0, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(200.f, 5.0f, 200.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		2					// smoothFactor
		);

	//terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	//terrain->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 20.0f);
	//terrain->setDebugDataVisible ( true );


	terrain2 = smgr->addTerrainSceneNode(
		"media/cave1.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 1500.f, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(200.f, -5.0f, 200.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		2										// smoothFactor
		);

	//terrain2->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain2->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	terrain2->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
	terrain2->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));

	terrain2->setMaterialType(video::EMT_DETAIL_MAP);

	terrain2->scaleTexture(1.0f, 20.0f);
	//terrain2->setVisible(false);
smgr->addLightSceneNode(camera,core::vector3df(0,0,0),video::SColorf(1,1,1),3000);

	}







	// create triangle selector for the terrain
	scene::ITriangleSelector* selector
		= smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);
	selector->drop();

	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		selector, camera, core::vector3df(60,100,60),
		core::vector3df(0,-1,0),
		core::vector3df(0,50,0));
	//camera->addAnimator(anim);
	anim->drop();

	// create event receiver
	MyEventReceiver receiver(terrain);
	device->setEventReceiver(&receiver);

   	// create skybox
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	smgr->addSkyBoxSceneNode(
		driver->getTexture("media/irrlicht2_up.jpg"),
		driver->getTexture("media/irrlicht2_dn.jpg"),
		driver->getTexture("media/irrlicht2_lf.jpg"),
		driver->getTexture("media/irrlicht2_rt.jpg"),
		driver->getTexture("media/irrlicht2_ft.jpg"),
		driver->getTexture("media/irrlicht2_bk.jpg"));

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

//	IAnimatedMesh* watermesh = smgr->addHillPlaneMesh("waterm",dimension2d<f32>(10,10),dimension2d<s32>(10,10));
//	ISceneNode* water = smgr->addWaterSurfaceSceneNode(watermesh->getMesh(0),1,200,5,0,-1,vector3df(2000,300,2000),vector3df(0,0,0),vector3df(100,50,100));



	int lastFPS = -1;

	core::vector3df camlastpos;

	while(device->run())
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, 0 );

		smgr->drawAll();
		env->drawAll();
		device->sleep(1);

		driver->endScene();

		// display frames per second in window title
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"Cave Renderer - Irrlicht Engine [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;


		}

		if(camera->getPosition().Y > 620)
			{
				core::vector3df camerapos = camera->getPosition();
				camerapos.Y = 620;
				camera->setPosition(camerapos);
				//camera->setPosition(camlastpos);
			}
		//camlastpos = camera->getPosition();
	}

	device->drop();

	return 0;
}

