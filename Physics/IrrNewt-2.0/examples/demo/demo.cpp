#include "../../include/IrrNewt.hpp"
#include <string>
//#include <direct.h>
#include <sstream>
#include <vector>
#include <iostream>
#include "irrlicht.h"

using namespace irr;

//------------------------------------------------------------
bool up=false,down=false,left=false,right=false,can_move=true;
bool high_rotation=false;

//---------------------------------------------------
const bool solve_model_bug=false;

const float rigell_gravity_y_force=-120.2f*40.0f;
const float rigell_rotation_speed=0.3f;
const float rigell_high_rotation_speed=1.15f;

const float rigell_speed=6;

const float sphere_gravity_y_force=-20.0f;
const float sphere_force = 1300;
const float sphere_size=6.2f;

const float fire_cube_size=10;
const int fire_cube_particle_system_scale=2;

//pin constants
const float pin_mass=1.0f;
const float pin_gravity_y_force=-5.0f*pin_mass;

const bool set_pin_linear_damping=true;
const float pin_linear_damping=0.4f;

const bool set_pin_level_friction=false;
const float pin_level_static_friction=0.9f;
const float pin_level_kinematic_friction=0.1f;


//GENERAL VARIABLES
const s32 resolution_x = 800,resolution_y = 600;
const bool fullscreen_mode = true;

IrrlichtDevice* device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera;
scene::IAnimatedMeshSceneNode* rigell_node;
scene::ISceneNode* sphere_node;
irr::SIrrlichtCreationParameters device_settings;

std::vector<irr::newton::IBody*> pins;
irr::newton::ICharacterController* rigell_p_node;
irr::newton::ICharacterController* sphere_p_node;
irr::newton::IBody* level_p_node;
irr::newton::IWorld* p_world;
irr::newton::IMaterial* fire_material;
irr::newton::IMaterial* character_material;
irr::newton::IMaterial* pin_material;
irr::newton::IMaterial* ball_material;

//a material wich refuse all contacts. used for temporany bodies (used for calculations)
irr::newton::IMaterial* temp_fire_material;
int debug_info=0;

//TEMP
irr::core::array< irr::core::line3d<f32> > all_lines;

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

//----------------------------------------------------------------

//game functions
//rigell animations
int last_anim=0;

void AnimateMs3d(scene::IAnimatedMeshSceneNode* node,int inizio, int fine,int totale){
	if (inizio==0)
		node->setFrameLoop(((s32)(1.984f*totale)), ((s32)(fine*41.65f)));
	if (inizio!=0)
		node->setFrameLoop( (s32)(inizio*41.65f),  (s32)(fine*41.65f) );
}

void Stand() {
	if(last_anim!=1) {
		last_anim=1;
		rigell_node->setFrameLoop(1,1);
		rigell_node->setAnimationSpeed(100);
		rigell_node->setLoopMode(false);
	}
}

void Run() {
	if(last_anim!=2) {
		last_anim=2;
		rigell_node->setMD2Animation("run");/*setFrameLoop(33, 350)/*, 94)*/;
		rigell_node->setAnimationSpeed(100);
		rigell_node->setLoopMode(true);
	}
}

//sow a tex in the center of the screen for some seconds
void ShowText(wchar_t* text) {
	static scene::ITextSceneNode* text_node=NULL;
	static irr::u32 last_time=device->getTimer()->getTime();

	const static irr::u32 life_time=2500;

	if(text!=NULL) {

	if(text_node!=NULL) {
		text_node->remove();
		text_node=NULL;
	}

	text_node=smgr->addTextSceneNode(
		device->getGUIEnvironment()->getBuiltInFont(),
		text,
		video::SColor(255, 255, 255, 255),
		0,
		core::vector3df(
			0,
			0,
			1)
		);

		text_node->setScale(core::vector3df(100,100,100));
		text_node->updateAbsolutePosition();
		camera->addChild(text_node);
		last_time=device->getTimer()->getTime();
	}//if(text_node!=NULL) {

	if((device->getTimer()->getTime()-last_time)>=life_time&&
		text_node!=NULL) {

		text_node->remove();
		text_node=NULL;
	}

}

