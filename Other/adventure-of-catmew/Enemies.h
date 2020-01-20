#ifndef ENEMY_H
#define ENEMY_H

#include "Configuration.h"
#include "Game.h"

typedef struct
{
	std::string Name;
	std::vector<ITexture*> Textures;
	int defHitPoints;
	char iden;
} EnemyType;

typedef struct
{
	EnemyType* type;
	ISprite* sprite;
	signed int HitPoints;
	int currAnim;
	bool dead;
	bool dieing;
	int dieProg;
	float posCycle;
} Enemy;

class Enemies
{
public:
	Enemies(Game* pCore);
	void update();
	std::vector<Enemy*> EnemyArray;
	std::vector<EnemyType> EnemyTypes;
private:
	Game* core;
	conf::Configuration* configMan;
	int animtime;
};

#endif
