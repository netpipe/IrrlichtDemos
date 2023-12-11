#ifndef _FLOWMANAGER_H
#define _FLOWMANAGER_H

#include "globals.h"
#include "main.h"
#include "zone.h"
#include "player.h"
#include "map.h"
#include "sound.h"

class CFlowManager
{
    public:
    CFlowManager(IVideoDriver* driver, ISceneManager* smgr, CPlayer* Player, CMap* Map, CSound* Sound);
    void AddZone(aabbox3d<f32> Dim, int Color, int type, int ID);
    void CheckZones();
    void LoadMapZones();
    void LoadMapTriggers();

    void remove();

    private:
    CMap* Map;
    IVideoDriver* driver;
    ISceneManager* smgr;
    CPlayer* Player;
    CSound* Sound;
    list<CZone *> Zones;

};

#endif
