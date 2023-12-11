#include "Position.h"
#pragma once
class Listner
{
public:
	Listner(void);
	Listner(float, float, float);
	Listner(Position* );
	void setPosition(float, float, float);
	~Listner(void);
	Position * position;
};

