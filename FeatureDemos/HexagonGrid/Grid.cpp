#include "Grid.h"

Grid::Grid()
{
}

Grid::~Grid()
{
}

void Grid::create(int width, int height, int side)
{
	Height = height;
	Width = width;

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			Hexagons[i][j] = Hexagon(width, height, side);
		}
	}

	float h = CalculateH(side);
	float r = CalculateR(side);
	float hexWidth = 0;
	float hexHeight = 0;
	float pixelWidth;
	float pixelHeight;

	hexWidth = r + r;
	hexHeight = side + h;
	pixelWidth = (width * hexWidth) + r;
	pixelHeight = (height * hexHeight) + h;

	bool inTopRow = false;
	bool inBottomRow = false;
	bool inLeftColumn = false;
	bool inRightColumn = false;
	bool isTopLeft = false;
	bool isTopRight = false;
	bool isBotomLeft = false;
	bool isBottomRight = false;

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(i == 0)
			{
				inTopRow = true;
			}
			else
			{
				inTopRow = false;
			}

			if(i == height - 1)
			{
				inBottomRow = true;
			}
			else
			{
				inBottomRow = false;
			}

			if(j == 0)
			{
				inLeftColumn = true;
			}
			else
			{
				inLeftColumn = false;
			}

			if(j == width - 1)
			{
				inRightColumn = true;
			}
			else
			{
				inRightColumn = false;
			}

			if(inTopRow && inLeftColumn)
			{
				isTopLeft = true;
			}
			else
			{
				isTopLeft = false;
			}

			if(inTopRow && inRightColumn)
			{
				isTopRight = true;
			}
			else
			{
				isTopRight = false;
			}

			if(inBottomRow && inLeftColumn)
			{
				isBotomLeft = true;
			}
			else
			{
				isBotomLeft = false;
			}

			if(inBottomRow && inRightColumn)
			{
				isBottomRight = true;
			}
			else
			{
				isBottomRight = false;
			}

			if (isTopLeft)
			{
				Hexagons[0][0] = Hexagon(0 + (int)r, 0, side);
			}
			else
			{
				if(inLeftColumn)
				{
					if (i % 2 == 0)
					{
						Hexagons[i][j] = Hexagon(Hexagons[i - 1][j].points[(int)BottomLeft], side);
					}
					else
					{
						Hexagons[i][j] = Hexagon(Hexagons[i - 1][j].points[(int)BottomRight], side);
					}

				}
				else
				{
					int x = Hexagons[i][j - 1].points[(int)UpperRight].X;
					int y = Hexagons[i][j - 1].points[(int)UpperRight].Y;
					x += (int)r;
					y -= (int)h;
					Hexagons[i][j] = Hexagon(x, y, side);	
				}
			}
		}
	}
}

void Grid::draw(IVideoDriver* pdrv)
{
	for(int i = 0; i < Height; i++)
	{
		for(int j = 0; j < Width; j++)
		{
			pdrv->draw2DPolygon(Hexagons[i][j].getPosition()+position2d<s32>(0, (int)Hexagons[i][j].getRadius())+position2d<s32>(35, 35), Hexagons[i][j].getRadius(), SColor(255, 255, 0, 0), 6);
		}
	}
}
