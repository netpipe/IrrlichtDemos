#include "../../include/IrrNewt.hpp"
#include <string>
#include <direct.h>
#include <sstream>
#include <vector>
#include <iostream>
#include "irrlicht.h"
#pragma comment(lib,"irrlicht.lib")
using namespace irr;

IrrlichtDevice* device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera;
irr::SIrrlichtCreationParameters device_settings;

irr::newton::IWorld* p_world;
irr::newton::IRagDoll* ragdoll;
newton::IMaterial* ragdoll_material;
newton::IMaterial* world_material;
newton::IMaterial* cube_material;

bool debug_info = false;

core::vector3df position(0.0f, 1000.0f, 0.0f);
const float distance = 250.0f;

//please note that total ragdoll is max_ragdollX * max_ragdollZ (10*10=100)
const int max_ragdollX = 6;
const int max_ragdollZ = 9;


const core::vector3df gravity(0.0f, -40.0f, 0.0f);



bool isFloorBelow(core::vector3df point) {

	irr::newton::SIntersectionPoint intPoint;

	core::line3d<f32> line(point, point);
	line.end.Y -= 999999999.9f;

	intPoint = p_world->getCollisionManager()->getCollisionFirstPointEx(line);

	return intPoint.body == NULL ? false : true;
}

bool axe = false;

class MyEventReceiver:public IEventReceiver {
public:
	virtual bool OnEvent(SEvent event) {
	if(event.EventType == EET_KEY_INPUT_EVENT) {
		if(event.KeyInput.PressedDown == true) {
		}//if(pressed_down==true)

		else if(event.KeyInput.PressedDown == false) { //pressed down false

			if(event.KeyInput.Key==KEY_SPACE) debug_info=!debug_info;
			if(event.KeyInput.Key==KEY_SHIFT) axe =! axe;


			//FORCES

			const float force = 50000;

			float force_to_apply = force;
		    axe?force_to_apply = -force_to_apply :true;

			//END

			//launch a cube
			if(event.KeyInput.Key==KEY_KEY_C)
				p_world->getUtils()->launchCube()->setMaterial(cube_material);


		}//else if(event.KeyInput.PressedDown == false) {
	}//OnEvent

	return false;
	}
}my_event_receiver;



//-------CREATE RAGDOLL-------------------------------------------------

void createRagdoll() {

	int coutX = 0;
	int coutZ = 0;

	for(float x = position.X ; ; x+= distance) {

		if(coutX >= max_ragdollX)  {
				return;
		}
		//increase count x and descrease count z
		++coutX;
		coutZ = 0;

		//z axis
		for(float z = position.Z ; ; z+= distance) {

			if(coutZ >= max_ragdollZ)  {
				break;
			}
			++coutZ;

			core::vector3df newPos = position + core::vector3df(x,0,z);


			//check for floor. If the floor is not below, don't create this ragdoll
			if( !isFloorBelow(newPos) ) continue;


	//-------------------------------------------------------------------------------------
	//CREATE RAGDOLL PART IRRLICHT
	//--------------------------------------------------------------------------------------

	scene::ISceneNode* ragdollNode =
		p_world->getUtils()->buildDefaultRagdoll(
		newPos,
		driver->getTexture("media/ragdoll-texture.jpg") );


	//-------------------------------------------------------------------------------------
	//CREATE RAGDOLL PART IRRNEWT
	//--------------------------------------------------------------------------------------

	newton::SRagDoll ragdollData;
	ragdollData.mainNode = ragdollNode;
	ragdoll = p_world->createRagDoll(ragdollData);

	//set gravity

	ragdoll->addForceContinuousToBones(gravity);
	ragdoll->setMaterialToBones(ragdoll_material);

	ragdoll->setFreezeToBones(true);
	ragdoll->setAutoFreezeToBones(true);


		}
	}


}


//-----------------END OF EVENT RECEIVER--------------------------

void GetDeviceSettings() {

	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5"\
		"\n (otherKey) exit\n");

	char i;
	std::cin>>i;

	switch(i)
	{
		case 'a': device_settings.DriverType = video::EDT_DIRECT3D9; break;
		case 'b': device_settings.DriverType = video::EDT_DIRECT3D8; break;
		case 'c': device_settings.DriverType = video::EDT_OPENGL;    break;
		default: exit(0);
	}

	device_settings.AntiAlias = false;
	device_settings.Bits = 16;
	device_settings.HighPrecisionFPU = false;
	device_settings.Fullscreen = false;
	device_settings.WindowSize = core::dimension2d<s32>(800,600);
}

int main(int argc, char** argv) {
	//change working dir
	_chdir("../../");

	GetDeviceSettings();
	device = createDeviceEx(device_settings);

	device->setEventReceiver(&my_event_receiver);
	smgr=device->getSceneManager();
	driver=device->getVideoDriver();

	driver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY, true);

	//load a bsp quake map
	device->getFileSystem()->addZipFileArchive("media/level/map-20kdm2.pk3");

	scene::IMesh* world_mesh = smgr->getMesh("media/level/20kdm2.bsp")->getMesh(0);


	scene::ISceneNode* world_node =	smgr->addOctTreeSceneNode(world_mesh);
//	world_node->setMaterialType(video::EMT_LIGHTMAP_M4);
	world_node->getMaterial(0).SpecularColor.set(0,0,0,0);
	world_node->updateAbsolutePosition();

	//try to scale the model and see that IrrNewt automatically
	//calculate the correct size of the shape
	//rigell_node->setScale(core::vector3df(8,2,8));

	//load an FPS camera
	camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition( position );

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);


	//INI PHYSICS
	p_world = irr::newton::createPhysicsWorld(device);
	ragdoll_material = p_world->createMaterial();
	world_material = p_world->createMaterial();
	cube_material = p_world->createMaterial();

	p_world->createBodyAuto(world_node,world_mesh)->setMaterial(world_material);


	//set ragdoll initial position and create them
	position.X = 450;
	position.Z = 200;
	createRagdoll();

	//set material settings
	//static 0.4 knematic 0.15
	world_material->setFriction(ragdoll_material, 0.35, 0.15);
	world_material->setSoftness(ragdoll_material, 0);
	world_material->setElasticity(ragdoll_material, 0);

	ragdoll_material->setElasticity(cube_material, 0.5	);

	world_material->setElasticity(cube_material, 0);
	world_material->setFriction(cube_material, 2, 2);

	while(device->run()) {

		if(device->isWindowActive()) {

			//update the physics
			//it's important that you call this function every game loop
		p_world->update();

		driver->beginScene(true,true,video::SColor(0,0,0,0));
		smgr->drawAll();

		if(debug_info) p_world->drawAllDebugInfos();

		core::stringw caption = L"FPS: ";
		caption += driver->getFPS();
		device->setWindowCaption(caption.c_str());

		driver->endScene();

		}//isWindowActive
	}

	device->drop();

	return 0;
}




