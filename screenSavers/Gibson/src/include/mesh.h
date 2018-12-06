/*
    Mesh.h
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

#ifndef MESH_H_
#define MESH_H_

// types of entities
#define MESH_TYPE  1
#define LIGHT_TYPE 2
#define MAP_TYPE   3
#define ANIM_TYPE  4
#define PARTICLE_SYS_TYPE 5


#include "globals.h"
#include "HardwareSkinCallback.h"

extern HWSkinCB hwSkinInstance;

class Entity
{
public:
  ILightSceneNode          *lightNode;
  //ISceneNode             *lightNode;
  ISceneNode		       *sceneNode;
  IAnimatedMeshSceneNode   *animNode;
  IParticleSystemSceneNode *pSys;
  IMesh 				   *tmesh;
  IAnimatedMesh			   *bmesh;
  IAnimatedMesh			   *mesh;
  ITexture				   *tex;
  IImage				   *img;

  vector2df particleSize;


  bool lit;        // if object should be affected by light
  int type;        // what kind of entity it is
  bool animated;   // if it's animated
  bool isVisible;  // if it's visible

  // holds transformation data
  f32 rx,ry,rz;
  f32 x,y,z;


  Entity();


  // Transformations //
  /////////////////////

  // local
  void rotate(float x, float y, float z);

  // local
  void translate(float x, float y, float z);

  // global
  void setScale(float x, float y, float z);

  // global
  void setRotation(float x, float y, float z);
  void rotateGlobal(f32 x, f32 y, f32 z);

  // global
  void setPosition(float x, float y, float z);
  void translateGlobal(f32 x, f32 y, f32 z);

  void setRadius(f32 r);


  // Creation //
  //////////////

  void createMap();
  void createMesh();
  void createLight(E_LIGHT_TYPE typ, f32 r=1, f32 g=1, f32 b=1, f32 a=1);
  void createAnimMesh();

  void createParticleSystem(Entity *parent, bool def, f32 sizeX, f32 sizeY, int fadetime = 1000);
  //void addMeshEmitter();
  void addBoxEmitter(vector3df boxDim = vector3df(3,3,3), vector3df dir = vector3df(0,0.3,0), vector2df pps = vector2df(50,100), vector2df size = vector2df(10,10), vector2df life = vector2df(1000,2000), f32 angle = 3);
  void addPointEmitter();
  void addSphereEmitter();
  void addCylinderEmitter();
  void addRingEmitter();

  void stopEmitting();


  void resetPSys();


  // data loading //
  //////////////////

  void loadMesh(char *filename, bool planar=false, bool hwSkin=useHwSkinning, int skinSpeed=hwSkinSpeed);
  void copyFrom(Entity ent);
  void loadTex(char *filename);
  void loadBsp(char *filename);
  void loadBumpmap(char *filename, f32 h);
  void loadBillboard(char *filename, f32 w, f32 h);

/*
  void loadBillboard(char *texture, f32 x, f32 y)
  {
    if(type == MESH_TYPE)
    {
      sceneNode = Scene->addBillboardSceneNode(sceneNode, dimension2d<f32>(x,y));
      sceneNode->setMaterialTexture(0, Video->getTexture(texture));
    }
    else if(type == LIGHT_TYPE)
    {
      lightNode = Scene->addBillboardSceneNode(lightNode, dimension2d<f32>(x,y));
      lightNode->setMaterialFlag(EMF_LIGHTING, false);
      lightNode->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
      lightNode->setMaterialTexture(0, Video->getTexture(texture));
    }
  }
*/
  // spiffy routines //
  /////////////////////

  void setFrameLoop(int b, int e);

  void hide(void);

  void show(void);

  void enableShadow();


  void addChild(Entity &child);

  void setLit(bool toggle);
  void setWire(bool toggle);

  void addCollision(Entity coll, f32 sx=30, f32 sy=50, f32 sz=30,   f32 tx=0, f32 ty=0, f32 tz=0, f32 gx=0, f32 gy=-10, f32 gz=0);

  void update();
  vector3df getRotation();
  vector3df getPosition();
  vector3df getGlobalPosition();


  //void addCameraChild(Camera &child);
};

#endif /* MESH_H_ */
