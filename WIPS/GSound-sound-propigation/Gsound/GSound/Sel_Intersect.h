#include"Wall.h"
#pragma once
class Sel_Intersect
{
public:
	Sel_Intersect(void);
	Sel_Intersect(Wall*, Position*, double);
	~Sel_Intersect(void);
	Position * position;
	Wall * wall;
	double distance;
};

