//include haders and link libraries
#include "../../include/IrrNewt.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "irrlicht.h"

using namespace irr;

//------------------------------------------------------------
//declare some variables.
bool up=false/*move up if this variable is set to true,exc..*/,down=false,left=false,right=false;
bool brake=false/*handbrake if this variable is set to true*/;
//---------------------------------------------------

//some screen variables setting
const s32 resolution_x=640,resolution_y=480;
const bool fullscreen_mode=false;
const bool FSAA=false;
const u32 color_depth=16;

//show body as a wireframe rappresentation?
int debug_info=0;

//irrlicht variables
IrrlichtDevice* device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera;
scene::ISceneNode* chassis_node;
irr::SIrrlichtCreationParameters device_settings;

//level body
irr::newton::IBody* level_body;
//chassis physics body
irr::newton::IBody* chassis_p_node;
//the car
irr::newton::ICar* car_body;
//physics world
irr::newton::IWorld* p_world;
//front left tire
irr::newton::IVehicleTire* fl_tire;
//front right tire
irr::newton::IVehicleTire* fr_tire;
//back left tire
irr::newton::IVehicleTire* bl_tire;
//back right tire
irr::newton::IVehicleTire* br_tire;

//define this flagt for soft brake, don't define for handbrake
//#define SOFT_BRAKE

//some constants

const float chassis_cube_size = 20.0f;
const float chassis_mass = 20.0f;
const float chassis_gravity_y_force = -5.0f * chassis_mass;

const bool chassis_set_linear_damping = false;
const float chassis_linear_damping = 0.0f;

const bool chassis_set_angular_damping = false;
const core::vector3df chassis_angular_damping = core::vector3df(0.001f,0.001f,0.001f);

const bool chassis_set_friction_with_level = true;
const irr::f32 chassis_level_static_friction = 0.0f;
const irr::f32 chassis_level_kinematic_friction = 0.0f;

const float tire_mass = 0.5f;

const float tire_max_torque = 2.0f;
const float tire_torque_increment = 0.02f;

const float tire_max_steer_angle = 0.5f;
const float tire_steer_angle_increment = 0.5;  /* 0.005f  */

const float tire_suspension_shock = 30.0f;
const float tire_suspension_spring = 200.0f;
const float tire_suspension_length = 1.2f;

//softbrake
#ifdef SOFT_BRAKE

const float tire_brake_acceleration        = 0.0f;
const float tire_brake_max_torque_friction = 1.2f;

//handbrake

#else

const float tire_brake_acceleration        = 0.0f;
const float tire_brake_max_torque_friction = 30.0f;

#endif

bool demo_pause=false;
bool fps_cam=true;

core::vector3df tire_offset(
		11.5,
		7.5,
		11.5);

//------------------------------------------------------
//append a number to a string
template<class Num> std::string NumString(Num num) {
		std::ostringstream stringa;
		stringa<<num;
		return stringa.str();
}

//convert a const char* to a wchar_t*
wchar_t* CharWChar_T(const char* s,int max_char_size) {
	wchar_t* w_string=new wchar_t[max_char_size];
	mbstowcs(w_string,s,max_char_size);
	return w_string;
}

