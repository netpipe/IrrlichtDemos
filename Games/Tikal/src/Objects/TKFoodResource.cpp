#include <irrlicht.h>
#include "TKFoodResource.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

TKFoodResource::TKFoodResource(ISceneManager* mgr, vector3df pos)
{
    smgr=mgr;
    foodMesh=smgr->getMesh("../media/Recursos/foodResource.b3d");
    foodNode=smgr->addMeshSceneNode(foodMesh);
    foodNode->setRotation(vector3df(0,rand()%360,0));
    foodNode->setPosition(pos);
    foodNode->setScale(vector3df(2,2,2));
    foodNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
}

void TKFoodResource::decreaseLife()
{
    resourceLife-=1;
}

s32 TKFoodResource::getLife()
{
    return resourceLife;
}

void TKFoodResource::setPosition(vector3df pos)
{
    foodNode->setPosition(pos);
}

vector3df TKFoodResource::getPosition()
{
    return foodNode->getPosition();
}
