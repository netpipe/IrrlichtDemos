/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "Polygon.h"

using namespace phoenix;

/*!-------------------------------
Implict constructor from Rectangle.
---------------------------------*/
Polygon::Polygon(const Rectangle& other)
	: verts(), pos( Vector2d(0,0) ), radius(0.0f)
{
	pos = Vector2d(other.getX()+(other.getWidth()/2.0f), other.getY()+(other.getHeight()/2.0f));
	addPoint( Vector2d( other.getX(), other.getY() ));
	addPoint( Vector2d( other.getX() + other.getWidth(), other.getY() ));
	addPoint( Vector2d( other.getX() + other.getWidth(), other.getY()+other.getHeight() ));
	addPoint( Vector2d( other.getX(), other.getY()+other.getHeight() ));
}
