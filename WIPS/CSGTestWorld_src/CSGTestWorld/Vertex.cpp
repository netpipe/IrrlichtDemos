
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "Vertex.h"
#include "VertexSet.h"

#include <stdio.h>

#include <math.h>

int Vertex::UNKNOWN = 1;
int Vertex::INSIDE = 2;
int Vertex::OUTSIDE = 3;
int Vertex::BOUNDARY = 4;

double Vertex::TOL = 1e-5f;
//double Vertex::TOL = 1e-10f;

Vertex::Vertex()
{
	x = 0;
	y = 0;
	z = 0;

	adjacentVertices = new VertexPointerSet(50);
	status = UNKNOWN;
}

Vertex::Vertex(const Vertex & v)
{
	adjacentVertices = new VertexPointerSet(50);

	for(int i = 0; i < v.adjacentVertices->length(); i++)
	{
		adjacentVertices->add(v.adjacentVertices->GetVertexPtr(i));
	}

	status = v.status;

	color.red = v.color.red;
	color.green = v.color.green;
	color.blue = v.color.blue;

	x = v.x;
	y = v.y;
	z = v.z;
}

Vertex & Vertex::operator=(const Vertex & v)
{
	for(int i = 0; i < v.adjacentVertices->length(); i++)
	{
		adjacentVertices->add(v.adjacentVertices->GetVertexPtr(i));
	}

	status = v.status;
	color = v.color;
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vertex::Vertex(const mlVector3D & position, const gxColor & colori)
{
	color.red = colori.red;
	color.green = colori.green;
	color.blue = colori.blue;
	
	x = position.x;
	y = position.y;
	z = position.z;
	
	adjacentVertices = new VertexPointerSet(10);
	status = UNKNOWN;
}

Vertex::Vertex(const mlVector3D & position, const gxColor & colori, int status)
{
	color.red = colori.red;
	color.green = colori.green;
	color.blue = colori.blue;
	
	x = position.x;
	y = position.y;
	z = position.z;
	
	adjacentVertices = new VertexPointerSet(10);
	this->status = status;
}

Vertex::Vertex(double x, double y, double z, const gxColor & colori)
{
	color.red = colori.red;
	color.green = colori.green;
	color.blue = colori.blue;
	
	this->x = x;
	this->y = y;
	this->z = z;
	
	adjacentVertices = new VertexPointerSet(10);
	status = UNKNOWN;
}

Vertex::Vertex(double x, double y, double z, const gxColor & colori, int status)
{
	color.red = colori.red;
	color.green = colori.green;
	color.blue = colori.blue;
	
	this->x = x;
	this->y = y;
	this->z = z;
	
	adjacentVertices = new VertexPointerSet(10);
	this->status = status;
}

Vertex::~Vertex()
{
	//printf("Deleting adjacent vertices\n");
	delete adjacentVertices;
}

/**
 * Checks if an vertex is equal to another. To be equal, they have to have the same
 * coordinates(with some tolerance) and color
 * 
 * @param anObject the other vertex to be tested
 * @return true if they are equal, false otherwise. 
 */

// Switching to a pointer, because the parameter for this seems to always be the vertex member variable of a Face.
bool Vertex::equals(Vertex * vertex)
{
	// Doesn't seem to worry about the color or adjacent vertices.

	bool bPositionMatch =
		(fabs(x-vertex->x)<TOL &&
		 fabs(y-vertex->y)<TOL &&
		 fabs(z-vertex->z)<TOL);

	bool bColorMatch =
		(color.red == vertex->color.red) ||
		(color.green == vertex->color.green) ||
		(color.blue == vertex->color.blue);

	return bPositionMatch && bColorMatch;
}

void Vertex::setStatus(int status)
{
	if(status>=UNKNOWN && status<=BOUNDARY)
	{
		this->status = status;
	}
}

mlVector3D Vertex::getPosition()
{
	return mlVector3D(x, y, z);
} 

VertexPointerSet * Vertex::getAdjacentVertices()
{
	return adjacentVertices;
	// No need to return a new copy of the pointer set????
}


int Vertex::getStatus()
{
	return status;
}

gxColor Vertex::getColor()
{
	return color;
}

void Vertex::addAdjacentVertex(Vertex * adjacentVertex)
{
	// Call add on adjacentVertices
	bool bAlready = false;
	for(int i = 0; i < adjacentVertices->GetNumVertexPointers(); i++)
	{
		Vertex * pVertexI = adjacentVertices->GetVertexPtr(i);
		if(pVertexI == adjacentVertex)
		{
			bAlready = true;
		}
	}

	if(!bAlready)
	{
		adjacentVertices->AddVertexPointer(adjacentVertex);
	}
}

void Vertex::mark(int status)
{
	this->status = status;

	//return;

	for(int i = 0; i < adjacentVertices->GetNumVertexPointers(); i++)
	{
		Vertex * pVertexI = adjacentVertices->GetVertexPtr(i);

		if(pVertexI->getStatus()==Vertex::UNKNOWN)
		{
			pVertexI->mark(status);
		}
	}
}