//-----------------------------------------------------------------
//the event receiver
class MyEventReceiver:public IEventReceiver {
public:
	virtual bool OnEvent(const SEvent &event) {
	if(event.EventType == EET_KEY_INPUT_EVENT) {
		if(event.KeyInput.PressedDown == true) {
			//when a key is pressed set apposite flag to true
			if(event.KeyInput.Key==KEY_KEY_A)
				left=true;

			if(event.KeyInput.Key==KEY_KEY_D)
				right=true;

			if(event.KeyInput.Key==KEY_KEY_W)
				up=true;

			if(event.KeyInput.Key==KEY_KEY_S)
				down=true;

			if(event.KeyInput.Key==KEY_CONTROL)
				brake=true;


			//show\hide debug info
			if(event.KeyInput.Key==KEY_SPACE) {
				debug_info++;
				if(debug_info>2) debug_info=0;
			}

		}//if(pressed_down==true)

		//when a key is released set apposite flag to false
		else if(event.KeyInput.PressedDown == false) { //pressed down false
			if(event.KeyInput.Key==KEY_KEY_W) {
				up=false;
				//rigell_p_node->setVelocityDirection(0,'u');
			}
			if(event.KeyInput.Key==KEY_KEY_S){
				down=false;
				//rigell_p_node->setVelocityDirection(0,'u');
			}

			if(event.KeyInput.Key==KEY_KEY_A)
				left=false;

			if(event.KeyInput.Key==KEY_KEY_D)
				right=false;

			if(event.KeyInput.Key==KEY_CONTROL)
				brake=false;


			if(event.KeyInput.Key==KEY_KEY_P)
				demo_pause=!demo_pause;


			//exit
			if(event.KeyInput.Key==KEY_ESCAPE)
				device->closeDevice();

			//launch a cube
			if(event.KeyInput.Key == KEY_KEY_C)
				p_world->getUtils()->launchCube(5);

			if(event.KeyInput.Key==KEY_KEY_R) {
				core::vector3df pos=chassis_p_node->getPosition();
				core::vector3df rot=chassis_node->getRotation();

				pos.Y+=50;
				rot.X=rot.Z=0;

				chassis_p_node->setPosition(pos);
				chassis_p_node->setRotation(rot);

				car_body->reset();
			}

			//switch camera fps-static camera (useful to capture a screenshot)

			/*
			if(event.KeyInput.Key==KEY_KEY_C) {
				//switch camera fps-non fps
				core::vector3df position=camera->getPosition();
				core::vector3df rotation=camera->getRotation();
				core::vector3df target=camera->getTarget();

				fps_cam=!fps_cam;
				camera->remove();

				if(fps_cam)
					camera=smgr->addCameraSceneNodeFPS();
				else
					camera=smgr->addCameraSceneNode();

				camera->setPosition(position);
				camera->setRotation(rotation);
				camera->setTarget(target);
			}
			*/



		}//else if(event.KeyInput.PressedDown == false) {
	}
	return false;
	}
};

//------------------------------------------------------------
//ask user to choose his favourite device settings
void GetDeviceSettings() {
	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5"\
		"\n (otherKey) exit\n");

	char i;
	std::cin>>i;

	switch(i)
	{
		case 'a': device_settings.DriverType = video::EDT_DIRECT3D9;break;
//		case 'b': device_settings.DriverType = video::EDT_DIRECT3D8;break;
		case 'c': device_settings.DriverType = video::EDT_OPENGL;   break;
		default: exit(0);
	}

	device_settings.Bits=color_depth;
	device_settings.AntiAlias=FSAA;
	device_settings.Fullscreen=fullscreen_mode;
	device_settings.WindowSize=core::dimension2d<s32>(resolution_x,resolution_y);
}

//-------------------------------------------------------------------
//create a irrlicht tire scene node
irr::scene::ISceneNode* createWheel() {
	scene::ISceneNode* wheel_node=smgr->addMeshSceneNode(smgr->getMesh("media/wheel.x")->getMesh(0));
	const float scale_value=0.04f;
	wheel_node->setScale(core::vector3df(scale_value,scale_value,scale_value));
	wheel_node->setRotation(core::vector3df(-90,0,0));
	wheel_node->setMaterialTexture(0,driver->getTexture("media/wheel_texture.jpg"));
	wheel_node->setMaterialFlag(video::EMF_LIGHTING,false);
	wheel_node->setMaterialType(video::EMT_SOLID);
	return wheel_node;
}

