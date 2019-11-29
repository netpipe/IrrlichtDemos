/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "Rectangle.h"

using namespace phoenix;

/*!--------------------------
Implicit conversation from Polygon,
Bounding box generation.
---------------------------*/
Rectangle::Rectangle( const Polygon& other )
{
	if( other.getVertexCount() >= 3 )
	{
		float minx = (other.getPosition() + other.getVertex(0) ).getX();
		float miny = (other.getPosition() + other.getVertex(0) ).getY();
		float maxx = (other.getPosition() + other.getVertex(0) ).getX();
		float maxy = (other.getPosition() + other.getVertex(0) ).getY();

		for(unsigned int i = 1; i < other.getVertexCount(); ++i)
		{
			Vector2d vertex = other.getPosition() + other.getVertex(i);
			if( vertex.getX() < minx ) minx = vertex.getX();
			if( vertex.getY() < miny ) miny = vertex.getY();
			if( vertex.getX() > maxx ) maxx = vertex.getX();
			if( vertex.getY() > maxy ) maxy = vertex.getY();
		}

		position.setX( minx );
		position.setY( miny );
		size.setX( maxx - minx );
		size.setY( maxy - miny );
	}
	else
	{
		*this = Rectangle();
	}
}
