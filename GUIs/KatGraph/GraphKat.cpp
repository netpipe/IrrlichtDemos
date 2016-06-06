/*******************************************************************************
 ** Copyright (C) 2008-2009 by Katsankat mccauley404@gmail.com                **
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

#include "GraphKat.h"

/*==============================================================================
  Constructor
==============================================================================*/
GraphKat::GraphKat(wchar_t* _caption, IrrlichtDevice* device, core::rect<s32> Rect)
{
  driver = device->getVideoDriver();

  L = Rect.UpperLeftCorner.X;
  T = Rect.UpperLeftCorner.Y;
  W = Rect.LowerRightCorner.X;
  H = Rect.LowerRightCorner.Y;

  // default colors
  white = SColor(255, 255,255,255);
  start = SColor(255, 255,170,120);
  end   = SColor(255, 85 ,32 ,50 );

  // draw text
  caption = _caption;
  IGUIEnvironment* guienv = device->getGUIEnvironment();
  font = guienv->getFont("lucida8.bmp");
  guienv->getSkin()->setFont(font);

  // init with random values
  u8 nbVals = W - L;
  Max = H - T - 10; // -10 to draw text

  for (u8 x=0; x<nbVals; x++)
    t.push_back(rand()%Max);
}
/*==============================================================================
  Adding a value will put it at the end of the list, and erase the first element
==============================================================================*/
void GraphKat::addValue(u8 val)
{
  t.push_back(val);
  t.erase(0);
}
/*==============================================================================
  simplest draw method
==============================================================================*/
void GraphKat::draw()
{
  drawBorder();

  //values
  for (u8 x=1; x<t.size(); x++)
    driver->draw2DLine (position2d<s32>(L+x, H-t[x-1]),
                        position2d<s32>(L+x, H-t[x]  ),   white);
}
/*==============================================================================
  Draw white border
==============================================================================*/
void GraphKat::drawBorder()
{
  driver->draw2DLine (position2d<s32>(L,T), position2d<s32>(W,T), white);
  driver->draw2DLine (position2d<s32>(L,T), position2d<s32>(L,H), white);
  driver->draw2DLine (position2d<s32>(L,H), position2d<s32>(W,H), white);
  driver->draw2DLine (position2d<s32>(W,T), position2d<s32>(W,H), white);
}
/*==============================================================================
  orange to red. full of f32s
==============================================================================*/
void GraphKat::drawOrange()
{
  drawBorder();

  static u8 nb = W - L - 10; // -10 to draw text above

  //since there are nbVals = W - L values, step is
  static f32 Rstep = (start.getRed()   - end.getRed()  ) / nb; // 170 / 90 = 1,8889
  static f32 Gstep = (start.getGreen() - end.getGreen()) / nb;
  static f32 Bstep = (start.getBlue()  - end.getBlue() ) / nb;

  font->draw (caption, core::rect<s32>(L+2,T+1,W,H), start);

  //values
  for (u8 x=1; x<t.size(); x++)
    driver->draw2DLine (position2d<s32>(L+x, H-t[x-1]),
                        position2d<s32>(L+x, H-t[x]  ),
                        SColor (255, 255-(u8)(x*Rstep), (u8)(x*Gstep), (u8)(x*Bstep)) );
}
/*==============================================================================
  Green dark to light
==============================================================================*/
void GraphKat::drawGreen()
{
  drawBorder();

  static u8 nb = W - L - 10; // -10 to draw text above

  static f32 Rstep = (start.getRed() - end.getRed()) / nb;

  font->draw (caption, core::rect<s32>(L+2,T+1,W,H), start);

  //values
  for (u8 x=1; x<t.size(); x++)
    driver->draw2DLine (position2d<s32>(L+x, H-t[x-1]),
                        position2d<s32>(L+x, H-t[x]  ),
                        SColor(255,(u8)(x*Rstep), 100, 10) );
}
/*==============================================================================
  Blue dark to light
==============================================================================*/
void GraphKat::drawBlue()
{
  //drawBorder();
  driver->draw2DLine (position2d<s32>(L,H), position2d<s32>(W,H), white);

  static u8 nb = W - L - 10; // -10 to draw text above

  static f32 Rstep = (start.getRed() - end.getRed()) / nb;

  font->draw (caption, core::rect<s32>(L+2,T-1,W,H), start);

  //values
  for (u8 x=1; x<t.size(); x++)
    driver->draw2DLine (position2d<s32>(L+x, H-t[x-1]),
                        position2d<s32>(L+x, H-t[x]  ),
                        SColor(255,(u8)(x*Rstep), 100, 100) );
}
/*==============================================================================
  dark Blue to dark blue
==============================================================================*/
void GraphKat::drawCrazy()
{
  drawBorder();

  static u8 nb = W - L-10; // -10 to draw text above

  //static SColor start (255, 255,170,120);
  //static SColor end   (255, 85 ,32 ,50);

  //static f32 Rstep = (f32)(start.getRed()  - end.getRed() ) / nb;
  static f32 Gstep = (f32)(start.getGreen() - end.getGreen()) / nb;
  static f32 Bstep = (f32)(start.getBlue() - end.getBlue()) / nb;

  //static f32 red = 0.f;
  static f32 green = 0.f;
  static f32 blue= 0.f;

  font->draw (caption, core::rect<s32>(L+2,T+1,W,H), start);

  //values
  for (u8 x=1; x<t.size(); x++)
  {
    driver->draw2DLine (position2d<s32>(L+x, H-t[x-1]),
                        position2d<s32>(L+x, H-t[x]  ),
                        SColor(255, 100, (u8)green, (u8)blue) );
    //red   += Rstep; if (red   > 255.f) red=0.f;
    green += Gstep; if (green > 255.f) green=0.f;
    blue  += Bstep; if (blue  > 255.f) blue=0.f;
  }
}
/*==============================================================================
  very special one
==============================================================================*/
void GraphKat::drawStrange()
{
  static u8 nb = W - L - 10;

  static f32 Rstep = (f32)(start.getRed()  - end.getRed() ) / nb;

  //font->draw (caption, core::rect<s32>(L+2,T+1,W,H), start);

  for (u8 x=0; x<t.size(); x++)
    driver->draw2DLine (position2d<s32>(L+1, H-t[x-1]),
                        position2d<s32>(L+x  , H-t[x]),
                        SColor(255,100, 255-(u8)(x*Rstep), 100) );
}
/*==============================================================================
  get Max Y value, it is lower right point Y - upper left point Y
==============================================================================*/
u8 GraphKat::getMax()
{
  return Max;
}
/*==============================================================================
  set Caption
==============================================================================*/
void GraphKat::setText(wchar_t* text)
{
  //if (caption) delete caption;
  caption = text;
}
/*==============================================================================
  Destructor
==============================================================================*/
GraphKat::~GraphKat()
{

}
