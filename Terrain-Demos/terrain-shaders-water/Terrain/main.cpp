

#include "stdafx.h"
#include <irrlicht.h>
#include <iostream>

using namespace irr;

#pragma comment(lib, "Irrlicht.lib")
#include "shaders.h"


int screen_w = 640;
int screen_h = 480;

int main()
{
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	IrrlichtDevice* device = createDevice(driverType, core::dimension2d<u32>(screen_w,screen_h),16,false,true);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);


	// add camera
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0,100,10);


	camera->setPosition(core::vector3df(18110,10000 - 5990,18110));
	camera->setFarValue(1200000.0f);

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);


	// add terrain scene node
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"heightmap.png",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 0.f, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(850.f, 100.4f, 850.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		4										// smoothFactor
		);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);

	// create triangle selector for the terrain
	scene::ITriangleSelector* selector	= smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);


	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		selector, camera, core::vector3df(60,100,60),
		core::vector3df(0,0,0),
		core::vector3df(0,50,0));
	selector->drop();
	camera->addAnimator(anim);
	anim->drop();




	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
    video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	Shader_Terrain_callback *terrCallback = new Shader_Terrain_callback();

    s32 terrMat =  gpu->addHighLevelShaderMaterial(
	  vertTerrainShader.c_str(), "main", video::EVST_VS_2_a,
	  fragTerrainShader.c_str(), "main", video::EPST_PS_2_b,
	  terrCallback, video::EMT_SOLID);


	terrain->setMaterialTexture(0, driver->getTexture("heightmap.png"));
	terrain->setMaterialTexture(1, driver->getTexture("grass.png"));
	terrain->setMaterialTexture(2, driver->getTexture("rock.png"));
	terrain->setMaterialTexture(3, driver->getTexture("basalt.png"));
	terrain->setMaterialType((irr::video::E_MATERIAL_TYPE) terrMat);

	int lastFPS = -1;
	while(device->run())

    if (device->isWindowActive())
	{
		driver->beginScene(true, true, 0 );
		smgr->drawAll();
  		driver->endScene();

		// display frames per second in window title
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"Terrain Renderer - Irrlicht Engine [";
			str += driver->getName();
			str += "] FPS:";
            str += fps;
			// Also print terrain height of current camera position
			// We can use camera position because terrain is located at coordinate origin
			str += " Height: ";
			str += terrain->getHeight(camera->getAbsolutePosition().X, camera->getAbsolutePosition().Z);

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}

	device->drop();

	return 0;
}

