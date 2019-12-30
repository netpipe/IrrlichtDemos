/*******************************************************************************
 Copyright (C) 2009 McCauley404@gmail.com
 First Person Weapon - zlib/BSD code by kat104
 ******************************************************************************/

/* todo
	- attach the weapon to hands with code, not in the model editor!
	- add progressive FOV when aiming
    - same as .obj with bump
*/

#ifndef firstPersonWeapon_H
#define firstPersonWeapon_H

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

// model animation frames
#define PLAYER_IDLE 1,36
#define PLAYER_DRAW 36,48
#define PLAYER_AIMING 49,79
#define PLAYER_HOLSTER 80,90
#define PLAYER_RELOAD 90,136
#define PLAYER_SHOOT_HIP 136,139
#define PLAYER_SHOOT_SHOULDER 140,143

// Animation currently played
enum FPW_ACTION
{
  FPW_IDLE=0,
  FPW_DRAW=1,
  FPW_AIM=2,
  FPW_HOLSTER=3,
  FPW_RELOAD=4,
  FPW_SHOOT_HIP=5,
  FPW_SHOOT_SHOULDER=6
};

class firstPersonWeapon : private IAnimationEndCallBack, public IEventReceiver
{




public:
  IAnimatedMeshSceneNode *node;
  virtual void OnAnimationEnd(IAnimatedMeshSceneNode* node);

  ICameraSceneNode* camera;
IrrlichtDevice* device2;
ISceneManager* smgr;

  bool wasAiming;
  u8 ammoLeftInMagazine;
  u8 nbMagazines;
  u8 nbshotsPerMagazine;

  //receiver
  bool LEFTBUTTONCLICKED;
  bool RIGHTBUTTONCLICKED;
  bool KeyDown[KEY_KEY_CODES_COUNT];
//USED TO BE PRIVATE ABOVE HERE
  firstPersonWeapon(IrrlichtDevice* device, ICameraSceneNode* cam);
  bool OnEvent(const SEvent& event);

  FPW_ACTION action;
  void idle();
  void draw();
  bool shoot();
  void reload();
  void holster();

  u8 getAmmo();
  IAnimatedMeshSceneNode* getNode(){return node;}
  ICameraSceneNode* getCam() { return camera; }

  //receiver
  bool isKeyDown(EKEY_CODE keyCode) const;
  void update(u32 now);

  ~firstPersonWeapon();
};

#endif
