
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef ML_TRIANGLE_H
#define ML_TRIANGLE_H

#include "ML_Vector.h"
#include "ML_Line.h"

#include "ML_Transform.h"

class mlTriangle
{
public:

	mlTriangle() : a(mlVector3DZero), b(mlVector3DZero), c(mlVector3DZero) {}
	mlTriangle(const mlVector3D & av, const mlVector3D & bv, const mlVector3D & cv) : a(av), b(bv), c(cv) {}
	mlTriangle(const mlVector3D & vNormal, const mlVector3D & vUp, const mlVector3D & point, mlFloat length);

	mlVector3D	a;
	mlVector3D	b;
	mlVector3D	c;

	mlVector3D	Normal(void) const;
	mlVector3D	Centroid(void) const;

	bool		IsInTriangle(const mlVector3D & point, mlAngle fToleranceAngle = 0.00174532925199433f) const;
	// 0.00174532925199433 = mlDegrees(10.0f) / 100.0f
	
	bool		IsOnPerimeter(const mlVector3D & point) const;
	bool		IsOnPlane(const mlVector3D & point);
	mlFloat		Area(void) const;
	
	mlTriangle	Invert() const;

	mlVector3D	ProjectPoint(const mlVector3D & point);
	mlVector3D	ProjectRay(const mlVector3D & rayPoint, const mlVector3D & rayVector);
	mlVector3D	MirrorPoint(const mlVector3D & vPoint);
	mlVector3D	ReflectRay(const mlVector3D & vRay);

	mlVector3D &		operator[](int index);
	const mlVector3D &	operator[](int index) const;

};

inline mlVector3D&	mlTriangle::operator[](int index)
{
	return reinterpret_cast<mlVector3D*>(this)[index];
}

inline const mlVector3D&	mlTriangle::operator[](int index) const
{
	return reinterpret_cast<const mlVector3D*>(this)[index];
}

mlVector3D mlThreePlaneIntersection(const mlTriangle & triangleA, const mlTriangle & triangleB, const mlTriangle & triangleC);
mlLine mlTwoPlaneIntersection(const mlTriangle & triangleA, const mlTriangle & triangleB);
bool mlTrianglesAreCoplanar(mlTriangle & triangleA, mlTriangle & triangleB);

void mlConvertTriangleToTransform(const mlTriangle & triangle, mlTransform & transform);

mlLine mlPerpendicularBisector(const mlTriangle & pointsonplane);
mlLine mlMiddleLine(const mlLine & a, const mlLine & b);

bool mlRayCrossesPlane(const mlVector3D & vP1, const mlVector3D & vP2, mlTriangle & triPlane);
bool mlBallCrossesPlane(const mlVector3D & vP1, const mlVector3D & vP2, mlTriangle & triPlane, float fRadius);
mlVector3D mlProjectBall(const mlVector3D & vPosition, float fRadius, mlTriangle & triPlane);

#endif // ML_TRIANGLE_H
