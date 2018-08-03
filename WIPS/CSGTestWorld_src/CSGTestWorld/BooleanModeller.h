
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef BOOLEANMODELLER_H
#define BOOLEANMODELLER_H

#include "Object3D.h"
#include "Solid.h"
#include "Face.h"

class Object3D;
class Solid;

class BooleanModeller
{
public:

	Object3D * m_pObject1;
	Object3D * m_pObject2;
	
	BooleanModeller(Solid * solid1, Solid * solid2);
	virtual ~BooleanModeller();

	Solid * getUnion();
	Solid * getIntersection();
	Solid * getDifference();

private:
	
	Solid * composeSolid(int faceStatus1, int faceStatus2, int faceStatus3);
	
	void groupObjectComponents(Object3D & pObject, VertexSet & vertices, IntSet & indices, ColorSet & colors, int faceStatus1, int faceStatus2);
};

#endif // BOOLEANMODELLER_H
