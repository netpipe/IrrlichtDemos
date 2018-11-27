#ifndef __TKCHARACTERMANAGER__
#define __TKCHARACTERMANAGER__

class TKCharacterManager;

#include <irrlicht.h>
#include "TKCharacter.h"
#include <vector>
#include "TKCore.h"
#include "TKNetwork.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

class TKCharacterManager
{
public:
    TKCharacterManager(ISceneManager* mgr, TKCore* ntkCore, stringc nplayer);
    ~TKCharacterManager();

    TKCharacter* createCharacter(stringc type,vector3df pos,stringc player);
    int getTotalCharactersSelected();
    ITexture* getMiniMapTexture();
    void moveSelectedTo(vector3df pos);
    void moveByIDTo(stringc id,vector3df pos,double rotation,stringc action);
    void setActionByID(stringc id, double rotation, stringc action);
    TKCharacter* selectCharacters(vector3df mouseClickPos);
    TKCharacter* selectCharacters(vector3df p1, vector3df p2);
    void selectNone();
    void updateAll(TKNetwork* tkNetwork,TKBuildingManager* tkBuildingManager);
    void updateCollisions(vector<ITriangleSelector*> selectors);
    void updateCollisions(ITriangleSelector* selector);
    void setResources(vector<vector3df> pos, vector<stringc> resType);

    void decreaseLife(stringc id);

    void bufferCharacterToKill(stringc id);
    void killCharactersBuffer();
    void killCharacter(int i);
    void killCharacter(stringc id);

    void setNearestEnemyBuilding(vector<vector3df> pos,vector<stringc> ids);

private:
    ISceneManager* smgr;
    TKCore* tkCore;

    vector<TKCharacter*> charactersAlive;
    vector<TKCharacter*> charactersSelected;

    ITexture* miniMapTexture;

    vector<ITriangleSelector*> selectorsOnMap;

    void updateMiniMapTexture();

    vector< stringc > resources;
    vector< vector3df > resoucesPos;

    stringc player;

    bool killingCharacter;

    vector<stringc> charactersToKill;

    vector<stringc> buildIDs;
    vector<vector3df> buildsPos;
};

#endif
