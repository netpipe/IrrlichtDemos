#include "Sel_Intersect.h"


Sel_Intersect::Sel_Intersect(void)
{
}

Sel_Intersect::Sel_Intersect(Wall *wall, Position *position, double distance)
{
	this->position = position;
	this->wall = wall;
	this->distance = distance;
}

Sel_Intersect::~Sel_Intersect(void)
{
}
