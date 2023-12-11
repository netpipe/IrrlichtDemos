/*

Skeleton Demo

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

scene::ICameraSceneNode *cam=0;

#include <EventReceiver.hpp>

#include <Skeleton.h>
#include <Input.hpp>

int main()
{

	cEventReceiver eventReceiver;

	//IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<s32>(800, 600), 32, false, true, true);
  // IrrlichtDevice *device = createDevice ( EDT_OPENGL, dimension2d<s32> ( 800, 600 ), 32,false, true, false, 0 );
    IrrlichtDevice* device = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(1000, 760));

	core::stringw wCaption="skeletal human animation (www.solar-ray.org) [fps: ", wCaptionTemp="";

	video::IVideoDriver *driver = device->getVideoDriver();
	ISceneManager *smgr = device->getSceneManager();
	IGUIEnvironment *guienv = device->getGUIEnvironment();

	IGUIStaticText* info = guienv->addStaticText(L"Exit:  ESC", rect<int>(10,10,200,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	info = guienv->addStaticText(L"Run:  W", rect<int>(10,25,200,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	info = guienv->addStaticText(L"Walk:  SHIFT + W", rect<int>(10,40,200,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	info = guienv->addStaticText(L"Jump:  SPACE", rect<int>(10,55,200,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	info = guienv->addStaticText(L"Strafe-Left:  A", rect<int>(10,70,200,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	info = guienv->addStaticText(L"Strafe-Right:  D", rect<int>(10,85,200,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	info = guienv->addStaticText(L"Head + Direction: Mouse", rect<int>(10,100,200,200),false);
	info->setOverrideColor(video::SColor(255,255,255,255));

	// setting event receiver
	device->setEventReceiver(&eventReceiver);

	// Creating skeleton class, the initialize method needs player scenenode
	// and a speed variable of u32. Now all u32 numbers are allowed but
	// note that numbers less than 2 or higher than 50 could cause problems
	scene::IAnimatedMesh *mesh = (IAnimatedMesh*) smgr->getMesh("media/player.x");
	scene::IAnimatedMeshSceneNode *skelNode = (IAnimatedMeshSceneNode*) smgr->addAnimatedMeshSceneNode(mesh);
	cSkeleton skeleton;
	skeleton.Initialize(skelNode, 8);

	//skelNode->addShadowVolumeSceneNode(-1,false, 100.f);

	// create input class
	cInput input(&eventReceiver);

	// some lights
	video::SLight l;
	scene::ILightSceneNode *light = smgr->addLightSceneNode(0,core::vector3df(50,50,50),video::SColorf(1,1,1,1.f),100);
	l = light->getLightData();
	l.Type = video::ELT_POINT;
	l.AmbientColor = video::SColorf(.35,.35,.35);
	l.SpecularColor = video::SColorf(.7,.7,.5);
	l.DiffuseColor = video::SColorf(1,1,1);
	l.CastShadows = true;

	light->setLightData( l );

	// setup camera
	cam = smgr->addCameraSceneNode(0, vector3df(0,8,15), vector3df(0,0,0));

	scene::ISceneNode* cube = smgr->addCubeSceneNode();
	cube->setScale( core::vector3df(50,0,50) );
	cube->setPosition( core::vector3df(0,-7.05,0) );
	cube->setMaterialTexture(0, device->getVideoDriver()->getTexture("cube.jpg"));

	s32 fps, fpsTemp;
	core::vector3df pos = skeleton.getSkeletonSceneNode()->getPosition();

	while((device->run()) && (eventReceiver.Keys[KEY_ESCAPE] == false))
	{

		driver->beginScene(true, true, SColor(255,62,48,38));

		smgr->drawAll();

		guienv->drawAll();

		driver->endScene();

		// the input class reacts to keyboard inputs
		// and is changing the animation types
		// of the skeleton, so have a look into it!
		input.handleInput(&eventReceiver,&skeleton);

		// this routine animates the skeleton
		core::position2di pos2d = core::position2di(eventReceiver.mouseX,eventReceiver.mouseY);
		core::line3df line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(pos2d,cam);
		skeleton.animSkeleton(pos2d);

		// Get and show frames
		fps = driver->getFPS();
		if (fps != fpsTemp) {
//			wCaptionTemp = wCaption + fps + "]";
			device->setWindowCaption(wCaptionTemp.c_str());
			fpsTemp = fps;
		}

		// setting player -----------------------------------------------------
		scene::ISceneNode *player = skeleton.getSkeletonSceneNode();
		player->setRotation(core::vector3df(0,.5f*eventReceiver.mouseX,0));

		core::vector3df playerLook = core::vector3df(1,0,0);
		playerLook.rotateXZBy(-player->getRotation().Y+90,core::vector3df(0,0,0));

		// rotate skeleton
		if (skeleton.getSkeletonVelocity().X != 0) {

			core::vector3df vec = playerLook;
			if (skeleton.getSkeletonVelocity().Y != 0)
				vec.rotateXZBy(skeleton.getSkeletonVelocity().Y,core::vector3df(0,0,0));
			player->setPosition( player->getPosition() + .5f * skeleton.getSkeletonVelocity().X * vec);

		}

		// jump
		if (skeleton.getAnimType() == CSK_ANIM_JUMP && skeleton.getSecondMotionVariable() < 340) {
			f32 wert = pow( 2.f/340.f*skeleton.getSecondMotionVariable()-1 , 2);
			core::vector3df pos = player->getPosition();
			pos.Y = 10.f * (-wert + 1);
			player->setPosition(pos);

		}

		// setting player -----------------------------------------------------

		// setting camera -----------------------------------------------------
		core::vector3df playerPos = player->getPosition();
		core::vector3df distancePlayerCam = playerPos - cam->getPosition();

		f32 distanceMult=15.f;
		f32 distanceTopMult=10.f;
		f32 playerVelo = 1.f;

		if (playerVelo > 1.f) {
			distanceMult /= 1.f*playerVelo;
			distanceTopMult /= 1.f*playerVelo;
		}

		playerLook.normalize();
		playerLook *= distanceMult;

		vector3df camPosZero = playerPos - 1.5f * playerLook;
		camPosZero.Y += distanceTopMult;

		core::vector3df camPos = cam->getPosition();

		// cam - drift
		if (camPos != camPosZero) {
			vector3df delta = camPos - camPosZero;

			camPos = camPos - (.065f * delta);

			cam->setPosition(camPos);
		}

		cam->setTarget(playerPos);
		device->sleep(20,0);
		// setting camera -----------------------------------------------------

	}

	device->drop();
	return 0;

}
