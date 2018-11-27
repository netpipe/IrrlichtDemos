#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

#ifndef __TKCAMERA__
#define __TKCAMERA__

class TKCamera
{
public:
    TKCamera(ISceneManager* mgr);
    ~TKCamera();

    int getCameraDirection(int axis);
    void setCameraDirection(int axis, f32 value);
    void setPosition(vector3df pos);
    void setLimits(s32 left,s32 right,s32 up,s32 down);
    void update();

private:
    ISceneManager* smgr;
    ICameraSceneNode* camera;
    vector3df direction;
    f32 zoom;

    s32 limitLeft;
    s32 limitRight;
    s32 limitUp;
    s32 limitDown;
};

#endif
