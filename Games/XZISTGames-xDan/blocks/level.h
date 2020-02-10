
#ifndef __LEVEL_H
#define __LEVEL_H

#include <irrlicht.h>
#include "info.h"
#include "events.h"
#include <ode/ode.h>
#include "odeMesh.h"
#include "block.h"
#include "player.h"
#include "magnet.h"
#include "collision.h"

class Level
{
public:
    void Refresh(f32);
    void Start(Info *info, int id);
    void Finish();
    int id;
private:
    int i;
    Info *info;
    PlayerEventReceiver playerEvent;    
    static void dNearCallback(void *data, dGeomID o1, dGeomID o2);
    dWorldID dWorld;
    dSpaceID dSpace;
    dJointGroupID dJointGroup;
    ICameraSceneNode *camera;
    
    // level edit
    Block *selectedBlock;
    Collision selectRayCollision;
    dGeomID dSelectRayGeom;
    array <Block *> selectedBlocks;
    array <dReal> selectedDepths;
    string <c8> levelString;
    void load();
    void save();
    
    // the player!!!!!!
    Player *player;
    bool jumped;
    
    enum {
        // configurable???
        KEY_FORWARDS = KEY_KEY_W,
        KEY_BACKWARDS = KEY_KEY_S,
        KEY_LEFT = KEY_KEY_A,
        KEY_RIGHT = KEY_KEY_D,
        KEY_UP = KEY_KEY_R,
        KEY_DOWN = KEY_KEY_F,
        KEY_JUMP = KEY_SPACE,
        KEY_MAGNETMODE = KEY_KEY_E,
        KEY_MAGRELEASE = KEY_KEY_G,
        
        // editor
        KEY_SAVE = KEY_F6,
        KEY_LOAD = KEY_F9
        };
    
    Magnet *mag;
    
    // blocks
    array <Block *> blocks;
    // static meshes
    array <odeMesh *> meshes;
};

#endif
