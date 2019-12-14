//this example show character controlling

#include <string>
#include <direct.h>
#include <iostream>
#include "irrlicht.h"
#include "../../include/IrrNewt.hpp"

#pragma comment(lib,"irrlicht.lib")
using namespace irr;

IrrlichtDevice* device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera;

irr::newton::ICharacterController* rigell_body;
irr::newton::IWorld* p_world;
irr::newton::ITreeBody* world_p_node;
bool debug_info=false;

const float rigell_speed=5;
const float rotation_speed = 2.0f * 100;
const float gravity_force=-30.0f;

bool up=false,down=false,left=false,right=false,jump=false,crouch=false;

class MyeventReceiver:public IEventReceiver {
public:
	virtual bool OnEvent(SEvent event) {
	if(event.EventType == EET_KEY_INPUT_EVENT) {

		if(event.KeyInput.PressedDown == true) {

			if(event.KeyInput.Key==KEY_KEY_A)
				left=true;
			if(event.KeyInput.Key==KEY_KEY_D)
				right=true;
			if(event.KeyInput.Key==KEY_KEY_W) 
				up=true;
			if(event.KeyInput.Key==KEY_KEY_S)
				down=true;
			if(event.KeyInput.Key==KEY_KEY_J)
				jump=true;
			if(event.KeyInput.Key==KEY_KEY_C)
				crouch=true;
		}

		if(event.KeyInput.PressedDown == false) {

			if(event.KeyInput.Key==KEY_KEY_A)
				left=false;
			if(event.KeyInput.Key==KEY_KEY_D)
				right=false;
			if(event.KeyInput.Key==KEY_KEY_W) 
				up=false;
			if(event.KeyInput.Key==KEY_KEY_S)
				down=false;
			if(event.KeyInput.Key==KEY_KEY_J)
				jump=false;
			if(event.KeyInput.Key==KEY_KEY_C)
				crouch=false;
			if(event.KeyInput.Key==KEY_SPACE)
				debug_info=!debug_info;
		
		}
	}
	return false;
	}
}my_event_receiver;

