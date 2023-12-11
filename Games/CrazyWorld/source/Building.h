#pragma once
#include "GameObject.h"
#include "Projectile.h"
#include "Tile.h"
#include "Effects.h"

class Building:
	public GameObject
{
public:
	Building(IrrlichtDevice* device, std::string mod, std::string owner, Tile* tile, int multiplier);
	~Building(void);
	void TickAI();
	void CheckCollide(GameObject *);
	
	double rocps; //rate of creating per seconds
	double multi;
	bool createFlag;
	Tile* parent;
	Effects* eft;
};
