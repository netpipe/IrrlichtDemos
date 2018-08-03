
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ML_HermiteCurveCalculator.h"
#include "ML_HermiteCurvePoint.h"

#include "ML_Matrix.h"

mlVector3D mlHermiteCurveCalculator::CalculatePoint(mlHermiteCurvePoint * i_poPointA, mlHermiteCurvePoint * i_poPointB, mlFloat i_fIndex)
{
	mlFloat p2 = i_fIndex * i_fIndex;
	mlFloat p3 = p2 * i_fIndex;

	mlVector3D point =
		(2.0f * p3 - 3.0f * p2 + 1.0f) * i_poPointA->m_vPoint +
		(-2.0f * p3 + 3.0f * p2) * i_poPointB->m_vPoint +
		(p3 - 2.0f * p2 + i_fIndex) * i_poPointA->m_vTangent +
		(p3 - p2) * i_poPointB->m_vTangent;

	return point;
}

mlVector3D mlHermiteCurveCalculator::CalculateVelocity(mlHermiteCurvePoint * i_poPointA, mlHermiteCurvePoint * i_poPointB, mlFloat i_fIndex)
{
	mlFloat p2 = i_fIndex * i_fIndex;

	mlVector3D velocity =
		(6.0f * p2 - 6.0f * i_fIndex) * i_poPointA->m_vPoint +
		(-6.0f * p2 + 6.0f * i_fIndex) * i_poPointB->m_vPoint +
		(3.0f * p2 - 4.0f * i_fIndex + 1) * i_poPointA->m_vTangent +
		(3.0f * p2 - 2.0f * i_fIndex) * i_poPointB->m_vTangent;

	return velocity;
}

mlVector3D mlHermiteCurveCalculator::CalculateAcceleration(mlHermiteCurvePoint * i_poPointA, mlHermiteCurvePoint * i_poPointB, mlFloat i_fIndex)
{
	mlVector3D acceleration =
		(12.0f * i_fIndex - 6.0f) * i_poPointA->m_vPoint +
		(-12.0f * i_fIndex + 6.0f) * i_poPointB->m_vPoint +
		(6.0f * i_fIndex - 4.0f) * i_poPointA->m_vTangent +
		(6.0f * i_fIndex - 2.0f) * i_poPointB->m_vTangent;

	return acceleration;
}

mlTransform mlHermiteCurveCalculator::CalculateTransform(mlHermiteCurvePoint * i_poPointA, mlHermiteCurvePoint * i_poPointB, mlFloat i_fIndex)
{
	mlMatrix3x3 mRotation;

	mRotation.I = CalculateVelocity(i_poPointA, i_poPointB, i_fIndex);
	mRotation.I.Normalise();

	mRotation.J = CalculateNormal(i_poPointA, i_poPointB, i_fIndex);
	mRotation.J.Normalise();

	mRotation.K = mlVector3D(0.0f, 0.0f, 1.0f);

	//mRotation.OrthoNormaliseOnY();

	mlVector3D vPosition = CalculatePoint(i_poPointA, i_poPointB, i_fIndex);

	mlTransform tTransform(mlQuaternionFromRotationMatrix(mRotation), vPosition);

	return tTransform;
}

mlVector3D mlHermiteCurveCalculator::CalculateNormal(mlHermiteCurvePoint * i_poPointA, mlHermiteCurvePoint * i_poPointB, mlFloat i_fIndex)
{
	mlQuaternion qSpinRight(mlVector3D(0.0f, 0.0f, 1.0f), mlDegrees(90.0f));

	mlVector3D vVelocity = CalculateVelocity(i_poPointA, i_poPointB, i_fIndex);

	mlVector3D vNormal = qSpinRight.TransformVector(vVelocity);

	return vNormal;
	
	//mlDisc disc = CalculateRadiusOfCurvature(i_poPointA, i_poPointB, i_fIndex);

	//return disc.m_vCenter - i_poPointA->m_vPoint;
}

mlDisc mlHermiteCurveCalculator::CalculateRadiusOfCurvature(
	mlHermiteCurvePoint * i_poPointA,
	mlHermiteCurvePoint * i_poPointB,
	mlFloat i_fIndex)
{
	mlFloat fStep = 0.01f;

	mlVector3D vPointA = mlHermiteCurveCalculator::CalculatePoint(i_poPointA, i_poPointB, i_fIndex - fStep);
	mlVector3D vPointB = mlHermiteCurveCalculator::CalculatePoint(i_poPointA, i_poPointB, i_fIndex);
	mlVector3D vPointC = mlHermiteCurveCalculator::CalculatePoint(i_poPointA, i_poPointB, i_fIndex + fStep);

	return mlDisc(vPointA, vPointB, vPointC);
}
