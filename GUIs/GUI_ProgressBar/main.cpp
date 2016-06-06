/*
  Copyright (C) 2008-2009 by "Katsankat" mccauley404@gmail.com

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is not granted to anyone to use this software for any purpose,
  including private and commercial applications, and to alter it and
  redistribute it.
*/
#include "CGUIBar.h"

int main()
{
  dimension2d<s32> Res(600,400);
  IrrlichtDevice* device = createDevice(EDT_OPENGL, Res, 16, false);
  if (!device) return 0;

  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr  = device->getSceneManager();
  device->setWindowCaption(L"progress bars");

  // add test bars
  CGUIBar* healthBar[4];

  // the 1st bar will go from 0 to 255
  u8 a=0;
  healthBar[0] = new CGUIBar(20,40, 200,60, a, driver);

  // while the other will go from 255 to 0
  u8 b = 255;
  healthBar[1] = new CGUIBar(20,80, 200,100, b, driver);

  // and two dummy bars respectively initialized to value 120 and 200
  healthBar[2] = new CGUIBar(20,120, 200,140, 120, driver);
  healthBar[3] = new CGUIBar(20,160, 200,180, 200, driver);

  u32 now=0, then=0;
  while(device->run())
  {
    if (device->isWindowActive())
    {
      now = device->getTimer()->getTime();
      if (then < now)
      {
         driver->beginScene(true, true, 0);
         //smgr->drawAll(); no scene here

         // increase first bar
         //a = healthBar[0]->getValue() + 1;
         a++;
         if (a == 255) break;
         healthBar[0]->setValue(a);

         // decrease second bar
         b--;
         if (b == 0) break;
         healthBar[1]->setValue(b);

         for (u8 n=0; n<4; n++)
           healthBar[n]->draw();

         driver->endScene();
         then = now + 30;
      }
      device->sleep(10,0);
    }
  }

  delete healthBar[0];
  delete healthBar[1];
  device->closeDevice();

  printf("Press any key to exit ...");
  getchar();
  return 0;
}
