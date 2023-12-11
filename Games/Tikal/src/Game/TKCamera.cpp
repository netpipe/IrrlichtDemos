#include "TKCamera.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

TKCamera::TKCamera(ISceneManager* mgr)
{
    smgr=mgr;
    zoom=25;
    camera=smgr->addCameraSceneNode(0,vector3df(0,zoom,0),vector3df(0,0,10));
    camera->setFarValue(80);
}

TKCamera::~TKCamera()
{
    camera->remove();
}

int TKCamera::getCameraDirection(int axis)
{
    if(axis==0) return direction.X;
    if(axis==2) return direction.Z;
    return 0;
}

void TKCamera::setCameraDirection(int axis, f32 value)
{
    if(axis==0) direction.X=value;
    if(axis==2) direction.Z=value;
}

void TKCamera::setPosition(vector3df pos)
{
    camera->setPosition(vector3df(pos.X,camera->getPosition().Y,pos.Z));
    camera->setTarget(camera->getPosition() + vector3df(0,-zoom,10));
}

void TKCamera::setLimits(s32 left,s32 right,s32 up,s32 down)
{
    limitLeft=left;
    limitRight=right;
    limitUp=up;
    limitDown=down;
}

void TKCamera::update()
{
    if(camera->getPosition().X <= limitLeft && direction.X<0) direction.X=0;
    if(camera->getPosition().X >= limitRight && direction.X>0) direction.X=0;
    if(camera->getPosition().Z >= limitUp && direction.Z>0) direction.Z=0;
    if(camera->getPosition().Z <= limitDown && direction.Z<0) direction.Z=0;

    camera->setPosition(camera->getPosition() + direction);
    camera->setTarget(camera->getPosition() + vector3df(0,-zoom,10));
}
