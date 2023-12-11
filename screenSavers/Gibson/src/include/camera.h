/*
    camera.h

    Created on: Mar 25, 2010



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

#ifndef CAMERA_H_
#define CAMERA_H_

#include "globals.h"
#include "mesh.h"

/* camera framework */
class Camera
{
public:

  ICameraSceneNode *camera;  // the actual camera
  ISceneNode       *top;     // above camera
  ISceneNode       *front;   // in front of camera

  f32 rx,ry,rz;
  f32 x,y,z;

  vector3df direction;


  // initialize a camera at 0,0,0 or the location passed in
  void init(float x=0, float y=0, float z=0);


  // locally translate the camera
  void translate(f32 x, f32 y, f32 z);
  void translateGlobal(f32 x, f32 y, f32 z);


  // locally rotate the camera
  void rotate(float x, float y, float z);
  // globally rotate the camera
  void rotateGlobal(float x, float y, float z);
  // rotate the camera globally and locally
  void rotateGlobalLocal(vector3df glob, vector3df loc);

  // update the camera, should be called at the end of mainloop
  void update(void);

  // sets the global rotation of the camera
  void setRotation(float x, float y, float z);

  // sets the global position of the camera
  void setPosition(float x, float y, float z);

  void addChild(Entity &child);

  void setFOV(f32 fov);

  void addCollision(Entity coll, f32 sx=30, f32 sy=50, f32 sz=30,   f32 tx=0, f32 ty=0, f32 tz=0, f32 gx=0, f32 gy=-10, f32 gz=0);

};

#endif /* CAMERA_H_ */
