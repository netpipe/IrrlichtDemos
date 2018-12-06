/*
   ray3d.h

   Created on: Mar 16, 2010


    Copyright © 2010 John Serafino
    This file is part of ray3d v0.01.

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

#ifndef RAY_H_
#define RAY_H_

#include <iostream>
using namespace std;

#include <irrlicht.h>
#include <stdio.h>

#include "globals.h"
#include "mesh.h"
#include "render.h"
#include "events.h"
#include "lighting.h"
#include "gui.h"
#include "camera.h"


/* Definition of the ray class which is used for initialization and miscalaneous things */
class RayTyp
{
public:
  // renderer
  RenderTyp Render;

  RayTyp();

  /* for initializing video */
  void init(E_DRIVER_TYPE driver, int w, int h, bool fs=false, bool shadows=true, bool hwSkin=true);


  void setWindowTitle(wchar_t *title);

  void hideCursor();
  void showCursor();
  void placeCursor(f32 x, f32 y);



  void importZipFile(char *filename);

  // returns weather or not ray3d wants to be running
  bool running(void);



  void exit(void);


  u32 getTime();

};

extern RayTyp Ray;

#endif /* RAY_H_ */
