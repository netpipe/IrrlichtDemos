/*******************************************************************************
 Copyright (C) 2009 McCauley404@gmail.com
 First Person Weapon - zlib/BSD code by kat104
 ******************************************************************************/

#include "firstPersonWeapon.h"

/*==============================================================================
  Constructor
==============================================================================*/
firstPersonWeapon::firstPersonWeapon(IrrlichtDevice* device, ICameraSceneNode* cam)
{
  camera = cam;
  ISceneManager* smgr = device->getSceneManager();
  IAnimatedMesh* mesh = smgr->getMesh("models/M4/1st_person/fpw_M4.b3d");
  node = smgr->addAnimatedMeshSceneNode(mesh);
  if (node)
  {
    //node->setPosition(vector3df(0,-1,0)); // done by artwork!
    node->setAnimationSpeed(25);
    camera->addChild(node);
  }
  mesh->drop();

  wasAiming = false;

  // particular values for M4 carbine
  nbshotsPerMagazine = 28;
  nbMagazines = 6;

  // and it is already loaded at the beginning
  ammoLeftInMagazine = nbshotsPerMagazine;

  //init receiver
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
       KeyDown[i] = false;

    LEFTBUTTONCLICKED = RIGHTBUTTONCLICKED = false;

  // show idle animation at startup
  this->idle();
}
/*==============================================================================
  events
==============================================================================*/
bool firstPersonWeapon::OnEvent(const SEvent& event)
{
  if (event.EventType == irr::EET_KEY_INPUT_EVENT)
  {
    KeyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
    return false;
  }
  else if (event.EventType == EET_MOUSE_INPUT_EVENT)
  {
    if     (event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN) LEFTBUTTONCLICKED = true;
    else if(event.MouseInput.Event==EMIE_LMOUSE_LEFT_UP     ) LEFTBUTTONCLICKED = false;
    else if(event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN) RIGHTBUTTONCLICKED = true;
    else if(event.MouseInput.Event==EMIE_RMOUSE_LEFT_UP     ) RIGHTBUTTONCLICKED = false;
  }
  return false;
}
/*==============================================================================
  isKeyDown
==============================================================================*/
bool firstPersonWeapon::isKeyDown(EKEY_CODE keyCode) const
{
  return KeyDown[keyCode];
}
/*==============================================================================
  those static will screw calculations if there are several instances
==============================================================================*/
void firstPersonWeapon::update(u32 now)
{
  static u32 nextAction = 0;
  static u32 nextShoot = 0;

  if (LEFTBUTTONCLICKED  && nextShoot < now) // shoot
  {
    shoot();
    nextShoot = now + 30;
  }

  else if (RIGHTBUTTONCLICKED && nextAction < now) // draw/holster
  {
    if (this->action != FPW_AIM)
      draw();
    else
      holster();
    nextAction = now + 700;
  }

  else if (isKeyDown(KEY_KEY_R) && nextAction < now) // reload
  {
    reload();
    nextAction = now + 3000;
  }
}
/*==============================================================================

==============================================================================*/
void firstPersonWeapon::idle()
{
  action = FPW_IDLE;
  node->setFrameLoop(PLAYER_IDLE);
  node->setLoopMode(true);
}
/*==============================================================================
  deploy weapon
==============================================================================*/
void firstPersonWeapon::draw()
{
  action = FPW_DRAW;
  camera->setFOV(1.f);
  node->setFrameLoop(PLAYER_DRAW);
  node->setLoopMode(false);
  node->setAnimationEndCallback(this);
}
/*==============================================================================
  opposite : put it down
==============================================================================*/
void firstPersonWeapon::holster()
{
  if (action == FPW_HOLSTER) return;
  action = FPW_HOLSTER;
  camera->setFOV(1.256f);
  node->setFrameLoop(PLAYER_HOLSTER);
  node->setLoopMode(false);
  node->setAnimationEndCallback(this);
}
/*==============================================================================
  returns true if the shot was successful (0 = no ammo or so)
==============================================================================*/
bool firstPersonWeapon::shoot()
{
  // if I am already shooting or reloading, dont shoot
  if (action == FPW_SHOOT_SHOULDER ||
      action == FPW_SHOOT_HIP ||
      action == FPW_RELOAD) return false;

  // ok now if there's at least one bullet in magazine,
  if (ammoLeftInMagazine > 0)
  {
    // show correct animation depending on if I am aiming or not
    if (action == FPW_AIM)
    {
      action = FPW_SHOOT_SHOULDER;
      node->setFrameLoop(PLAYER_SHOOT_SHOULDER);
    }
    else
    {
      action = FPW_SHOOT_HIP;
      node->setFrameLoop(PLAYER_SHOOT_HIP);
    }
    node->setLoopMode(false);
    node->setAnimationEndCallback(this);

    // playSound("sound/M4/shoot.wav");

    // emit particules

    // throw a bullet

    // remove bullet from chamber
    ammoLeftInMagazine--;

    // spawn one empty shell

    printf("%d\n", ammoLeftInMagazine);
    return true;
  }
  // no ammo left in magazine!
  else
  {
    //playSound("sound/clic.wav");
  }
  return false;
}
/*==============================================================================

==============================================================================*/
void firstPersonWeapon::reload()
{
  if (action == FPW_RELOAD || nbMagazines == 0) return;

  //ammoBar->setValue((nbMagazines-1) * NBSHOTSPERMAGAZINE);

  //playSound("sound/M4/reload.wav", false);

  if (--nbMagazines > 0)
  {
    action = FPW_RELOAD;
    wasAiming = (action == FPW_AIM);
    node->setFrameLoop(PLAYER_RELOAD);
    node->setLoopMode(false);
    node->setAnimationEndCallback(this);
  }
}
/*==============================================================================
  code executed when animation ends
==============================================================================*/
void firstPersonWeapon::OnAnimationEnd(IAnimatedMeshSceneNode* node)
{
  if (action == FPW_DRAW)
  {
    action = FPW_AIM;
    node->setFrameLoop(PLAYER_AIMING);
    node->setLoopMode(true);
  }

  else if (action == FPW_SHOOT_SHOULDER)
  {
    action = FPW_AIM;
    node->setFrameLoop(PLAYER_AIMING);
    node->setLoopMode(true);
  }

  else if (action == FPW_SHOOT_HIP) idle();

  else if (action == FPW_HOLSTER) idle();

  else if (action == FPW_RELOAD)
  {
    //NOW gun is ready
    ammoLeftInMagazine = nbshotsPerMagazine;
    //magazineBar->setValue(ammoLeftInMagazine);

    if (wasAiming) // if i was aiming before reloading, back to aim
    {
      action = FPW_AIM;
      node->setFrameLoop(PLAYER_AIMING);
    }
    else
    {
      action = FPW_IDLE;
      node->setFrameLoop(PLAYER_IDLE);
    }

    node->setLoopMode(true);
    wasAiming = false;
  }
}
/*==============================================================================

==============================================================================*/
u8 firstPersonWeapon::getAmmo()
{
  return ammoLeftInMagazine;
}
/*==============================================================================
  Destructor
==============================================================================*/
firstPersonWeapon::~firstPersonWeapon()
{
  node->remove();
}

