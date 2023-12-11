
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef INCLUDE_ML_HERMITECURVERENDERER_H
#define INCLUDE_ML_HERMITECURVERENDERER_H

#include "ML_Vector.h"

class mlHermiteCurvePoint;

class mlHermiteCurveRenderer
{
public:

	static void RenderSegment(
		mlHermiteCurvePoint * i_poPointA,
		mlHermiteCurvePoint * i_poPointB,
		mlFloat fStepSize);
};

#endif
