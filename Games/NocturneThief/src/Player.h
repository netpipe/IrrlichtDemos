#ifndef __PLAYER__
#define __PLAYER__

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

enum ANIM
{
    PLAYER_ANIM_WALK, PLAYER_ANIM_WAIT, PLAYER_ANIM_WALK_GOLD, PLAYER_ANIM_WAIT_GOLD
};

enum PWALK
{
    PWALK_L,PWALK_R,PWALK_U,PWALK_D,PWAIT
};

class Player
{
public:
    Player(ISceneManager* mgr);

    void setAnimation(ANIM anim);
    ISceneNode* getNode();
    vector3df getPosition();
    void setPosition(vector3df pos);

    void walk(PWALK walkDir, bool ouroNoSaco);

private:
    ISceneManager* smgr;

    IAnimatedMesh* mesh;
    IAnimatedMeshSceneNode* node;

    IAnimatedMesh* meshOuro;
    IAnimatedMeshSceneNode* nodeOuro;

    IMesh* shadowMesh;
    IMeshSceneNode* shadowNode;

    PWALK currentAction;

    f32 walkSpeed;

    ICameraSceneNode* cam;
};

#endif
