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
/*
 control with Left Mouse Button, and Mouse Wheel.
 todo
     shader to blend night and day texture
     put markers
     we can imagine a toolbar
     rotate at real speed to show sun exposition

 changes
     CEarth in its own file, this class is now derived from IEventReceiver.
     Movement is more smooth
*/

#include "CEarth.h"
#include <stdio.h>
#include <vector>

using namespace std;

/*==============================================================================
// get fullscreen resolution
==============================================================================*/
core::dimension2du getScreenResolution()
{
 IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
 core::dimension2du Res = nulldevice->getVideoModeList()->getDesktopResolution();
 Res = dimension2du (800,600);
 nulldevice -> drop();
 return Res;
}

/*==============================================================================
// Entry Point
==============================================================================*/
int main()
{
  core::dimension2du Res = getScreenResolution();

  IrrlichtDevice *device = createDevice(EDT_OPENGL, Res, 32, false, false, false);
  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();

  ICameraSceneNode* cam = smgr->addCameraSceneNode();
                    cam->setPosition(vector3df(0,100,2060));
                    cam->setTarget(vector3df(0,0,0));

  CEarth* earth = new CEarth(device, Res, cam ,vector3df (0,400,500));
  CEarth* earth2 = new CEarth(device, Res, cam, vector3df (0,700,500));
  CEarth* earth3 = new CEarth(device, Res, cam, vector3df (0,1000,500));
  //CEarth* earth4 = new CEarth(device, Res, cam, vector3df (0,1700,500));
  //CEarth* earth5 = new CEarth(device, Res, cam, vector3df (0,2200,500));

//   for (int i; i > 5;i++)
//   {
//        std::vector<CEarth*> Planets = new CEarth(device, Res, cam, vector3df (0,300+1000,500));
//
//        Planets(0)->addLights(smgr);
//        // move it a bit at the beginning
//        Planets(1)->setYimpulsion(.51f);
//    }
//

        //earth = new CEarth(device, Res, cam, vector3df (0,300+1000,500));
        earth->addLights(smgr);
//        // move it a bit at the beginning
        earth->setYimpulsion(.51f);

 // earth2->set

 // device->setEventReceiver(earth);


  u32 then = device->getTimer()->getTime();

  while(device->run())
  {
    if (device->isWindowActive())
    {
      if (earth->pressedESCAPE()) break;

      u32 const now = device->getTimer()->getTime();
      if (then < now)
      {
        driver->beginScene(true, true,  video::SColor(255,200,200,200));
        earth->render();
        smgr->drawAll();
        driver->endScene();
        then = now;
      }
      else device->sleep(30);
    }
    else device->sleep(200);
  }

  device->drop();
  return 0;
}


