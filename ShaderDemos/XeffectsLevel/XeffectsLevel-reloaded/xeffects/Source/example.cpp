// Includes, remember to include "effectWrapper.h", effectWrapper.cpp", and "CShaderPre.cpp"
// in every project that uses XEffects.
#include <irrlicht.h>
#include <iostream>
#include "effectWrapper.h"

// Namespaces.
using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

int main()
{
	char C = 0;

	E_DRIVER_TYPE dType = EDT_OPENGL;


	// Ask the user for the shadow map size.
	std::cout	<< "Please select the ShadowMap resolution:\n"
				<< "1 - 512x512\n"
				<< "2 - 1024x1024\n"
				<< "3 - 2048x2048\n"
				<< "4 - 4096x4096\n";

	std::cin >> C;

	dimension2d<s32> shadowDimen = dimension2d<s32>(1024,1024);

	switch(C)
	{
	case '1':
		shadowDimen = dimension2d<s32>(512,512);
		break;
	case '2':
		shadowDimen = dimension2d<s32>(1024,1024);
		break;
	case '3':
		shadowDimen = dimension2d<s32>(2048,2048);
		break;
	case '4':
		shadowDimen = dimension2d<s32>(4096,4096);
		break;
	}

	// Ask the user for the filter type.
	std::cout	<< "Please select the ShadowMap filtering:\n"
				<< "1 - none\n"
				<< "2 - 4 PCF\n"
				<< "3 - 8 PCF\n"
				<< "4 - 12 PCF\n"
				<< "5 - 16 PCF\n";

	std::cin >> C;
	E_FILTER_TYPE filterType = (E_FILTER_TYPE)core::clamp<u32>((u32)C - '1', 0, 4);

	// Initialise Irrlicht device.
	IrrlichtDevice* device = createDevice(dType, dimension2d<s32>(800,600), 32);

	if(!device)
		return 1;

	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	// Create a basic fps camera.
	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0,100,10);
	cam->setPosition(vector3df(0,10,0));
	cam->setFarValue(50);
	cam->setNearValue(0.1f);

	// This is the screen buffer resolution, it should preferably be the exact same
	// size as the screen, although some ATI cards have trouble with that so for
	// the purpose of this demo it will only be (512, 512) on ATI cards.
	// (This is also used in some of the shadow map calculations, so it can affect
	// the shadow map quality a little also.)
	dimension2d<s32> ScreenRTT =
		// Comment JUST the next line if "getVendorInfo()" is not defined.
		!driver->getVendorInfo().equals_ignore_case("NVIDIA Corporation") ? dimension2d<s32>(512,512) :
		driver->getScreenSize();

	// Initialise the effectHandler, pass it the working Irrlicht device, the shadow
	// dimensions, the shader directory address, and the screen buffer resolution.
	// (The screen buffer resolution need not be the same as the screen resolution.)
	effectHandler* effect = new effectHandler(device, shadowDimen, "shaders", ScreenRTT);

	// Load a basic room mesh.
	IMeshSceneNode* room = smgr->addMeshSceneNode(smgr->getMesh("media/ShadRoom.b3d")->getMesh(0));
	room->setScale(vector3df(3,2,3));
	room->setPosition(vector3df(4.5f,0.5f,4));
	room->setMaterialTexture(0, driver->getTexture("media/wall.bmp"));

	// We disable lighting on the mesh, because the lighting will be handled by
	// the shadow map overlay, the new additive system in XEffects which allows
	// us to realitically utilize multiple lights. You can still choose to perform
	// your own lighting alongside if you wish to however.
	room->getMaterial(0).Lighting = false;

	// Add the room to the shadow node list, using the chosen filtertype.
	// It will use the default shadow mode, ESM_BOTH, which allows it to
	// both cast and receive shadows.
	effect->addShadowToNode(room, filterType);

	// Set a global ambient color. A very dark gray.
	effect->setAmbientColor(SColor(255, 32, 32, 32));

	// Load the dwarf mesh.
	IAnimatedMesh* dwarfmesh = smgr->getMesh("media/dwarf.x");

	// We create 9 dwarves arranged in a grid in a nested loop.
	for(int g = 0;g < 3;g++)
	{
		for(int v = 0;v < 3;v++)
		{
			// Add a dwarf animated mesh scene node.
			IAnimatedMeshSceneNode* dwarf = smgr->addAnimatedMeshSceneNode(dwarfmesh);
			dwarf->setScale(vector3df(0.05f, 0.05f, 0.05f));
			dwarf->setPosition(vector3df(g * 4.5f, 0.5f, v * 3.5f + 1.0f));

			// Disable the lighting here too, XEffects overlay will handle it.
			dwarf->getMaterial(0).Lighting = false;
			dwarf->getMaterial(1).Lighting = false;

			// If its the first row add some effects.
			if(g == 0)
			{
				dwarf->setMaterialTexture(0,driver->getTexture("media/red.bmp"));
				effect->addEffectToNode(dwarf, (E_EFFECT_TYPE)(v + 1));
			}

			// Apply a shadow to the dwarf, notice we are applying it to all of them
			// including the ones that have effects applied.
			effect->addShadowToNode(dwarf, filterType);
		}
	}

	// Set the background clear color to orange.
	effect->setClearColour(SColor(255, 250, 100, 0));

	// Add two light scene nodes. We will not actually use them for lighting in this
	// demo, but add animators to them instead and make the shadow lights follow them.
	ILightSceneNode* light = smgr->addLightSceneNode();
	light->addAnimator(smgr->createFlyCircleAnimator(vector3df(5, 20, 5), 38));
	ILightSceneNode* light2 = smgr->addLightSceneNode();
	light2->addAnimator(smgr->createFlyCircleAnimator(vector3df(5, 20, 5), 38, -0.001));

	// Add some post processing effects, a very subtle bloom here.
	core::stringc shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";

	effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurHP") + shaderExt);
	effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurVP") + shaderExt);
	effect->addPostProcessingEffectFromFile(core::stringc("shaders/BloomP") + shaderExt);

	// Add two shadow lights. The first parameter is the position, the second parameter
	// is the target, the next is the light color, and the next two are very important
	// values, the nearValue and farValue, be sure to set them appropriate to the current
	// scene. The last parameter is the FOV (Field of view), since the light is similar to
	// a spot light, the field of view will determine its area of influence. Anything that
	// is outside of a lights frustum (Too close, too far, or outside of it's field of view)
	// will be unlit by this particular light, similar to how a spot light works.
	// We will add one red light and one yellow light.
	effect->addShadowLight(SShadowLight(vector3df(0,0,0), vector3df(5, 0, 5),
		SColor(0, 255, 0, 0), 20.0f, 60.0f, 60.0f * DEGTORAD));
	effect->addShadowLight(SShadowLight(vector3df(0,0,0), vector3df(5, 0, 5),
		SColor(0, 0, 255, 0), 20.0f, 60.0f, 60.0f * DEGTORAD));

	u32 oldFps = 0;

	// Main loop here.
	while(device->run())
	{
		if(oldFps != driver->getFPS())
		{
			core::stringw windowCaption = L"Shadow Map Demo FPS: ";
			windowCaption += driver->getFPS();
			device->setWindowCaption(windowCaption.c_str());
			oldFps = driver->getFPS();
		}

		// Get the shadow lights and set their positions to the positions
		// of the light scene nodes.
		//effect->getShadowLight(0).setPosition(light->getPosition());
		//effect->getShadowLight(1).setPosition(light2->getPosition());

		matrix4 vMat;
		vMat.buildCameraLookAtMatrixLH(light->getPosition(), vector3df(5, 0, 5), vector3df(0,1,0));
		effect->getShadowLight(0).setViewMatrix(vMat);
		vMat.buildCameraLookAtMatrixLH(light2->getPosition(), vector3df(5, 0, 5), vector3df(0,1,0));
		effect->getShadowLight(1).setViewMatrix(vMat);

		driver->beginScene(true, true, SColor(0x0));

		// effectHandler->update() replaces smgr->drawAll(). It handles all
		// of the shadow maps, render targets switching, post processing, etc.
		effect->update();

		driver->endScene();
		device->sleep(50,0);
	}

	device->drop();

	return 0;
}
