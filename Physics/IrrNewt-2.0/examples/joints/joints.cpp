//include irrnewt header
#include "IrrNewt.hpp"

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

scene::ISceneNode* cube1;
scene::ISceneNode* cube2;

scene::ISceneNode* universal_cube1;
scene::ISceneNode* universal_cube2;

scene::ISceneNode* corkscrew_cube1;
scene::ISceneNode* corkscrew_cube2;

scene::ISceneNode* slider_cube1;
scene::ISceneNode* slider_cube2;

scene::ISceneNode* ball_cube1;
scene::ISceneNode* ball_cube2;

//the physics world
irr::newton::IWorld* p_world;

irr::newton::IBody* cube1_body;
irr::newton::IBody* cube2_body;

irr::newton::IBody* universal_cube1_body;
irr::newton::IBody* universal_cube2_body;

irr::newton::IBody* corkscrew_cube1_body;
irr::newton::IBody* corkscrew_cube2_body;

irr::newton::IBody* slider_cube1_body;
irr::newton::IBody* slider_cube2_body;

irr::newton::IBody* ball_cube1_body;
irr::newton::IBody* ball_cube2_body;


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


			//launch a cube
			if(event.KeyInput.Key==KEY_KEY_C)
				p_world->getUtils()->launchCube();


		}
	}
	return false;
	}
}my_event_receiver;

void createUniversalJoint() {
	//STEP 1:IRRLICHT NODES

	//universal first node
	universal_cube1 =	smgr->addCubeSceneNode(22);
	universal_cube1->setMaterialFlag(video::EMF_LIGHTING,false);
	universal_cube1->setScale(core::vector3df(1.0f,1.0f,0.18f));
	universal_cube1->setMaterialTexture(0,driver->getTexture("media\\wheel_texture.jpg"));
	universal_cube1->setPosition(core::vector3df(1469.6f,250.474f,1363.34f));

	//universal second node
	//the ellipsoid is only a scaled sphere
	universal_cube2=smgr->addCubeSceneNode(22);
	universal_cube2->setMaterialFlag(video::EMF_LIGHTING,false);
	universal_cube2->setScale(core::vector3df(1.0f,1.0f,0.18f));
	universal_cube2->setMaterialTexture(0,driver->getTexture("media\\wheel_texture.jpg"));
	universal_cube2->setPosition(core::vector3df(1494.6f,250.474f,1363.34f)); /*x=1490.6 don't contact*/

	//STEP 2:BODIES
	//create bodies
	irr::newton::SBodyFromNode bodyData;
	bodyData.Node=universal_cube1;
	//make the parent body a static body
	bodyData.Mass=0;
	//create the body
	universal_cube1_body=p_world->createBody(bodyData);

	bodyData.Node=universal_cube2;
	bodyData.Mass=1.0f;
	//create the body
	universal_cube2_body=p_world->createBody(bodyData);

	//STEP 3:JOINT
	//CREATE UNIVERSAL JOINT
	core::line3d<f32> line;
	line.start=universal_cube1->getPosition();
	line.end=universal_cube2->getPosition();

	core::vector3df pivot,pin0,pin1;
	pivot.set(line.getMiddle());

	irr::newton::SJointUniversal joint;
	joint.Pin0.set(0.0f,1.0f,0.0f);
	joint.Pin1.set(1.0f,0.0f,0.0f);
	joint.ParentBody=universal_cube1_body;
	joint.ChildBody=universal_cube2_body;
	joint.Pivot=pivot;
	irr::newton::IJointUniversal* bjoint=p_world->createJoint(joint);
	bjoint->setCollisionState(true);

}

void createCorkscrewJoint() {
	//STEP 1:IRRLICHT NODES

	//universal first node
	corkscrew_cube1 =	smgr->addCubeSceneNode(22);
	corkscrew_cube1->setMaterialFlag(video::EMF_LIGHTING,false);
	corkscrew_cube1->setScale(core::vector3df(1.0f,1.0f,0.18f));
	corkscrew_cube1->setMaterialTexture(0,driver->getTexture("media\\wheel_texture.jpg"));
	corkscrew_cube1->setPosition(core::vector3df(1469.6f,200.474f,1363.34f));

	//universal second node
	//the ellipsoid is only a scaled sphere
	corkscrew_cube2=smgr->addCubeSceneNode(22);
	corkscrew_cube2->setMaterialFlag(video::EMF_LIGHTING,false);
	corkscrew_cube2->setScale(core::vector3df(1.0f,1.0f,0.18f));
	corkscrew_cube2->setMaterialTexture(0,driver->getTexture("media\\wheel_texture.jpg"));
	corkscrew_cube2->setPosition(core::vector3df(1494.6f,200.474f,1363.34f)); /*x=1490.6 don't contact*/

	//STEP 2:BODIES
	//create bodies
	irr::newton::SBodyFromNode bodyData;
	bodyData.Node=corkscrew_cube1;
	//make the parent body a static body
	bodyData.Mass=0;
	//create the body
	corkscrew_cube1_body=p_world->createBody(bodyData);

	bodyData.Node=corkscrew_cube2;
	bodyData.Mass=1.0f;
	//create the body
	corkscrew_cube2_body=p_world->createBody(bodyData);

	//STEP 3:JOINT
	//CREATE CORKSREW JOINT
	core::line3d<f32> line;
	line.start=corkscrew_cube1->getPosition();
	line.end=corkscrew_cube2->getPosition();

	core::vector3df pivot,pin0,pin1;
	pivot.set(line.getMiddle());

	irr::newton::SJointCorkscrew joint;
	joint.PinDir.set(1.0f,0.0f,0.0f);
	joint.ParentBody=corkscrew_cube1_body;
	joint.ChildBody=corkscrew_cube2_body;
	joint.Pivot=pivot;
	newton::IJointCorkscrew* bjoint=p_world->createJoint(joint);
	bjoint->setCollisionState(true);
}

