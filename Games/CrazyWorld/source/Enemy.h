#pragma once
#include "GameObject.h"
#include "Projectile.h"
#include "Waypoint.h"
#include "Effects.h"

class Enemy :
	public GameObject
{
public:
	Enemy(void);
	Enemy(IrrlichtDevice*, std::string, std::string, core::vector3df, core::vector3df, double,Waypoint*);
	~Enemy(void);

	void TickAI();
	void CheckCollide(GameObject *);
	void SpillBlood();
	void Pain();

	int detectRad; // detection radius
	double rofps; //rate of fire per seconds
	bool isDead;
	scene::IBillboardSceneNode* ibb;
	core::array<video::ITexture*> texture;
	Waypoint *dest;
	Effects* eft;
};
