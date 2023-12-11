
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_Color.h"

#include "GL/glut.h"

#include <stdlib.h>

extern const gxColor gxColorRed(255, 0, 0);
extern const gxColor gxColorGreen(0, 255, 0);
extern const gxColor gxColorBlue(0, 0, 255);
extern const gxColor gxColorWhite(255, 255, 255);
extern const gxColor gxColorBlack(0, 0, 0);

extern const gxColor gxColorAqua(0, 255, 255);
extern const gxColor gxColorPurple(255, 0, 255);
extern const gxColor gxColorYellow(255, 255, 0);

gxColor::gxColor(int redv, int greenv, int bluev, int alphav) : red(redv), green(greenv), blue(bluev), alpha(alphav)
{
}

gxColor::gxColor(int redv, int greenv, int bluev) : red(redv), green(greenv), blue(bluev), alpha(255)
{
}

gxColor::gxColor() : red(0), green(0), blue(0), alpha(0)
{
}

void gxColor::SetOpenGLColor() const
{
	glColor4ub(
		red,
		green,
		blue,
		alpha);
}

gxColor gxMakeRandomColor()
{
	return gxColor(rand() % 256, rand() % 256, rand() % 256);
}

gxColor gxColorMix(const gxColor & colorA, const gxColor & colorB)
{
	gxColor color;

	color.alpha = 255;

	color.red = colorA.red + colorB.red;
	color.green = colorA.green + colorB.green;
	color.blue = colorA.blue + colorB.blue;

	if(color.red > 255) color.red = 255;
	if(color.green > 255) color.green = 255;
	if(color.blue > 255) color.blue = 255;

	return color;
}

gxColor gxColorSubtract(const gxColor & colorA, const gxColor & colorB)
{
	gxColor color;

	color.alpha = 255;

	color.red = colorA.red - colorB.red;
	color.green = colorA.green - colorB.green;
	color.blue = colorA.blue - colorB.blue;

	if(color.red < 0) color.red = 0;
	if(color.green < 0) color.green = 0;
	if(color.blue < 0) color.blue = 0;

	return color;
}

gxColor gxColorAverage(const gxColor & colorA, const gxColor & colorB)
{
	gxColor color;

	color.alpha = 255;

	color.red = (colorA.red + colorB.red) / 2;
	color.green = (colorA.green + colorB.green) / 2;
	color.blue = (colorA.blue + colorB.blue) / 2;

	return color;
}

gxColor gxColorWeightedAverage(const gxColor & colorA, const gxColor & colorB, mlFloat fWeight)
{
	gxColor color;

	color.alpha = 255;

	color.red = int((mlFloat(colorA.red) * fWeight) + (mlFloat(colorB.red) * (1.0f - fWeight)));
	color.green = int((mlFloat(colorA.green) * fWeight) + (mlFloat(colorB.green) * (1.0f - fWeight)));
	color.blue = int((mlFloat(colorA.blue) * fWeight) + (mlFloat(colorB.blue) * (1.0f - fWeight)));

	return color;
}
