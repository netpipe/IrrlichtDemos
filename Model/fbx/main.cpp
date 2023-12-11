#include <irrlicht.h>
#include "CFBXMeshFileLoader.h"
#include <iostream>

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

using namespace irr;

int main(int argc, char** argv)
{
	if (!(argc > 1))
	{
		std::cout << "Usage: loader [file]" << std::endl;
	//	return 1;
	}

	IrrlichtDevice* device = createDevice(video::EDT_OPENGL,
					  core::dimension2d<u32>(640, 480), 16, false);
	if (!device)
	{
		std::cout << "Unable to create OpenGL device. Quitting..." << std::endl;
		return 1;
	}

	device->getSceneManager()->addExternalMeshLoader(new scene::CFBXMeshFileLoader(device->getSceneManager()));

#define mesho "./Batman/Batman.FBX"
#define mesho "./Superman/SuperMan.FBX"
	scene::IAnimatedMesh* mesh = device->getSceneManager()->getMesh(mesho);

	if (!mesh)
	{
		std::cout << "Unable to load mesh: " << std::endl;
		return 1;
	}

	scene::ILightSceneNode* light = device->getSceneManager()->addLightSceneNode();
	light->setRadius(10000.f);
	light->setPosition(core::vector3df(500,500,500));

	scene::ISceneNode* node = device->getSceneManager()->addAnimatedMeshSceneNode(mesh);
	if (!node)
	{
		std::cout << "Unable to create node with mesh: " << argv[1] << std::endl;
		return 1;
	}

//	node->setDebugDataVisible(scene::EDS_FULL&~scene::EDS_NORMALS);
//	node->setDebugDataVisible(scene::EDS_HALF_TRANSPARENCY);
	node->setDebugDataVisible(scene::EDS_SKELETON);
//	node->setMaterialFlag(video::EMF_LIGHTING, false);
//	node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	scene::ICameraSceneNode* cam =
		device->getSceneManager()->addCameraSceneNodeFPS();
	device->getSceneManager()->setAmbientLight(video::SColor(0xff333333));

	if (!cam)
	{
		std::cout << "Unable to create camera. Who knows why." << std::endl;
		return 1;
	}
	cam->setFarValue(20000);

	while (device->run())
	{
		device->getVideoDriver()->beginScene(true, true, video::SColor(255,100,128,188));
		device->getSceneManager()->drawAll();
		device->getVideoDriver()->endScene();
		core::stringw str(L"FPS: ");
		str.append(core::stringw(device->getVideoDriver()->getFPS()));
		str += L" Tris: ";
		str.append(core::stringw(device->getVideoDriver()->getPrimitiveCountDrawn()));
		device->setWindowCaption(str.c_str());
	}
	device->drop();

	return 0;
}

