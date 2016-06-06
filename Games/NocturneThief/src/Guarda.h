#ifndef __GUARDA__
#define __GUARDA__

#include <irrlicht.h>
#include "aStar/AStar.h"
#include "aStar/Graph.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

enum GUARDA_ANIM
{
    GUARDA_WALK, GUARDA_LOOK
};

class Guarda
{
public:
    Guarda(ISceneManager* mgr);

    void setAnimation(GUARDA_ANIM anim);
    void setPosition(vector3df pos);

    bool update(vector3df playerPos);

    void setWaypoints(Graph* ngraphWaypoints);

    void alarm(vector3df alarmPos);

private:
    ISceneManager* smgr;

    IAnimatedMesh* mesh;
    IAnimatedMeshSceneNode* node;

    IMesh* meshVision;
    IMeshSceneNode* nodeVision;

    IMesh* shadowMesh;
    IMeshSceneNode* shadowNode;

    GUARDA_ANIM currentAction;
    vector3df moveToPos;

    vector<vector3df> path;

    Graph* graphWaypoints;

    AStar* aStar;

    void movePath(vector<vector3df> newpath);
    int getNearestWaypoint(vector3df pos);
    double grad2rad(double grad);

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

