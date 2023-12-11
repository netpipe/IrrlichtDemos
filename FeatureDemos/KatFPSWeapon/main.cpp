/*******************************************************************************
 Copyright (C) 2009 McCauley404@gmail.com
 Test Application for First Person Weapon Class - zlib/BSD code by kat104
 ******************************************************************************/

#include "firstPersonWeapon.h"

/*==============================================================================

==============================================================================*/
void apply_light2node(ISceneNode* node)
{
  u8 n = 0;
  SMaterial *mat = 0;
  while(mat = &node->getMaterial(n))
  {
    mat->EmissiveColor = video::SColor(10, 230,90,70);
    mat->Lighting  = true;
    mat->FogEnable = false;
    n++;
  }
  node->getMaterial(0).SpecularColor.set(0,0,0,0);
}
/*==============================================================================
  Entry Point
==============================================================================*/
int main()
{
  irr::SIrrlichtCreationParameters param;
  param.AntiAlias = true;
  param.HighPrecisionFPU= true;
  param.Vsync= true;
  param.DriverType = EDT_OPENGL;
  param.WindowSize = core::dimension2di(800,600);
  param.Bits = 32;
  param.Stencilbuffer = true;
  param.Fullscreen = false;

  IrrlichtDevice* device=createDeviceEx(param);

  if (!device) return 0;

  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr  = device->getSceneManager();
  device->getCursorControl()->setVisible(false);
  device->setWindowCaption(L"test");


  // ambient light

  smgr->addLightSceneNode(0, core::vector3df(0,200,0),
		video::SColorf(1.f, 1.f,.74f,.0f), 400.0f);

  smgr->setShadowColor(video::SColor(150,0,0,0));

  // load room

  IAnimatedMesh* mesh = smgr->getMesh("../../media/room.3ds");
  IAnimatedMeshSceneNode *node;
  node = smgr->addAnimatedMeshSceneNode(mesh);
  if (node)
  {
    node->setMaterialTexture(0, driver->getTexture("../../media/rockwall.bmp"));
	smgr->getMeshManipulator()->makePlanarTextureMapping(mesh->getMesh(0), 0.014f);
    apply_light2node(node);
  }
  mesh->drop();

  // and a ninja

  mesh = smgr->getMesh("../../media/ninja.b3d");
  scene::IAnimatedMeshSceneNode* ninja;
  ninja = smgr->addAnimatedMeshSceneNode(mesh);
  if (ninja)
  {
    ninja->setFrameLoop(184,205);
    ninja->setAnimationSpeed(15);
    ninja->setScale(core::vector3df(8.f,8.f,8.f));
    ninja->setPosition(core::vector3df(200.f,0.f,0.f));
    apply_light2node(ninja);
	ninja->addShadowVolumeSceneNode();
  }
  mesh->drop();

  // and a gun to test the 3d person model

  mesh = smgr->getMesh("models/M4/3d_person/M4A1d.3ds");
  scene::IAnimatedMeshSceneNode* agun;
  agun = smgr->addAnimatedMeshSceneNode(mesh);
  if (agun)
  {
    agun->setMaterialTexture(0, driver->getTexture("models/M4/1st_person/M4A1.jpg"));
    agun->setScale(core::vector3df(1.2f, 1.2f, 1.2f));
    agun->setPosition(core::vector3df(180.f,1.5f,0.f));
    agun->setRotation(core::vector3df(0.f,0.f,90.f));
    apply_light2node(agun);
	agun->addShadowVolumeSceneNode();
  }
  mesh->drop();

  // now camera

  scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, .1f);
                           cam->setPosition(core::vector3df(-200,100,100));
                           cam->setTarget(core::vector3df(0,0,0));

  // create the 1st person weapon and parent it to the camera;
  // the pointer to cam is used later to change its FOV when aiming
  firstPersonWeapon* M4;
  M4 = new firstPersonWeapon(device, cam);
  apply_light2node(M4->getNode());


  // main loop

  device->setEventReceiver(M4);

  while(device->run())
  {
    if (!device->isWindowActive()) device->sleep(200);

    // manage animations and input from keyboard
    M4->update(device->getTimer()->getTime());

    if (M4->isKeyDown(KEY_ESCAPE)) break;

    driver->beginScene(true, true, 0);
    smgr->drawAll();
    driver->endScene();
    device->sleep(30);
  }

  device->closeDevice();
  //printf("exited gracefully, press enter to exit\n");
  //getchar();
  return 0;
}
