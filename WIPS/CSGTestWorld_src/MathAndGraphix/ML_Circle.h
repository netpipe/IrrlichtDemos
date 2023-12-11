
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef ML_CIRCLE_H
#define ML_CIRCLE_H

#include "ML_Vector.h"

class mlCircle
{
public:

	mlCircle();
	mlCircle(const mlVector2D & center, mlFloat radius);
	mlCircle(const mlVector2D & p1, const mlVector2D & p2, const mlVector2D & p3);

	bool Contains(const mlVector2D & point);

	mlVector2D m_center;
	mlFloat m_radius;

};

#endif // ML_CIRCLE_H
