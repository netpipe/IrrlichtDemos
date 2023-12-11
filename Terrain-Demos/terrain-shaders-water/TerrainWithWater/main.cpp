

#include "stdafx.h"
#include <irrlicht.h>
#include <iostream>

using namespace irr;

#pragma comment(lib, "Irrlicht.lib")
#include "shader_terrain.h"
#include "shader_water.h"
#include "shader_terrain.h"



int main()
{
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	IrrlichtDevice* device = createDevice(driverType, core::dimension2d<u32>(640, 480));

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);


	// add camera
	scene::ICameraSceneNode* camera =
		smgr->addCameraSceneNodeFPS(0,100.0f,10.f);

	camera->setPosition(core::vector3df(18110,10000,18110));
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
		core::vector3df(850.f,53.4f, 850.f),		// scale
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


	terrain->setMaterialTexture(0, driver->getTexture("terrmap.bmp"));
	terrain->setMaterialTexture(1, driver->getTexture("rock.png"));
	terrain->setMaterialTexture(2, driver->getTexture("soil.png"));
	terrain->setMaterialTexture(3, driver->getTexture("grass.png"));
	terrain->setMaterialType((irr::video::E_MATERIAL_TYPE) terrMat);





	IMeshSceneNode *node_plane = smgr->addMeshSceneNode(smgr->getMesh("plane.ms3d"));
	node_plane->getMaterial(0).setFlag(::EMF_LIGHTING, false);
	node_plane->setMaterialTexture(0, driver->getTexture("grass.png"));
	node_plane->setScale(vector3df(1400,1,1400));
	node_plane->setPosition(vector3df(0,5500,0));


	// create offscreen render target
	ITexture *tex_offscr = driver->addRenderTargetTexture(core::dimension2d<u32>(512,512));


	// instanitate callback
	Shader_Water_callback *callback = new Shader_Water_callback();
	callback->device = device;
	callback->driver = driver;
	callback->smgr = smgr;

	// create material, based on shader
	s32 material_water = gpu->addHighLevelShaderMaterial(
	  vertWaterShader.c_str(), "main", video::EVST_VS_2_a,
	  fragWaterShader.c_str(), "main", video::EPST_PS_2_b,
	  callback, video::EMT_TRANSPARENT_ALPHA_CHANNEL);


	node_plane->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	node_plane->setMaterialTexture(0, tex_offscr);
	node_plane->setMaterialTexture(1, driver->getTexture("normalmap.bmp"));
    node_plane->getMaterial(0).MaterialTypeParam = 0.01f;
	node_plane->setMaterialType((video::E_MATERIAL_TYPE) material_water);


	int lastFPS = -1;
	while(device->run())
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, video::SColor(0,100,100,100));

		// render scene without water plane
		driver->setRenderTarget(tex_offscr, true, true, video::SColor(0,0,0,0));
	    node_plane->setVisible(false);
		smgr->drawAll();

		// render final scene
  	    driver->setRenderTarget(0);
		node_plane->setVisible(true);
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

