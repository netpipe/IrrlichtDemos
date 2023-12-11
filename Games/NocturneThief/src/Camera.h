#ifndef __CAMERA__
#define __CAMERA__

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class Camera
{
public:
    Camera(ISceneManager* mgr);
    void setPosition(vector3df pos);
    bool update(vector3df playerPos);
    vector3df getPosition();

private:
    ISceneManager* smgr;

    IMesh* mesh;
    IMeshSceneNode* node;

    IMesh* meshVision;
    IMeshSceneNode* nodeVision;

    f32 angle;
    bool turnRight;

    static const float rotateSpeed=0.5;

    ISceneNode* c1;
    ISceneNode* c2;
    ISceneNode* c3;
    ISceneNode* c2Pivot;
    ISceneNode* c3Pivot;

    vector3df p1;
    vector3df p2;
    vector3df p3;
};

#endif

