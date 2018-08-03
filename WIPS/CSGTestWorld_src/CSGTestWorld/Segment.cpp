
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "Segment.h"

int Segment::VERTEX = 1;
int Segment::FACE = 2;
int Segment::EDGE = 3;

//double Segment::TOL = 1e-10f;
double Segment::TOL = 1e-5f;

Segment::Segment()
{
}

// Keep both chunks.
Segment::Segment(Line & line, Face & face, int sign1, int sign2, int sign3)
{
	//TOL = 1e-10f;

	(*this).line = line;
	index = 0;
	
	//VERTEX is an end
	if(sign1 == 0)
	{
		setVertex(face.v1);
		//other vertices on the same side - VERTEX-VERTEX VERTEX
		if(sign2 == sign3)
		{
			setVertex(face.v1);
		}
	}
	
	//VERTEX is an end
	if(sign2 == 0)
	{
		setVertex(face.v2);
		//other vertices on the same side - VERTEX-VERTEX VERTEX
		if(sign1 == sign3)
		{
			setVertex(face.v2);
		}
	}
	
	//VERTEX is an end
	if(sign3 == 0)
	{
		setVertex(face.v3);
		//other vertices on the same side - VERTEX-VERTEX VERTEX
		if(sign1 == sign2)
		{
			setVertex(face.v3);
		}
	}
	
	//There are undefined ends - one or more edges cut the planes intersection line
	if(getNumEndsSet() != 2)
	{
		//EDGE is an end
		if((sign1==1 && sign2==-1)||(sign1==-1 && sign2==1))
		{
			setEdge(face.v1, face.v2);
		}
		//EDGE is an end
		if((sign2==1 && sign3==-1)||(sign2==-1 && sign3==1))
		{
			setEdge(face.v2, face.v3);
		}
		//EDGE is an end
		if((sign3==1 && sign1==-1)||(sign3==-1 && sign1==1))
		{
			setEdge(face.v3, face.v1);
		}
	}
}

//-------------------------------------GETS-------------------------------------//

/**
 * Gets the start vertex
 * 
 * @return start vertex
 */
Vertex * Segment::getStartVertex()
{
	return startVertex;
}

/**
 * Gets the end vertex
 * 
 * @return end vertex
 */
Vertex * Segment::getEndVertex()
{
	return endVertex;
}

/**
 * Gets the distance from the origin until the starting point
 * 
 * @return distance from the origin until the starting point
 */
double Segment::getStartDistance()
{
	return startDist;
}

/**
 * Gets the distance from the origin until ending point
 * 
 * @return distance from the origin until the ending point
 */
double Segment::getEndDistance()
{
	return endDist;
}

/**
 * Gets the type of the starting point
 * 
 * @return type of the starting point
 */
int Segment::getStartType()
{
	return startType;
}

/**
 * Gets the type of the segment between the starting and ending points
 * 
 * @return type of the segment between the starting and ending points
 */
int Segment::getIntermediateType()
{
	return middleType;
}

/**
 * Gets the type of the ending point
 * 
 * @return type of the ending point
 */
int Segment::getEndType()
{
	return endType;
}

/**
 * Gets the number of ends already set
 *
 * @return number of ends already set
 */
int Segment::getNumEndsSet()
{
	return index;
}

/**
 * Gets the starting position
 * 
 * @return start position
 */
mlVector3D Segment::getStartPosition()
{
	return startPos;
}

/**
 * Gets the ending position
 * 
 * @return ending position
 */
mlVector3D Segment::getEndPosition()
{
	return endPos;
}

//------------------------------------OTHERS------------------------------------//

/**
 * Checks if two segments intersect
 * 
 * @param segment the other segment to check the intesection
 * @return true if the segments intersect, false otherwise
 */
bool Segment::intersect(const Segment & segment)
{
	if(endDist<segment.startDist+TOL ||segment.endDist<startDist+TOL)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//---------------------------------PRIVATES-------------------------------------//

/**
 * Sets an end as vertex (starting point if none end were defined, ending point otherwise)
 * 
 * @param vertex the vertex that is an segment end 
 * @return false if all the ends were already defined, true otherwise
 */
bool Segment::setVertex(Vertex * vertex)
{
	//none end were defined - define starting point as VERTEX
	if(index == 0)
	{
		startVertex = vertex;
	 	startType = VERTEX;
	 	startDist = line.computePointToPointDistance(vertex->getPosition());
	 	startPos = startVertex->getPosition();
	 	index++;
	 	return true;
	}
	//starting point were defined - define ending point as VERTEX
	if(index == 1)
	{
		endVertex = vertex;
		endType = VERTEX;
		endDist = line.computePointToPointDistance(vertex->getPosition());
		endPos = endVertex->getPosition();
		index++;
		
		//defining middle based on the starting point
		//VERTEX-VERTEX-VERTEX
		//if(startVertex.equals(endVertex)) // Need close-enough-to...
		if(true)
		{
			middleType = VERTEX;
		}
		//VERTEX-EDGE-VERTEX
		else if(startType==VERTEX)
		{
			middleType = EDGE;
		}
		
		//the ending point distance should be smaller than  starting point distance 
		if(startDist>endDist)
		{
			swapEnds();
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * Sets an end as edge (starting point if none end were defined, ending point otherwise)
 * 
 * @param vertex1 one of the vertices of the intercepted edge 
 * @param vertex2 one of the vertices of the intercepted edge
 * @return false if all ends were already defined, true otherwise
 */
bool Segment::setEdge(Vertex * vertex1, Vertex * vertex2)
{
	mlVector3D point1 = vertex1->getPosition();
	mlVector3D point2 = vertex2->getPosition();

	mlVector3D edgeDirection(point2.x - point1.x, point2.y - point1.y, point2.z - point1.z);
	Line edgeLine(edgeDirection, point1);
	
	if(index==0)
	{
		startVertex = vertex1;
		startType = EDGE;
		startPos = line.computeLineIntersection(&edgeLine);
		startDist = line.computePointToPointDistance(startPos);
		middleType = FACE;
		index++;
		return true;		
	}
	else if(index==1)
	{
		endVertex = vertex1;
		endType = EDGE;
		endPos = line.computeLineIntersection(&edgeLine);
		endDist = line.computePointToPointDistance(endPos);
		middleType = FACE;
		index++;
		
		//the ending point distance should be smaller than  starting point distance 
		if(startDist>endDist)
		{
		  	swapEnds();
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

/** Swaps the starting point and the ending point */	
void Segment::swapEnds()
{
	double distTemp = startDist;
	startDist = endDist;
	endDist = distTemp;
	
	int typeTemp = startType;
	startType = endType;
	endType = typeTemp;
	
	Vertex * vertexTemp = startVertex;
	startVertex = endVertex;
	endVertex = vertexTemp;
	
	mlVector3D posTemp = startPos;
	startPos = endPos;
	endPos = posTemp;		
}
