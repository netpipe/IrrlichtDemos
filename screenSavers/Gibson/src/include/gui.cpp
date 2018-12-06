/*
 * gui.cpp

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

#include "gui.h"

void Font::loadFont(char * filename)
{
  font = Gui->getFont(filename);
}

Font::Font()
{
  //font = Gui->getBuiltInFont();
}


void drawText(Font locFont, const wchar_t *text, int x, int y, int r, int g, int b, int a)
{
  dimension2d<u32> size = locFont.font->getDimension(text);
  locFont.font->draw(text,rect<s32>(x,y, (x + size.Width),(y + size.Height)), SColor(a,r,g,b));
}








void Image::loadImg(char *filename, int r, int g, int b, int a)
{
  img = Video->getTexture(filename);
  Video->makeColorKeyTexture(img, SColor(a,r,g,b));

  dimension2d<u32> size = img->getOriginalSize();
  w = size.Width;
  h = size.Height;
}

void Image::draw(int x, int y, int r, int g, int b, int a)
{
  Video->draw2DImage(img, position2d<s32>(x,y), rect<s32>(0,0, w, h),0, SColor(a,r,g,b), true);
}
