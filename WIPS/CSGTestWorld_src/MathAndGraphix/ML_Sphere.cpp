
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ML_Sphere.h"

mlSphere::mlSphere()
{
	m_center = mlVector3DZero;
	m_radius = 0.0f;
}

mlSphere::mlSphere(const mlVector3D & center, mlFloat radius)
{
	m_center = center;
	m_radius = radius;
}

bool mlSphere::Contains(const mlVector3D & point)
{
	if((m_center - point).Magnitude() < m_radius)
		return true;
	else
		return false;
}

mlVector3D mlSphere::ProjectPoint(const mlVector3D & point)
{
	mlVector3D lineToPoint = point - m_center;
	lineToPoint.Normalise();
	mlVector3D projectedPoint = m_center + lineToPoint * m_radius;

	return projectedPoint;
}

bool mlSphere::Contains(const mlSphere & mSphere)
{
	if((m_center - mSphere.m_center).Magnitude() < (m_radius + mSphere.m_radius))
		return true;
	else
		return false;
}

mlVector3D mlSphere::ProjectSphere(const mlSphere & mSphere)
{
	mlVector3D lineToPoint =  mSphere.m_center - m_center;
	lineToPoint.Normalise();
	mlVector3D projectedPoint = m_center + lineToPoint * (m_radius + mSphere.m_radius);

	return projectedPoint;
}

mlVector3D mlSphere::ProjectRay(const mlVector3D & rayPoint, const mlVector3D & rayVector)
{
	mlVector3D otherLinePoint = rayPoint + rayVector;

	mlFloat a = (otherLinePoint - rayPoint).MagnitudeSquared();
	mlFloat b = 2 * ((otherLinePoint - rayPoint) * (rayPoint - m_center));
	mlFloat c = m_center.MagnitudeSquared() + rayPoint.MagnitudeSquared() - (2 * (m_center * rayPoint)) - (m_radius * m_radius);

	if(a == 0.0f)
		return mlVector3DZero;

	mlFloat determinant = (b*b) - 4 * a * c;

	if(determinant < 0.0f)
		return mlVector3DZero;

	mlFloat u1 = (-b + mlSqrt(determinant)) / ( 2 * a );
	mlFloat u2 = (-b - mlSqrt(determinant)) / ( 2 * a );

	mlVector3D point1 = rayPoint + u1 * rayVector;
	mlVector3D point2 = rayPoint + u2 * rayVector;

	if((point1 - rayPoint).MagnitudeSquared() < (point2 - rayPoint).MagnitudeSquared())
		return point1;
	else
		return point2;
}
