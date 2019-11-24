/*
This example illustrates materials and shapes. Read hello world example first
*/

#include "..\\..\\include\\irrnewt.hpp"

#include <string>
#include <direct.h>
#include "irrlicht.h"
#pragma comment(lib,"irrlicht.lib")
using namespace irr;

IrrlichtDevice* device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera;

irr::newton::IBody* rigell_p_node;
irr::newton::IBody* rigell2_p_node;
irr::newton::IWorld* p_world;

bool debug_info;

class MyeventReceiver:public IEventReceiver {
public:
	virtual bool OnEvent(SEvent event) {
	if(event.EventType == EET_KEY_INPUT_EVENT) {
		if(event.KeyInput.PressedDown == false) {

			//to positiong nodes and\or adda  force to they
			if(event.KeyInput.Key==KEY_KEY_A)
				rigell_p_node->addForce(core::vector3df(0,150,-150));
			
			if(event.KeyInput.Key==KEY_KEY_S)
				rigell_p_node->setPosition(camera->getPosition());
			

			if(event.KeyInput.Key==KEY_KEY_D)
				rigell2_p_node->addForce(core::vector3df(0,150,-150));
			

			if(event.KeyInput.Key==KEY_KEY_F)
				rigell2_p_node->setPosition(camera->getPosition());
			

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
	scene::IAnimatedMesh* world_mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* world_node =
		smgr->addOctTreeSceneNode(world_mesh->getMesh(0));
	world_node->setPosition(core::vector3df(45,0,0));
	scene::ITriangleSelector* world_tri_selector=smgr->createOctTreeTriangleSelector(world_mesh->getMesh(0),world_node);

	//load a model
	scene::IAnimatedMesh* rigell_mesh = smgr->getMesh("media\\rigell.md2");
	scene::ISceneNode* rigell_node =smgr->addMeshSceneNode(
		rigell_mesh->getMesh(50));
	rigell_node->setMaterialFlag(video::EMF_LIGHTING,false);
	rigell_node->setMaterialTexture(0,driver->getTexture("media\\rigell.jpg"));
	rigell_node->setPosition(core::vector3df(1469.6f,150.474f,1363.34f));

	//load an other scene node based on the same model
	scene::ISceneNode* rigell2_node = 
		smgr->addSphereSceneNode(20);
	
	rigell2_node->setMaterialFlag(video::EMF_LIGHTING,false);
	rigell2_node->setMaterialTexture(0,driver->getTexture("media\\wheel_texture.jpg"));
	rigell2_node->setPosition(
		core::vector3df(1439.6f,150.474f,1333.34f));
	//end of rigell2 node

	//load an FPS camera
	camera=smgr->addCameraSceneNodeFPS();
	camera->setPosition(
		core::vector3df(1469.6f,146.474f,1363.34f));


	//INI PHYSICS
	p_world=irr::newton::createPhysicsWorld(device);
	irr::newton::SBodyFromNode rigellData;
	rigellData.Node=rigell_node;
	rigellData.Mesh=rigell_mesh->getMesh(50);
	rigellData.Type=newton::EBT_CONVEX_HULL;
	rigell_p_node=p_world->createBody(rigellData);


	//rigell 2
	//create another body based on the same model
	rigellData.Node=rigell2_node;
	rigellData.Mesh=NULL;
	rigellData.Type=newton::EBT_AUTODETECT;
	rigell2_p_node=p_world->createBody(rigellData);

	irr::newton::SBodyFromNode treeData;
	treeData.Node=world_node;
	treeData.Mesh=world_mesh->getMesh(0);
	irr::newton::IBody* world_p_node=p_world->createBody(treeData);

	//materials!!
	//istanziate variables for all materials
	irr::newton::IMaterial* rigell_material;
	irr::newton::IMaterial* rigell2_material;
	irr::newton::IMaterial* level_material;

	//create all materilas
	rigell_material=p_world->createMaterial();
	rigell2_material=p_world->createMaterial();
	level_material=p_world->createMaterial();

	//add they to the respective bodies
	//you can add the same materials to lots of different bodies
	//but viceversa is not possible, a body has got only one material
	rigell_p_node->setMaterial(rigell_material);
	rigell2_p_node->setMaterial(rigell2_material);
	world_p_node->setMaterial(level_material);

	//set the elasticity of the second body respect to the map
	rigell_material->setElasticity(level_material,1.3f);

	//rigell_material->setElasticity(level_material,0.05f);

	//END OF INIT PHISICS

	//simulate gravity
	rigell_p_node->addForceContinuous(core::vector3df(0,-2,0));
	rigell2_p_node->addForceContinuous(core::vector3df(0,-2,0));

	rigell2_p_node->addForce(core::vector3df(3.5,0,3.5));

	while(device->run()) {

		//update physics
		p_world->update();

		driver->beginScene(true,true,video::SColor(0,0,0,0));
		smgr->drawAll();

		if(!debug_info) {
			rigell_p_node->drawDebugInfo();
			rigell2_p_node->drawDebugInfo();
		}
		else p_world->drawAllDebugInfos();

		driver->endScene();
	}

	return 0;
}




