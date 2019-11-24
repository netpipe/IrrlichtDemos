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
scene::IMesh* rigell_mesh;
scene::ISceneNode* rigell_node;

irr::newton::IBody* rigell_p_node;
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


			if(event.KeyInput.Key==KEY_SPACE) 
				debug_info=!debug_info;
			
			//launch a cube
			if(event.KeyInput.Key == KEY_KEY_C) 
				p_world->getUtils()->launchCube();		


		}
	}
	return false;
	}
}my_event_receiver;

core::array<newton::ICollision*> createCollisions() {

	core::array<newton::ICollision*> collisions;

	irr::newton::SBodyFromNode rigellData;
	rigellData.Node = rigell_node;
	rigellData.Mesh = rigell_mesh;
	rigellData.Type = newton::EBT_PRIMITIVE_BOX;



		//collision1
	const float offsetx = 20;
	const float offsetz = 20;

	const float offsety = 40;


	//collision 3
	rigellData.Type = newton::EBT_CONVEX_HULL;
	rigellData.BodyScaleOffsetFromNode = core::vector3df(1.5, 1, 1.5);
	rigellData.BodyOffsetFromNode.setTranslation(core::vector3df(0, 0, 0 ));
	collisions.push_back(p_world->createCollision(rigellData));


	//collision 1
	rigellData.BodyScaleOffsetFromNode = core::vector3df(1, 0.6f, 1);

	rigellData.Type = newton::EBT_PRIMITIVE_ELLIPSOID;
	rigellData.BodyOffsetFromNode.setTranslation(core::vector3df(0, -offsety ,0));
	collisions.push_back(p_world->createCollision(rigellData));

	//collision2
	
	rigellData.Type = newton::EBT_PRIMITIVE_ELLIPSOID;
	rigellData.BodyOffsetFromNode.setTranslation(core::vector3df(0, offsety , 0 ));
	collisions.push_back(p_world->createCollision(rigellData));



	//collision cylinder 1
	rigellData.BodyScaleOffsetFromNode = core::vector3df(1, 0.6f, 1);
	rigellData.BodyOffsetFromNode.setRotationDegrees( core::vector3df(0, 0, 90) );
	rigellData.Type = newton::EBT_PRIMITIVE_CAPSULE;

	rigellData.BodyOffsetFromNode.setTranslation(core::vector3df(offsetx, -offsety/2 ,0));
	collisions.push_back(p_world->createCollision(rigellData));


	//collision cylinder 2
	rigellData.BodyOffsetFromNode.setTranslation(core::vector3df(-offsetx, offsety/2 , 0 ));
	collisions.push_back(p_world->createCollision(rigellData));


	return collisions;

}

int main(int argc, char** argv) {
	//change working dir
	_chdir("../../");

	device=createDevice(video::EDT_DIRECT3D9);
	device->setEventReceiver(&my_event_receiver);
	smgr=device->getSceneManager();
	driver=device->getVideoDriver();

	//load a bsp quake map
	device->getFileSystem()->addZipFileArchive("media/level/map-20kdm2.pk3");
	scene::IAnimatedMesh* world_mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* world_node =	smgr->addOctTreeSceneNode(world_mesh->getMesh(0));
	world_node->setPosition(core::vector3df(45,0,0));
	scene::ITriangleSelector* world_tri_selector=smgr->createOctTreeTriangleSelector(world_mesh->getMesh(0),world_node);

	//load a model
	rigell_mesh = smgr->getMesh("media/rigell.md2")->getMesh(1);
    rigell_node =smgr->addMeshSceneNode(rigell_mesh);

	rigell_node->setMaterialFlag(video::EMF_LIGHTING,false);
	rigell_node->setMaterialTexture(0,driver->getTexture("media/rigell.jpg"));
	rigell_node->setPosition(core::vector3df(1469.6f,150.474f,1363.34f));


	//load an FPS camera
	camera=smgr->addCameraSceneNodeFPS();
	camera->setPosition(
		core::vector3df(1469.6f,146.474f,1363.34f));


	//INI PHYSICS
	core::array<newton::ICollision*> collisions;

	p_world = irr::newton::createPhysicsWorld(device);

	collisions = createCollisions();

	newton::ICollision* compound = p_world->createCollision(collisions);

	rigell_p_node = p_world->createBody(rigell_node, compound);


	//release all collision objects
	//first the compund collision
	compound->release();
	//then all the collision of the compound
	for(int i=0;i<collisions.size();i++)
		collisions[i]->release();



	irr::newton::SBodyFromNode treeData;
	treeData.Node = world_node;
	treeData.Mesh = world_mesh->getMesh(0);
	irr::newton::IBody* world_p_node = p_world->createBody(treeData);


	//materials!!
	//istanziate variables for all materials
	irr::newton::IMaterial* rigell_material;
	irr::newton::IMaterial* level_material;

	//create all materilas
	rigell_material = p_world->createMaterial();
	level_material  = p_world->createMaterial();

	//add they to the respective bodies
	//you can add the same materials to lots of different bodies
	//but viceversa is not possible, a body has got only one material
	rigell_p_node->setMaterial(rigell_material);
	world_p_node->setMaterial(level_material);

	//set the elasticity of the second body respect to the map
	rigell_material->setElasticity(level_material, 1.2f);

	//rigell_material->setElasticity(level_material,0.05f);

	//END OF INIT PHISICS

	//simulate gravity
	rigell_p_node->addForceContinuous(core::vector3df(0, -3, 0));

	while(device->run()) {

		//update physics
		p_world->update();

		driver->beginScene(true,true,video::SColor(0,0,0,0));
		smgr->drawAll();

		if(!debug_info) {
			rigell_p_node->drawDebugInfo();
		}
		else p_world->drawAllDebugInfos();

		driver->endScene();
	}

	return 0;
}




