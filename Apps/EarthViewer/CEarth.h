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

#ifndef CEarth_H
#define CEarth_H

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

/*==============================================================================
// Here is the simple earth class
==============================================================================*/
class CEarth : public IEventReceiver
{
 scene::ISceneNode* node;
 f32 forceY;
 f32 forceZ;
 f32 strength; // strength of movement


  ICameraSceneNode* cam;
  f32 forceCam;
  f32 camStrength;

  //bool KeyIsDown[KEY_KEY_CODES_COUNT];
  bool pressESCAPE;
  bool LMB;
  u16 oldX;
  u16 oldZ;



public:

 CEarth(IrrlichtDevice *device, core::dimension2du Res, ICameraSceneNode* _cam,vector3df position);

 void addLights(ISceneManager* smgr);

 void render();

 virtual bool OnEvent(const SEvent& event);

 void setYimpulsion(f32 imp);

 void setCamImpulsion(f32 imp);

 scene::ISceneNode* getNode();

 bool pressedESCAPE();

 ~CEarth();

};

#endif