//-----------------------------------------------------------------------
//create the car
void createCar() {

	//STEP 1:CREATE CHASSIS BODY
	irr::newton::SBodyFromNode chassisData;

	chassisData.Type = newton::EBT_PRIMITIVE_BOX;

	chassisData.Node = chassis_node;
	chassisData.Mass = chassis_mass;
	chassisData.BodyOffsetFromNode.setScale(core::vector3df(1,1,1));
	chassis_p_node = p_world->createBody(chassisData);

	//down car's centre of mass

	core::vector3df centre_mass = chassis_p_node->getCentreOfMass();
//	centre_mass.Y -= 0.5f ;
	chassis_p_node->setCentreOfMass(centre_mass);


	//add gravity force (recursive)
	chassis_p_node->addForceContinuous(core::vector3df(0,chassis_gravity_y_force,0));

	//STEP 2: CREATE ALL TIRES SCENE NODE
	irr::scene::ISceneNode* fr_wheel_node = createWheel();
	irr::scene::ISceneNode* fl_wheel_node = createWheel();
	irr::scene::ISceneNode* br_wheel_node = createWheel();
	irr::scene::ISceneNode* bl_wheel_node = createWheel();

	//STEP 3:BUILD CAR DATA

	irr::newton::SCar car_data;
	car_data.TiresMass   = tire_mass;

	/*
	car_data.SuspensionShock  = 30 /6 ;
	car_data.SuspensionSpring = 200 /4;
	ca_data.SuspensionLength = 5;
	*/

	car_data.ChassisBody = chassis_p_node;
	car_data.TireNodeFR  = fr_wheel_node;
	car_data.TireNodeFL  = fl_wheel_node;
	car_data.TireNodeBR  = br_wheel_node;
	car_data.TireNodeBL  = bl_wheel_node;

	//auto calculate tires position. if don't set TiresOffsetFromChassis the tires position will be not modify
	car_data.TiresOffsetFromChassisBack = tire_offset;  ///added back to this to make it work

	//I wrote the line below only because in the wheel model file the model is rotated on X on 90 degrees.
	//Generally you don't need to do this
	car_data.TiresBodyOffsetFromSceneNode.Rotation.X = -90;

	//STEP 5: CERATE THE CAR
	car_body = p_world->createCar(car_data);

	//STEP 6:GET TIRES (OPTIONAL)
	br_tire = car_body->getBRTire();
	bl_tire = car_body->getBLTire();
	fr_tire = car_body->getFRTire();
	fl_tire = car_body->getFLTire();

	//STEP 7: SET MAX AND MIN STEER ANGLE AND TORQUE
	car_body->setMaxSteerAngle(tire_max_steer_angle);
	car_body->setSteerAngleIncrement(tire_steer_angle_increment);

	car_body->setMaxTorque(tire_max_torque);
	car_body->setTorqueIncrement(tire_torque_increment);

}

void setMaterialBehaviour() {
}

int main(int argc, char** argv) {

	GetDeviceSettings();
	device = createDeviceEx(device_settings);
MyEventReceiver my_event_receiver;
	device->setEventReceiver(&my_event_receiver);
	smgr=device->getSceneManager();
	driver=device->getVideoDriver();

	//change working dir
	//device->getFileSystem()->changeWorkingDirectoryTo("../../");

	driver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY,true);

	camera=smgr->addCameraSceneNodeFPS();

	//load a bsp quake map
	device->getFileSystem()->addZipFileArchive("media/level/prndm1.pk3");

	scene::IMesh* world_mesh = smgr->getMesh("media/level/prndm1.bsp")->getMesh(0);

	scene::ISceneNode* world_node=
		smgr->addOctreeSceneNode(world_mesh);

	world_node->setScale(core::vector3df(2,2,2));
	world_node->setMaterialType(video::EMT_LIGHTMAP_ADD);
	world_node->setPosition(core::vector3df(10,10,10));
	world_node->getMaterial(0).SpecularColor.set(0,0,0,0);
	world_node->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	world_node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS,true);

	//add a directional light
