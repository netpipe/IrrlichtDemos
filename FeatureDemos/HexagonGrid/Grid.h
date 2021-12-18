#ifndef GRID_H_
#define GRID_H_

#include "Main.h"
#include "Hexagon.h"

class Grid
{
public:
	Grid();
	~Grid();

	void create(int pwidth, int pheight, int pside);
	void draw(IVideoDriver* pdrv);

private:
	int Height;
	int Width;

	Hexagon Hexagons[20][20];
};

#endif
