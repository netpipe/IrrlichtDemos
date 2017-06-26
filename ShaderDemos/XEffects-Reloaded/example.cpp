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

	// Ask the user for the shadow map size.
	std::cout	<< "Please select the ShadowMap resolution:\n"
				<< "1 - 512x512\n"
				<< "2 - 1024x1024\n"
				<< "3 - 2048x2048\n"
				<< "4 - 4096x4096\n";

	std::cin >> C;

	u32 shadowDimen = 1024;

	switch(C)
	{
	case '1':
		shadowDimen = 512;
		break;
	case '2':
		shadowDimen = 1024;
		break;
	case '3':
		shadowDimen = 2048;
		break;
	case '4':
		shadowDimen = 4096;
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
	IrrlichtDevice* device = createDevice(dType, dimension2du(800,600), 32);

	if(!device)
		return 1;

	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	// Create a basic fps camera.
	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.05f);
	cam->setPosition(vector3df(0.0f, 10.0f, 0.0f));
	cam->setFarValue(50.0f);
	cam->setNearValue(0.1f);
	
	// Initialise the EffectHandler, pass it the working Irrlicht device and the screen buffer resolution.
	// Shadow map resolution setting has been moved to SShadowLight for more flexibility.
	// (The screen buffer resolution need not be the same as the screen resolution.)
	// The second to last parameter enables VSM filtering, see example 6 for more information.
	// The last parameter enables soft round spot light masks on our shadow lights.
	EffectHandler* effect = new EffectHandler(device, driver->getScreenSize(), false, true);

	// Load a basic room mesh.
	IMeshSceneNode* room = smgr->addMeshSceneNode(smgr->getMesh("media/ShadRoom.b3d")->getMesh(0));
	room->setScale(vector3df(3.0f, 2.0f, 3.0f));
	room->setPosition(vector3df(4.5f, 0.5f, 4.0f));
	room->setMaterialTexture(0, driver->getTexture("media/wall.jpg"));
	
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

	// We create 4 dwarves arranged in a grid in a nested loop.
	for(int g = 0;g < 2;g++)
	{
		for(int v = 0;v < 2;v++)
		{
			// Add a dwarf animated mesh scene node.
			IAnimatedMeshSceneNode* dwarf = smgr->addAnimatedMeshSceneNode(dwarfmesh);
			dwarf->setScale(vector3df(0.05f, 0.05f, 0.05f));
			dwarf->setPosition(vector3df(g * 4.5f + 1.0f, 0.5f, v * 3.5f + 2.0f));

			// Disable the lighting here too, XEffects overlay will handle it.
			for(u32 i = 0;i < dwarf->getMaterialCount();++i)
				dwarf->getMaterial(i).Lighting = false;

			dwarf->setAnimationSpeed(20.0f);

			// Apply a shadow to the dwarf, notice we are applying it to all of them
			// including the ones that have effects applied.
			effect->addShadowToNode(dwarf, filterType);
		}
	}

	// Add a transparent cube.
	ISceneNode* cube = smgr->addCubeSceneNode(3.0f);
	cube->setPosition(vector3df(5, 6, 5));
	cube->getMaterial(0).setTexture(0, driver->getTexture("media/xeffects.png"));
	cube->getMaterial(0).MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
	effect->addShadowToNode(cube, filterType, ESM_CAST);

	// Set the background clear color to orange.
	effect->setClearColour(SColor(255, 250, 100, 0));

	// Add two light scene nodes. We will not actually use them for lighting in this
	// demo, but add animators to them instead and make the shadow lights follow them.
	ILightSceneNode* light = smgr->addLightSceneNode();
	light->addAnimator(smgr->createFlyCircleAnimator(vector3df(5, 20, 5), 38));
	ILightSceneNode* light2 = smgr->addLightSceneNode();
	light2->addAnimator(smgr->createFlyCircleAnimator(vector3df(5, 20, 5), 38, -0.001f));

	// Add some post processing effects, a very subtle bloom here.
	const stringc shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";

	effect->addPostProcessingEffectFromFile(core::stringc("shaders/BrightPass") + shaderExt);
	effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurHP") + shaderExt);
	effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurVP") + shaderExt);
	effect->addPostProcessingEffectFromFile(core::stringc("shaders/BloomP") + shaderExt);

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
	effect->addShadowLight(SShadowLight(shadowDimen, vector3df(0, 0, 0), vector3df(5, 0, 5), 
		SColor(0, 255, 0, 0), 20.0f, 60.0f, 30.0f * DEGTORAD));
	effect->addShadowLight(SShadowLight(shadowDimen, vector3df(0, 0, 0), vector3df(5, 0, 5),
		SColor(0, 0, 255, 0), 20.0f, 60.0f, 30.0f * DEGTORAD));

	// We will add a particle system here to show that you can use transparent materials in this new
	// XEffects version (This code is copied from the SpecialFX Irrlicht Example).
	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-1,0,-1,1,1,1), // emitter size
		core::vector3df(0.0f,0.005f,0.0f),  // initial direction
		80, 100,                            // emit rate
		video::SColor(0,255,255,255),       // darkest color
		video::SColor(0,255,255,255),       // brightest color
		800, 1500, 0,                       // min and max age, angle
		core::dimension2df(1.f,1.f),        // min size
		core::dimension2df(2.f,2.f));       // max size

	ps->setEmitter(em); // this grabs the emitter
	em->drop(); // so we can drop it here without deleting it

	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

	ps->addAffector(paf); // same goes for the affector
	paf->drop();

	ps->setPosition(core::vector3df(-2.5f, 2.5f, -3.0f));
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("media/fireball.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

	// We should exclude the particles from the lighting calculations as 
	// their lighting is set to false.
	effect->excludeNodeFromLightingCalculations(ps);

	s32 oldFps = 0;
	
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
		effect->getShadowLight(0).setPosition(light->getPosition());
		effect->getShadowLight(1).setPosition(light2->getPosition());

		effect->getShadowLight(0).setTarget(vector3df(5, 0, 5));
		effect->getShadowLight(1).setTarget(vector3df(5, 0, 5));

		driver->beginScene(true, true, SColor(0x0));

		// EffectHandler->update() replaces smgr->drawAll(). It handles all
		// of the shadow maps, render targets switching, post processing, etc.
		effect->update();

		driver->endScene();
	}

	device->drop();

	return 0;
}
