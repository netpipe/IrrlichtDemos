#pragma once
#include <irrlicht.h>
#include "AI.h"
#include "Projectile.h"

class Player
{
public:
	Player(void);
	Player(IrrlichtDevice* device, AI* ai,  std::string owner, core::vector3df pos, core::vector3df dir, int dmg);
	~Player(void);

	//void UpdateSelf(void);
	void Attack();
	void TickAI();
	void Update();
	void SetCamera(scene::ICameraSceneNode* camera){this->camera = camera;}
	IrrlichtDevice* device;
	scene::ISceneManager* smgr;
	video::IVideoDriver* driver;
	scene::ICameraSceneNode* camera;

	double rofps; //rate of fire per seconds
	Projectile* pr;
	AI* ai;
	irr::u32 lastFrame;
	irr::f32 timePassed;
	vector3df positionVec;
	vector3df lastPositionVec;
	vector3df directionVec;
	bool isFiring;

private:
	int dmg;
	int dmg_multiplier;
	int score;
	std::string o;
};
