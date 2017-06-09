#ifndef _PLAYER_H
#define _PLAYER_H

#include "irrlicht.h"
#include "ship.h"
#include "keylistener.h"
#include "turret.h"
#include "projectile.h"

#include "vector"

using namespace irr;
using namespace scene;


class Player
{
public:
	int last_time;

	int hullPoints;
	int armorPoints;
	int shieldPoints;

	int maxHullPoints;
	int maxArmorPoints;
	int maxShieldPoints;

	int maxVelocity;
	float velocity;
	float maxTurn;

	irr::scene::IAnimatedMeshSceneNode *ship;

	irr::IrrlichtDevice *graphics;
//	irrklang::ISoundEngine *sound;

	core::vector3df pRot;
	core::vector3df pPos;
	core::vector3df rotSlow;
	core::vector3df posSlow;

	scene::IBoneSceneNode *turret_node[4];


	scene::IBoneSceneNode *exhaust_01;
	scene::IBoneSceneNode *exhaust_02;
	scene::IBoneSceneNode *exhaust_03;

	std::vector<turret*> turret_manager;


	struct Ccrew
	{
		int health;
		int members;
		int moral;
	};
	struct turningCircle
	{
		float X;
		float Y;
		float Z;
	};

	struct cannonStats
	{
		int primary_time;
		bool primary;
		int secondary_time;
		bool secondary;
		int flak_time;
	};
	struct Ccrew crew;
	struct turningCircle turn;
	struct cannonStats cannonFired;

//	irrklang::ISound *engineSound;

	Player(irr::IrrlichtDevice *graphics, KeyListener *receiver, const core::vector3df& ship_position);
	virtual ~Player();
	void playerRun(f32 frameDeltaTime);
	void manageTurrets(CShip* player_target, f32 frameDeltaTime);
	void shoot();
	void damagePlayer(int damage);



	core::vector3df getRot() const;
	core::vector3df getPos() const;
	void setPos(const core::vector3df& pPos);
	void setRot(const core::vector3df& pRot);
	void setVelocity(float& velocity);
	float getVelocity() const;
	int getMaxSpeed();
	float getMaxTurn();
	int getHull() const;
	int getArmor() const;
	int getShield() const;

};




#endif
