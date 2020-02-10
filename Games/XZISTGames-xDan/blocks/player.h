
#ifndef __PLAYER_H
#define __PLAYER_H

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

class Player
{
public:
    Player(Info *, dWorldID, dSpaceID);
    ~Player();
    void Refresh();
    void setPosition(vector3df);
    vector3df getPosition();
    void setAngle(vector3df);
    dBodyID getBody() {return dBody;}
    
    void goForward();
    void goBackward();
    void goLeft();
    void goRight();
    void goUp();
    void goDown();
    void jump();
    bool onGround();
    
private:
    
    IAnimatedMeshSceneNode *node;
    IAnimatedMesh *mesh;
    vector3df yawAngle;
    f32 vParallel, vPerpendicular, moveVel;
    bool vUp, vDown;
    
    // ODE
    dBodyID dBody;
    dGeomID dGeom;
    Collision collision;
    
    dGeomID dGroundRayGeom;
    Collision groundRayCollision;
    
    // misc
    void QuaternionToEuler(const dQuaternion quaternion, vector3df &euler);
};

#endif
