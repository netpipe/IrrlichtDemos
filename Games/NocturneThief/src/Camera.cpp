#include "Camera.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __TRIANGLEP__
#define __TRIANGLEP__

float fAB2(float px,float py,float x1,float y1,float x2,float y2,float x3,float y3)
{
    return ((py-y1)*
        (x2-x1) -
        (px-x1)*
        (y2-y1));
}

float fBC2(float px,float py,float x1,float y1,float x2,float y2,float x3,float y3)
{
    return ((py-y2)*
        (x3-x2) -
        (px-x2)*
        (y3-y2));
}

float fCA2(float px,float py,float x1,float y1,float x2,float y2,float x3,float y3)
{
    return ((py-y3)*
        (x1-x3) -
        (px-x3)*
        (y1-y3));
}

bool isInside2(float px,float py,float x1,float y1,float x2,float y2,float x3,float y3)
{
    if (fAB2(px,py,x1,y1,x2,y2,x3,y3)*fBC2(px,py,x1,y1,x2,y2,x3,y3)>0 && fBC2(px,py,x1,y1,x2,y2,x3,y3)*fCA2(px,py,x1,y1,x2,y2,x3,y3)>0)
        return true;
    else
        return false;
}

#endif

Camera::Camera(ISceneManager* mgr)
{
    smgr=mgr;

    mesh=smgr->getMesh("../media/camera.b3d");
    node=smgr->addMeshSceneNode(mesh);

    meshVision=smgr->getMesh("../media/visaoNPC.b3d");
    nodeVision=smgr->addMeshSceneNode(meshVision,node);
    nodeVision->setPosition(vector3df(0,0.01,0));
    nodeVision->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    nodeVision->setMaterialFlag(EMF_LIGHTING,false);

    angle=rand()%360;
    turnRight=true;

    node->setScale(vector3df(2,2,2));

    c2Pivot=smgr->addEmptySceneNode();
    c3Pivot=smgr->addEmptySceneNode();
    c1=smgr->addEmptySceneNode(0);
    c2=smgr->addEmptySceneNode(c2Pivot);
    c3=smgr->addEmptySceneNode(c3Pivot);
    c2->setPosition(vector3df(0,0,-4.3));
    c3->setPosition(vector3df(0,0,-4.3));
}

void Camera::setPosition(vector3df pos)
{
    node->setPosition(pos);
}

bool Camera::update(vector3df playerPos)
{
    c1->setPosition(node->getPosition());
    c2Pivot->setPosition(node->getPosition());
    c3Pivot->setPosition(node->getPosition());
    c2Pivot->setRotation(node->getRotation() - vector3df(0,25,0));
    c3Pivot->setRotation(node->getRotation() + vector3df(0,25,0));

    p1=c1->getAbsolutePosition();
    p2=c2->getAbsolutePosition();
    p3=c3->getAbsolutePosition();

    if(isInside2(playerPos.X,playerPos.Z,p1.X,p1.Z,p2.X,p2.Z,p3.X,p3.Z))
    {
        return true;
    }

    if(turnRight)
    {
        angle+=rotateSpeed;
    }
    else
    {
        angle-=rotateSpeed;
    }

    node->setRotation(vector3df(0,angle,0));

    //if(angle>45) turnRight=false;
    //if(angle<-45) turnRight=true;
    if(angle>360) angle = 0;

    return false;
}

vector3df Camera::getPosition()
{
    return node->getAbsolutePosition();
}
