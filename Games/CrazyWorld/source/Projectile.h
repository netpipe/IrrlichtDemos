#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile(IrrlichtDevice* device, std::string mod, std::string, core::vector3df dir,core::vector3df pos,double spd,irr::u32 age,irr::u32 dmg);
	~Projectile(void);
	void TickAI();
	void CheckCollide(GameObject *);

	irr::u32 max_age; // how long the projectile can live
	irr::u32 dmg;
};