int main(int argc, char** argv) {

	//info
	printf("Hi. This is irrnewt character controlling example. "
		"The scope of this example is to show IrrNewt functions of character controlling.\n"
		"These functions don't include only movement, but also jumping, crouching and stair climbing\n\n"
		"Yes!!! IrrNewt has got an internal algorithm that check if there is a stair in body "
		"movement direction and if yes make the body climb it!!!!\n"
		"The scope of this tutorial IS NOT to show irrlicht animation system, so the body don't have animation"
		"\n\nKeys:\nw,s\tmove the body\na,d\trotate the body\nj\t jump\nc\t crouch\narrows\t move camera\nmouse\t rotate camera"
		"\n\nPlease try to walk onto astair WITHOUT jumping to see stair climbing algorithm in action! Also with irregular movements!!"
		"\n\nPress enter to continue....");

	int i;
	std::cin>>i;

	//change working dir
	_chdir("../../");

	device=createDevice(video::EDT_DIRECT3D9);
	device->setEventReceiver(&my_event_receiver);
	smgr=device->getSceneManager();
	driver=device->getVideoDriver();

	//load a bsp quake map
	device->getFileSystem()->addZipFileArchive("media/level/map-20kdm2.pk3");
	scene::IAnimatedMesh* world_mesh = smgr->getMesh("media/level/20kdm2.bsp");
	scene::ISceneNode* world_node =
		smgr->addOctTreeSceneNode(world_mesh->getMesh(0));
	world_node->setPosition(core::vector3df(45,0,0));
	scene::ITriangleSelector* world_tri_selector=smgr->createOctTreeTriangleSelector(world_mesh->getMesh(0),world_node);

	//load a model
	scene::IAnimatedMesh* rigell_mesh = smgr->getMesh("media/rigell.md2");
	scene::ISceneNode* rigell_node =smgr->addMeshSceneNode(
		rigell_mesh->getMesh(1) );
	rigell_node->setMaterialFlag(video::EMF_LIGHTING,false);
	rigell_node->setMaterialTexture(0,driver->getTexture("media/rigell.jpg"));
	rigell_node->setPosition(core::vector3df(1469.6f,250.474f,1363.34f));

	//load an FPS camera
	camera=smgr->addCameraSceneNodeFPS();
	camera->setPosition(
		core::vector3df(1469.6f,146.474f,1363.34f));

	//avoid that the camera trought the walls
	/*
	scene::ISceneNodeAnimatorCollisionResponse* anim=
		smgr->createCollisionResponseAnimator(
		world_tri_selector,camera,core::vector3df(30,30,30),core::vector3df(0,0,0));
	camera->addAnimator(anim);
	anim->drop();
	*/

	//PHYSICS

	//create world
	p_world = irr::newton::createPhysicsWorld(device);

	//create level tree
	newton::SBodyFromNode worldData;
	worldData.Node = world_node;
	worldData.Mesh = world_mesh->getMesh(0);
	world_p_node = (irr::newton::ITreeBody*) p_world->createBody(worldData);

	//body 1
	newton::SBodyFromNode bodyData;
	bodyData.Type = newton::EBT_PRIMITIVE_CAPSULE;
	bodyData.Node = rigell_node;
//	bodyData.Mesh = rigell_mesh->getMesh(1);

	rigell_body = p_world->createCharacterController(p_world->createBody(bodyData));
	rigell_body->setRotationUpdate(false);
	rigell_body->setContinuousCollisionMode(true);

	//the body rotate never
	p_world->getUtils()->avoidRotationOnAllAxes(rigell_body);

	//materials!! see materials and shapes example
	irr::newton::IMaterial* rigell_material;
	irr::newton::IMaterial* level_material;

	rigell_material = p_world->createMaterial();
	level_material  = p_world->createMaterial();

	rigell_body->setMaterial(rigell_material);
	world_p_node->setMaterial(level_material);

	//rigell<->level comportaments
	rigell_material->setElasticity(level_material, 0.0f);
	rigell_material->setFriction(level_material, 0.0f,0.0f);
	rigell_material->setSoftness(level_material, 1.0f);



	// set the force to apply if in front of the character there is a stair
	rigell_body->setClimbStairForce(core::vector3df(0,4,0));

	//set the max stair-step height from floor for the first stair-step
	//from the below stair-step for all the stairs-step except the first
	rigell_body->setStairMaxHeight( 40 );

	//add gravity
	rigell_body->addForceContinuous(core::vector3df(0,
		gravity_force,
		0));
	
	//loop
	while(device->run()) {

		//CHARACTER CONTROLLING
		core::vector3df rigell_rotation=rigell_node->getRotation();

		//STEP 1: ROTATING
		//make node rotation frame rate indipendent by multiply it by time elasped from last update
		float rotation_to_apply = rotation_speed * p_world->getTimeElapsed();

		if(right) 
			rigell_rotation.Y+=rotation_to_apply;

		if(left) 
			rigell_rotation.Y-=rotation_to_apply;

		rigell_node->setRotation(rigell_rotation);

		//STEP 2: MOVING
		core::vector3df velocity = rigell_body->getVelocity();
		velocity.X=velocity.Z=0;

		if(up) //without FRI in function name the movement is not frame rate indipendent
			velocity += rigell_body->FRIgetDirectionPositionY(core::vector3df(rigell_speed,0,0));
		if(down)
			velocity += rigell_body->FRIgetDirectionPositionY(core::vector3df(-rigell_speed,0,0));

		rigell_body->setVelocity(velocity);
		
		//STEP 3:JUMPING
		//the function jump(f32) take the velocity of the jumping
		if(jump)
			rigell_body->jump(6);

		//STEP 4:CROUNCHING
		//the crouch() function takes the new scale of y of the node for crouching
		//example 0.5 will reduce the character height by 1/2 (  so new_height = current_height - (current_height/2)  )
		//0.25 will reduce the chracter height by 1/4 (  so new_height = current_height - (current_height/2)  )
		//2 will double the character height

		const f32 crouch_value = 0.001f;

		if(crouch)
			rigell_body->crouch(crouch_value);

		else 
			rigell_body->stand(0);


		//END OF CHARACTER CONTROLLING

		p_world->update();

		driver->beginScene(true,true,video::SColor(0,0,0,0));
		smgr->drawAll();

		//show debug info
		if(debug_info)
			p_world->drawAllDebugInfos();

		driver->endScene();

	}

	device->drop();
	return 0;
}




