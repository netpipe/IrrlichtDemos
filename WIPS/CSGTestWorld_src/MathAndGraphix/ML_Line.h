
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef ML_LINE_H
#define ML_LINE_H

#include "ML_Vector.h"

class mlLine
{
public:

	mlLine() : a(mlVector3DZero), b(mlVector3DZero) {}
	mlLine(const mlVector3D & av, const mlVector3D & bv) : a(av), b(bv) {}

	mlVector3D	a;
	mlVector3D	b;

	mlVector3D	Centroid(void) const;
	mlVector3D	Midpoint(void) const;
	mlVector3D	Interpolate(mlFloat fT) const;
	mlFloat		Length(void) const;
	mlVector3D	ProjectPoint(const mlVector3D & point) const;
	mlLine		ShortestLineToLine(const mlLine & line) const;

	mlVector3D	RotatePoint(mlVector3D vPoint, mlFloat fAngle);

	bool		IsOnLine(const mlVector3D & point) const;

	bool		ProjectionIsOnLine(const mlVector3D & point) const;
	
	void		SetLengthFromA(mlFloat fNewLength);
	mlVector3D	MoveInSameDirection(const mlVector3D & vStart, float fDistance) const;

	mlVector3D &		operator[](int index);
	const mlVector3D &	operator[](int index) const;

};

inline mlVector3D&	mlLine::operator[](int index)
{
	return reinterpret_cast<mlVector3D*>(this)[index];
}

inline const mlVector3D&	mlLine::operator[](int index) const
{
	return reinterpret_cast<const mlVector3D*>(this)[index];
}

#endif // ML_LINE_H
