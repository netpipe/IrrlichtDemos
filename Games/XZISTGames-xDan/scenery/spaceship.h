
#ifndef __SPACESHIP_H
#define __SPACESHIP_H

#include "entity.h"
#include "servo.h"
#include <vector>

class SpaceShip: public Entity
{
public:
    SpaceShip(irrInfo *);
    ~SpaceShip();
    
    // general functions
    void Refresh();
    void setPosition(dReal, dReal, dReal);
    dReal *getPosition();
    vector3df getRotation();
    const dReal *getRotationMatrix();
    
    // update function must position the irrlicht scene node using the ODE geom.
    // also may do other things like apply forces based on movement flags, particle effects...
    virtual void Update() = 0;
    
    // movement functions. used by both AI and player control.
    // sets movement flags.
    void goForwards();
    void goLeft();
    void goRight();
    
protected:
    
    // each spaceship will have ONE body and ONE geom.
    // they must initialise these themselves.
    dBodyID dBody;
    dGeomID dGeom;
    dMass mass;
    
    // each spaceship must tie this collision structure to its geom
    Collision collision;
    
    // each spaceship can have ONE node. It will add its own mesh to it.
    IAnimatedMeshSceneNode *irrNode;
    
    // movement flags are acted upon in an individual manner by all classes derived from this (within update() function)
    // could also be used by child classes to determine whether particle effects (engine smoke etc) are needed.
    bool goingForwards, goingLeft, goingRight;
    
    // is ship crashing or not? if so various effects could occur...
    bool crashing;
    
private:
    // clears movement flags. called at end of each refresh()
    // (after derived classes have acted upon the info in their update()
    void clearGoingFlags();
    
    // servo systems for use by AI
    std::vector <Servo> servos;
};

#endif
