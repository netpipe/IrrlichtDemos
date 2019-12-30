#ifndef _Collision_HPP_
#define _Collision_HPP_
#include <irrlicht.h>
//#include "Globals.hpp"
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
class Collision
{
  private:

    IrrlichtDevice *irrDevice;

  public:

    IMetaTriangleSelector* metaSelector;
    ISceneNodeAnimatorCollisionResponse* anim;
    vector3df ellipsoidRadius;



    static Collision *Instance();

    void recursiveFillMetaSelector(ISceneNode* node, IMetaTriangleSelector* meta);
    void createRootCollision(void);
    void createPlayerCollision(void);
    vector3df getEllipsoidRadius(void);

    aabbox3d<f32> expandBox(aabbox3d<f32> box, f32 amount)
    {
      aabbox3d<f32> newBox;
      newBox = box;
      amount = amount / 2;
      //newBox.MinEdge.X = box.MinEdge.X - amount;
      newBox.MinEdge.Y = box.MinEdge.Y - amount;
      //newBox.MinEdge.Z = box.MinEdge.Z - amount;

      //newBox.MaxEdge.X = box.MaxEdge.X + amount;
      newBox.MaxEdge.Y = box.MaxEdge.Y + amount;
      //newBox.MaxEdge.Z = box.MaxEdge.Z + amount;

      return newBox;
    }

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }

  protected:

    static Collision *m_collision;
    static Collision m_default_collision_buffer;

};

#endif

