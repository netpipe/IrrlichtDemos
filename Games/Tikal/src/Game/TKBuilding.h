#include <irrlicht.h>
#include <vector>
#include "TKHudInfo.h"

#ifndef __BUILDING__
#define __BUILDING__

class TKBuilding;

#include "TKNetwork.h"
#include "TKCore.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

class TKBuilding
{
public:
    TKBuilding(ISceneManager* mgr, stringc newtype,vector3df pos);
    vector3df getPosition();
    stringc getType();
    TKHudInfo* getInfo();
    void setPosition(vector3df pos);
    ITriangleSelector* getTriangleSelector();
    void setSelected(bool sel);
    void decreaseLife(TKNetwork* tkNetwork,TKCore* tkCore);
    void setSolid(bool solid);
    stringc getID();
    void setID(stringc nid);
    stringc getPlayer();
    void setPlayer(stringc nplayer);

private:
    ISceneManager* smgr;
    IMesh* mesh;
    IMesh* shadowMesh;
    IMeshSceneNode* node;

    IMesh* rubbleMesh;
    IMeshSceneNode* nodeRubble;

    ISceneNode* shadowNode;
    ISceneNode* selectedNode;
    stringc type;
    ITriangleSelector* selector;

    TKHudInfo* info;

    stringc id;
    stringc player;
};

#endif
