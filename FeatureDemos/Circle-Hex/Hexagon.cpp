#include "Hexagon.h"

Hexagon::Hexagon() : X(0), Y(0), Side(0)
{
}

Hexagon::Hexagon(int px, int py, int pside) : X(px), Y(py), Side(pside)
{
	CalculateVertices();
}

Hexagon::Hexagon(vector2d<s32> point, int pside) : X(point.X), Y(point.Y), Side(pside)
{
	CalculateVertices();
}

Hexagon::~Hexagon()
{
}

void Hexagon::CalculateVertices()
{
	H = CalculateH(Side);
	R = CalculateR(Side);

	// All the int converting is to get rid of warnings
	points.push_back(vector2d<s32>(X, Y));
	points.push_back(vector2d<s32>(X + (int)R, Y + (int)H));
	points.push_back(vector2d<s32>(X + (int)R, Y + Side + (int)H));
	points.push_back(vector2d<s32>(X, Y + Side + (int)H + (int)H));
	points.push_back(vector2d<s32>(X - (int)R, Y + Side + (int)H));
	points.push_back(vector2d<s32>(X - (int)R, Y + (int)H));
}
