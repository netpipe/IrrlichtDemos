
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ML_Line.h"

#include "ML_Maths.h"

#include "ML_Transform.h"

mlVector3D mlLine::Centroid(void) const
{
	return (a + b) * 0.5f;
}

mlVector3D mlLine::Midpoint(void) const
{
	return Centroid();
}

mlVector3D	mlLine::Interpolate(mlFloat fT) const
{
	return ((b - a) * fT) + a;
}

mlFloat mlLine::Length(void) const
{
	return (a - b).Magnitude();
}

void mlLine::SetLengthFromA(mlFloat fNewLength)
{
	mlVector3D vDirection = (b - a).Normalised();
	b = a + vDirection * fNewLength;
}

mlVector3D mlLine::ProjectPoint(const mlVector3D & point) const
{
	mlVector3D forward = b - a;
	mlVector3D forwardNormalised = forward.Normalised();

	mlFloat dotProduct = forwardNormalised * (point - a);

	return a + dotProduct * forwardNormalised;
}

mlLine mlLine::ShortestLineToLine(const mlLine & line) const
{
	mlVector3D p13 = a - line.a;
	mlVector3D p43 = line.b - line.a;
	mlVector3D p21 = b - a;

	mlFloat d1343 = p13 * p43;
	mlFloat d4321 = p43 * p21;
	mlFloat d1321 = p13 * p21;
	mlFloat d4343 = p43 * p43;
	mlFloat d2121 = p21 * p21;

	mlFloat denom = d2121 * d4343 - d4321 * d4321;
	mlFloat numer = d1343 * d4321 - d1321 * d4343;

	mlFloat mua = numer / denom;
	mlFloat mub = (d1343 + d4321 * (mua)) / d4343;

	return mlLine(a + mua * p21, line.a + mub * p43);
}

bool mlLine::ProjectionIsOnLine(const mlVector3D & point) const
{
	mlVector3D vProjected = ProjectPoint(point);

	mlFloat length = Length();

	mlVector3D aToPoint = a - point;
	mlFloat aToPointLength = aToPoint.Magnitude();

	mlVector3D bToPoint = b - point;
	mlFloat bToPointLength = bToPoint.Magnitude();

	if(aToPointLength > length)
		return false;

	if(bToPointLength > length)
		return false;

	return true;
}

bool mlLine::IsOnLine(const mlVector3D & point) const 
{
	mlVector3D vProjected = ProjectPoint(point);
	mlFloat fDistance = (point - vProjected).Magnitude();

	if(fDistance > 0.01f)
	{
		return false;
	}

	//mlVector3D forward = a - b;
	//mlFloat length = forward.Magnitude();
	mlFloat length = Length();
	//forward.Normalise();

	mlVector3D aToPoint = a - point;
	mlFloat aToPointLength = aToPoint.Magnitude();
	//aToPoint.Normalise();

	mlVector3D bToPoint = b - point;
	mlFloat bToPointLength = bToPoint.Magnitude();
	//bToPoint.Normalise();

	//if((aToPoint - forward).Magnitude() > 0.01f)
	//	return false;

	if(aToPointLength > length)
		return false;

	if(bToPointLength > length)
		return false;

	return true;
}

mlVector3D mlLine::RotatePoint(mlVector3D vPoint, mlFloat fAngle)
{
	mlVector3D vCenter = ProjectPoint(vPoint);

	mlVector3D vRadial = vPoint - vCenter;
	mlFloat fRadius = vRadial.Magnitude();
	vRadial.Normalise();

	mlVector3D vAxis = a - b;
	vAxis.Normalise();

	//mlVector3D vTangent = mlVectorCross(vRadial, vAxis);
	mlVector3D vTangent = mlVectorCross(vAxis, vRadial);

	mlMatrix3x3 matSpace(vRadial, vTangent, vAxis);
	//mlMatrix3x3 matSpace(vRadial, vAxis, vTangent);
	//mlMatrix3x3 matSpace(vTangent, vRadial, vAxis);
	//mlMatrix3x3 matSpace(vTangent, vAxis, vRadial);
	//mlMatrix3x3 matSpace(vAxis, vTangent, vRadial);
	//mlMatrix3x3 matSpace(vAxis, vRadial, vTangent);

	mlTransform trnSpace(mlQuaternionFromRotationMatrix(matSpace), vCenter);

	// Test begin
	mlVector3D vForward = trnSpace.TransformVector(mlVector3D(0,0,1));
	mlVector3D vRight = trnSpace.TransformVector(mlVector3D(1,0,0));
	mlVector3D vUp = trnSpace.TransformVector(mlVector3D(0,1,0));
	// Test end

	trnSpace.ApplyRotation(mlQuaternion(vForward, fAngle));

	mlVector3D vPushback;

	vPushback = trnSpace.TransformVector(mlVector3D(-fRadius, 0, 0));
	//vPushback = trnSpace.TransformVector(mlVector3D(0, fRadius, 0));
	//vPushback = trnSpace.TransformVector(mlVector3D(0, 0, fRadius));

	mlVector3D vRotated = vCenter - vPushback;

	return vRotated;
}

mlVector3D mlLine::MoveInSameDirection(const mlVector3D & vStart, float fDistance) const
{
	mlVector3D vDir = b - a;
	vDir.Normalise();
	return vStart + (vDir * fDistance);
}
