#ifndef _CORE_H
#define _CORE_H

#include "globals.h"
#include "main.h"
#include "map.h"
#include "player.h"
#include "physics.h"
#include "HUD.h"
#include "powerup.h"
#include "effectsmanager.h"
#include "NPCmanager.h"
#include "flowmanager.h"
#include "sound.h"
#include "GUI.h"

class CCore
{
    public:
    bool Init(std::string Settings_Filename);
    CMap* LoadMap(core::stringc filename);
    CPlayer* CreatePlayer();
    CPhysics* CreatePhysicsWorld(btVector3 Gravity);
    CHUD* CreateHUD();
    CPowerup* CreatePowerup(int Type, vector3df Position, int Value);
    CEffectsManager* CreateEffectsManager();
    CNPCManager* CreateNPCManager();
    CFlowManager* CreateFlowManager();
    CSound* CreateSoundEngine();
    CGUI* CreateGUI();
    void Update();

    void Clean();

    bool isMap();
    bool isPlayer();
    bool isWeapons();

    IrrlichtDevice* getDevice();
    IVideoDriver* getDriver();
    ISceneManager* getSMGR();
    IGUIEnvironment* getGUI();
    CEventReceiver* getReceiver();
    ITimer* getTimer();

    private:
    float PlayerMoveSpeed;
    float PlayerJumpSpeed;
    float PlayerRotSpeed;
    list<CPowerup *> Powerups;
    CGUI* GUI;
    CHUD* HUD;
    CMap* Map;
    CSound* Sound;
    CPlayer* Player;
    CEffectsManager* EffectsManager;
    CFlowManager* FlowManager;
    CNPCManager* NPCManager;
    CPhysics* Physics;
    bool MapExists;
    bool PlayerExists;
    bool WeaponsLoaded;
    IrrlichtDevice* device;
    IVideoDriver* driver;
    ISceneManager* smgr;
    IGUIEnvironment* gui;
    ITimer* timer;

    CEventReceiver* receiver;
};

#endif
