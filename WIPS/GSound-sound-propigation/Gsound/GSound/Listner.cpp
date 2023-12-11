#include "Listner.h"

Listner::Listner(void)
{
}

Listner::Listner(float x, float y, float z)
{
	this->position = new Position(x,y,z);
}

void Listner::setPosition(float x, float y, float z){

	this->position->x = x;
	this->position->y = y;
	this->position->z = z;
}

Listner::Listner(Position * position)
{
	this->position = position;
}

Listner::~Listner(void)
{
}
