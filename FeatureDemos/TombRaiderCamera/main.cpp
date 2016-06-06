/*

Animation Demo

*/

#include <iostream>
#include <irrlicht.h>

using namespace std;

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#pragma comment(lib, "Irrlicht.lib")


#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

#include "EventReceiver.h"
//#include <windows.h>
#include "Skeleton.h"
#include "Input.h"
#include "3DP.h"

	float char_speed= 0.3;
	float char_direc=0;
	float char_direcY=0;
	float mouse_sensitivity=3;
	float grav_force=9;
	float Distanciaance3= 30;
	int char_standing=0;
	int char_walking=0;
	int screen_width=1024;
	int screen_height=768;
	int mousex_init=screen_width/2;
	int mousey_init=screen_height/6;
	int full_screen=0;
	int cursor_visible=0;
	int anim = 0;
	int Shadow = 0;


	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, dimension2d<s32>(screen_width, screen_height), 32, full_screen, true, true);
	position2d<s32> mousePos = device->getCursorControl()->getPosition();
	position2d<s32> lastMousePos = device->getCursorControl()->getPosition();


int main()
{

	cEventReceiver eventReceiver;


    device->getCursorControl()->setVisible(cursor_visible);
    device->getCursorControl()->setPosition(mousex_init,mousey_init);

	core::stringw wCaption="skeletal human animation [fps: ", wCaptionTemp="MeshNil.x";

	video::IVideoDriver *driver = device->getVideoDriver();
	ISceneManager *smgr = device->getSceneManager();
	IGUIEnvironment *guienv = device->getGUIEnvironment();
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
	device->getFileSystem()->addZipFileArchive("Player.zip");
/*
	IGUIStaticText* info = guienv->addStaticText(L"Sair:  ESC", rect<int>(10,10,200,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	info = guienv->addStaticText(L"Correr:  W", rect<int>(10,25,200,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	info = guienv->addStaticText(L"Andar:  SHIFT + W", rect<int>(10,40,200,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));
	scene::info = guienv->addStaticText(L"Pular:  Botão Direito do Mouse", rect<int>(10,55,200,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));
	*/
	// setting event receiver
	device->setEventReceiver(&eventReceiver);

	// Creating skeleton class, the initialize method needs player scenenode
	// and a speed variable of u32. Now all u32 numbers are allowed but
	// note that numbers less than 2 or higher than 50 could cause problems
	scene::IAnimatedMesh *mesh = (IAnimatedMesh*) smgr->getMesh("player.x");
	scene::IAnimatedMeshSceneNode *skelNode = (IAnimatedMeshSceneNode*) smgr->addAnimatedMeshSceneNode(mesh);
	cSkeleton skeleton;
	skeleton.Initialize(skelNode, 6);
	//ITriangleSelector* selector=0;

/*a classe skelNode é um objeto carregado em .x por causa da rotação
senão ela roda o personagem tbm*/

	skelNode->addShadowVolumeSceneNode(skelNode->getMesh(),false, 100.f);

	// create input class
	cInput input(&eventReceiver);

	video::SLight l;
	scene::ILightSceneNode *light = smgr->addLightSceneNode(0,core::vector3df(50,50,50),video::SColorf(1,1,1,.1f),1000);
	l = light->getLightData();
	l.Type = video::ELT_POINT;
	l.AmbientColor = video::SColorf(96,96,96);
	l.SpecularColor = video::SColorf(0.5,0.5,0.5);
	l.DiffuseColor = video::SColorf(1,1,1);
	l.CastShadows = Shadow;

	light->setLightData( l );
gui::IGUIEnvironment* env = device->getGUIEnvironment();

	// setup camera
ICameraSceneNode *cam = smgr->addCameraSceneNode(0,core::vector3df(0,0,0),core::vector3df(0,0,0));
	//cam->setParent(skelNode);
	scene::ISceneNode* cube = smgr->addCubeSceneNode();
	cube->setScale( core::vector3df(5,0,5) );
	cube->setPosition( core::vector3df(0,-7.05,0) );
	cube->setMaterialTexture(0, device->getVideoDriver()->getTexture("cube.jpg"));
	cube->setMaterialTexture(1, device->getVideoDriver()->getTexture("MaPZone[Lava_Rock_normal].jpg"));
	//cube->setMaterialType(video::EMT_DETAIL_MAP);


	CScene3PCamera *camAnim;

		// cria uma sky box

		scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
		driver->getTexture("SkyBoxes/Desert/nemesis_up.jpg"),
		driver->getTexture("SkyBoxes/Desert/nemesis_dn.jpg"),
		driver->getTexture("SkyBoxes/Desert/nemesis_lf.jpg"),
		driver->getTexture("SkyBoxes/Desert/nemesis_rt.jpg"),
		driver->getTexture("SkyBoxes/Desert/nemesis_ft.jpg"),
		driver->getTexture("SkyBoxes/Desert/nemesis_bk.jpg"));



	camAnim = new CScene3PCamera(smgr,device->getCursorControl(),
		skelNode,Distanciaance3,180,0,false,irr::core::vector3df(0,0,0),10);
cam->addAnimator(camAnim);



	u32 i=1;
	s32 fps, fpsTemp;
	f32 cosAlphaTwo;
	f32 alpha;
	f32 rad;
	u32 a;
	scene::ISceneNode *node;

	// create triangle selector for the terrain


	// create collision response animator and attach it to the camera

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	while((device->run()) && (eventReceiver.Keys[KEY_ESCAPE] == false))
	{


	char_direc = skelNode->getRotation().Y;


		driver->beginScene(true, true, SColor(255,255,255,255));

		smgr->drawAll();

		driver->endScene();


		// the input class reacts to keyboard inputs
		// and is changing the animation types
		// of the skeleton, so have a look into it!
		input.handleInput(&eventReceiver,&skeleton);

		// this routine animates the skeleton
		skeleton.animSkeleton();

		/*// Get and show frames
		fps = driver->getFPS();
		if (fps != fpsTemp) {
			wCaptionTemp = wCaption + fps + "]";
			device->setWindowCaption(wCaptionTemp.c_str());
			fpsTemp = fps;
		}*/
		//mouse_look();
	node = skeleton.getSkeletonSceneNode();
	//	if (skeleton.getAnimType() == CSK_ANIM_JUMP) {

			i = skeleton.getMotionVariable();
			a = i%360;
			rad = 3.14159/180;
			if (a>180)
				a+=180;
			alpha = 1 * a * rad;
			cosAlphaTwo = cos(alpha);
			core::vector3df vec = core::vector3df(0,0,0);
			vec.Y = .4f*(1 * (cosAlphaTwo + 0));

			node->setPosition(node->getPosition() + vec);

	//	}

	}

	return 0;

}
