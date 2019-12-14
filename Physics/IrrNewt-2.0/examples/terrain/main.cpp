#include <irrlicht.h>
#include <iostream>
//#include <direct.h>

#include "../../include/IrrNewt.hpp"

using namespace irr;

#pragma comment(lib, "Irrlicht.lib")

irr::newton::IWorld* p_world;
irr::newton::IBody* terrain_body;

irr::newton::IMaterial* camera_material;
irr::newton::IMaterial* cube_material;
irr::newton::IMaterial* terrain_material;

const float camera_speed=/*450.0f*/ 6.0f;

const float mouse_sensitive = /*50.2f*/ 25;

class MyMaterialCallback:public irr::newton::IMaterialCollisionCallback{
		public:
			virtual int ContactBegin(
				irr::newton::IMaterialPair* material_pair,
				irr::newton::IBody* body0,
				irr::newton::IBody* body1) {
				//refuse the contact (return 0)
				return 0;
			}
			virtual int ContactProcess(
				irr::newton::IMaterialPairAndContact* material_pair_and_contact
				) {
				//refuse the contact (return 0)
				return 0;
			}
}camera_cube_callback;

irr::newton::ICameraBodyFPS* camera_body;

scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera;
IrrlichtDevice* device;
video::IVideoDriver* driver=NULL;

bool debug_info=false;

class MyEventReceiver : public IEventReceiver
{
public:

	MyEventReceiver(scene::ISceneNode* terrain)
	{
		// store pointer to terrain so we can change its drawing mode
		Terrain = terrain;
	}

	virtual bool OnEvent(const SEvent& event)
	{
		camera_body->OnEvent(event);

		// check if user presses the key 'W' or 'D'
		if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
			!event.KeyInput.PressedDown)
		{

			switch (event.KeyInput.Key)
			{

			case irr::KEY_KEY_W: // switch wire frame mode
				Terrain->setMaterialFlag(video::EMF_WIREFRAME, !Terrain->getMaterial(0).Wireframe);
				Terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
				return true;

			case irr::KEY_KEY_M: // toggle detail map
				Terrain->setMaterialType(
					Terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
					video::EMT_DETAIL_MAP : video::EMT_SOLID);
				return true;

			case irr::KEY_SPACE:
					debug_info=!debug_info;
					return true;

			case irr::KEY_KEY_C:
				p_world->getUtils()->launchCube(5);
			//	p_world->getUtils()->launchCube();
				return true;

			}
		}

		return false;
	}

private:
	scene::ISceneNode* Terrain;
};

