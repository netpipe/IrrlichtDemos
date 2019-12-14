//include irrnewt header
#include "../../include/IrrNewt.hpp"

#include <string>
//#include <direct.h>
#include "irrlicht.h"

using namespace irr;

IrrlichtDevice* device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera;

//the physics world
irr::newton::IWorld* p_world;
irr::newton::IBody* sphere_body;
irr::newton::IBody* sphere2_body;
irr::newton::IBody* sphere3_body;

bool debug_info;


//IRRLICHT EVENT RECEIVER
//see irrlicht examples
class MyeventReceiver:public IEventReceiver {
public:
	virtual bool OnEvent(const SEvent& event) {
	if(event.EventType == EET_KEY_INPUT_EVENT) {
		if(event.KeyInput.PressedDown == false) {

			if(event.KeyInput.Key==KEY_SPACE) {
				//change debug drawing mode when the user press space.
				//press space to active debug mode, re press space another time to deactive it
				debug_info=!debug_info;
			}

			if(event.KeyInput.Key==KEY_KEY_L) {
				//change debug drawing mode when the user press space.
				//press space to active debug mode, re press space another time to deactive it
				sphere_body->setScale(core::vector3df(1,3.0,1));
			}

			//launch a cube
		//	if(event.KeyInput.Key == KEY_KEY_C)
//				p_world->getUtils()->launchCube();


		}
	}
	return false;
	}
}my_event_receiver;


#include <iostream>
using namespace std;

