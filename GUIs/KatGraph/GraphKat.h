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

#ifndef BX_GraphKat_H
#define BX_GraphKat_H

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

class GraphKat
{
 IVideoDriver* driver;

 //caption
 wchar_t* caption;
 IGUIEnvironment* guienv;
 IGUIFont* font;

 // The array of points to be drawn
 core::array<u8> t;

 //physical properties
 u16 L,T,W,H;
 u8 Max; // max Y value
 SColor white;
 SColor start, end;

 void drawBorder();

public:
 GraphKat(wchar_t* _caption, IrrlichtDevice* device, core::rect<s32> rect);

 void setText(wchar_t* text);

 void addValue(u8 val);

 void draw();
 void drawOrange();
 void drawGreen();
 void drawBlue();
 void drawCrazy();
 void drawStrange();

 u8 getMax();

 ~GraphKat();
};

#endif
