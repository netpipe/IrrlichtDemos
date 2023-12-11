#include "Vector2D.h"
#include "Position.h"
#pragma once

class Wall   : Vector2D < int > 
{
public:
	Wall (void);
	Wall(Position*, Position*);
	~Wall (void);
	Position* start;
	Position* end;
	int attenuation;
	//double Calc_tita(Ray *ray1);
};

