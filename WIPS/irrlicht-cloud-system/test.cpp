#include <irrlicht.h>
#include <stdio.h>
#include <time.h>
#include "clouds.h"

#define PI 3.141592

using namespace irr;


int main()
{	
	// Create device
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
	
	// start up the engine
	SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
	params.AntiAlias = true;
	params.DriverType = video::EDT_OPENGL;
	params.WindowSize = core::dimension2d<u32>(800, 700);
	IrrlichtDevice *device = createDeviceEx(params);
	
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* scenemgr = device->getSceneManager();
	device->setWindowCaption(L"shader test");
	
	video::IGPUProgrammingServices * gpu = driver->getGPUProgrammingServices();
	
	bool bCanDoGLSL_1_1 = false;
	if (gpu && (driverType == video::EDT_OPENGL)) {
		bCanDoGLSL_1_1 = true; // provisionally accept
		if (!driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1)) {
			printf("queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1) failed\n");
			bCanDoGLSL_1_1 = false;
		}
		if (!driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1)) {
			printf("queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1) failed\n");
			bCanDoGLSL_1_1 = false;
		}
	}

	// add a first person shooter style user controlled camera
	scene::ISceneNode* camera = scenemgr->addCameraSceneNodeFPS(0, 100, 0.01);
	camera->setPosition(core::vector3df(3,2,0));
	camera->setRotation(core::vector3df(PI/6, PI/2, 0));
	// initialize cloud material
	setup_cloud_material(driver, driver->getTexture("clouds-group.png"), 4, 4);
	//setup_cloud_material(driver, driver->getTexture("cloud.png"),       1, 1);
	
	scene::ILightSceneNode* light  =  scenemgr->addLightSceneNode(0, core::vector3df(0,0,0), video::SColorf(0.8, 0.9, 1.0), 8);
	scene::ILightSceneNode* light2 =  scenemgr->addLightSceneNode(0, core::vector3df(10,8,12), video::SColorf(1.0, 0.3, 0.2), 8);
	scene::ILightSceneNode* light3 =  scenemgr->addLightSceneNode(0, core::vector3df(-10,-8,-12), video::SColorf(0.2, 0.5, 1.0), 8);
	scene::ILightSceneNode* light4 =  scenemgr->addLightSceneNode(0, core::vector3df(30,0,0), video::SColorf(0.6, 0.9, 1.0), 8);
	scene::ILightSceneNode* light5 =  scenemgr->addLightSceneNode(0, core::vector3df(30,8,3), video::SColorf(0.6, 0.9, 1.0), 1);
	
	// create a cloud
	CloudNode * cloud = new CloudNode(scenemgr->getRootSceneNode(), scenemgr, vector3df(1,0,0), vector3df(10,8,12));
	CloudNode * cloud2 = new CloudNode(scenemgr->getRootSceneNode(), scenemgr, vector3df(30,0,0), vector3df(20,18,29));
	//node->update_geometry(camera);
	// draw everything
	while(device->run() && driver)
	{
		driver->beginScene(true, true, video::SColor(0,0,0,0));
		scenemgr->drawAll();
		driver->endScene();
	}
	// delete device
	device->drop();
	return 0;
}
