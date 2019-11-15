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

// We need to pass the view projection matrix to the SSAO shader, so we create a class that inherits from
// IPostProcessingRenderCallback and pass the shader constant in OnPreRender using setPostProcessingEffectConstant.
class SSAORenderCallback : public IPostProcessingRenderCallback
{
public:
	SSAORenderCallback(irr::s32 materialTypeIn) : materialType(materialTypeIn) {}

	void OnPreRender(EffectHandler* effect)
	{
		IVideoDriver* driver = effect->getIrrlichtDevice()->getVideoDriver();
		viewProj = driver->getTransform(ETS_PROJECTION) * driver->getTransform(ETS_VIEW);
		effect->setPostProcessingEffectConstant(materialType, "mViewProj", viewProj.pointer(), 16);
	}

	void OnPostRender(EffectHandler* effect) {}

	core::matrix4 viewProj;
	s32 materialType;
};

int main()
{
	char C = 0;

	// Ask the user for the driver type.
	std::cout	<< "Welcome to the X-Platform Effect Wrapper Demo 3\n"
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

	// Ask the user for the preferred scene.
	std::cout	<< "Would you like the cube scene or the room scene?\n"
				<< "1 - Room\n"
				<< "2 - Cube\n";

	std::cin >> C;

	// Initialise Irrlicht device.
	IrrlichtDevice* device = createDevice(dType, dimension2du(800,600), 32);

	if(!device)
		return 1;

	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	// Create a basic fps camera.
	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.1f);
	cam->setFarValue(50.0f);
	cam->setNearValue(0.1f);
	
	// Initialise the EffectHandler, pass it the working Irrlicht device.
	// Shadow map resolution setting has been moved to SShadowLight for more flexibility.
	// (The screen buffer resolution need not be the same as the screen resolution).
	EffectHandler* effect = new EffectHandler(device, dimension2du(0, 0), false, false, true);

	// Add the SSAO post processing effect followed by a blur and a combine with the screen texture.
	core::stringc shaderExt = (device->getVideoDriver()->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";

	irr::s32 SSAO = effect->addPostProcessingEffectFromFile(core::stringc("shaders/SSAO") + shaderExt);
	irr::s32 BlurH = effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurHP") + shaderExt);
	irr::s32 BlurV = effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurVP") + shaderExt);
	irr::s32 SSAOCombine = effect->addPostProcessingEffectFromFile(core::stringc("shaders/SSAOCombine") + shaderExt);

	// Create our custom IPostProcessingRenderCallback and pass it to setPostProcessingRenderCallback
	// so that we can perform operations before and after the SSAO effect is rendered.
	SSAORenderCallback* ssaoCallback = new SSAORenderCallback(SSAO);
	effect->setPostProcessingRenderCallback(SSAO, ssaoCallback);

	if(C == '1')
	{
		// Load a basic room mesh.
		IMeshSceneNode* room = smgr->addMeshSceneNode(smgr->getMesh("media/ShadRoom.b3d")->getMesh(0));
		room->setScale(vector3df(3, 2, 3));
		room->setPosition(vector3df(4.5f, 0.5f, 4.0f));
		room->setMaterialTexture(0, driver->getTexture("media/wall.jpg"));
		
		// We disable lighting on the mesh.
		room->getMaterial(0).Lighting = false;

		effect->addNodeToDepthPass(room);

		srand(device->getTimer()->getRealTime());

		// Add a few spheres.
		for(u32 i = 0;i < 10;++i)
		{
			ISceneNode* sphere = smgr->addSphereSceneNode(2.0);
			sphere->setPosition(vector3df(rand()%100 * 0.1f, rand()%100 * 0.08f, rand()%100 * 0.1f + 1.0f));
			sphere->setMaterialTexture(0, driver->getTexture("media/wall.jpg"));
			sphere->getMaterial(0).Lighting = false;
			
			effect->addNodeToDepthPass(sphere);
		}

		cam->setPosition(vector3df(12, 12, 12));
		cam->setTarget(vector3df(0, 0, 0));
	}
	else
	{
		// Add a crap-load of cube nodes! (This lovely comment courtesy of Halifax)
		for(s32 i = 0;i < 6;++i)
		{
			for(s32 j = 0; j < 6;++j)
			{
				for(s32 k = 0; k < 6;++k)
				{
					ISceneNode* cube = smgr->addCubeSceneNode(4.0f);
					cube->setPosition(vector3df(i * 4.0f + 2.0f , j * 5.0f + 1.0f, k * 6.0f + 3.0f));
					cube->setRotation(vector3df((f32)(rand() % 360), (f32)(rand() % 360), (f32)(rand() % 360)));
					cube->getMaterial(0).setTexture( 0, driver->getTexture( "media/wall.jpg" ) );
					cube->getMaterial(0).setFlag( EMF_LIGHTING, false );

					effect->addNodeToDepthPass(cube);
				}
			}
		}

		cam->setPosition(vector3df(30, 40, 40));
		cam->setTarget(vector3df(0, 0, 0));
	}

	// Set a global ambient color. A very dark gray.
	effect->setAmbientColor(SColor(255, 32, 32, 32));

	// Set the background clear color to orange.
	effect->setClearColour(SColor(255, 250, 100, 0));

	// Here we enable the depth pass and add the room mesh scene node to the depth pass.
	effect->enableDepthPass(true);

	// Generate and set the random vector user texture for the SSAO to use.
	ITexture* randVecTexture = effect->generateRandomVectorTexture(dimension2du(512, 512));
	effect->setPostProcessingUserTexture(randVecTexture);

	s32 oldFps = 0;

	// We need to pass the view projection matrix to the SSAO effect. We declare it here so we can have
	// a valid pointer to the matrix during the draw loop.
	core::matrix4 viewProj;
	
	// Main loop here.
	while(device->run())
	{
		if(oldFps != driver->getFPS())
		{
			core::stringw windowCaption = L"SSAO Demo FPS: ";
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
