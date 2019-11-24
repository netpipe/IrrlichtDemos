/*
This examples shows irrnewt raycast functions to dect collision between a line and a body or lots of bodies
*/
#include "..\\..\\include\\IrrNewt.hpp"
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
	virtual bool OnEvent(const SEvent& event) {
	if(event.EventType == EET_KEY_INPUT_EVENT) {
		if(event.KeyInput.PressedDown == false) {

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


			if(event.KeyInput.Key == KEY_KEY_C)
				p_world->getUtils()->launchCube();

		}
	}
	return false;
	}
}my_event_receiver;

int main(int argc, char** argv) {
	//change working dir
	_chdir("../../");

	device=createDevice(video::EDT_OPENGL);
	device->setEventReceiver(&my_event_receiver);
	smgr=device->getSceneManager();
	driver=device->getVideoDriver();

	//load a bsp quake map
	device->getFileSystem()->addZipFileArchive("media\\level\\map-20kdm2.pk3");
	scene::IAnimatedMesh* world_mesh = smgr->getMesh("media\\level\\20kdm2.bsp");
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
		smgr->addMeshSceneNode(
		rigell_mesh->getMesh(50));
	rigell2_node->setMaterialFlag(video::EMF_LIGHTING,false);
	rigell2_node->setMaterialTexture(0,driver->getTexture("media\\rigell.jpg"));
	rigell2_node->setPosition(
		core::vector3df(1439.6f,150.474f,1333.34f));
	//end of rigell2 node

	//load an FPS camera
	camera=smgr->addCameraSceneNodeFPS();
	camera->setPosition(
		core::vector3df(1469.6f,146.474f,1363.34f));

	//avoid that the camera trought the walls
	scene::ISceneNodeAnimatorCollisionResponse* anim=
		smgr->createCollisionResponseAnimator(
		world_tri_selector,camera,core::vector3df(30,30,30),core::vector3df(0,0,0));
	camera->addAnimator(anim);
	anim->drop();

	//PHYSICS

	p_world=irr::newton::createPhysicsWorld(device);

	//body 1
	newton::SBodyFromNode bodyData;
	bodyData.Node=rigell_node;
	bodyData.Mesh=rigell_mesh->getMesh(50);
	//set mass of teh body, 1 for default
	bodyData.Mass=2;

	rigell_p_node=p_world->createBody(bodyData);

	//rigell 2
	bodyData.Node  = rigell2_node;
	rigell2_p_node = p_world->createBody(bodyData);

	p_world->getUtils()->avoidRotationOnAllAxes(rigell_p_node);
	p_world->getUtils()->avoidRotationOnAllAxes(rigell2_p_node);

	//create map body
	//recycle the same struct :-)
	bodyData.Node=world_node;
	bodyData.Mesh=world_mesh->getMesh(0);
	irr::newton::IBody* world_p_node=
		p_world->createBody(bodyData);

	//materials!! see materials and shapes example
	irr::newton::IMaterial* rigell_material;
	irr::newton::IMaterial* rigell2_material;
	irr::newton::IMaterial* level_material;

	rigell_material=p_world->createMaterial();
	rigell2_material=p_world->createMaterial();
	level_material=p_world->createMaterial();

	rigell_p_node->setMaterial(rigell_material);
	rigell2_p_node->setMaterial(rigell2_material);
	world_p_node->setMaterial(level_material);

	rigell_material->setElasticity(level_material,/*1.25f*/0);
	rigell2_material->setElasticity(level_material,0.05f);

	//END OF INIT PHISICS

	/*
	Here are your first ray cast irrnewt function
	*/
	//create a line wich start at body position and end below the body
	//check intersection with map
	core::line3d<f32> line_fall;
	line_fall.start=line_fall.end=rigell_node->getPosition();
	line_fall.end.Y-=99999.0f;

	//struct to store result informations
	irr::newton::SIntersectionPoint out;

	//use getCollisionPoint to ray cast a line with a single body
	if(p_world->getCollisionManager()->getCollisionPoint(
		world_p_node,//the body
		line_fall,//the line wich intersecate the body
		out//store result informations
		)) {//getCollisionPoint return true if a collision is checked, otherwise false

		//set body position in the cheked point
		//(positiong the body on to the floor)
		//improve point y position to leave the space for the body
		out.point.Y += rigell_p_node->getShapeSize().Y/2;
		rigell_p_node->setPosition(out.point);
	}

	//the same with the other body
	line_fall.start = line_fall.end = rigell2_node->getPosition();
	line_fall.end.Y -= 99999.0f;

	if(p_world->getCollisionManager()->getCollisionPoint(
		world_p_node,
		line_fall,
		out)) {

		out.point.Y+=rigell_p_node->getShapeSize().Y/2;
		rigell2_p_node->setPosition(out.point);
	}
	//end of fall down

	//now check collision between a line and multiple bodies
	//getcollisionpoints test
	//build a line
	core::line3d<f32> line;
	rigell_node->updateAbsolutePosition();
	line.start=line.end=rigell_node->getPosition();
	line.end.Z-=9999.0f;
	line.end.X-=9999.0f;

	//an array wich contain all raycasts output informations (an element to the array correspond to a cheked collision with a body)
	irr::core::array<irr::newton::SIntersectionPoint> inter_point;

	//getCollisionPoints get the colllisions between the line and all bodies in the scene
	//if no collision is cheked the array will be empty
	p_world->getCollisionManager()->getCollisionPoints(line,&inter_point);

	//draw an image for every collision point
	for(int i=0;i<inter_point.size();i++) {
		scene::IBillboardSceneNode* bill=smgr->addBillboardSceneNode();
		bill->setMaterialTexture(0,driver->getTexture("billboard.bmp"));
		bill->setPosition(inter_point[i].point);
	}

	//gravity
	rigell_p_node->addForceContinuous(core::vector3df(0,-2,0));
	rigell2_p_node->addForceContinuous(core::vector3df(0,-2,0));

	while(device->run()) {

		p_world->update();

		driver->beginScene(true,true,video::SColor(0,0,0,0));
		smgr->drawAll();


		//draw ray cast line
		core::matrix4 mat;
		video::SMaterial material;
		material.setTexture(0,0);
		material.Lighting = false;
		device->getVideoDriver()->setTransform(video::ETS_WORLD, mat);
		device->getVideoDriver()->setMaterial(material);
		driver->draw3DLine(line.start,line.end,video::SColor(255, 255, 255, 0));
		//end

		//p_world->DrawDebugInfo();

		if(!debug_info) {
			rigell_p_node->drawDebugInfo();
			rigell2_p_node->drawDebugInfo();
		}
		else p_world->drawAllDebugInfos();

		driver->endScene();
	}

	device->drop();
	return 0;
}