void createSliderJoint() {
	//STEP 1:IRRLICHT NODES

	//universal first node
	slider_cube1 =	smgr->addCubeSceneNode(22);
	slider_cube1->setMaterialFlag(video::EMF_LIGHTING,false);
	slider_cube1->setScale(core::vector3df(1.0f,1.0f,0.18f));
	slider_cube1->setMaterialTexture(0,driver->getTexture("media\\wheel_texture.jpg"));
	slider_cube1->setPosition(core::vector3df(1469.6f,150.474f,1363.34f));

	//universal second node
	//the ellipsoid is only a scaled sphere
	slider_cube2=smgr->addCubeSceneNode(22);
	slider_cube2->setMaterialFlag(video::EMF_LIGHTING,false);
	slider_cube2->setScale(core::vector3df(1.0f,1.0f,0.18f));
	slider_cube2->setMaterialTexture(0,driver->getTexture("media\\wheel_texture.jpg"));
	slider_cube2->setPosition(core::vector3df(1494.6f,150.474f,1363.34f)); /*x=1490.6 don't contact*/

	//STEP 2:BODIES
	//create bodies
	irr::newton::SBodyFromNode bodyData;
	bodyData.Node=slider_cube1;
	//make the parent body a static body
	bodyData.Mass=0;
	//create the body
	slider_cube1_body=p_world->createBody(bodyData);

	bodyData.Node=slider_cube2;
	bodyData.Mass=1.0f;
	//create the body
	slider_cube2_body=p_world->createBody(bodyData);

	//STEP 3:JOINT
	//CREATE SLIDER JOINT
	core::line3d<f32> line;
	line.start=slider_cube1->getPosition();
	line.end=slider_cube2->getPosition();

	core::vector3df pivot,pin0,pin1;
	pivot.set(line.getMiddle());

	irr::newton::SJointSlider joint;
	joint.PinDir.set(1.0f,0.0f,0.0f);
	joint.ParentBody=slider_cube1_body;
	joint.ChildBody=slider_cube2_body;
	joint.Pivot=pivot;
	newton::IJointSlider* bjoint=p_world->createJoint(joint);
	bjoint->setCollisionState(true);

}

void createBallJoint() {
	//STEP 1:IRRLICHT NODES

	//universal first node
	ball_cube1 =	smgr->addSphereSceneNode(22);
	ball_cube1->setMaterialFlag(video::EMF_LIGHTING,false);
	//ball_cube1->setScale(core::vector3df(1.0f,1.0f,0.18f));
	ball_cube1->setMaterialTexture(0,driver->getTexture("media\\wheel_texture.jpg"));
	ball_cube1->setPosition(core::vector3df(1469.6f,100.474f,1363.34f));

	//universal second node
	//the ellipsoid is only a scaled sphere
	ball_cube2=smgr->addCubeSceneNode(22);
	ball_cube2->setMaterialFlag(video::EMF_LIGHTING,false);
	ball_cube2->setScale(core::vector3df(1.0f,1.0f,0.18f));
	ball_cube2->setMaterialTexture(0,driver->getTexture("media\\wheel_texture.jpg"));
	ball_cube2->setPosition(core::vector3df(1506.6f,100.474f,1363.34f)); /*x=1490.6 don't contact*/

	//STEP 2:BODIES
	//create bodies
	irr::newton::SBodyFromNode bodyData;
	bodyData.Node=ball_cube1;
	//make the parent body a static body
	bodyData.Mass=0;
	//create the body
	ball_cube1_body=p_world->createBody(bodyData);

	bodyData.Node=ball_cube2;
	bodyData.Mass=1.0f;
	//create the body
	ball_cube2_body=p_world->createBody(bodyData);

	//STEP 3:JOINT
	//CREATE SLIDER JOINT
	core::line3d<f32> line;
	line.start=ball_cube1->getPosition();
	line.end=ball_cube2->getPosition();

	core::vector3df pivot,pin0,pin1;
	pivot.set(line.getMiddle());

	irr::newton::SJointBall joint;
	joint.ParentBody=ball_cube1_body;
	joint.ChildBody=ball_cube2_body;
	joint.Pivot=pivot;
	newton::IJointBall* bjoint=p_world->createJoint(joint);
	bjoint->setCollisionState(true);

}

