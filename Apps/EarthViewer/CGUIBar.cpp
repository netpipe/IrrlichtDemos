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

#include "CGUIBar.h"

/*==============================================================================
  Constructor
==============================================================================*/
CGUIBar::CGUIBar(u16 _x1, u16 _y1, u16 _x2, u16 _y2, u8 start, IVideoDriver* _driver) 
{
   driver = _driver;

   x1 = _x1;
   y1 = _y1;
   x2 = _x2;
   y2 = _y2;

   barSize = (x2-3) - (x1+3);

   // color of the 3 border rects, and the "main" rect
   c[0] = SColor(255, 100,100,100);
   c[1] = SColor(255, 125,125,125);
   c[2] = SColor(255, 150,120,120);
   c[3] = SColor(255, 180,180,  0);

   // 3 rects for the borders
   rect[0] = core::rect<s32> (x1, y1, x2, y2);
   rect[1] = core::rect<s32> (x1+1, y1+1, x2-1, y2-1);
   rect[2] = core::rect<s32> (x1+3, y1+3, x2-3, y2-3);

   //current bar value
   barValue = start;
}

/*==============================================================================
  draw
==============================================================================*/
void CGUIBar::draw()
{
   // ... Draw border rectangles for a cool look 
   driver->draw2DRectangle(c[0], rect[0]);
   driver->draw2DRectangle(c[1], rect[1]);
   driver->draw2DRectangle(c[2], rect[2]);

   // Adjust bar color according to its value: red for high, decreasing to
   //yellow till green. (invert v1 and v2 for the opposite)
   u8 v1 = 255 - barValue;
   u8 v2 = barValue;

   SColor color(255, v1,v2, 0);

   #define BARMAXVALUE 255
   driver->draw2DRectangle(core::rect<s32>(x1+3, y1+3,
                           x1+3+(s32)(barSize*barValue/BARMAXVALUE), y2-3),
                           color, color, c[3], c[3]);
}

/*==============================================================================
  set value
==============================================================================*/
void CGUIBar::setValue(u8 val)
{
   barValue = val;
}

/*==============================================================================
  get value
==============================================================================*/
u8 CGUIBar::getValue()
{
   return barValue;
}

/*==============================================================================
  Destructor
==============================================================================*/
CGUIBar::~CGUIBar()
{

}
