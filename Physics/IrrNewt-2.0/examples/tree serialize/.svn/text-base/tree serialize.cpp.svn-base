#include "../../include/IrrNewt.hpp"
#include <string>
#include <direct.h>
#include <iostream>
#include "irrlicht.h"
#pragma comment(lib,"irrlicht.lib")
#pragma comment(lib,"newton.lib")
using namespace irr;

IrrlichtDevice* device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
irr::newton::IWorld* p_world;

int main(int argc, char** argv) {
	//change working dir
	_chdir("../../");

	device=createDevice(video::EDT_NULL);
	smgr=device->getSceneManager();
	driver=device->getVideoDriver();

	//load a bsp quake map
	device->getFileSystem()->addZipFileArchive("media\\level\\map-20kdm2.pk3");
	scene::IMesh* world_mesh = smgr->getMesh("media\\level\\20kdm2.bsp")->getMesh(0);
	scene::ISceneNode* world_node =
		smgr->addOctTreeSceneNode(world_mesh);
	world_node->setPosition(core::vector3df(45,0,0));

	//INI PHYSICS
	p_world = irr::newton::createPhysicsWorld(device);

	//create tree body
	newton::SBodyFromNode data;

	data.Node = world_node;
	data.Mesh = world_mesh;
	data.Type = newton::EBT_TREE;

	irr::newton::ITreeBody* world_p_node = (irr::newton::ITreeBody*)p_world->createBody(data);

	//save the tree collision into a file
	world_p_node->serialize("media\\level.ntc");

	device->getLogger()->log("\n\nTree collision serialized in media\\level.ntc. Now you can run tree \"tree deserialize.exe\"\nPress a key to continue...");
	int c = getchar();

	
	/*
	smgr->addCameraSceneNodeFPS();
	while(device->run()) {
		p_world->update();
		driver->beginScene(true,true,video::SColor(0,0,0,0));
		smgr->drawAll();
		p_world->drawAllDebugInfos();
		driver->endScene();
	}
	*/

	//drop the device and close application
	device->drop();

	return 0;
}




