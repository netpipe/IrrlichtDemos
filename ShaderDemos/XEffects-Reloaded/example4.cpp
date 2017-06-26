#include <irrlicht.h>
#include <iostream>

// Remember to include "XEffects.h" in every project that uses XEffects, and add all the XEffects .cpp
// files to your project/makefile.
#include "XEffects.h"

// Namespaces.
using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

#pragma comment(lib, "irrlicht.lib")

int main()
{
	char C = 0;

	// Ask the user for the driver type.
	std::cout	<< "Welcome to the X-Platform Effect Wrapper Demo 4\n"
				<< "Please select the driver type:\n"
				<< "1 - OpenGL\n"
				<< "2 - Direct3D9\n";

	std::cin >> C;

	E_DRIVER_TYPE dType = EDT_OPENGL;

	switch(C)
	{
	case '1':
		dType = EDT_OPENGL;
		break;
	case '2':
		dType = EDT_DIRECT3D9;
		break;
	}

	// Initialise Irrlicht device.
	IrrlichtDevice* device = createDevice(dType, dimension2du(800,600), 32);

	if(!device)
		return 1;

	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	// Create a basic fps camera.
	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.05f);
	
	cam->setPosition(vector3df(300.0f, 100.0f, 00.0f));
	cam->setTarget(vector3df(200.0f, 0.0f, 50.0f));

	cam->setFarValue(500.0f);
	cam->setNearValue(1.0f);
	
	// Initialise the EffectHandler, pass it the working Irrlicht device.
	// Shadow map resolution setting has been moved to SShadowLight for more flexibility.
	// (The screen buffer resolution need not be the same as the screen resolution).
	EffectHandler* effect = new EffectHandler(device);

	// Set a global ambient color. A very dark gray.
	effect->setAmbientColor(SColor(255, 32, 32, 32));

	// Set the background clear color to sky blue.
	effect->setClearColour(SColor(255, 50, 100, 200));

	// Here we enable the depth pass and add the terrain scene node to the depth pass.
	effect->enableDepthPass(true);

	// Set the water normal map as our post processing user texture.
	effect->setPostProcessingUserTexture(driver->getTexture("media/waterNormal.png"));

	// Add our post-processing effect.
	const stringc shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";
	effect->addPostProcessingEffectFromFile(core::stringc("shaders/ScreenWater") + shaderExt);

	// Add a terrain scene node.
	ISceneNode* terrain = smgr->addTerrainSceneNode("media/terrain-heightmap.bmp", 0, -1,
		vector3df(0.0f, -20.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 0.2f, 1.0f),
		SColor(0xffffffff), 5, ETPS_17, 1);

	terrain->getMaterial(0).setTexture(0, driver->getTexture("media/terrain-texture.jpg"));
	terrain->getMaterial(0).Lighting = false;

	// Add terrain to depth pass.
	effect->addNodeToDepthPass(terrain);

	s32 oldFps = 0;

	// Main loop here.
	while(device->run())
	{
		if(oldFps != driver->getFPS())
		{
			core::stringw windowCaption = L"Post-Processing Water Demo FPS: ";
			windowCaption += driver->getFPS();
			device->setWindowCaption(windowCaption.c_str());
			oldFps = driver->getFPS();
		}

		driver->beginScene(true, true, SColor(0x0));

		// EffectHandler->update() replaces smgr->drawAll(). It handles all
		// of the shadow maps, render targets switching, post processing, etc.
		effect->update();

		driver->endScene();
	}

	device->drop();

	return 0;
}
