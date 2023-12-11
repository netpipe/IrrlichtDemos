
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef SEGMENT_H
#define SEGMENT_H

#include "ML_Vector.h"

#include "Line.h"
#include "Face.h"
#include "Vertex.h"

class Segment
{
public:

	Segment();
	Segment(Line & line, Face & face, int sign1, int sign2, int sign3);

	/** line resulting from the two planes intersection */
	Line line;
	/** shows how many ends were already defined */
	int index;
	
	/** distance from the segment starting point to the point defining the plane */
	double startDist;
	/** distance from the segment ending point to the point defining the plane */
	double endDist;
	
	/** starting point status relative to the face */
	int startType;
	/** intermediate status relative to the face */
	int middleType;
	/** ending point status relative to the face */
	int endType;
	
	/** nearest vertex from the starting point */
	Vertex * startVertex;
	/** nearest vertex from the ending point */
	Vertex * endVertex; 
	
	/** start of the intersection point */
	mlVector3D startPos;
	/** end of the intersection point */
	mlVector3D endPos;

	/** define as vertex one of the segment ends */
	/** define as face one of the segment ends */
	/** define as edge one of the segment ends */
	static int VERTEX;
	static int FACE;
	static int EDGE;
	
	/** tolerance value to test equalities */
	static double TOL;// = 1e-10f;
	
	Vertex * getStartVertex();
	Vertex * getEndVertex();

	double getStartDistance();
	double getEndDistance();

	int getStartType();
	int getIntermediateType();
	int getEndType();

	int getNumEndsSet();

	mlVector3D getStartPosition();
	mlVector3D getEndPosition();

	bool intersect(const Segment & segment);

private:

	bool setVertex(Vertex * vertex);
	bool setEdge(Vertex * vertex1, Vertex * vertex2);

	void swapEnds();
};

#endif // SEGMENT_H
