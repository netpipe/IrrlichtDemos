#ifndef H_LEVEL
#define H_LEVEL

#include <string>
#include <sstream>
#include <stdio.h>
#include <vector>
#include "Game.h"
#include "Enemies.h"

//#define USE_CRACKS

enum E_TILE_TYPE
{
ETT_EMPTY = 0, // Empty space, not a tile
ETT_UNDEF, // Is a tile, but undefined
ETT_WATER, // A water tile

ETT_COL, // Anything greater than this is subject to collision
ETT_MIDDLE, // A tile in the middle 
ETT_TOPLEFT, // A tile in the top left
ETT_TOPRIGHT, // A tile in the top right
ETT_TOP, // A tile in the top but not on the edge

ETT_COUNT // Tile enum count

// Any number greater than ETT_COUNT means its an enemy not a tile

};


typedef struct
{
ISprite* sprite;
Enemy* enemy;
bool isEnemy;
int type; // Sacrificing type-safety for the sake of the enemy implementation here
} Tile;

#include <iostream>
class LevelManager
{
public:
LevelManager(Game* pCore, Enemies* enem);
bool loadLevel(std::string levelName);
std::vector<Tile> getTiles()
{
	return Tiles;
};

Tile getTile(int num)
{
	return Tiles[num];
};

int getTileNumber()
{
	return Tiles.size();
};

Color SkyCol1;
Color SkyCol2;
private:
Game* core;
Enemies* enemies;
int SizeX;
int SizeY;
std::vector<Tile> Tiles;
ITexture* texTop;
ITexture* texTopLeft;
ITexture* texTopRight;
ITexture* texMiddle;
#ifdef USE_CRACKS
ITexture* texCrack;
#endif
};

#endif
