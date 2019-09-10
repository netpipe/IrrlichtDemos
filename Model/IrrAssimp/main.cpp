#include <iostream>
#include <irrlicht.h>
#include "IrrAssimp/IrrAssimp.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int main()
{
	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"IrrAssimp Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the IrrAssimp demo!",
		rect<s32>(10,10,260,22), true);

    // The assimp loader can be used in a separate system and not directly as a meshLoader to give the choice of the loader to use (Irrlicht or Assimp) to the user
	IrrAssimp assimp(smgr);
    IAnimatedMesh* mesh = assimp.getMesh("Media/ninja.b3d");

    // It can also be used as a classic mesh loader :
    // smgr->addExternalMeshLoader(new IrrAssimpImport(smgr));
    // IAnimatedMesh* mesh = smgr->getMesh("Media/dwarf.x");


	if (!mesh)
	{
	    std::cout << "Error : Fail to load Media/ninja.b3d" << std::endl;
		device->drop();
		return 1;
	}

    // Export with assimp
	assimp.exportMesh(mesh, "obj", "Media/exportNinja.obj");

	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
	if (node)
	{
	    node->setAnimationSpeed(mesh->getAnimationSpeed()); // Fixed by r5097
		node->setDebugDataVisible(scene::EDS_SKELETON | scene::EDS_BBOX_ALL);
		node->setScale(core::vector3df(10, 10, 10));
	}

	smgr->addCameraSceneNodeFPS();
	smgr->addLightSceneNode(smgr->getActiveCamera(), core::vector3df(0, 0, 0), video::SColorf(1.0f, 1.0f, 1.0f), 3000.0f);

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}
