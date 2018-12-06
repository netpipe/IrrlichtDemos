/*
 * render.cpp

    Copyright © 2010 John Serafino
    This file is part of ray3d.

    Ray3d is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ray3d v0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with ray3d If not, see <http://www.gnu.org/licenses/>.
 */

#include "render.h"

  void RenderTyp::clearScreen(int r, int g, int b, int a)
  {
    Video->beginScene(true, true, SColor(a,r,g,b));
  }
  void RenderTyp::drawScreen(void)
  {
    Scene->drawAll();
    Gui->drawAll();
  }
  void RenderTyp::endScene(void)
  {
    Video->endScene();
  }

  void RenderTyp::setSkinningMode(bool hwSkin, int speed)
  {
	  hwSkinSpeed = speed;
	  useHwSkinning = hwSkin;
  }

  void RenderTyp::takeScreenshot()
  {
     irr::video::IVideoDriver* const driver = irrlicht->getVideoDriver();

     //get image from the last rendered frame
     irr::video::IImage* const image = driver->createScreenShot();
     if (image) //should always be true, but you never know. ;)
     {
        //construct a filename, consisting of local time and file extension
        irr::c8 filename[64];
        snprintf(filename, 64, "screenshot_%u.png", irrlicht->getTimer()->getRealTime());

        //write screenshot to file
        if (!driver->writeImageToFile(image, filename))
           irrlicht->getLogger()->log(L"Failed to take screenshot.", irr::ELL_WARNING);

        //Don't forget to drop image since we don't need it anymore.
        image->drop();
     }
  }
