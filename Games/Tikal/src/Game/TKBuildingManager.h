#include <irrlicht.h>
#include <vector>
#include "TKBuilding.h"
#include <iostream>

#ifndef __TKBUILDINGMANAGER__
#define __TKBUILDINGMANAGER__

class TKBuildingManager;

#include "TKCore.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

class TKBuildingManager
{
public:
    TKBuildingManager(ISceneManager* mgr, stringc nplayer, TKCore* ntkCore);
    void setTKNetwork(TKNetwork* ntkNetwork);
    TKBuilding* createMilitaryCamp(vector3df pos);
    TKBuilding* createHouse(vector3df pos);
    TKBuilding* createTemple(vector3df pos);
    vector<ITriangleSelector*> getTriangleSelectorVector();
    TKBuilding* selectBuilding(vector3df mouseClickPos);
    void setSelected(TKBuilding* build);
    vector<vector3df> getEnemyBuildingsPos();
    vector<stringc> getEnemyBuildingsIDs();
    void decreaseLife(stringc id);

private:
    ISceneManager* smgr;
    vector<TKBuilding*> buildingsAlive;
    vector<ITriangleSelector*> triangleSelectors;

    stringc player;

    TKNetwork* tkNetwork;
    TKCore* tkCore;
};

#endif
