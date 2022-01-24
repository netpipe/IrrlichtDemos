// This example is mostly uncommented (Except for psuedo point light
// specific information found far below). For educational comments,
// please refer to the first example.

#include <irrlicht.h>
#include <iostream>
#include "effectWrapper.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace io;
using namespace gui;

int main()
{
	char C = 0;

	std::cout	<< "Welcome to the X-Platform Effect Wrapper Demo\n"
				<< "(Psuedo point light edition!)\n"
				<< "Please select the driver type:\n";



	std::cout	<< "Please select the ShadowMap resolution:\n"
				<< "(For this demo a low resolution is recommended)\n"
				<< "1 - 512x512\n"
				<< "2 - 1024x1024\n"
				<< "3 - 2048x2048\n"
				<< "4 - 4096x4096\n";

	std::cin >> C;

	dimension2d<u32> shadowDimen = dimension2d<u32>(1024,1024);

	switch(C)
	{
	case '1':
		shadowDimen = dimension2d<u32>(512,512);
		break;
	case '2':
		shadowDimen = dimension2d<u32>(1024,1024);
		break;
	case '3':
		shadowDimen = dimension2d<u32>(2048,2048);
		break;
	case '4':
		shadowDimen = dimension2d<u32>(4096,4096);
		break;
	}

	std::cout	<< "Please select the ShadowMap filtering:\n"
				<< "1 - none\n"
				<< "2 - 4 PCF\n"
				<< "3 - 8 PCF\n"
				<< "4 - 12 PCF\n"
				<< "5 - 16 PCF\n";

	std::cin >> C;

	E_FILTER_TYPE filterType = (E_FILTER_TYPE)core::clamp<u32>((u32)C - '1', 0, 4);

	IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<u32>(800,600), 32);

	if(!device)
		return 1;

	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0,100,0.05);
	cam->setPosition(vector3df(0,10,0));
	cam->setNearValue(0.1f);
	cam->setFarValue(50);

	dimension2d<u32> ScreenRTT =
		// Comment JUST the next line if "getVendorInfo()" is not defined.
		!driver->getVendorInfo().equals_ignore_case("NVIDIA Corporation") ? dimension2d<u32>(512,512) :
		driver->getScreenSize();

	effectHandler* effect = new effectHandler(device, shadowDimen, "shaders", ScreenRTT);

	IMeshSceneNode* room = smgr->addMeshSceneNode(smgr->getMesh("media/ShadRoom.b3d")->getMesh(0));
	room->setScale(vector3df(3,2,3));
	room->setPosition(vector3df(4.5f,0.5f,4));
	room->setMaterialTexture(0, driver->getTexture("media/wall.bmp"));
	room->getMaterial(0).Lighting = false;
	effect->addShadowToNode(room, filterType);

	effect->setAmbientColor(SColor(0x22222222));

	IAnimatedMesh* dwarfmesh = smgr->getMesh("media/dwarf.x");

	for(int g = 0;g < 3;g++)
	{
		for(int v = 0;v < 3;v++)
		{
			IAnimatedMeshSceneNode* dwarf = smgr->addAnimatedMeshSceneNode(dwarfmesh);
			dwarf->setScale(vector3df(0.05f, 0.05f, 0.05f));
			dwarf->setPosition(vector3df(g * 4.5f, 0.5f, v * 3.5f + 1.0f));
			dwarf->getMaterial(0).Lighting = false;
			dwarf->getMaterial(1).Lighting = false;

			if(g == 0)
			{
				dwarf->setMaterialTexture(0,driver->getTexture("media/red.bmp"));
				effect->addEffectToNode(dwarf, (E_EFFECT_TYPE)(v + 1));
			}

			effect->addShadowToNode(dwarf, filterType);
		}
	}

	effect->setClearColour(SColor(255, 250, 100, 0));

	ILightSceneNode* light = smgr->addLightSceneNode();
	light->addAnimator(smgr->createFlyCircleAnimator(vector3df(5, 4.5f, 5), 5));

	core::stringc shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";

	effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurHP") + shaderExt);
	effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurVP") + shaderExt);
	effect->addPostProcessingEffectFromFile(core::stringc("shaders/BloomP") + shaderExt);

	u32 oldFps = 0;

	// A fairly close farValue is ok for this point light.
	f32 farValue = 22.0f;

	// The FOV is nearly 90 degrees. I set it a little lower to make the seems less visible.
	f32 fov = 89.99f * DEGTORAD;

	// We just initialize positions and targets to (0,0,0), we will set them
	// later in the draw loop.
	for(u32 i = 0;i < 6;++i)
	{
		effect->addShadowLight(SShadowLight(vector3df(0,0,0), vector3df(0,0,0),
			SColor(0, 255, 0, 0), 1.0f, farValue, fov));
	}

	// Make a little sphere to follow the light around.
	ISceneNode* sphere = smgr->addSphereSceneNode(0.2, 16, light);
	sphere->getMaterial(0).Lighting = false;

	while(device->run())
	{
		if(oldFps != driver->getFPS())
		{
			core::stringw windowCaption = L"Shadow Map Demo FPS: ";
			windowCaption += driver->getFPS();
			device->setWindowCaption(windowCaption.c_str());
			oldFps = driver->getFPS();
		}

		// Set all the shadow lights to the same position as the light scene node.
		for(u32 i = 0;i < 6;++i)
			effect->getShadowLight(i).setPosition(light->getPosition());

		// So the idea here is to make each of the six lights face a different
		// direction, since they all cover a 90.0 degree area, having six aligned
		// correctly will cover the entire range visible from a single point.
		effect->getShadowLight(0).setTarget(light->getPosition() + vector3df(0,0,1));
		effect->getShadowLight(1).setTarget(light->getPosition() + vector3df(0,0,-1));
		effect->getShadowLight(2).setTarget(light->getPosition() + vector3df(1,0,0));
		effect->getShadowLight(3).setTarget(light->getPosition() + vector3df(-1,0,0));
		effect->getShadowLight(4).setTarget(light->getPosition() + vector3df(0,1,0));
		effect->getShadowLight(5).setTarget(light->getPosition() + vector3df(0,-1,0));

		driver->beginScene(true, true, SColor(0x0));

		// effectHandler->update() now replaces smgr->drawAll()!
		effect->update();
device->sleep(4);
		driver->endScene();
	}

	device->drop();

	return 0;
}
