// 3rdPersonCam.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <windows.h>
#include <irrlicht.h>
using namespace irr;
using namespace irr::core;
using namespace irr::scene;


#pragma comment(lib, "Irrlicht.lib")
scene::ITriangleSelector *terrainTSelector;
scene::ITerrainSceneNode* terrain;
scene::IAnimatedMeshSceneNode* Player1, *Player2, *Player3, *Player4, *Player5;
video::IVideoDriver* driver;

scene::ISceneManager* smgr ;
scene::ISceneNode* node = 0;
scene::ICameraSceneNode* camnode = 0;

IrrlichtDevice* device = 0;

#include "RANDfunction.h"

#include "MovePlayerfunction.h"

#include <iostream>

int main()
{
	int fps;
	MyEventReceiver receiver;

	// let user select driver type

	video::E_DRIVER_TYPE driverType = video::EDT_DIRECT3D9;

	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
		" (d) Software Renderer\n (e) Apfelbaum Software Renderer\n"\
		" (f) NullDevice\n (otherKey) exit\n\n");

	char i;
	std::cin >> i;

	switch(i)
	{
		case 'a': driverType = video::EDT_DIRECT3D9;break;
		case 'b': driverType = video::EDT_DIRECT3D8;break;
		case 'c': driverType = video::EDT_OPENGL;   break;
		case 'd': driverType = video::EDT_SOFTWARE; break;
		case 'e': driverType = video::EDT_SOFTWARE2;break;
		case 'f': driverType = video::EDT_NULL;     break;
		default: return 1;
	}

	// create device

	device = createDevice(driverType, core::dimension2d<s32>(640, 480),irr::u32(16),true,false,false,&receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();


Player1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/sydney.md2"));
	if (Player1){
		Player1->setMaterialFlag(video::EMF_LIGHTING, false);
		Player1->setMaterialTexture(0, driver->getTexture("media/sydney.bmp"));
		}

Player2 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/faerie.md2"));
	if (Player2){
		Player2->setMaterialFlag(video::EMF_LIGHTING, false);
		Player2->setMaterialTexture(0, driver->getTexture("media/faerie2.bmp"));
		}

Player3 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/sydney.md2"));
	if (Player3){
		Player3->setMaterialFlag(video::EMF_LIGHTING, false);
		Player3->setMaterialTexture(0, driver->getTexture("media/sydney.bmp"));
		}

Player4 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/sydney.md2"));
	if (Player4){
		Player4->setMaterialFlag(video::EMF_LIGHTING, false);
		Player4->setMaterialTexture(0, driver->getTexture("media/sydney.bmp"));
		}

Player5 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/sydney.md2"));
	if (Player5){
		Player5->setMaterialFlag(video::EMF_LIGHTING, false);
		Player5->setMaterialTexture(0, driver->getTexture("media/sydney.bmp"));
		}


	camnode=smgr->addCameraSceneNode(0, core::vector3df(0,0,0), core::vector3df(1,0,1),-1);
	camnode->setFarValue(8000);

My3rdCam1.setTarget(Player1);
My3rdCam1.setOffset(vector3df(0,20,0));
My3rdCam1.attachCamera(camnode);
MainPlayer.SetGroundOffset(vector3df(0,25,0));
BP1.SetGroundOffset(vector3df(0,23,0));
BP2.SetGroundOffset(vector3df(0,23,0));
BP3.SetGroundOffset(vector3df(0,23,0));
BP4.SetGroundOffset(vector3df(0,23,0));
BP5.SetGroundOffset(vector3df(0,23,0));
MainPlayer.attachMesh(Player1);
BP1.attachMesh(Player1);
BP2.attachMesh(Player2);
BP3.attachMesh(Player3);
BP4.attachMesh(Player4);
BP5.attachMesh(Player5);
MainPlayer.SetDevice(device);
BP1.SetDevice(device);
BP2.SetDevice(device);
BP3.SetDevice(device);
BP4.SetDevice(device);
BP5.SetDevice(device);

WASD1.attachPlayerBP(&MainPlayer);

	terrain = smgr->addTerrainSceneNode( "media/maze1bmp.bmp");
	terrain->setScale(core::vector3df(64,4, 64));
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setPosition(vector3df(0,0,0));
	terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));
	terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->scaleTexture(5.0f,3.0f);


	Player1->setPosition(core::vector3df(1100,1300,1100));
	Player2->setPosition(core::vector3df(1200,1350,1100));
	Player3->setPosition(core::vector3df(1300,1400,1200));
	Player4->setPosition(core::vector3df(1400,1450,1200));
	Player5->setPosition(core::vector3df(1500,1500,1300));

   	// create skybox
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	smgr->addSkyBoxSceneNode(
		driver->getTexture("media/irrlicht2_up.jpg"),
		driver->getTexture("media/irrlicht2_dn.jpg"),
		driver->getTexture("media/irrlicht2_lf.jpg"),
		driver->getTexture("media/irrlicht2_rt.jpg"),
		driver->getTexture("media/irrlicht2_ft.jpg"),
		driver->getTexture("media/irrlicht2_bk.jpg"));
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
	int lastFPS = -1;


float OldTime,NewTime;
ITimer *FrameTimer;
FrameTimer=device->getTimer();

OldTime=NewTime=FrameTimer->getRealTime();
float ETime;
	while(device->run())
	{

	NewTime=FrameTimer->getRealTime();
	ETime=NewTime-OldTime;
	MovePlayer(ETime);
	MainPlayer.SetGround(device,terrain);

if(PlayerSel!=1){
BP1.SetGround(device,terrain);
BP1.IdleMode();
BP1.ApplyGravity(ETime);}

if(PlayerSel!=2){
BP2.SetGround(device,terrain);
BP2.IdleMode();
BP2.ApplyGravity(ETime);}

if(PlayerSel!=3){
BP3.SetGround(device,terrain);
BP3.IdleMode();
BP3.ApplyGravity(ETime);}

if(PlayerSel!=4){
BP4.SetGround(device,terrain);
BP4.IdleMode();
BP4.ApplyGravity(ETime);}

if(PlayerSel!=5){
BP5.SetGround(device,terrain);
BP5.IdleMode();
BP5.ApplyGravity(ETime);}

	OldTime=NewTime;
	Sleep(1000/32);
		driver->beginScene(true, true, video::SColor(255,113,113,133));

			smgr->drawAll(); // draw the 3d scene
			device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)
			driver->endScene();


		fps= driver->getFPS();

	}
	device->drop();

	return 0;
}
