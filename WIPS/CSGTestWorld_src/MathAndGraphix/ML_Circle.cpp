
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ML_Circle.h"

mlCircle::mlCircle()
{
	m_center = mlVector2DZero;
	m_radius = 0.0f;
}

mlCircle::mlCircle(const mlVector2D & center, mlFloat radius)
{
	m_center = center;
	m_radius = radius;
}

mlCircle::mlCircle(const mlVector2D & p1, const mlVector2D & p2, const mlVector2D & p3)
{
	mlFloat cp = mlVectorCross2D(p2 - p1, p3 - p1);
	if (cp != 0.0)
	{
		mlFloat p1Sq, p2Sq, p3Sq;
		mlFloat num;
		mlFloat cx, cy;

		p1Sq = p1.MagnitudeSquared();
		p2Sq = p2.MagnitudeSquared();
		p3Sq = p3.MagnitudeSquared();

		num = p1Sq*(p2.y - p3.y) + p2Sq*(p3.y - p1.y) + p3Sq*(p1.y - p2.y);
		cx = num / (2.0f * cp);

		num = p1Sq*(p3.x - p2.x) + p2Sq*(p1.x - p3.x) + p3Sq*(p2.x - p1.x);
		cy = num / (2.0f * cp);

		m_center = mlVector2D(cx, cy);
	}

	m_radius = (m_center - p1).Magnitude();
}

bool mlCircle::Contains(const mlVector2D & point)
{
	if((m_center - point).Magnitude() < m_radius)
		return true;
	else
		return false;
}
