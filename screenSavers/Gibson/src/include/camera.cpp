/*
 * camera.cpp

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
#include "camera.h"


  // initialize a camera at 0,0,0 or the location passed in
  void Camera::init(float x, float y, float z)
  {
	// create camera scene node
    camera = Scene->addCameraSceneNode(NULL, vector3df(x,y,z));

    // empty reference nodes
    top   = Scene->addEmptySceneNode();
    front = Scene->addEmptySceneNode();

    // parent the reference nodes
    camera->addChild(top);
    camera->addChild(front);

    // put the reference nodes in place
    front->setPosition(vector3df(0,0,1));
    top->setPosition(vector3df(0,1,0));

    // set lookat
    camera->setUpVector(top->getAbsolutePosition() - camera->getAbsolutePosition());
    camera->setTarget(front->getAbsolutePosition() - camera->getAbsolutePosition());

    direction = vector3df(0,0,1);

    //camera = Scene->addCameraSceneNodeFPS();

    rx=ry=rz=0;
    x=y=z=0;

    camera->updateAbsolutePosition();
  }

  // locally translate the camera
  void Camera::translate(float x, float y, float z)
  {
	// translate the camera locally
	translateNode(camera, vector3df(x,y,z), false);

	// update reference nodes
	front->updateAbsolutePosition();
	top->updateAbsolutePosition();
	camera->updateAbsolutePosition();
  }

  void Camera::translateGlobal(f32 x, f32 y, f32 z)
  {
    vector3df pos = camera->getPosition();
    pos.X += x;
    pos.Y += y;
    pos.Z += z;

    camera->setPosition(pos);

	// update reference nodes
	front->updateAbsolutePosition();
	top->updateAbsolutePosition();
	camera->updateAbsolutePosition();
  }

  // locally rotate the camera
  void Camera::rotate(f32 x, f32 y, f32 z)
  {
    rotateNode(camera, vector3df(x, y, z));


    vector3df pos = camera->getRotation();

    rx=pos.X;
    ry=pos.Y;
    rz=pos.Z;
    camera->updateAbsolutePosition();
  }

  // update the camera, should be called at the end of mainloop
  void Camera::update(void)
  {
    camera->updateAbsolutePosition();

    front->updateAbsolutePosition();
    top->updateAbsolutePosition();
    camera->setTarget(front->getAbsolutePosition());
    camera->setUpVector(top->getAbsolutePosition() - camera->getAbsolutePosition());

    vector3df pos;
    pos = camera->getAbsolutePosition();
    x=pos.X;
    y=pos.Y;
    z=pos.Z;

    pos = camera->getRotation();

    rx=pos.X;
    ry=pos.Y;
    rz=pos.Z;

    if(rx > 360)
      rx -= 360;

    camera->setRotation(vector3df(rx,ry,rz));

    camera->updateAbsolutePosition();
  }

  // sets the global rotation of the camera
  void Camera::setRotation(float x, float y, float z)
  {
    rx=x;
    ry=y;
    rz=z;

    camera->setRotation(vector3df(rx,ry,rz));
  }

  // rotates globally
  void Camera::rotateGlobal(float x, float y, float z)
  {
	vector3df rot;
	rot.X = x;
	rot.Y = y;
	rot.Z = z;
	rot += camera->getRotation();

    camera->setRotation(rot);

   	camera->updateAbsolutePosition();
  }

  void Camera::rotateGlobalLocal(vector3df glob, vector3df loc)
  {
    matrix4 m;
    m.setRotationDegrees(camera->getRotation() + glob);
    matrix4 n;
    n.setRotationDegrees(loc);
    m *= n;

    camera->setRotation( m.getRotationDegrees() );
    camera->updateAbsolutePosition();

    front->updateAbsolutePosition();
    top->updateAbsolutePosition();
    camera->setTarget(front->getAbsolutePosition());
    camera->setUpVector(top->getAbsolutePosition() - camera->getAbsolutePosition());
  }

  // sets the global position of the camera
  void Camera::setPosition(float x, float y, float z)
  {
    camera->setPosition(vector3df(x,y,z));

	front->updateAbsolutePosition();
	top->updateAbsolutePosition();
	camera->updateAbsolutePosition();
  }


  void Camera::addChild(Entity &child)
  {
    if(child.type == MESH_TYPE)
      camera->addChild(child.sceneNode);
    if(child.type == ANIM_TYPE)
      camera->addChild(child.animNode);
    if(child.type == LIGHT_TYPE)
      camera->addChild(child.lightNode);
  }

  void Camera::addCollision(Entity coll, f32 sx,f32 sy,f32 sz,   f32 tx,f32 ty,f32 tz, f32 gx, f32 gy, f32 gz)
  {
    ITriangleSelector* selector = 0;

    if(coll.type == MESH_TYPE)
    {
      selector = Scene->createTriangleSelector(coll.tmesh, coll.sceneNode);
      coll.sceneNode->setTriangleSelector(selector);
    }
    if(coll.type == ANIM_TYPE)
    {
      selector = Scene->createTriangleSelector(coll.animNode->getMesh(), coll.animNode);
      coll.animNode->setTriangleSelector(selector);
    }
    if(coll.type == MAP_TYPE)
    {
      selector = Scene->createOctreeTriangleSelector(coll.bmesh->getMesh(0), coll.sceneNode);
      coll.sceneNode->setTriangleSelector(selector);
    }


    ISceneNodeAnimator* anim;
    anim = Scene->createCollisionResponseAnimator(selector, camera, vector3df(sx,sy,sz), \
      vector3df(gx,gy,gz), vector3df(tx,ty,tz));
    selector->drop();
    camera->addAnimator(anim);
    anim->drop();
  }


void Camera::setFOV(f32 fov)
{
  camera->setFOV(fov);
}
