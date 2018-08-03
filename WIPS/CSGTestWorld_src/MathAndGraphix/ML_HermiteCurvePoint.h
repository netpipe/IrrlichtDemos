
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef INCLUDE_ML_HERMITECURVEPOINT_H
#define INCLUDE_ML_HERMITECURVEPOINT_H

#include "ML_Vector.h"

class mlHermiteCurvePoint
{
public:

	mlHermiteCurvePoint()
		: m_vPoint(mlVector3D(0.0f, 0.0f, 0.0f)), m_vTangent(mlVector3D(0.0f, 0.0f, 0.0f)) {}

	mlHermiteCurvePoint(const mlVector3D & i_vPoint, const mlVector3D & i_vTangent)
		: m_vPoint(i_vPoint), m_vTangent(i_vTangent) {}

	virtual ~mlHermiteCurvePoint() {}

	void operator=(const mlHermiteCurvePoint & i_oRightCurvePoint)
	{
		m_vPoint = i_oRightCurvePoint.m_vPoint;
		m_vTangent = i_oRightCurvePoint.m_vTangent;
	}

	mlVector3D		RealTangentPosition() { return m_vPoint + m_vTangent; }
	void			SetRealTangentPosition(const mlVector3D & i_vPoint) { m_vTangent = i_vPoint - m_vPoint; }

	mlVector3D		m_vPoint;
	mlVector3D		m_vTangent;
};

#endif