int main(int argc, char** argv) {
	//change working dir
//	_chdir("../../");

	//create irrlicht device
	device=createDevice(video::EDT_OPENGL);
	//set event receiver
	device->setEventReceiver(&my_event_receiver);
	//get scene manager to avoid to write device->getSceneManager every time
	smgr=device->getSceneManager();
	//get video driver for the same reason
	driver=device->getVideoDriver();

	//load a map as an oct tree scene node
	//add a zip file
	device->getFileSystem()->addZipFileArchive("media/level/map-20kdm2.pk3");
	//get model mesh
	scene::IMesh* world_mesh = smgr->getMesh("media/level/20kdm2.bsp")->getMesh(0);
	//create the scene node
	scene::ISceneNode* world_node =	smgr->addOctreeSceneNode(world_mesh);
	//position the scene node
	world_node->setPosition(core::vector3df(45,0,0));

	//create ellipsoid
	//the ellipsoid is only a scaled sphere
	//create the sphere scene node

	scene::ISceneNode* sphere_node =
		smgr->addSphereSceneNode(22);

	//the node is not affected by lights
	sphere_node->setMaterialFlag(video::EMF_LIGHTING,false);

	//scale the sphere to make it an ellipsoid
	sphere_node->setScale(core::vector3df(1.0f,1.0f,1.0f));

	//set a texture on it
	sphere_node->setMaterialTexture(0,driver->getTexture("media/wheel_texture.jpg"));
	//set position of the ellipsoid
	sphere_node->setPosition(core::vector3df(1469.6f,150.474f,1350.34f));

	//SECOND SPHERE
	scene::ISceneNode* sphere2_node =smgr->addSphereSceneNode(22);
	sphere2_node->setMaterialFlag(video::EMF_LIGHTING,false);
	sphere2_node->setScale(core::vector3df(1.0f,1.0f,1.0f));
	sphere2_node->setMaterialTexture(0,driver->getTexture("media/wheel_texture.jpg"));
	sphere2_node->setPosition(core::vector3df(1469.6f,150.474f,1450.34f));

	//THIRD SPHERE
	scene::ISceneNode* sphere3_node =smgr->addSphereSceneNode(22);
	sphere3_node->setMaterialFlag(video::EMF_LIGHTING,false);
	sphere3_node->setScale(core::vector3df(1.0f,1.0f,1.0f));
	sphere3_node->setMaterialTexture(0,driver->getTexture("media/wheel_texture.jpg"));
	sphere3_node->setPosition(sphere2_node->getPosition());
	core::vector3df sphere3_position=sphere2_node->getPosition();

	//CAMERA
	//create an FPS camera
	camera=smgr->addCameraSceneNodeFPS();
	camera->setPosition(
		core::vector3df(1469.6f,146.474f,1363.34f));

	//ANIMATOR
	scene::ISceneNodeAnimator* anim=smgr->createFlyCircleAnimator(
		sphere_node->getPosition(),
		(sphere_node->getPosition()-sphere2_node->getPosition()).getLength(),
		0.001f
		);

	sphere2_node->addAnimator(anim);
	anim->drop();

	//sphere node animtor
	/*
	anim=smgr->createFlyCircleAnimator(
		sphere2_node->getPosition(),
		(sphere2_node->getPosition()-sphere_node->getPosition()).getLength(),
		0.001f
		);

	sphere_node->addAnimator(anim);
	anim->drop();
	*/

	//PHYSICS
	//Let's add physics to our project
	//We want that the ellipsoid bounce on the map.
	//Let's create physics world first
	//create the physics world. a physics world indicates the world
	//where our bodies is placed
	p_world=irr::newton::createPhysicsWorld(device);

	//then create a body relative to the sphere scene node
	//let's instantiate a irr::newton::SBodyFromNode object
	//to store our body data (like mass, node, mesh,exc..)
	irr::newton::SBodyFromNode sphereData;
	//set the node attached to the body as the sphere scene node
	sphereData.Node=sphere_node;

	//create the body relative to the sphere
	sphere_body=p_world->createBody(sphereData);

	sphereData.Node=sphere2_node;
	sphere2_body=p_world->createBody(sphereData);

	sphereData.Node=sphere3_node;
	sphere3_body=p_world->createBody(sphereData);

	//let's create the body for our map
	//if we don't do this, the ellipsoid trought the map
	//the creation methos is the same
	irr::newton::SBodyFromNode mapData;
	mapData.Node=world_node;
	//because the node is a oct tree scene node and not a simple scene node we need to pass the mesh also
	mapData.Mesh=world_mesh;

	//create the body
	irr::newton::IBody* world_p_node=p_world->createBody(mapData);

	//END OF INIT PHISICS

	//add the gravity force to the ellipsoid
	//we use addForceContinuous and not addForce because addForce add the force one time
	//and addForceContinuous add the force recursive (always), and a body is always affected by gravity

	//sphere_body->addForceContinuous(core::vector3df(0,-2,0));

	while(device->run()) {

		sphere3_body->setPosition(sphere3_position);

		//update physics
		//it's very important that you cann this function inside game loop
		p_world->update();

		//clear z buffer
		driver->beginScene(true,true,video::SColor(0,0,0,0));
		//draw the scene
		smgr->drawAll();

		if(debug_info)
			//this function will show all newton body as a wireframe rappresentation (the lines are yellow).
			//Generally the newton bodies are transparent
			p_world->drawAllDebugInfos();

		//draw line with conjuct closes points
		sphere2_node->updateAbsolutePosition();
		sphere2_body->setPosition(sphere2_node->getPosition());

		core::matrix4 mat;
		video::SMaterial material;
		material.setTexture(0,0);
		material.Lighting = false;
		device->getVideoDriver()->setTransform(video::ETS_WORLD, mat);
		device->getVideoDriver()->setMaterial(material);

		core::vector3df start,end,normal;
		p_world->getCollisionManager()->getClosestPoint(
			sphere_body,
			sphere2_body,
			start,end,normal);

		//draw line in red
		driver->draw3DLine(start,end,video::SColor(255, 255, 0, 0));
		//end

		//CONTACT POINTS
		irr::core::array<irr::core::vector3df> contacts;
		irr::core::array<irr::core::vector3df> normals;
		irr::core::array<irr::f32> penetration;

		p_world->getCollisionManager()->collide(
			sphere2_body,
			sphere3_body,
			96,//max size
			contacts,normals,penetration);

		//draw points
		for(u32 i=0;i<contacts.size();i++) {
			core::vector3df end=contacts[i];
			end+=(normals[i]*10);
			driver->draw3DLine(contacts[i],end,video::SColor(255, 0, 255, 0));

		}
		//END OF CONTACT POINTS




		//put the drawed scene on the screen
		driver->endScene();
	}

	return 0;
}




