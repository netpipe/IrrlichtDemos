
#ifndef __BLOCK_H
#define __BLOCK_H

#include <irrlicht.h>
#include <ode/ode.h>
#include "info.h"
#include "collision.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Block
{
public:
    Block(Info *, dWorldID, dSpaceID, int);
    void setPosition(vector3df pos);
    void setRotation(vector3df rot);
    void Refresh();
    vector3df getPosition();
    vector3df getRotation();
    dBodyID getBody() { return dBody; }
    int type;
private:
    int i;
    
    IAnimatedMeshSceneNode *node;
    IAnimatedMesh *mesh;
    
    // ODE
    dBodyID dBody;
    dGeomID dGeom;
    
    // collision
    Collision collision;
    
    // misc
    void QuaternionToEuler(const dQuaternion quaternion, vector3df &euler);
};

#endif
