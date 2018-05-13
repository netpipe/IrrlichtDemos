#include "archer.h"
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

Archer::Archer(IrrlichtDevice* idevice)
{
    device=idevice;
    smgr=device->getSceneManager();

    archerMesh=smgr->getMesh("../media/archer.b3d");
    archer=smgr->addAnimatedMeshSceneNode(archerMesh);
    archer->setScale(vector3df(10,10,10));
    archer->setAnimationSpeed(20);
    archer->setMaterialFlag(EMF_FOG_ENABLE,true);
    archer->setRotation(vector3df(0,90,0));
    archer->setLoopMode(false);

    mainNode=archer->getJointNode("MAIN");
    archer->setJointMode(EJUOR_CONTROL);

    this->setDistanceFromTarget(200);

    this->setAnimationWait();
}

double Archer::getDistanceFromTarget()
{
    return -archer->getPosition().X;
}

vector3df Archer::getPosition()
{
    return archer->getPosition();
}

void Archer::animate()
{
    archer->animateJoints();
}

IAnimatedMeshSceneNode* Archer::getMesh()
{
    return archer;
}

void Archer::rotateMainNode(float angle)
{
    mainNode->setRotation(vector3df(angle,mainNode->getRotation().Y,mainNode->getRotation().Z));
}

void Archer::setAnimationAim(double angle)
{
    archer->setFrameLoop(2,2);
    mainNode->setRotation(vector3df(angle,mainNode->getRotation().Y,mainNode->getRotation().Z));
}

void Archer::setAnimationShoot()
{
    archer->setFrameLoop(2,30);
}

void Archer::setAnimationWait()
{
    archer->setFrameLoop(30,30);
}

void Archer::setDistanceFromTarget(double distance)
{
    archer->setPosition(vector3df(-distance,-30,2));
}


