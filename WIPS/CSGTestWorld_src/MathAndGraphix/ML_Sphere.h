
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef ML_SPHERE_H
#define ML_SPHERE_H

#include "ML_Vector.h"

class mlSphere
{
public:

	mlSphere();
	mlSphere(const mlVector3D & center, mlFloat radius);

	bool Contains(const mlVector3D & point);

	mlVector3D ProjectPoint(const mlVector3D & point);
	mlVector3D ProjectRay(const mlVector3D & rayPoint, const mlVector3D & rayVector);

	bool Contains(const mlSphere & mSphere);
	mlVector3D ProjectSphere(const mlSphere & mSphere);

	mlVector3D	m_center;
	mlFloat		m_radius;

};

#endif // ML_SPHERE_H