//create a box
void CreatePin(core::vector3df pos) {

	scene::ISceneNode* pin_node=smgr->addCubeSceneNode(20);
	pin_node->setMaterialTexture(0,
		driver->getTexture("media\\wheel_texture.jpg"));
	pin_node->setMaterialFlag(video::EMF_LIGHTING,false);
	pin_node->setPosition(pos);

	pin_node->setScale(core::vector3df(0.5f,1.5f,0.5f));

	//physics
	newton::SBodyFromNode boxData;
	boxData.Node=pin_node;
	boxData.Mass=pin_mass;
	irr::newton::IBody* pin_body=p_world->createBody(boxData);

	pin_body->addForceContinuous(core::vector3df(0,
		pin_gravity_y_force,0));

	pin_body->setContinuousCollisionMode(true);
	pin_body->setMaterial(pin_material);

	if(set_pin_linear_damping)
		pin_body->setLinearDamping(pin_linear_damping);

	pins.push_back(pin_body);
}

//--------------MATERIAL COLLISION CALLBACK-----------------
//callback between character and blazing sphere

class Material_Character_Fire_class:public irr::newton::IMaterialCollisionCallback{
public:
	virtual int ContactBegin(
		irr::newton::IMaterialPair* material,
		irr::newton::IBody* body0,
		irr::newton::IBody* body1
		) {

		/*
		irr::f32 value=-(rigell_speed*100);
		rigell_node->updateAbsolutePosition();

		core::vector3df force=rigell_p_node->FRIgetDirectionPositionY(core::vector3df(
			value,
			0,
			0));

		//rigell_p_node->setRotation(last_rot);

		//get sphere body
		newton::IBody* sphere_body;
		if(body0==rigell_p_node) sphere_body=body1;
		else sphere_body=body0;


		//improve dimension of sphere body by creating a new bigger sphere
		scene::ISceneNode* temp_sphere=smgr->addSphereSceneNode(fire_cube_size*2.8);
		temp_sphere->setPosition(sphere_body->getPosition());
		temp_sphere->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);

		newton::SBodyFromNode newSphereData;
		newSphereData.Node=temp_sphere;
		sphere_body=p_world->createBody(newSphereData);

		sphere_body->setMaterial(temp_fire_material);

		//create line
		core::line3d<f32> line;
		line.start=rigell_p_node->getPosition();
		line.end=rigell_p_node->getPosition()+force;
		line.start.Y=line.end.Y=sphere_body->getPosition().Y;

		//ceck collision between line and sphere
		irr::newton::SIntersectionPoint point;
		bool result=p_world->getCollisionManager()->getCollisionPoint(sphere_body,line,point);

		//if collide move in the opposite direction
		if(result) {
			core::vector3df line_vector=line.getVector();
			line_vector.invert();

			force=rigell_p_node->FRIgetDirectionPositionY(
			core::vector3df(
			-value,
			0,
			0));
		}


		rigell_p_node->addForce(force);
		*/

//		rigell_p_node->setVelocity(core::vector3df(0,0,0));
//		rigell_p_node->setForce(core::vector3df(0,0,0));

		core::vector3df last_direction=rigell_p_node->getLastDirectionParameter();
		irr::f32 last_time_elapsed=rigell_p_node->getLastTimeElapsed();
		//last_direction/=last_time_elapsed;

		last_direction+=rigell_node->getPosition();
		last_direction.invert();
		last_direction=last_direction.normalize();
		last_direction*=150.0f;//acceleration
		last_direction*=40.0f;//rigell_mass
		last_direction.Y=0;

		//debug
		std::string c=NumString(last_direction.X);
		c+=",";
		c+=NumString(last_direction.Y);
		c+=",";
		c+=NumString(last_direction.Z);
		c+=";\n";
		device->getLogger()->log(c.c_str());
		//debug end


  		rigell_p_node->addForce(last_direction);

		rigell_p_node->cantSetVelocityWhileMove();
		//rigell_p_node->cantForceWhileMove();


		//remove the temporany sphere
		//sphere_body->remove();

		ShowText(L"Life -");

		return 1;
	}

