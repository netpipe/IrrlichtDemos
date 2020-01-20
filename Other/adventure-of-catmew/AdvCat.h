#include <string>
#include <sstream>
#include "Game.h"
#include "Player.h"
#include "Cloud.h"
#include "Shooter.h"
#include "Level.h"
#include "Collision.h"
#include "Enemies.h"

class AdvCat
{
	public:
		AdvCat();
		~AdvCat();
		void update();
		Game* core;

	private:
		Player* catMewMew;
		CloudManager* cloudManage;
		LevelManager* levelManage;
		MewMewShooter* mewShooter;
		Enemies* enemies;
		gfx::ISprite* sky;
		IText* tFPS;
		CollisionManager* collisionManage;
};
