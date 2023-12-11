#include <irrlicht.h>
#include <iostream>

/// Remember to include "XEffects.h" in every project that uses XEffects, and add all the XEffects .cpp files to your project/makefile.
#include "XEffects.h"

// Namespaces.
using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

#pragma comment(lib, "irrlicht.lib")

int main()
{


	// Initialise Irrlicht device.
	IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2du(800,600), 32);
	if(!device){ return 1; }

	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	// Create a basic fps camera.
	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();
	cam->setPosition(vector3df(100,100,100));
	cam->setTarget(vector3df(0,0,0));


	// Initialise the EffectHandler, pass it the working Irrlicht device and the screen buffer resolution.
	// Shadow map resolution setting has been moved to SShadowLight for more flexibility.
	// (The screen buffer resolution need not be the same as the screen resolution.)
	// The third to last parameter enables VSM filtering, which we will enable.
	// The second to last parameter enables soft round spot light masks on our shadow lights.
	// The last parameter enables 32-bit depth buffers which is recommended for VSM shadows.
	EffectHandler* effect = new EffectHandler(device, driver->getScreenSize(), true, true, true);
	// Set a global ambient color:
	effect->setAmbientColor(SColor(255, 60, 60, 60));




    // Load all:
	device->getFileSystem()->addZipFileArchive("media/world.zip");

	// Load a basic stadium mesh:
	IMeshSceneNode* node1 = smgr->addMeshSceneNode(smgr->getMesh("stadium.3ds")->getMesh(0));
	node1->setPosition(vector3df(0, 0, 0));
	for(u32 i = 0;i < node1->getMaterialCount();++i){ node1->getMaterial(i).Lighting = false; }
	effect->addShadowToNode(node1);





	IMesh* carroMesh = smgr->getMesh("carro.x")->getMesh(0);
	// Add carro to mesh scene node:
	IMeshSceneNode* node2 = smgr->addMeshSceneNode(carroMesh);
	node2->setScale(vector3df(40, 40, 40));
	// stNode->setPosition(vector3df(0, -5, 0));
	// Disable lighting on the stadium.
	node2->setMaterialFlag(EMF_LIGHTING, false);
	// Apply a shadow to the node2. It is not recommended to use anything other than EFT_NONE in conjunction with VSM shadows.
	effect->addShadowToNode(node2, EFT_NONE);

	// Set the background clear color to black.
	/// effect->setClearColour(SColor(0x0));

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

	effect->addShadowLight(SShadowLight(512, vector3df(260,175,340),  vector3df(0,0,0), SColor(255,255,255,255), 0, 1000, 40 * DEGTORAD));
	effect->addShadowLight(SShadowLight(512, vector3df(-260,175,340), vector3df(0,0,0), SColor(255,255,255,255), 0, 1000, 40 * DEGTORAD));

	effect->addShadowLight(SShadowLight(512, vector3df(-260,175,-340),vector3df(0,0,0), SColor(255,255,255,255), 0, 1000, 40 * DEGTORAD));
	effect->addShadowLight(SShadowLight(512, vector3df(260,175,-340), vector3df(0,0,0), SColor(255,255,255,255), 0, 1000, 40 * DEGTORAD));







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

		// EffectHandler->update() replaces smgr->drawAll(). It handles all of the shadow maps, render targets switching, post processing, etc.
		effect->update();
		driver->endScene();
	}

	device->drop();

	return 0;
}
