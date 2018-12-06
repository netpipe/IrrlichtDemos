/*
  render.h

   Created on: Mar 16, 2010

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

#ifndef RENDER_H_
#define RENDER_H_

#include "globals.h"
#include "camera.h"

class RenderTyp
{
public:

  void clearScreen(int r=0, int g=0, int b=0, int a=255);

  void drawScreen(void);

  void endScene(void);

  void setSkinningMode(bool hwSkin, int speed=60);

  void takeScreenshot();

};

#endif /* RENDER_H_ */
