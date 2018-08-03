
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef FACE_H
#define FACE_H

#include "Bound.h"
#include "Vertex.h"
#include "Object3D.h"

class Face
{
public:
	Face();

	Face(Vertex * v1, Vertex * v2, Vertex * v3);
	virtual ~Face();

	void CloneFrom(Face * pFace);

	bool equals_pointersmatch(Face * pFace);
	bool equals(Face * pOtherObject);
	
	Bound getBound();
	mlVector3D getNormal();
	double getArea();
	
	void invert();
		
	bool simpleClassify();
	void rayTraceClassify(Object3D & object);

//private:

	bool hasPoint(const mlVector3D & point);

	Vertex * v1;
	Vertex * v2;
	Vertex * v3;

	//enum Status
	//{
	//	UNKNOWN,
	//	INSIDE,
	//	OUTSIDE,
	//	SAME,
	//	OPPOSITE
	//};
	//
	//Status status;

	//Status getStatus();

	//enum LinePosition
	//{
	//	UP,
	//	DOWN,
	//	ON,
	//	NONE
	//};

	static int UNKNOWN;
	static int INSIDE;
	static int OUTSIDE;
	static int SAME;
	static int OPPOSITE;

	static int UP;
	static int DOWN;
	static int ON;
	static int NONE;

	int status;

	static double TOL;

	int getStatus();

	int linePositionInX(const mlVector3D & point, const mlVector3D & pointLine1, const mlVector3D & pointLine2);
	int linePositionInY(const mlVector3D & point, const mlVector3D & pointLine1, const mlVector3D & pointLine2);
	int linePositionInZ(const mlVector3D & point, const mlVector3D & pointLine1, const mlVector3D & pointLine2);
};

#endif // FACE_H