/*
The start of the main function starts like in most other example. We ask the user
for the desired renderer and start it up.
*/
int main()
{
	//change working dir
//	_chdir("../../");

	// let user select driver type

	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
		" (d) Software Renderer\n (e) Apfelbaum Software Renderer\n"\
		" (f) NullDevice\n (otherKey) exit\n\n");

	char i;
	std::cin >> i;

	switch(i)
	{
		case 'a': driverType = video::EDT_DIRECT3D9;break;
//		case 'b': driverType = video::EDT_DIRECT3D8;break;
		case 'c': driverType = video::EDT_OPENGL;   break;

		case 'd': driverType = video::EDT_SOFTWARE; break;

#ifndef COMPILE_WITH_1_3
		case 'e': driverType = video::EDT_SOFTWARE2; break;
#else
     	case 'e': driverType = video::EDT_BURNINGSVIDEO ; break;
#endif

		case 'f': driverType = video::EDT_NULL;     break;

		default: return 1;
	}

	// create device

	device = createDevice(driverType, core::dimension2d<u32>(800, 600));

	if (device == 0)
		return 1; // could not create selected driver.

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	// add some help text
	gui::IGUIStaticText* text = env->addStaticText(
		L"Press 'W' to change wireframe mode\nPress 'M' to toggle detail map\nPress 'D' to show debug informations",
		core::rect<s32>(10,450,200,490), true, true, 0, -1, true);

	// add camera
	camera=smgr->addCameraSceneNode();
	camera->setPosition(core::vector3df(1900*2,255*2,3700*2));
	camera->setFarValue(12000.0f);

	// disable mouse cursor
	device->getCursorControl()->setVisible(true);
	device->getCursorControl()->setPosition(core::position2d<f32>(0.5,0.5));

	/*
	Here comes the terrain renderer scene node: We add it just like any
	other scene node to the scene using ISceneManager::addTerrainSceneNode().
	The only parameter we use is a file name to the heightmap we use. A heightmap
	is simply a gray scale texture. The terrain renderer loads it and creates
	the 3D terrain from it.
	To make the terrain look more big, we change the scale factor of it to (40, 4.4, 40).
	Because we don't have any dynamic lights in the scene, we switch off the lighting,
	and we set the file terrain-texture.jpg as texture for the terrain and
	detailmap3.jpg as second texture, called detail map. At last, we set
	the scale values for the texture: The first texture will be repeated only one time over
	the whole terrain, and the second one (detail map) 20 times.
	*/

	// add terrain scene node
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"media/terrain-heightmap.bmp");

	terrain->setScale(core::vector3df(40, 4.4f, 40));

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));
	terrain->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
	terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->scaleTexture(1.0f, 20.0f);

	//PHYSICS
	//create physics world
	p_world=irr::newton::createPhysicsWorld(device);

	//create terrain body
	//create it as we create any others body
	//body type EBT_AUTODETECT (default) will automatically detect that the node is a terrain node
	//and create it correctly
	//but for terrain we can use a reserved parameter

	irr::newton::SBodyFromNode terrainData;
	terrainData.Node=terrain;

	//here are terrain reserved parameter
	//we can use it for specify LOD (level of detail) of the terrain (3 by default)
	//the max of this parameter is the parameter maxLod (5 for default) that you pass to addTerrainSceneNode
	//see irrlicht documentation for more detail
	//so the value of TerrainLOD is between 0 and 5 for default
	//generally a big value of TerrainLOD indicates less precision collision (faster)
	//a less value indicates a precise collision (slower)
	terrainData.TerrainLOD = 1;

	terrain_body = p_world->createBody(terrainData);

	//create camera body

	//we need to move it so we create a character controller
	irr::newton::SBodyFromNode cameraData;
	cameraData.Node = camera;
	cameraData.Type = irr::newton::EBT_PRIMITIVE_ELLIPSOID;

	//a camera does not have box size so we need to set it manually
	cameraData.BodyScaleOffsetFromNode = core::vector3df(55,55,55);

	camera_body = p_world->createCameraFPS(cameraData);
	camera_body->setContinuousCollisionMode(true);

	//INI CAMERA
	camera_body->setMouseSensitive(mouse_sensitive);
	camera_body->setMoveSpeed(camera_speed);
	camera_body->setType(newton::ECFT_NORMAL);
	 //END OF CAMERA INITIALIZE

	camera_material=p_world->createMaterial();
	cube_material=p_world->createMaterial();
	terrain_material=p_world->createMaterial();

	//set materials to their bodies
	camera_body->setMaterial(camera_material);
	terrain_body->setMaterial(terrain_material);

	//set elasticity between terrain<->camera
	//camera_body->setLinearDamping(1.0f);

	camera_material->setFriction(terrain_material,
		0,//static
		0//dynamic
		);
	camera_material->setElasticity(terrain_material,0.0f);
	camera_material->setSoftness(terrain_material,0.0f);
	camera_material->setCollidable(cube_material,false);

	//----------------------------------------------------------------------

	//END OF PHYSICS

	//gravity to the camera
	//camera_body->addForceContinuous(core::vector3df(0,-300/*800*/,0));

	// create event receiver
	MyEventReceiver receiver(terrain);
	device->setEventReceiver(&receiver);

	int lastFPS = -1;


	while(device->run()) {
	if (device->isWindowActive()){

		//update physics
		p_world->update();

		driver->beginScene(true, true, video::SColor(255,200,200,200));

		smgr->drawAll();

		//center the mouse position

		if(debug_info)
			p_world->drawAllDebugInfos();

		driver->endScene();

		// display frames per second in window title
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"Terrain Renderer - Irrlicht Engine [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}


	}//is window active
	}//while (device->run())

	device->drop();

	return 0;
}

