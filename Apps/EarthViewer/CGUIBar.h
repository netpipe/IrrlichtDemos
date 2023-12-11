/*******************************************************************************
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

 Following the previous developments of eMgz and pera

  http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=25017

 The code was changed a bit, mainly with performances in mind :

  * Working only with u8 no floats
    So the range of the bar is now 0-255. MaxValue is always 255.
  * Removed texture and name of the bar (can be implemented via derived class)
  * Removed unused variables.
  * Renamed methods.
  * Removed initialisations and calculations from the draw() method.

*/

#ifndef CGUIBar_H
#define CGUIBar_H

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")


class CGUIBar
{
   IVideoDriver* driver;
   u16 x1, y1, x2, y2; // bar 2D coords
   u16 barSize;
   u8 barValue; // Current bar value
   SColor c[4];
   irr::core::rect<s32> rect[3];

public:
   CGUIBar(u16 x1, u16 y1, u16 x2, u16 y2, u8 start, IVideoDriver* driver);
   void draw();
   void setValue(u8 val);
   u8 getValue();
   ~CGUIBar();
};

#endif
