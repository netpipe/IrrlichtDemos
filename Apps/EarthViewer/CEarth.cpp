/*******************************************************************************
| _ \_ _(_)_ __ (_) |_(_)_ _____
|  _/ '_| | '  \| |  _| \ V / -_)
|_| |_| |_|_|_|_|_|\__|_|\_/\___|
                     _     ___          _   _
 __ _ ___  ___  __ _| |___| __|__ _ _ _| |_| |_
/ _` / _ \/ _ \/ _` | / -_) _|/ _` | '_|  _| ' \
\__, \___/\___/\__, |_\___|___\__,_|_|  \__|_||_|
|___/          |___/

 ** Copyright (C) 2008-2009 by "Katsankat" mccauley404@gmail.com              **
 **                                                                           **
 ** This software is provided 'as-is', without any express or implied		  **
 ** warranty.  In no event will the authors be held liable for any damages	  **
 ** arising from the use of this software.					                  **
 **										                                      **
 ** Permission is not granted to anyone to use this software for any purpose, **
 ** including private and commercial applications, and to alter it and		  **
 ** redistribute it.								                          **
 **										                                      **
 ******************************************************************************/

#include "CEarth.h"
#include "CGUIBar.h"

/*==============================================================================
  Constructor
==============================================================================*/
CEarth::CEarth(IrrlichtDevice *device, core::dimension2d<s32> Res, ICameraSceneNode* _cam)
{
  cam = _cam;
  forceCam = 0.1f;
  camStrength = .0001f;

  LMB = pressESCAPE = false;
  oldX = oldZ = 0;

  //for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
    //KeyIsDown[i] = false;

  strength = .00001f;
  forceY = forceZ = 0.f;

  ISceneManager* smgr = device->getSceneManager();
  IVideoDriver* driver = device->getVideoDriver();

  IGUIEnvironment* guienv = device->getGUIEnvironment();
  //IGUIFont* font = guienv->getFont("boston.bmp");
  //guienv->getSkin()->setFont(font);

  // ping zero presents ...
  u16 width  = 640;
  u16 height = 431;
  u16 left = (u16)((Res.Width - width) / 2);
  u16 top  = (u16)((Res.Height -height)/ 2);

  ITexture* tex = driver->getTexture("earthviewer.jpg");
  IGUIImage* i = guienv->addImage(core::rect<s32>(left,top, left+width,top+height));
             i->setImage(tex);
             i->setScaleImage(true);
  //tex->drop();

  // progressbar
  CGUIBar* progressBar = new CGUIBar(left+width/2,top+125,
                                      left+width-20,top+150, 0, driver);
  // loading loop
  u8 a = 0;
  u32 then = 0;
  while (device->run())
  {
    u32 const now = device->getTimer()->getTime();

    if (then < now)
    {
      driver->beginScene(true, true, 0);
      //smgr->drawAll();
      guienv->drawAll();

      if ((++a) == 122) break;
      progressBar->setValue(a);
      progressBar->draw();

      driver->endScene();

      then = now + 10;
    }
  }

  // loading large stuff
  node = smgr->addSphereSceneNode(5.f, 64);
  if (node)
  {
    node->setPosition(core::vector3df(0,0,0));
    //node->setRotation(vector3df(0,-90,0));
    node->setScale(vector3df(32,32,32));
    node->setMaterialTexture(0, driver->getTexture("1_earth_8k.jpg"));
    //node->setMaterialTexture(0, driver->getTexture("2_no_clouds_8k.jpg"));
    //node->setDebugDataVisible(EDS_FULL);
    //node->setMaterialFlag(video::EMF_WIREFRAME,true);
  }

  // finish bar for your eyes only
  while (device->run())
  {
    u32 const now = device->getTimer()->getTime();

    if (then < now)
    {
      driver->beginScene(true, true, 0);
      //smgr->drawAll();
      guienv->drawAll();

      progressBar->setValue(a);
      progressBar->draw();

      if ((++a) == 255) break;

      driver->endScene();

      then = now + 10;
    }
  }
 }

/*==============================================================================
// add Lights
==============================================================================*/
void CEarth::addLights(ISceneManager* smgr)
{
  ILightSceneNode* sun[2];

  vector3df pos[2] = {
    vector3df(4000.0f, 1000.0f, -4000.0f),
    vector3df(4000.0f, 1000.0f, 4000.0f)
  };

  video::SColorf color[2] = {
    video::SColorf(.1f,.47f,.45f, .1f),
    video::SColorf(.6f,.47f,.45f, 1.f)
  };

  f32 radius = 600000.0f;

  for (u8 n=0; n<2; n++)
  {
   sun[n] = smgr->addLightSceneNode(0, pos[n], color[n]);
   sun[n]->setRadius(600000.0f);
  }
}

/*==============================================================================
  Events
==============================================================================*/
bool CEarth::OnEvent(const SEvent& event)
{
    if (event.EventType == irr::EET_KEY_INPUT_EVENT
            && event.KeyInput.Key == KEY_ESCAPE)
      pressESCAPE = true;
         //KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

    if (event.EventType == EET_MOUSE_INPUT_EVENT)
    {
      if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
      {
        oldX = event.MouseInput.X;
        oldZ = event.MouseInput.Y;
        LMB = true;
      }

      if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
      {
        this->setYimpulsion(((event.MouseInput.X - oldX)>0) ? -.01f : .01f);

        this->setCamImpulsion( ((event.MouseInput.Y - oldZ)>0) ? .01f : -.01f);

        LMB = false;
      }

      // mouse wheel
      else if(event.MouseInput.Event == EMIE_MOUSE_WHEEL)
      {
        vector3df pos = cam->getPosition();
        pos.Z += -event.MouseInput.Wheel;
        cam->setPosition(pos);
      }
    }

    return false;
}

/*==============================================================================

==============================================================================*/
void CEarth::render()
{
    if (forceY != 0)
    {
     vector3df rot = node->getRotation();

     rot.Y += forceY;
     forceY += (forceY > 0) ? -strength : strength;

     node->setRotation(rot);
    }

    //cam
    if (forceCam != 0)
    {
     vector3df pos = cam->getPosition();
     pos.Y += forceCam;
     forceCam += (forceCam > 0) ? -strength : strength;
     cam->setPosition(pos);
    }
}

/*==============================================================================

==============================================================================*/
void CEarth::setYimpulsion(f32 imp)
{
   forceY += imp;
}

/*==============================================================================

==============================================================================*/
void CEarth::setCamImpulsion(f32 imp)
{
   forceCam += imp;
}

/*==============================================================================

==============================================================================*/
scene::ISceneNode* CEarth::getNode()
{
   return node;
}

/*==============================================================================

==============================================================================*/
bool CEarth::pressedESCAPE()
{
    return pressESCAPE;
}

/*==============================================================================

==============================================================================*/
CEarth::~CEarth()
{
   node->remove();
}
