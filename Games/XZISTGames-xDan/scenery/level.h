
#ifndef __LEVEL_H
#define __LEVEL_H

#include <TileMap.h>
#include <vector>
#include <event.h>
#include <irrInfo.h>
#include "collision.h"
#include <hovercraft.h>

using namespace tilemap;

class Level
{
public:
    void Start(irrInfo *info);
    void unPause();
    void Finish();
    void refresh(float delta_t);
    
private:
    int i;
    irrInfo *info;
    TileMap *tilemap;
    ICameraSceneNode *camera;
    ILightSceneNode *mainLight;
    ITexture *textures[3];
    
    // ODE
    dWorldID dWorld;
    dSpaceID dSpace;
    dJointGroupID dJointGroup;
    static void dNearCallback(void *data, dGeomID o1, dGeomID o2);
    
    std::vector <Entity*> entities;
    
    Entity *cameraEntity;
    
    void DestroyEntities();
    void addSky();
    PlayerEventReceiver playerEvent;
    f32 cameraDistance;
    f32 cameraDistanceDelta;
};

#endif
