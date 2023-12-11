#ifndef HEXAGON_H_
#define HEXAGON_H_

#include "Main.h"

enum Vertice
{
	Top = 0,
	UpperRight = 1,
	BottomRight = 2,
	Bottom = 3,
	BottomLeft = 4,
	TopLeft = 5
};

class Hexagon
{
public:
	Hexagon();
	Hexagon(int px, int py, int pside);
	Hexagon(vector2d<s32> point, int pside);

	~Hexagon();

	float getRadius() { return R; }
	position2d<s32> getPosition() { return position2d<s32>(X, Y); }
	array<vector2d<s32> > points;

private:
	int X;
	int Y;
	int Side;

	float R;
	float H;

	void CalculateVertices();
};

#endif
