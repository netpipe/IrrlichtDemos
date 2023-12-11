/*******************************************************************************
 Copyright (C) 2009 McCauley404@gmail.com
 First Person Weapon - zlib/BSD code by kat104
 ******************************************************************************/

#include "firstPersonWeapon.h"

int aniSpeed=25;
int speedSkill=2;

//smgr->addLightSceneNode(NULL, core::vector3df(0,10,0), video::SColorf(5.0f, 255.0f, 255.0f), 2500, -1);

/*==============================================================================
  Constructor  Destructor
==============================================================================*/
firstPersonWeapon::~firstPersonWeapon()
{
  node->remove();
}

firstPersonWeapon::firstPersonWeapon(IrrlichtDevice* device, ICameraSceneNode* cam)
{
  camera = cam;
  device2 = device;
   smgr = device->getSceneManager();
  IAnimatedMesh* mesh = smgr->getMesh("./data/models/weapons/M4/1st_person/fpw_M4.b3d");
  node = smgr->addAnimatedMeshSceneNode(mesh);
  if (node)
  {
    //node->setPosition(vector3df(0,-1,0)); // done by artwork!
    node->setAnimationSpeed(25);
    camera->addChild(node);
  }
  mesh->drop();

  // particular values for M4 carbine
  nbshotsPerMagazine = 28;
  nbMagazines = 4;

  // and it is already loaded at the beginning
  ammoLeftInMagazine = nbshotsPerMagazine;

  //init receiver
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
       KeyDown[i] = false;

    LEFTBUTTONCLICKED = RIGHTBUTTONCLICKED = false;

  // show idle animation at startup
  this->idle();
  wasAiming=false;
}


///==============================================================================
///  events
///==============================================================================

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


bool firstPersonWeapon::isKeyDown(EKEY_CODE keyCode) const
{
  return KeyDown[keyCode];
}


///==============================================================================
///EVENT LOOP
///==============================================================================
//  those static will screw calculations if there are several instances
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
    if (wasAiming){
        holster();
        wasAiming = false;
    //                    node->setAnimationSpeed(aniSpeed*speedSkill);  // makes gun holseter animation slow
    } else  {
        wasAiming = true;
        draw();
       // node->setAnimationSpeed(aniSpeed/speedSkill); // sets down draw animation
    }
    nextAction = now + 700;
  }

  else if (isKeyDown(KEY_KEY_R) && nextAction < now) // reload
  {
 if (ammoLeftInMagazine<28){
    reload();
    nextAction = now + (3000/speedSkill); // makes it lock the animation for given time
  }
  }

      if (isKeyDown(    KEY_SPACE))
    {    //  m_cPlayer->jump();
            vector3df ha = camera->getAbsolutePosition();
      //  printf("Jump position: %f %f %f \n", pos[0], pos[1], pos[2]);
        camera->setPosition(vector3df( ha.X, ha.Y+40, ha.Z));
    }


}


void firstPersonWeapon::idle()
{
  action = FPW_IDLE;
  node->setFrameLoop(PLAYER_IDLE);
  node->setLoopMode(true);
  node->setAnimationSpeed(aniSpeed/speedSkill);
}


void firstPersonWeapon::draw()
{
  action = FPW_DRAW;
  camera->setFOV(1.f);
  node->setFrameLoop(PLAYER_DRAW);
  node->setLoopMode(false);
  node->setAnimationEndCallback(this);
    node->setAnimationSpeed(aniSpeed*speedSkill);
}


void firstPersonWeapon::holster()
{
  if (action == FPW_HOLSTER) return;
  action = FPW_HOLSTER;
  camera->setFOV(1.256f);
  node->setFrameLoop(PLAYER_HOLSTER);
  node->setLoopMode(false);
  node->setAnimationEndCallback(this);
  node->setAnimationSpeed(aniSpeed*speedSkill); //*scare effect :
}

void firstPersonWeapon::reload()
{
    if (action == FPW_RELOAD || nbMagazines == 0) return;
    //ammoBar->setValue((nbMagazines-1) * NBSHOTSPERMAGAZINE);

    //playSound("sound/M4/reload.wav", false);
    holster();
  if (--nbMagazines > 0)
  {
    action = FPW_RELOAD;
 //   wasAiming = (action==wasAiming);
    node->setFrameLoop(PLAYER_RELOAD);
    node->setLoopMode(false);
    node->setAnimationEndCallback(this);
  }
 // wasAiming=true;
}

bool firstPersonWeapon::shoot()
{
   node->setAnimationSpeed(aniSpeed);
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

    // create bullet
        //Muzzle Flash
        ISceneNode*bnode;
        vector3df bpos=camera->getAbsolutePosition();
        int randx=rand()%20;
        int randy=rand()%10;
        int randgun=rand()%3;
        u32 time;
        if (!wasAiming)
        {
            bnode = smgr->addBillboardSceneNode(camera,core::dimension2d<f32>(randx,randy),vector3df(1.f,-3,12) );
             time = 333;
        }
        else
        {    bnode = smgr->addBillboardSceneNode(camera,core::dimension2d<f32>(randx,randy),vector3df(0,0,42) );
        time = 333;
        }
            bnode->setMaterialFlag(video::EMF_LIGHTING, false);
            bnode->setMaterialTexture(0, device2->getVideoDriver()->getTexture("data/textures/muzzleflash.tga"));
            bnode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

            bnode->setRotation(vector3df(90+randgun,10,180+randgun));// was gunnode i changed it to bnode

            scene::ISceneNodeAnimator* anim;
            anim = smgr->createDeleteAnimator(time);
            bnode->addAnimator(anim);
            anim->drop();

//scene::ISceneNode* what = smgr->addLightSceneNode(NULL, core::vector3df(0,10,10), video::SColorf(5.0f, 255.0f, 255.0f), 1000, -1);

    // spawn one empty shell

    printf("%d\n", ammoLeftInMagazine);
    return true;
  }
  // no ammo left in magazine!
  else
  {
      reload();
    //playSound("sound/clic.wav");
  }
  return false;
}



void firstPersonWeapon::OnAnimationEnd(IAnimatedMeshSceneNode* node)
{
  if (action == FPW_DRAW)
  {
    action = FPW_AIM;
    node->setFrameLoop(PLAYER_AIMING);
    node->setLoopMode(true);
            node->setAnimationSpeed(aniSpeed/2); // play the gunsights slow
  }

  else if (action == FPW_SHOOT_SHOULDER)
  {
    action = FPW_AIM;
    node->setFrameLoop(PLAYER_AIMING);
    node->setLoopMode(true);
 //   idle(); //unless scared or rushe
  }

  else if (action == FPW_SHOOT_HIP) idle();

  else if (action == FPW_HOLSTER) ;//idle();

  else if (action == FPW_RELOAD)
  {
    //NOW gun is ready
    ammoLeftInMagazine = nbshotsPerMagazine;
    //magazineBar->setValue(ammoLeftInMagazine);

    if (wasAiming)
    {
        draw();
        action = FPW_AIM;
        node->setFrameLoop(PLAYER_AIMING);
        node->setAnimationSpeed(aniSpeed);
    }
    else
    {
        action = FPW_IDLE;
        node->setFrameLoop(PLAYER_IDLE);
        node->setAnimationSpeed(aniSpeed);
    }

    node->setLoopMode(true);
    wasAiming = false;
  }
}



u8 firstPersonWeapon::getAmmo()
{
  return ammoLeftInMagazine;
}





