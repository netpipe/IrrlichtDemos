
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef INCLUDE_ML_HERMITECURVECALCULATOR_H
#define INCLUDE_ML_HERMITECURVECALCULATOR_H

#include "ML_Vector.h"
#include "ML_Transform.h"

#include "ML_Disc.h"

class mlHermiteCurvePoint;

namespace mlHermiteCurveCalculator
{
	mlVector3D CalculatePoint(
		mlHermiteCurvePoint * i_poPointA,
		mlHermiteCurvePoint * i_poPointB,
		mlFloat i_fIndex);
	mlVector3D CalculateVelocity(
		mlHermiteCurvePoint * i_poPointA,
		mlHermiteCurvePoint * i_poPointB,
		mlFloat i_fIndex);
	mlVector3D CalculateAcceleration(
		mlHermiteCurvePoint * i_poPointA,
		mlHermiteCurvePoint * i_poPointB,
		mlFloat i_fIndex);
	mlVector3D CalculateNormal(
		mlHermiteCurvePoint * i_poPointA,
		mlHermiteCurvePoint * i_poPointB,
		mlFloat i_fIndex);
	mlTransform CalculateTransform(
		mlHermiteCurvePoint * i_poPointA,
		mlHermiteCurvePoint * i_poPointB,
		mlFloat i_fIndex);
	mlDisc CalculateRadiusOfCurvature(
		mlHermiteCurvePoint * i_poPointA,
		mlHermiteCurvePoint * i_poPointB,
		mlFloat i_fIndex);
};

#endif
