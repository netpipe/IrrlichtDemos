
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef SOLID_H
#define SOLID_H

#include "GX_Color.h"
#include "ML_Transform.h"

#include "IntSet.h"
#include "VectorSet.h"
#include "ColorSet.h"

#include <string>

class Solid
{
public:

	IntSet indices;
	VectorSet vertices;
	ColorSet colors;
	
	Solid(const std::string & sFileName, const gxColor & colBase, int dRed, int dGreen, int dBlue);
	Solid(VectorSet * vertices, IntSet * indices, ColorSet * colors);
	virtual ~Solid();
	
	VectorSet * getVertices();
	IntSet * getIndices();
	ColorSet * getColors();

	bool isEmpty();

	void setData(VectorSet & vertices, IntSet & indices, ColorSet & colors);
	
	void setColor(const gxColor & color);

	void Translate(const mlVector3D & t);
	void Rotate(const mlVector3D & a, const mlVector3D & b, float angle);
	void Scale(const mlVector3D & s);

	void Render();

	void loadCoordinateFile(const std::string & sFileName, const gxColor & colBase, int dRed, int dGreen, int dBlue);

	mlVector3D getMean();

	mlTransform		m_correctionalTransform;
};

#endif // SOLID_H
