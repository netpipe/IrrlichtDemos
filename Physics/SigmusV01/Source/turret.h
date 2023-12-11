#ifndef _TURRET_H
#define _TURRET_H

#include "globals.h"
#include "main.h"
#include "player.h"
#include "physics.h"

class CTurret
{
public:

	CTurret(const stringw Name, float TurnRate, IAnimatedMeshSceneNode* Node, ISceneManager* smgr, IVideoDriver* driver, IGUIEnvironment* gui, ITimer* Timer, CPlayer* Player, CPhysics* Physics, vector3df Muzzle1, vector3df Muzzle2);

	bool isActive();
	bool checkShotLast();
	int getTurnRate();
	bool isDebugVisual();
	vector3df getPosition();
	ISceneNode* getNode();
	line3d<f32> getDebugRay();
	void setProjectileSize(float Size);
	void setProjectileMass(float Mass);
	int getHealth();

	void setActive(bool active);
	void setTurnRate(int TurnRate);
	void setDebugVisual(bool active);
	void setPosition(vector3df Position);
	void UpdateTurret();
	void setHealth(int Health);
	void UpdateVisuals();
	void setHUDColors(SColor Active, SColor Inactive);
	void setDebugColor(SColor Color);
	void setShotTexture(ITexture* Texture);
	void setHUDActive(bool Active);
	void setOverrideText(stringw Text);
	void setOverrideTextActive(bool Active);
	void setShotRate(u32 Milliseconds);
	void setDamage(int Damage);
	void setShotSpeed(int Speed);

    void remove();

private:
    CPhysics* Physics;
    CPlayer* Player;
	ISceneNode* Node;
	vector3df Position;
	stringw Name;
	float TurnRate;
	bool HUDEnabled;
	bool Active;
	bool DrawDebug;
	IGUIEnvironment* gui;
	ISceneManager* smgr;
	IVideoDriver* driver;
	line3d<f32> ray;
	int isHit;
	bool shotLast;
	float shotSize;
	float shotMass;
	int health;
	recti HUDSquare;
	IGUIStaticText* HUDIsActive;
	SColor HUDActiveColor;
	SColor HUDInactiveColor;
	SColor DebugColor;
	ITexture* shotTexture;
	bool useOverrideText;
	stringw OverrideText;
	ITimer* Timer;
	int Damage;
	IMeshSceneNode* Muzzle1;
	IMeshSceneNode* Muzzle2;
	IMeshSceneNode* Back;
	int shotSpeed;
	btRigidBody* TurretPhysics;

	u32 shotTimeLast;
	u32 shotTimeNow;
	u32 shotRate; //In milliseconds between shots

	struct SProjectiles
	{
		ISceneNode* Node;
		btRigidBody* rigidBody;
		vector3df oldPos;
		vector3df newPos;
		float Tick;
	};

	list<SProjectiles *> Projectiles;

	bool CheckProjectiles(CTurret::SProjectiles* Projectil);
	void RemoveProjectile(CTurret::SProjectiles* Projectiles);
};

#endif
