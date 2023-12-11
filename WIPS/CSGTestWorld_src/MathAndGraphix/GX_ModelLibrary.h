
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef MODELS_H
#define MODELS_H

#include "ML_Maths.h"
#include "ML_Vector.h"

#include "ML_Triangle.h"

#include "GX_Color.h"

class gxModel;

class ModelMaker
{
public:

	static gxModel * MakeJetModel();
	static gxModel * MakeGridModel(const mlVector3D & corner, mlFloat length, mlFloat width, mlFloat spacing, mlVector3D normal = mlVector3D(0.0f, 0.0f, 1.0f));
	static gxModel * MakeSheetModel(const mlVector3D & corner, mlFloat length, mlFloat width);
	static gxModel * MakeCubeModel(const mlVector3D & corner, mlFloat length, const gxColor & color, const gxColor & lineColor);
	static gxModel * MakeRectangleModel(const mlVector3D & cornerA, const mlVector3D & cornerB, const gxColor & color, const gxColor & lineColor);
	
	static void AddRenderingTriangle(const mlTriangle & tTriangle, const mlVector3D & vPolytopeCentroid, gxModel * pModel, const gxColor & color);
};

#endif // MODELS_H
