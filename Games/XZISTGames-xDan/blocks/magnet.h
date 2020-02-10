
#ifndef __MAGNET_H
#define __MAGNET_H

#include <irrlicht.h>
#include <ode/ode.h>
#include "info.h"
#include "collision.h"
#include "block.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Magnet
{
public:
    Magnet(Info *, dWorldID, dSpaceID, vector3df);
    ~Magnet();
    vector3df getEndPos();
    void translate(vector3df);
    void attachBlock(Block *block, dContactGeom);
    void release();
    void Refresh();
    
private:
    int i;
    dWorldID dWorld;
    vector3df pos;
    
    // chain
    dJointID worldJoint;
    array <IAnimatedMeshSceneNode *> nodes;
    array <dBodyID> bodies;
    array <dGeomID> geoms;
    
    // magnet collision
    Collision colMagnet;
    array <dJointID> blockJoints;
    
    // misc
    void QuaternionToEuler(const dQuaternion quaternion, vector3df &euler);
};

#endif
