#ifndef _NPCMANAGER_H
#define _NPCMANAGER_H

#include "globals.h"
#include "main.h"

#include "turret.h"
#include "player.h"
#include "physics.h"

class CNPCManager
{
public:
	CNPCManager(CPlayer* Player, CPhysics* Physics, ISceneManager *smgr, IVideoDriver *driver, IGUIEnvironment* gui, ITimer* Timer);
	CTurret* AddTurret(const stringw Name, float TurnRate, IAnimatedMeshSceneNode* Node, vector3df Muzzle1, vector3df Muzzle2);

	void Update();

	void remove();

private:
	ISceneManager *smgr;
	IVideoDriver *driver;
	IGUIEnvironment *gui;
	ITimer* Timer;
	CPlayer* Player;
	CPhysics* Physics;
	list<CTurret* > Turrets;

	void _UpdateAllCollisions(CTurret* Turret, CPlayer::SProjectiles* Projectile);
	void _UpdateAll(CTurret* Turret);
};

#endif