int main(int argc, char** argv) {
	//change working dir
	_chdir("../../");

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
	device->getFileSystem()->addZipFileArchive("media\\level\\map-20kdm2.pk3");
	//get model mesh
	scene::IMesh* world_mesh = smgr->getMesh("media\\level\\20kdm2.bsp")->getMesh(0);
	//create the scene node
	scene::ISceneNode* world_node =	smgr->addOctTreeSceneNode(world_mesh);
	//position the scene node
	world_node->setPosition(core::vector3df(45,0,0));

	//CREATE FIRST NODE
	//the ellipsoid is only a scaled sphere
	//create the sphere scene node
	cube1 =	smgr->addCubeSceneNode(22);
	//the node is not affected by lights
	cube1->setMaterialFlag(video::EMF_LIGHTING,false);
	//scale the sphere to make it an ellipsoid
	cube1->setScale(core::vector3df(1.0f,1.0f,0.18f));
	//set a texture on it
	cube1->setMaterialTexture(0,driver->getTexture("media\\wheel_texture.jpg"));
	//set position of the ellipsoid
	cube1->setPosition(core::vector3df(1469.6f,300.474f,1363.34f));

	//CREATE SECOND NODE
	//the ellipsoid is only a scaled sphere
	//create the sphere scene node
	cube2 =	smgr->addCubeSceneNode(22);

	//the node is not affected by lights
	cube2->setMaterialFlag(video::EMF_LIGHTING,false);

	//scale the sphere to make it an ellipsoid
	cube2->setScale(core::vector3df(1.0f,1.0f,0.18f));

	//set a texture on it
	cube2->setMaterialTexture(0,driver->getTexture("media\\wheel_texture.jpg"));

	//set position of the ellipsoid
	cube2->setPosition(core::vector3df(1494.6f,300.474f,1363.34f)); /*x=1490.6 don't contact*/

	//create an FPS camera
	camera=smgr->addCameraSceneNodeFPS();
	camera->setPosition(
		core::vector3df(1469.6f,146.474f,1363.34f));

	//PHYSICS
	//Here are your first IrrNewt example!!!!!!!!! Let's add physics to our project
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
	sphereData.Node=cube1;

	//make the parent body a static body
	sphereData.Mass=0;

	//create the body relative to the sphere
	cube1_body=p_world->createBody(sphereData);

	//create cube2 body
	sphereData.Node=cube2;
	//make the child body a dynamic body
	sphereData.Mass=1.0f;
	cube2_body=p_world->createBody(sphereData);

	//let's create the body for our map
	//if we don't do this, the ellipsoid trought the map
	//the creation methos is the same
	irr::newton::SBodyFromNode mapData;
	mapData.Node=world_node;
	//because the node is a oct tree scene node and not a simple scene node we need to pass the mesh also
	mapData.Mesh=world_mesh;
	//create the body
	irr::newton::IBody* world_p_node=p_world->createBody(mapData);


	//CREATE HINGE JOINT
	core::line3d<f32> line;
	line.start=cube1->getPosition();
	line.end=cube2->getPosition();

	core::vector3df pivot,pin0,pin1;
	pivot.set(line.getMiddle());
	pin0.set(0.0f,1.0f,0.0f);

	irr::newton::SJointHinge joint;
	joint.PinDir.set(0.0f,1.0f,0.0f);
	joint.ParentBody=cube1_body;
	joint.ChildBody=cube2_body;
	joint.Pivot=pivot;
	newton::IJointHinge* bjoint=p_world->createJoint(joint);
	bjoint->setCollisionState(true);
	//END OF CREATING HINGE JOINT

	//create other joints
	createUniversalJoint();
	createCorkscrewJoint();
	createSliderJoint();
	createBallJoint();

	//END OF INIT PHISICS
	//add the gravity force to the ellipsoid
	//we use addForceContinuous and not addForce because addForce add the force one time
	//and addForceContinuous add the force recursive (always), and a body is always affected by gravity

	//cube1_body->addForceContinuous(core::vector3df(0,-2,0));

	while(device->run()) {

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

		//put the drawed scene on the screen
		driver->endScene();
	}

	return 0;
}




