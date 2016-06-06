#ifndef _ZONE_H
#define _ZONE_H

#include "globals.h"
#include "main.h"
#include "trigger.h"
#include "player.h"
#include "sound.h"

class CZone
{
    public:
    CZone(aabbox3d<f32> dim, int color, int type, int ID, ISceneManager* smgr, IVideoDriver* driver, CPlayer* Player, CSound* Sound);
    bool isPointInside(vector3df Point);
    bool isActive();
    bool isVisible();
    void setActive(bool Active);
    void setVisible(bool Visible);
    int getType();
    int getID();
    void ActivateTriggers();

    void remove();

    list<CTrigger* > Triggers;

    private:
    CPlayer* Player;
//    ISound* Sound_Object;
//    CSound* Sound;
    IVideoDriver* driver;
    ISceneManager* smgr;
    IMeshSceneNode* Node;
    aabbox3d<f32>* Dim;
    bool Sound_Playing;
    int type;
    int ID;
    bool Active;
    bool Visible;
};

#endif
