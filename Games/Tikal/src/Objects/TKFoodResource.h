#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __TKFOODRESOURCE__
#define __TKFOODRESOURCE__

class TKFoodResource
{
public:
    TKFoodResource(ISceneManager* mgr, vector3df pos);
    void decreaseLife();
    s32 getLife();
    void setPosition(vector3df pos);
    vector3df getPosition();

private:
    ISceneManager* smgr;
    s32 resourceLife;//quanto ainda resta...
    IMesh* foodMesh;
    IMeshSceneNode* foodNode;
};

#endif
