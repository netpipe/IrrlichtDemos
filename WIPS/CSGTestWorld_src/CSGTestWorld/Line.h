
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef LINE_H
#define LINE_H

#include "ML_Vector.h"
#include "Face.h"

class Line
{
public:

	mlVector3D point;
	mlVector3D direction;
	
	static double TOL;

	Line();

	Line(Face * face1, Face * face2);
	Line(const mlVector3D & direction, const mlVector3D & point);
	
	mlVector3D getPoint();
	mlVector3D getDirection();
	void setPoint(const mlVector3D & point);
	void setDirection(const mlVector3D & direction);
		
	double computePointToPointDistance(const mlVector3D & otherPoint);
	
	mlVector3D computeLineIntersection(Line * otherLine);
	mlVector3D computePlaneIntersection(const mlVector3D & normal, const mlVector3D & planePoint, bool & bResult);

	void perturbDirection();
};

#endif // LINE_H
