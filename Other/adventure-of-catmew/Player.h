#ifndef H_PLAYER
#define H_PLAYER

#include "Game.h"
#include <string.h>
#include <sstream>

class Player
{
public:
	Player(Game* pCore);
	void update();
	gfx::ISprite* getCatDude()
	{
		return catDude;
	};
	
	void die()
	{
		velocity = Vector(0,0);
		//catDude->setPosition(LastGroundPlace);
		catDude->setPosition(Vector(400,400));
		core->getRoot()->getSceneManager()->setWorldPosition(Vector(0,0));
	};
	
	Vector velocity;
	bool jumpSwitch;
	char wallJumpState;
	Vector LastGroundPlace;
private:
	Game* core;
	gfx::ISprite* catDude;
	gfx::ITexture* catstand;
	gfx::ITexture* catwalkL[2];
	gfx::ITexture* catwalkR[2];
	Vector WorldPos;
	bool animbool;
	int animtime;
};

#endif