	virtual int ContactProcess(
				irr::newton::IMaterialPairAndContact* material) {
		return 0;//refuse contact
	}
}Material_Character_Fire;


//create a blazing sphere
void CreateFire(core::vector3df pos) {
	scene::ISceneNode* cube=smgr->addSphereSceneNode(fire_cube_size);
	cube->setPosition(pos);

	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(
		false);
	ps->setParticleSize(core::dimension2d<f32>(
		/*20.0f, 10.0f*/ 22.5f,12.5f));

	scene::IParticleAffector* paf =ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();

	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0,driver->getTexture("media\\fire.jpg"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

	scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-7,0,-7,7,1,7), //box
		/*core::vector3df(0.0f,0.03f,0.0f)*/
		core::vector3df(0.0f,0.03f,0.0f),//direction
		80,//minParticlesPerSecond
		100,//maxParticlesPerSecond
		video::SColor(0,255,255,255),//minStartColor
		video::SColor(0,255,255,255),//maxStartColor
		800,//life time min
		2000, //life time max
		0 //max angle degrees
		);

	ps->setEmitter(em);
	em->drop();

	ps->setScale(core::vector3df(
		fire_cube_particle_system_scale,
		fire_cube_particle_system_scale,
		fire_cube_particle_system_scale));

	//add the particle system as child of the cube
	cube->addChild(ps);

	//physics
	newton::SBodyFromNode fireData;
	fireData.Node=cube;
	fireData.Mass=2.0f;
	irr::newton::IBody* fire_body=p_world->createBody(fireData);

	fire_body->setMaterial(fire_material);
	fire_body->addForceContinuous(core::vector3df(0,-5,0));
}
//--------------------EVENT RECEIVER BEGIN-------------------------------
enum E_GAME_PHASE{
	EGP_NORMAL,
	EGP_SPHERE_GETTED
};

E_GAME_PHASE game_phase=EGP_NORMAL;

