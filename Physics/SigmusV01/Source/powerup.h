#ifndef _POWERUP_H
#define _POWERUP_H

#include "globals.h"
#include "main.h"
#include "player.h"

class CPowerup
{
    public:
    CPowerup(int Type, vector3df Position, int Value, ISceneManager* smgr, IVideoDriver* driver, CPlayer* Player);
    void Update();
    void PickedUp();
    IAnimatedMeshSceneNode* getNode();

    private:
    CPlayer* Player;
    ISceneManager* smgr;
    IVideoDriver* driver;
    int Type;
    vector3df Position;
    int Value;
    IAnimatedMeshSceneNode* Node;
};

#endif
