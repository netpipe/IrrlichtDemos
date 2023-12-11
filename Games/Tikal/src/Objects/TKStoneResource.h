#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __TKSTONERESOURCE__
#define __TKSTONERESOURCE__

class TKStoneResource
{
public:
    TKStoneResource(ISceneManager* mgr, vector3df pos);
    void decreaseLife();
    s32 getLife();
    void setPosition(vector3df pos);
    vector3df getPosition();

private:
    ISceneManager* smgr;
    s32 resourceLife;//quanto ainda resta...
    IMesh* stoneMesh;
    IMeshSceneNode* stoneNode;
};

#endif
