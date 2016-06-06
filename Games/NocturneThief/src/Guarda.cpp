#include "Guarda.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __TRIANGLEP__
#define __TRIANGLEP__

float fAB(float px,float py,float x1,float y1,float x2,float y2,float x3,float y3)
{
    return ((py-y1)*
        (x2-x1) -
        (px-x1)*
        (y2-y1));
}

float fBC(float px,float py,float x1,float y1,float x2,float y2,float x3,float y3)
{
    return ((py-y2)*
        (x3-x2) -
        (px-x2)*
        (y3-y2));
}

float fCA(float px,float py,float x1,float y1,float x2,float y2,float x3,float y3)
{
    return ((py-y3)*
        (x1-x3) -
        (px-x3)*
        (y1-y3));
}

bool isInside(float px,float py,float x1,float y1,float x2,float y2,float x3,float y3)
{
    if (fAB(px,py,x1,y1,x2,y2,x3,y3)*fBC(px,py,x1,y1,x2,y2,x3,y3)>0 && fBC(px,py,x1,y1,x2,y2,x3,y3)*fCA(px,py,x1,y1,x2,y2,x3,y3)>0)
        return true;
    else
        return false;
}

#endif

Guarda::Guarda(ISceneManager* mgr)
{
    smgr=mgr;

    mesh=smgr->getMesh("../media/guarda.b3d");
    node=smgr->addAnimatedMeshSceneNode(mesh);

    node->setAnimationSpeed(30);
    node->setScale(vector3df(1.5,1.5,1.5));

    node->setMaterialFlag(EMF_NORMALIZE_NORMALS,true);

    meshVision=smgr->getMesh("../media/visaoNPC.b3d");
    nodeVision=smgr->addMeshSceneNode(meshVision,node);
    nodeVision->setPosition(vector3df(0,0.01,0));
    nodeVision->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    nodeVision->setMaterialFlag(EMF_LIGHTING,false);
    nodeVision->setScale(vector3df(1.33,1.33,1.33));

    this->setAnimation(GUARDA_WALK);

    shadowMesh=smgr->getMesh("../media/sombra.b3d");
    shadowNode=smgr->addMeshSceneNode(shadowMesh,node);
    shadowNode->setPosition(vector3df(0,0.02,0));
    shadowNode->setScale(vector3df(0.5,0.5,0.7));
    shadowNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

    c2Pivot=smgr->addEmptySceneNode();
    c3Pivot=smgr->addEmptySceneNode();
    c1=smgr->addEmptySceneNode(0);
    c2=smgr->addEmptySceneNode(c2Pivot);
    c3=smgr->addEmptySceneNode(c3Pivot);
    c2->setPosition(vector3df(0,0,-4.3));
    c3->setPosition(vector3df(0,0,-4.3));
}

int Guarda::getNearestWaypoint(vector3df pos)
{
    float menorDistancia=32000;
    int waypoint=-1;

    for(int i=0;i<graphWaypoints->getNodes().size();i++)
    {
        if( graphWaypoints->getNodeByID(i)->getPosition().getDistanceFrom(pos) < menorDistancia )
        {
            waypoint=i;
            menorDistancia=graphWaypoints->getNodeByID(i)->getPosition().getDistanceFrom(pos);
        }
    }

    return waypoint;
}

double Guarda::grad2rad(double grad)
{
   return (grad*PI/180);
}

void Guarda::movePath(vector<vector3df> newpath)
{
    path.clear();
    path=newpath;
    currentAction = GUARDA_WALK;
}

void Guarda::setAnimation(GUARDA_ANIM anim)
{
    if(anim==GUARDA_WALK)
    {
        node->setAnimationSpeed(20);
        node->setFrameLoop(10,30);
    }
    if(anim==GUARDA_LOOK)
    {
        node->setAnimationSpeed(15);
        node->setFrameLoop(70,110);
    }
}

void Guarda::setPosition(vector3df pos)
{
    node->setPosition(pos);
}

void Guarda::setWaypoints(Graph* ngraphWaypoints)
{
    graphWaypoints=ngraphWaypoints;
    aStar = new AStar(graphWaypoints);
}

void Guarda::alarm(vector3df alarmPos)
{
    path.clear();

    int w1=getNearestWaypoint(node->getPosition());

    int w2=getNearestWaypoint(alarmPos);

    if(w1 == 0) w1=1;
    if(w2 == 0) w2=1;

    vector<Node*> realPath =  aStar->AStarAlgorithm(graphWaypoints->getNodeByID(w1),graphWaypoints->getNodeByID(w2));

    for(int i=realPath.size()-1;i>=0;i--)
    {
        path.push_back( graphWaypoints->getNodeByID(realPath[i]->getId() )->getPosition() );
    }

    currentAction = GUARDA_WALK;
}

bool Guarda::update(vector3df pos)
{
    c1->setPosition(node->getPosition());
    c2Pivot->setPosition(node->getPosition());
    c3Pivot->setPosition(node->getPosition());
    c2Pivot->setRotation(node->getRotation() - vector3df(0,25,0));
    c3Pivot->setRotation(node->getRotation() + vector3df(0,25,0));

    p1=c1->getAbsolutePosition();
    p2=c2->getAbsolutePosition();
    p3=c3->getAbsolutePosition();

    //printf("PX%f PZ:%f  X:%f Z:%f X:%f Z:%f X:%f Z:%f\n",pos.X,pos.Z,p1.X,p1.Z,p2.X,p2.Z,p3.X,p3.Z);

    if(isInside(pos.X,pos.Z,p1.X,p1.Z,p2.X,p2.Z,p3.X,p3.Z))
    {
        return true;
    }



    if(path.size() == 0)
    {
        int w1=getNearestWaypoint(node->getPosition());

        int w2=rand()%graphWaypoints->getNodes().size(); //getNearestWaypoint(vector3df(rand()%32, 0, rand()%32));///TODO: randomizar ate o total de nos

        if(w1 == 0) w1=1;
        if(w2 == 0) w2=1;

        vector<Node*> realPath =  aStar->AStarAlgorithm(graphWaypoints->getNodeByID(w1),graphWaypoints->getNodeByID(w2));

        for(int i=realPath.size()-1;i>=0;i--)
        {
            path.push_back( graphWaypoints->getNodeByID(realPath[i]->getId() )->getPosition() );
        }

        currentAction = GUARDA_WALK;
    }

    if(path.size()>0)
    {
        moveToPos=path[0];
    }

    if(currentAction == GUARDA_WALK)
    {
        vector3df offsetVector = moveToPos - node->getPosition();

        vector3df rot = (-offsetVector).getHorizontalAngle();

        rot.X=0;
        rot.Z=0;

        node->setRotation(rot);

        vector3df pos=node->getPosition();
        pos.Z -= cos(grad2rad(node->getRotation().Y))*0.04;
        pos.X -= sin(grad2rad(node->getRotation().Y))*0.04;

        if(path.size()>0 && node->getPosition().getDistanceFrom(path[0]) <= 1)
        {
            path.erase(path.begin());
        }

        node->setPosition(pos);
    }

    return false;
}
