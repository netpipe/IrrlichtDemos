#include "Wall.h"
#pragma once
class Room
{
public:
	Room(void);
	~Room(void);
	Wall wall1,wall2,wall3,wall4;
	Room(Wall,Wall,Wall,Wall);
};

