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
	std::cout	<< "Welcome to the X-Platform Effect Wrapper Demo\n"
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
	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.01f);
	cam->setPosition(vector3df(10.0f, 10.0f, -10.0f));
	cam->setTarget(vector3df(5.0f, 5.0f, 5.0f));
	cam->setFarValue(50.0f);
	cam->setNearValue(0.1f);
	
	// Initialise the EffectHandler, pass it the working Irrlicht device and the screen buffer resolution.
	// Shadow map resolution setting has been moved to SShadowLight for more flexibility.
	// (The screen buffer resolution need not be the same as the screen resolution.)
	// The third to last parameter enables VSM filtering, which we will enable.
	// The second to last parameter enables soft round spot light masks on our shadow lights.
	// The last parameter enables 32-bit depth buffers which is recommended for VSM shadows.
	EffectHandler* effect = new EffectHandler(device, driver->getScreenSize(), true, true, true);

	// Load a basic room mesh.
	IMeshSceneNode* room = smgr->addMeshSceneNode(smgr->getMesh("media/ShadRoom.b3d")->getMesh(0));
	room->setScale(vector3df(3.0f, 2.0f, 3.0f));
	room->setPosition(vector3df(4.5f, 0.5f, 4.0f));
	room->setMaterialTexture(0, driver->getTexture("media/wall.jpg"));
	
	// We disable lighting on the mesh.
	room->getMaterial(0).Lighting = false;

	// Add the room to the shadow node list.
	effect->addShadowToNode(room);

	// Set a global ambient color. A very dark gray.
	effect->setAmbientColor(SColor(255, 16, 16, 16));

	// Load the dragon mesh.
	// Dragon model courtesy of TurboSquid.com.
	IMesh* dragonMesh = smgr->getMesh("media/dragon.obj")->getMesh(0);

	// Add a dragon mesh scene node.
	IMeshSceneNode* dragon = smgr->addMeshSceneNode(dragonMesh);
	dragon->setScale(vector3df(1.5f, 1.5f, 1.5f));
	dragon->setPosition(vector3df(5.0f, 9.0f, 3.0f));

	// Disable lighting on the dragon.
	dragon->setMaterialFlag(EMF_LIGHTING, false);

	// Apply a shadow to the dragon. It is not recommended to use anything other than EFT_NONE
	// in conjunction with VSM shadows.
	effect->addShadowToNode(dragon);

	// Set the background clear color to black.
	effect->setClearColour(SColor(0x0));

	// Add two shadow lights. 
	// The first parameter specifies the shadow map resolution for the shadow light.
	// The shadow map is always square, so you need only pass 1 dimension, preferably
	// a power of two between 512 and 2048, maybe larger depending on your quality
	// requirements and target hardware. We will just pass the value the user picked.
	// The second parameter is the light position, the third parameter is the (look at)
	// target, the next is the light color, and the next two are very important
	// values, the nearValue and farValue, be sure to set them appropriate to the current
	// scene. The last parameter is the FOV (Field of view), since the light is similar to
	// a spot light, the field of view will determine its area of influence. Anything that
	// is outside of a lights frustum (Too close, too far, or outside of it's field of view)
	// will be unlit by this particular light, similar to how a spot light works.
	// We will add one red light and one yellow light.
	effect->addShadowLight(SShadowLight(1024, vector3df(-15, 30, -15), vector3df(5, 0, 5), 
		SColor(255, 255, 255, 255), 20.0f, 60.0f, 30.0f * DEGTORAD));

	s32 oldFps = 0;
	
	// Main loop here.
	while(device->run())
	{
		if(oldFps != driver->getFPS())
		{
			core::stringw windowCaption = L"VSM Shadow Mapping Demo FPS: ";
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
