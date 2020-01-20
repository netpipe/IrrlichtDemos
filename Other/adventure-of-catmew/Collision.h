#include "Player.h"
#include "Level.h"

class CollisionManager
{
public:
	CollisionManager(Game* pCore, LevelManager* levelMan, Player* catguy);

	bool update();
private:
	Game* core;
	Player* catMewMew;
	LevelManager* levelManage;
};