class MyEventReceiver:public IEventReceiver {
public:
	bool OnEvent(const SEvent event) {
		can_move=true;
	if(event.EventType == EET_KEY_INPUT_EVENT) {
		if(event.KeyInput.PressedDown == true) {

			if(event.KeyInput.Key==KEY_KEY_A) {
				left=true;
				high_rotation=false;
			}
			if(event.KeyInput.Key==KEY_KEY_D) {
				right=true;
				high_rotation=false;
			}
			if(event.KeyInput.Key==KEY_KEY_E) {
				left=true;
				high_rotation=true;
			}
			if(event.KeyInput.Key==KEY_KEY_R) {
				right=true;
				high_rotation=true;
			}


			if(event.KeyInput.Key==KEY_KEY_W) {
				Run();
				up=true;
			}
			if(event.KeyInput.Key==KEY_KEY_S) {
				Run();
				down=true;
			}
			if(event.KeyInput.Key==KEY_SPACE) {
				debug_info++;
				if(debug_info>2) debug_info=0;
			}

		}//if(pressed_down==true)

		else if(event.KeyInput.PressedDown == false) { //pressed down false
			if(event.KeyInput.Key==KEY_KEY_F) {
				CreateFire(camera->getPosition());
			}

			if(event.KeyInput.Key==KEY_KEY_W) {
				Stand();

				up=false;
				//rigell_p_node->setVelocityDirection(0,'u');
			}
			if(event.KeyInput.Key==KEY_KEY_S){
				Stand();

				down=false;
				//rigell_p_node->setVelocityDirection(0,'u');
			}

			if(event.KeyInput.Key==KEY_KEY_A) {
				left=false;
			}
			if(event.KeyInput.Key==KEY_KEY_D) {
				right=false;
			}
			if(event.KeyInput.Key==KEY_KEY_E) {
				left=false;
				high_rotation=false;
			}
			if(event.KeyInput.Key==KEY_KEY_R) {
				right=false;
				high_rotation=false;
			}

			if(event.KeyInput.Key==KEY_ESCAPE) {
				device->closeDevice();
			}

			if(event.KeyInput.Key==KEY_KEY_C)
//				p_world->getUtils()->launchCube();


			//get the ball!!!
			if(event.KeyInput.Key==KEY_KEY_G) {
				const float distance_sphere_rigell=60.0f;

				if(game_phase!=EGP_SPHERE_GETTED) {

				if(rigell_node->getPosition().getDistanceFrom(sphere_node->getPosition())<=distance_sphere_rigell) {

						//get the ball
						game_phase=EGP_SPHERE_GETTED;

						//refuse contact between character and ball
						ball_material->setCollidable(character_material,false);

						//annul gravity force
						sphere_p_node->addForceContinuous(core::vector3df(0,sphere_gravity_y_force,0));

						//freeze
						sphere_p_node->setFreeze(true);

						sphere_node->setPosition(rigell_node->getPosition());
						sphere_node->updateAbsolutePosition();
						sphere_p_node->setNodeMatrixToBody();
					}//if(rigell_node->getPosition().getDistanceFrom(

					else{
					ShowText(
						L"You are too far for getting the sphere");
						}//else{
				}//if(game_phase!=EGPSPHERE_GETTED) {

				else{
					ShowText(
						L"You already have the sphere");
				}//else
			}//if(event.KeyInput.Key==KEY_KEY_G) {

			if(event.KeyInput.Key==KEY_KEY_M) {
				core::vector3df rot=sphere_node->getRotation();
				rot.Y=rigell_node->getRotation().Y;
				sphere_p_node->setRotation(rot);

				//sphere_p_node->addTorque(core::vector3df(20000,0,0));
			}

			//lauch the ball
			if(event.KeyInput.Key==KEY_KEY_L) {
				if(game_phase==EGP_SPHERE_GETTED) {
					game_phase=EGP_NORMAL;

					//unfreeze the sphere
					sphere_p_node->setFreeze(false);

					//accept contact between character and ball
					ball_material->setCollidable(character_material,true);

					//stop it (?)
					sphere_p_node->setVelocity(core::vector3df(0,0,0));

					//restore gravity force
					sphere_p_node->addForceContinuous(core::vector3df(0,sphere_gravity_y_force,0));

					core::vector3df pos = rigell_p_node->FRIgetDirectionAbsolutePositionY(core::vector3df(30,0,0));
					sphere_p_node->setPosition(pos);

					//sphere_node->setPosition(pos);
					//sphere_node->updateAbsolutePosition();

					//to fall the sphere

					irr::newton::SIntersectionPoint int_point;
					core::line3d<f32> line(
						sphere_node->getPosition(),
						sphere_node->getPosition());
					line.end.Y-=999999.0f;

					int_point=p_world->getCollisionManager()->getCollisionFirstPointEx(
						line);

					if(int_point.body!=NULL) {
						int_point.point.Y+=(sphere_p_node->getShapeSize().Y/2)+0.01;
						sphere_p_node->setPosition(int_point.point);
					}

					sphere_node->updateAbsolutePosition();
					//sphere_p_node->posRotScaleAsNode();

					//sphere_p_node->adjustRotationForDirectionFunctions();

					//set y rotation like character y rotation
					//otherwise addForceDirection move the sphere
					//considerating y rotation as 0

					rigell_node->updateAbsolutePosition();

					core::vector3df rot = sphere_node->getRotation();
					rot.Y = rigell_node->getRotation().Y;
					sphere_p_node->setRotation(rot);

					sphere_node->updateAbsolutePosition();

					core::vector3df force = rigell_p_node->FRIgetDirectionPositionY(
						core::vector3df(sphere_force,0,0));
					sphere_p_node->addForce(force);

				}

				else{
					ShowText(
						L"First get the sphere");

				}//else{
			}//if(event.KeyInput.Key==KEY_KEY_L) {
		}//else if(event.KeyInput.PressedDown == false) {
	}

	return false;
	}
};

