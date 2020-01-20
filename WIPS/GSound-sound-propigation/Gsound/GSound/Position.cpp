#include "Position.h"
#include <math.h>

Position::Position(void)
{
}
Position::Position(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Position::Position(const Position*  position)
{
	this->x = position->x;
	this->y = position->y;
	this->z = position->z;
}
Position::~Position(void)
{
}

double Position::distanceTo(Position* position)
{
	double a = pow( this->x - position->x ,2);
	double b = pow( this->y - position->y ,2);
	return sqrt(a + b);
}
