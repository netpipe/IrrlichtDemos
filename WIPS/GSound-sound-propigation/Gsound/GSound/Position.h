#pragma once
class Position
{
public:
	Position(void);
	Position(double, double, double);
	Position(const Position*  );
	~Position(void);
	double distanceTo(Position* position);
	double x,y,z;
};