/*
	scene::ILightSceneNode* light=smgr->addLightSceneNode();
	video::SLight& lightData=light->getLightData();
	//lightData.Type=video::ELT_DIRECTIONAL;
	lightData.Position=core::vector3df(25,100,790);
	//lightData.Position.normalize();
	lightData.Radius=100;
*/
	//end of adding light

	chassis_node = smgr->addCubeSceneNode(chassis_cube_size);
	chassis_node->setScale(core::vector3df(1.0f,0.5f,1.0f));
	chassis_node->setPosition(core::vector3df(
		25.4911f,
		200.6005f,
		790.138f));

	//INI PHYSICS

	driver->beginScene(true,true,video::SColor(255,0,0,0));
	smgr->drawAll();
	driver->endScene();
	driver->beginScene(true,true,video::SColor(255,0,0,0));
	driver->endScene();

	p_world=irr::newton::createPhysicsWorld(device);

	//level body
	irr::newton::SBodyFromNode levelData;
	levelData.Node = world_node;
	levelData.Mesh = world_mesh;
	level_body = p_world->createBody(levelData);

	//create car
	createCar();

	//MATERIALS
	irr::newton::IMaterial* vehicle_material = p_world->createMaterial();
	chassis_p_node -> setMaterial(vehicle_material);

	irr::newton::IMaterial* level_material = p_world->createMaterial();
	level_body -> setMaterial(level_material);

	if(chassis_set_linear_damping)
		chassis_p_node->setLinearDamping(chassis_linear_damping);

	if(chassis_set_angular_damping)
		chassis_p_node->setAngularDamping(chassis_angular_damping);

	vehicle_material -> setElasticity(level_material, 0.0f);
	vehicle_material -> setSoftness(level_material, 0.0f);

	if(chassis_set_friction_with_level) {

	vehicle_material->setFriction(
		level_material,
		chassis_level_static_friction,
		chassis_level_kinematic_friction);

	}

	//main loop
	while(device->run()) {

		if(device->isWindowActive()) {
			//update the physics
			//it's important that you call this function every game loop

			if(!demo_pause)
				p_world->update();


	if(up && !down)
		car_body->forward();

	if(down && !up)
		car_body->backward();

	if( (!up && !down) || (up && down) )
		car_body->noMove();

	if(right && !left)
		car_body->turnRight();

	if(left && !right)
		car_body->turnLeft();


	if( (!right && !left) || (right && left) )
		car_body->noTurn();


	//brake
	if(brake) {
		br_tire->setCalculateMaxBrakeAcceleration(true);
		bl_tire->setCalculateMaxBrakeAcceleration(true);

		br_tire->setBrakeMaxFrictionTorque(tire_brake_max_torque_friction);
		bl_tire->setBrakeMaxFrictionTorque(tire_brake_max_torque_friction);

		fr_tire->setCalculateMaxBrakeAcceleration(true);
		fl_tire->setCalculateMaxBrakeAcceleration(true);

		fr_tire->setBrakeMaxFrictionTorque(tire_brake_max_torque_friction);
		fl_tire->setBrakeMaxFrictionTorque(tire_brake_max_torque_friction);

	}

	else{
		br_tire->setCalculateMaxBrakeAcceleration(false);
		bl_tire->setCalculateMaxBrakeAcceleration(false);

		fr_tire->setCalculateMaxBrakeAcceleration(false);
		fl_tire->setCalculateMaxBrakeAcceleration(false);

	}
	//end of brake


	/*
	car_body->getBRTire()->addTorque(torque);
	p_world->getAllCars()[0]->getBLTire()->addTorque(torque);
	p_world->getAllCars()[0]->getFRTire()->addSteerAngle(steer);
	p_world->getAllCars()[0]->getFLTire()->addSteerAngle(steer);
	*/

	/*
	//OLD
	br_tire->setTorque(torque);
	bl_tire->setTorque(torque);
	fr_tire->setSteerAngle(steer);
	fl_tire->setSteerAngle(steer);
	*/

	//vehicle controle end

	driver->beginScene(true,true,video::SColor(0,0,0,0));
	smgr->drawAll();

	if(debug_info==1) {
	}
	else if(debug_info==2) p_world->drawAllDebugInfos();
	driver->endScene();

	}//isWindowActive
}//while

	//device->drop();
	return 0;
}




