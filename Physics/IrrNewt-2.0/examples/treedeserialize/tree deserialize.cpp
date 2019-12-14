#include "../../include/irrnewt.hpp"
#include <string>
#include <direct.h>
#include "irrlicht.h"
#pragma comment(lib,"irrlicht.lib")
#pragma comment(lib,"newton.lib")
using namespace irr;

IrrlichtDevice* device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera;

irr::newton::IBody* rigell_p_node;
irr::newton::IWorld* p_world;
bool debug_info;

class MyeventReceiver:public IEventReceiver {
public:
	virtual bool OnEvent(SEvent event) {
	if(event.EventType == EET_KEY_INPUT_EVENT) {
		if(event.KeyInput.PressedDown == false) {

			if(event.KeyInput.Key==KEY_KEY_A)
				rigell_p_node->addForce(core::vector3df(0,150,-150));
			
			if(event.KeyInput.Key==KEY_KEY_S)
				rigell_p_node->setPosition(camera->getPosition());
			
			if(event.KeyInput.Key==KEY_SPACE)
				debug_info=!debug_info;

		    //launch a cube
			if(event.KeyInput.Key==KEY_KEY_C)
				p_world->getUtils()->launchCube();
			
		}
	}
	return false;
	}
}my_event_receiver;

int main(int argc, char** argv) {
	//change working dir
	_chdir("../../");

	device=createDevice(video::EDT_DIRECT3D9);
	device->setEventReceiver(&my_event_receiver);
	smgr=device->getSceneManager();
	driver=device->getVideoDriver();

	//load a bsp quake map
	device->getFileSystem()->addZipFileArchive("media\\level\\map-20kdm2.pk3");
	scene::IAnimatedMesh* world_mesh = smgr->getMesh("media\\level\\20kdm2.bsp");
	scene::ISceneNode* world_node =
		smgr->addOctTreeSceneNode(world_mesh->getMesh(0));
	world_node->setPosition(core::vector3df(0,45,0));

	//load a model
	scene::IAnimatedMesh* rigell_mesh = smgr->getMesh("media\\rigell.md2");
	scene::ISceneNode* rigell_node =smgr->addMeshSceneNode(
		rigell_mesh->getMesh(50));
	rigell_node->setMaterialFlag(video::EMF_LIGHTING,false);
	rigell_node->setMaterialTexture(0,driver->getTexture("media\\rigell.jpg"));
	rigell_node->setPosition(core::vector3df(1469.6f,150.474f,1363.34f));

	//load an FPS camera
	camera=smgr->addCameraSceneNodeFPS();
	camera->setPosition(
		core::vector3df(1469.6f,146.474f,1363.34f));


	//INI PHYSICS
	//world
	p_world = irr::newton::createPhysicsWorld(device);

	//load the tree from file!!!
	irr::newton::ITreeBody* world_p_node=
		p_world->deserializeTreeBody(
		world_node,//the attached node
		"media\\level.ntc"//the file
		);

	if(world_p_node==NULL) {
		device->getLogger()->log(
			"FATAL ERROR: File level.ntc not found. "
			"Please run tree_serialize example before run "
			"this program. Press enter to exit",
			ELL_ERROR);
		int i=getchar();
		device->drop();
		exit(-1);
	}

	newton::SBodyFromNode rigellData;

	rigellData.Node = rigell_node;
	rigellData.Mesh = rigell_mesh->getMesh(50);
	rigellData.Type = newton::EBT_PRIMITIVE_BOX;

	rigell_p_node = p_world->createBody(rigellData);

	rigell_p_node->setMass(2);
	rigell_p_node->calculateMomentOfInertia();
	rigell_p_node->addForceContinuous(core::vector3df(0,-2,0));

	while(device->run()) {

		p_world->update();

		driver->beginScene(true,true,video::SColor(0,0,0,0));
		smgr->drawAll();

		if(!debug_info) {
			rigell_p_node->drawDebugInfo();
		}
		else p_world->drawAllDebugInfos();

		driver->endScene();
	}

	device->drop();
	return 0;
}




