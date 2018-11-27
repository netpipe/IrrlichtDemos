#include <irrlicht.h>

#ifndef __TKCHARACTER__
#define __TKCHARACTER__

class TKCharacter;

#include "TKHudInfo.h"
#include "TKCore.h"
#include "TKNetwork.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class TKCharacter
{
public:
    TKCharacter(ISceneManager* mgr, TKCore* ntkCore, stringc type);
    ~TKCharacter();

    stringc getPlayer();
    vector3df getPosition();
    TKHudInfo* getInfo();
    bool isSelected();
    void moveTo(vector3df pos);
    void setNearestResource(vector3df resPos, stringc resType);
    void setNearestEnemyBuilding(vector3df buildPos);
    void setPlayer(stringc newPlayer);
    void setPosition(vector3df pos);
    void setRotation(double rotation);
    void setAnimation(stringc anim);
    void setSelected(bool selected);
    vector3df update(TKNetwork* tkNetwork);
    void addCollisionResponseAnimator(ITriangleSelector* newselector);
    void removeCollisions();
    ITriangleSelector* getTriangleSelector();
    void setID(stringc nid);
    stringc getID();

    void decreaseLife();

    void setDead();///TODO: remove TKCharacter::setDead(), não esta sendo usada mais!

private:
    ISceneManager* smgr;
    ISceneNode* node;
    TKCore* tkCore;

    IAnimatedMesh* villagerWaitMesh;
    IAnimatedMeshSceneNode* villagerWaitNode;
    IAnimatedMesh* villagerWalkMesh;
    IAnimatedMeshSceneNode* villagerWalkNode;
    IAnimatedMesh* villagerFoodMesh;
    IAnimatedMeshSceneNode* villagerWoodNode;
    IAnimatedMesh* villagerStoneMesh;
    IAnimatedMeshSceneNode* villagerStoneNode;
    IAnimatedMesh* villagerWoodMesh;
    IAnimatedMeshSceneNode* villagerFoodNode;

    IMesh* selectedCharacterMesh;
    IMeshSceneNode* selectedCharacterNode;

    IMesh* shadowCharacterMesh;
    IMeshSceneNode* shadowCharacterNode;

    bool selected;

    stringc currentAction;
    vector3df moveToPos;

    stringc player;//player dono do character

    f32 moveSpeed;

    float PI=3.1415926535;
    double grad2rad(double grad);

    ITriangleSelector* selector;

    void hideAllNodes();

    TKHudInfo* info;

    vector3df nearestResource;
    stringc   nearestResourceType;

    void lookAt(vector3df pos);

    stringc id;

    bool alive;

    vector3df nearestEnemyBuilding;
};

#endif
