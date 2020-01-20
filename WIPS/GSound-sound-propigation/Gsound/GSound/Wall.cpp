#include "Wall.h"


Wall::Wall(void)
{
}
Wall::Wall(Position* start,Position* end)
{
	this->start = start;
	this->end = end;
}

Wall::~Wall(void)
{
}
