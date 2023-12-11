/*
 * ray3d.cpp

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

#include "ray3d.h"

  RayTyp::RayTyp()  {  }

  /* for initializing video */
  void RayTyp::init(E_DRIVER_TYPE driver, int w, int h, bool fs, bool shadows, bool hwSkin)
  {
    irrlicht=createDevice(driver, dimension2d<u32>(w,h), 32, fs, shadows, false, &rcv);

    Video=irrlicht->getVideoDriver();
    Scene=irrlicht->getSceneManager();
    Gui=irrlicht->getGUIEnvironment();

    // default window title
    irrlicht->setWindowCaption(L"ray3d v0.01 experimental");

    // demand 32 bit textures
    Video->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);

    // 2d image filtering
    Video->getMaterial2D().TextureLayer[0].BilinearFilter=true;
    Video->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;


    // set skinning mode
    useHwSkinning = hwSkin;
  }

  void RayTyp::setWindowTitle(wchar_t *title)
  {
	irrlicht->setWindowCaption(title);
  }

  void RayTyp::hideCursor()
  {
    irrlicht->getCursorControl()->setVisible(false);
  }
  void RayTyp::showCursor()
  {
	 irrlicht->getCursorControl()->setVisible(true);
  }
  void RayTyp::placeCursor(f32 x, f32 y)
  {
    irrlicht->getCursorControl()->setPosition(x,y);
  }

  void RayTyp::importZipFile(char *filename)
  {
    irrlicht->getFileSystem()->addZipFileArchive(filename);
  }

  // returns weather or not ray3d wants to be running
  bool RayTyp::running(void)
  {
    return irrlicht->run();
  }


  void RayTyp::exit(void)
  {
    irrlicht->drop();
  }

  u32 RayTyp::getTime()
  {
    return irrlicht->getTimer()->getTime();
  }

RayTyp Ray;
