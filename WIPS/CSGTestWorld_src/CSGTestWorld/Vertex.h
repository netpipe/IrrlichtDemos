
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef VERTEX_H
#define VERTEX_H

#include "ML_Vector.h"
#include "GX_Color.h"

class VertexPointerSet;

class Vertex
{
public:

	double x;
	double y;
	double z;

	gxColor color;

	static int UNKNOWN;
	static int INSIDE;
	static int OUTSIDE;
	static int BOUNDARY;

	int status;

	static double TOL;

	Vertex();

	Vertex(const Vertex & otherVertex);
		
	Vertex(const mlVector3D & position, const gxColor & color);
	Vertex(const mlVector3D & position, const gxColor & color, int eStatus);
	
	Vertex(double x, double y, double z, const gxColor & color);
	Vertex(double x, double y, double z, const gxColor & color, int eStatus);

	virtual ~Vertex();
	
	//String toString();
	bool equals(Vertex * vertex);
		
	void setStatus(int eStatus);
	mlVector3D getPosition();
	VertexPointerSet * getAdjacentVertices();
	
	int getStatus();

	gxColor getColor();

	void addAdjacentVertex(Vertex * adjacentVertex);

	void mark(int eStatus);

	Vertex & operator=(const Vertex & v);

private:

	VertexPointerSet * adjacentVertices;
};

#endif // VERTEX_H
