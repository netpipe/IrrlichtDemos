#include "Arrow.h"

Arrow::Arrow(IrrlichtDevice* idevice)
{
    device=idevice;
    smgr=device->getSceneManager();

    arrowMesh=smgr->getMesh("../media/arrow.b3d");
    arrow=smgr->addMeshSceneNode(arrowMesh);
    arrow->setScale(vector3df(15,15,15));
    arrow->setRotation(vector3df(0,-90,0));
    arrow->setMaterialFlag(EMF_FOG_ENABLE,true);
}

void Arrow::hideArrow()
{
    arrow->setVisible(false);
}

vector3df Arrow::getPosition()
{
    return arrow->getPosition();
}

void Arrow::showArrow()
{
    arrow->setVisible(true);
}

vector3df Arrow::getRotation()
{
    return arrow->getRotation();
}

void Arrow::setRotation(vector3df rot)
{
    arrow->setRotation(rot);
}

void Arrow::setPosition(double X,double Y,double Z)
{
    arrow->setPosition(vector3df(X,Y,Z));
}

void Arrow::setPosition(vector3df pos)
{
    arrow->setPosition(pos);
}