//-----------------END OF EVENT RECEIVER--------------------------

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

	printf("\n\nPlease press y if you want to active FSAA,"
		"otherwise press another key\n");

	std::cin>>i;
	switch(i)
	{
		case 'y': device_settings.AntiAlias = true;break;
		default: device_settings.AntiAlias = false;break;
	}

	device_settings.Bits=16;
	device_settings.Fullscreen=fullscreen_mode;
	device_settings.WindowSize=core::dimension2d<s32>(resolution_x,resolution_y);
}

//MAIN FUNCTION

int main(int argc, char** argv) {

	GetDeviceSettings();
	device=createDeviceEx(device_settings);

	//change working dir
	//std::string c = device->getFileSystem()->getWorkingDirectory();
	//c += "/../../";
	//device->getLogger()->log(c.c_str());
	//_chdir(c.c_str());
  //  MyEventReceiver my_event_receiver;
	//device->setEventReceiver(&my_event_receiver);
	smgr=device->getSceneManager();
	driver=device->getVideoDriver();

	driver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY,true);

	//load a bsp quake map
	device->getFileSystem()->addZipFileArchive("media/level/prndm1.pk3");

	scene::IMesh* world_mesh = smgr->getMesh("media/level/prndm1.bsp")->getMesh(0);

	//world_mesh=smgr->getMeshManipulator()->createMeshWithTangents(world_mesh);
	//smgr->getMeshManipulator()->makePlanarTextureMapping(world_mesh,0.015f);
	//smgr->getMeshManipulator()->flipSurfaces(world_mesh);

	scene::ISceneNode* world_node=
		smgr->addOctreeSceneNode(world_mesh);

	world_node->setScale(core::vector3df(2,2,2));

	world_node->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);

	world_node->setMaterialType(video::EMT_LIGHTMAP_ADD);

	world_node->setPosition(core::vector3df(10,10,10));

	world_node->getMaterial(0).SpecularColor.set(0,0,0,0);

	//load a model
	scene::IAnimatedMesh* rigell_mesh = smgr->getMesh("media\\rigell.md2");
	rigell_node =smgr->addAnimatedMeshSceneNode(rigell_mesh);
	rigell_node->setMaterialFlag(video::EMF_LIGHTING,false);
	rigell_node->setMaterialTexture(0,driver->getTexture("media\\rigell.jpg"));

	rigell_node->setPosition(core::vector3df(
		14.4911f,
		80.6005f,
		175.138f));

	rigell_node->setRotation(core::vector3df(
		0,
		-90,
		0));


	//try to scale the model and see that IrrNewt automatically
	//calculate the correct size of the shape
	//rigell_node->setScale(core::vector3df(8,2,8));

	Stand();

	//create a sphere
	sphere_node = smgr->addSphereSceneNode(
		sphere_size);
	sphere_node->setMaterialFlag(video::EMF_LIGHTING,false);
	sphere_node->setMaterialTexture(
		0,driver->getTexture("media\\bowling_ball_texture.jpg"));
	sphere_node->setPosition(core::vector3df(
		7.03517f,
		61.707f,
		270.943f));

	//load an FPS camera
	camera=smgr->addCameraSceneNodeFPS();
	camera->setPosition(
		rigell_node->getPosition());

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	//INI PHYSICS

	driver->beginScene(true,true,video::SColor(255,0,0,0));
	smgr->drawAll();
	driver->endScene();
	driver->beginScene(true,true,video::SColor(255,0,0,0));
	driver->endScene();

	p_world=irr::newton::createPhysicsWorld(device);

	//rigell
	irr::newton::SBodyFromNode character_data;
	character_data.Type=newton::EBT_PRIMITIVE_CHAMFER_CYLINDER;
	character_data.Node=rigell_node;
	character_data.Mesh=smgr->getMesh("media\\rigell.md2")->getMesh(0);
	character_data.BodyOffsetFromNode.setScale(core::vector3df(1.0f,1.0f,2.0f));

	rigell_p_node=
		p_world->createCharacterController(
		p_world->createBody(
		character_data
		));

	rigell_p_node->setMass(40.0f);
	rigell_p_node->calculateMomentOfInertia();
	rigell_p_node->setRotationUpdate(false);
	rigell_p_node->setContinuousCollisionMode(true);


	//rotation constraint. avoid that the body rotate on x and z
	irr::newton::SJointUpVector upVectorData;
	upVectorData.ParentBody=rigell_p_node;
	upVectorData.PinDir.set(0.0f,1.0f,0.0f);
	irr::newton::IJointUpVector* rigell_p_node_up_vector_joint=
		p_world->createJoint(upVectorData);

	//rotation constraint. avoid that the body rotate on y
	upVectorData.PinDir.set(1.0f,0.0f,1.0f);
	rigell_p_node_up_vector_joint=
		p_world->createJoint(upVectorData);

	//create the bowling ball
	newton::SBodyFromNode sphereData;
	sphereData.Node = sphere_node;
	sphereData.Type = newton::EBT_PRIMITIVE_ELLIPSOID;

	sphere_p_node = p_world->createCharacterController(p_world->createBody(sphereData));
	sphere_p_node->setContinuousCollisionMode(true);
	sphere_p_node->setPosition(rigell_node->getPosition());

	//level
	level_p_node=
		p_world->createBodyAuto(
		world_node,
		world_mesh
		);

	if(solve_model_bug) {

	smgr->getMeshManipulator()->flipSurfaces(world_mesh);

	p_world->createBodyAuto(
		world_node,
		world_mesh
		);

	}

	//materials!!
	character_material=p_world->createMaterial();
	pin_material=p_world->createMaterial();
	ball_material=p_world->createMaterial();
	temp_fire_material=p_world->createMaterial();
	irr::newton::IMaterial* level_material=p_world->createMaterial();
	fire_material=p_world->createMaterial();

	rigell_p_node->setMaterial(character_material);
	sphere_p_node->setMaterial(ball_material);
	level_p_node->setMaterial(level_material);

	if(set_pin_level_friction) {
		pin_material->setFriction(
			level_material,
			pin_level_static_friction,
			pin_level_kinematic_friction);
	}

	fire_material->setCollisionCallback(character_material,&Material_Character_Fire);

	//refuse all contacts, temp material
	temp_fire_material->setCollidable(level_material,false);
	temp_fire_material->setCollidable(character_material,false);
	temp_fire_material->setCollidable(pin_material,false);
	temp_fire_material->setCollidable(fire_material,false);
	temp_fire_material->setCollidable(level_material,false);

	//character material and fire
	character_material->setElasticity(fire_material,0.0f);
	character_material->setFriction(fire_material,0.0f,0.0f);
	character_material->setSoftness(fire_material,0.0f);
	character_material->setCollisionCallback(fire_material,&Material_Character_Fire);

	//character material and level

	//rigell_p_node->setLinearDamping(0.0f);
	//rigell_p_node->setAngularDamping(core::vector3df(0.0f,0,0));

	character_material->setFriction(level_material,0.37f,0.10f);
	character_material->setSoftness(level_material,0.0f);
	character_material->setElasticity(level_material,0.0f);

	//end of materials

	//create the pins
	CreatePin(core::vector3df(
		-27.8529f,
		61.7861f,
		677.379f));
	CreatePin(core::vector3df(
		-18.8529f,
		61.7861f,
		667.379f));
	/*
	CreatePin(core::vector3df(
		0.8529f,
		61.7861f,
		655.379f));
		*/
	//end of pins creation

	//gravity forces
	rigell_p_node->addForceContinuous(core::vector3df(0,rigell_gravity_y_force,0));
	sphere_p_node->addForceContinuous(core::vector3df(0,sphere_gravity_y_force,0));


	//move the body
	/*
	rigell_p_node->setPosition(
		rigell_p_node->FRIgetDirectionAbsolutePositionY(core::vector3df(600,0,0))
		);
		*/

	//END OF INIT PHYSICS


	//simulate the pression of the 'f' key
	//to create a fire
	core::vector3df cam_pos=camera->getPosition();
	camera->setPosition(core::vector3df(0,100,-250));
	SEvent event;
	event.EventType=EET_KEY_INPUT_EVENT;
	event.KeyInput.PressedDown=false;
	event.KeyInput.Key=KEY_KEY_F;
	device->postEventFromUser(event);
	camera->setPosition(cam_pos);

	//create another balzing sphere
	cam_pos=camera->getPosition();
	camera->setPosition(core::vector3df(0,100,250));
	event.EventType=EET_KEY_INPUT_EVENT;
	event.KeyInput.PressedDown=false;
	event.KeyInput.Key=KEY_KEY_F;
	device->postEventFromUser(event);
	camera->setPosition(cam_pos);


	//simulate the pression of the 'g' key
	//to get the sphere
	event.EventType=EET_KEY_INPUT_EVENT;
	event.KeyInput.PressedDown=false;
	event.KeyInput.Key=KEY_KEY_G;
	device->postEventFromUser(event);

	while(device->run()) {

		if(device->isWindowActive()) {

			//update the physics
			//it's important that you call this function every game loop
		p_world->update();

		if(game_phase==EGP_SPHERE_GETTED) {
			sphere_p_node->setPosition(rigell_node->getPosition());
		}

	core::vector3df rigell_rotation=rigell_node->getRotation();

	if(right&&!high_rotation)
		rigell_rotation.Y+=rigell_rotation_speed;
	if(left&&!high_rotation)
		rigell_rotation.Y-=rigell_rotation_speed;

	if(right&&high_rotation)
		rigell_rotation.Y+=rigell_high_rotation_speed;
	if(left&&high_rotation)
		rigell_rotation.Y-=rigell_high_rotation_speed;

	rigell_node->setRotation(rigell_rotation);

	//moving
	core::vector3df velocity=core::vector3df();

	if(up)
		velocity+=rigell_p_node->FRIgetDirectionPositionY(core::vector3df(rigell_speed,0,0));
	if(down)
		velocity+=rigell_p_node->FRIgetDirectionPositionY(core::vector3df(-rigell_speed,0,0));

	rigell_p_node->setVelocity(velocity);



		driver->beginScene(true,true,video::SColor(0,0,0,0));
		smgr->drawAll();

		if(debug_info==1) {
			rigell_p_node->drawDebugInfo();
			sphere_p_node->drawDebugInfo();
		}
		else if(debug_info==2) p_world->drawAllDebugInfos();

		ShowText(NULL);

		//DRAW THE AIM LINE
		core::matrix4 mat;
		video::SMaterial material;

#ifndef COMPILE_WITH_1_3
		material.Texture1 = 0;
#else
///		material.Textures[0] = 0;
#endif

		material.Lighting = false;
		device->getVideoDriver()->setTransform(video::ETS_WORLD, mat);
		device->getVideoDriver()->setMaterial(material);

		core::line3d<f32> line;
		line.start=rigell_p_node->getPositionBody();
		line.end=rigell_p_node->FRIgetDirectionAbsolutePositionY(core::vector3df(600,0,0));

		core::vector3df shape_size=rigell_p_node->getShapeSize();
		shape_size.Y/=2;
		shape_size.Y-=7;

		line.start.Y-=shape_size.Y;
		line.end.Y-=shape_size.Y;

		driver->draw3DLine(
			line.start, line.end, video::SColor(255, 255, 255, 0));

		//END OF DRAW THE AIM LINE

		/*
		core::stringc message;
		message+=p_world->getTimeElapsed();
		message+=" ";
		message+=rigell_p_node->getNetAppliedForce().Y;
		message+=" ";
		message+=rigell_p_node->getNetAppliedForce().Z;
		message+=" ";
		device->getLogger()->log(message.c_str());
		*/

		driver->endScene();

		}//isWindowActive
	}

	device->drop();
	return 0;
}




