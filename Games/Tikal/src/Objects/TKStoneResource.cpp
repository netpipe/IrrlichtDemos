#include <irrlicht.h>
#include "TKStoneResource.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

TKStoneResource::TKStoneResource(ISceneManager* mgr, vector3df pos)
{
    smgr=mgr;
    stoneMesh=smgr->getMesh("../media/Recursos/minaPedra.b3d");
    stoneNode=smgr->addMeshSceneNode(stoneMesh);
    stoneNode->setRotation(vector3df(0,rand()%360,0));
    stoneNode->setPosition(pos);
    stoneNode->setScale(vector3df(3,3,3));
}

void TKStoneResource::decreaseLife()
{
    resourceLife-=1;
}

s32 TKStoneResource::getLife()
{
    return resourceLife;
}

void TKStoneResource::setPosition(vector3df pos)
{
    stoneNode->setPosition(pos);
}

vector3df TKStoneResource::getPosition()
{
    return stoneNode->getPosition();
}
