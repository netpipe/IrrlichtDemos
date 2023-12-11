
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ML_Triangle.h"

#include "ML_Maths.h"

#include "ML_Matrix.h"

#include "UT_Intersection.h"

mlTriangle::mlTriangle(const mlVector3D & vNormal, const mlVector3D & vUp, const mlVector3D & point, mlFloat length)
{
	mlQuaternion rtnCoordinateSpace = mlQuaternionFromDirection(vNormal, vUp);
	a = point;
	b = point + rtnCoordinateSpace.TransformVector(mlVector3D(length, 0, 0));

	// This doesn't seem right... c is along the up vector!
	//c = point + rtnCoordinateSpace.TransformVector(mlVector3D(0, length, 0));

	// Lets make c along the forward vector (which is along the negative Z axis)
	c = point + rtnCoordinateSpace.TransformVector(mlVector3D(0, 0, -length));
}

mlVector3D mlTriangle::Normal(void) const
{
	mlVector3D triangleRayAtoB = a - b;
	mlVector3D triangleRayAtoC = a - c;
	
	return mlVectorCross(triangleRayAtoB, triangleRayAtoC);
}

mlVector3D mlTriangle::Centroid(void) const
{
	return (a + b + c) * (1.0f / 3.0f);
}

mlTriangle mlTriangle::Invert() const
{
	return mlTriangle(c,b,a);
}

bool mlTriangle::IsInTriangle(const mlVector3D & point, mlAngle fToleranceAngle) const
{
	mlVector3D centroid = Centroid();

	mlVector3D pointToCentroid = point - centroid;

	mlFloat pointToCentroidLength = pointToCentroid.Magnitude();

	mlFloat centroidToALength = (centroid - a).Magnitude();
	mlFloat centroidToBLength = (centroid - b).Magnitude();
	mlFloat centroidToCLength = (centroid - c).Magnitude();

	if(	pointToCentroidLength > centroidToALength &&
		pointToCentroidLength > centroidToBLength &&
		pointToCentroidLength > centroidToCLength)
		return false;

	mlVector3D pointToANormalised = (point - a).Normalised();
	mlVector3D pointToBNormalised = (point - b).Normalised();
	mlVector3D pointToCNormalised = (point - c).Normalised();

	mlAngle angleAToPointToB = mlArcCos(mlClamp(pointToANormalised * pointToBNormalised, -1.0f, 1.0f));
	mlAngle angleBToPointToC = mlArcCos(mlClamp(pointToBNormalised * pointToCNormalised, -1.0f, 1.0f));
	mlAngle angleCToPointToA = mlArcCos(mlClamp(pointToCNormalised * pointToANormalised, -1.0f, 1.0f));

	mlAngle totalAngle = angleAToPointToB + angleBToPointToC + angleCToPointToA;

	//mlAngle toleranceAngle = mlDegrees10 / 100.0f;

	if(mlFabs(totalAngle - mlTwoPi) <= fToleranceAngle)
		return true;
	else
		return false;
}

bool mlTriangle::IsOnPlane(const mlVector3D & point)
{
	mlVector3D projectedPoint = ProjectPoint(point);

	if((point - projectedPoint).Magnitude() < 0.001f)
		return true;
	else
		return false;
}

mlFloat mlTriangle::Area(void) const
{
	// The area of a triangle equals half the magnitude of the cross product of any two sides.

	mlVector3D sideAB = b - a;
	mlVector3D sideAC = c - a;
	
	mlVector3D productVector = mlVectorCross(sideAB, sideAC);
	
	mlFloat productScalar = productVector.Magnitude();
	mlFloat area = 0.5f * productScalar;
	
	return area;
}

mlVector3D mlTriangle::ProjectPoint(const mlVector3D & point)
{
	utIntersectionResult result = utIntersectionPosition(point, Normal().Normalised(), *this);

	return result.intersectionPosition;
}

mlVector3D mlTriangle::ProjectRay(const mlVector3D & rayPoint, const mlVector3D & rayVector)
{
	utIntersectionResult result = utIntersectionPosition(rayPoint, rayVector, *this);

	return result.intersectionPosition;
}

mlVector3D mlTriangle::MirrorPoint(const mlVector3D & vPoint)
{
	mlVector3D vProjection = ProjectPoint(vPoint);
	mlVector3D vVectorToProjection = vPoint - vProjection;
	mlVector3D vMirroredPoint = vProjection - vVectorToProjection;
	
	return vMirroredPoint;
}

mlVector3D	mlTriangle::ReflectRay(const mlVector3D & vRay)
{
	//mlVector3D vPointNearTriangle = a - vRay;
	mlVector3D vPointOnOtherSideOfTriangle = a + vRay;
	mlVector3D vMirroredPoint = MirrorPoint(vPointOnOtherSideOfTriangle);
	mlVector3D vReflectedRay = vMirroredPoint - a;
	return vReflectedRay;
}

bool mlTriangle::IsOnPerimeter(const mlVector3D & point) const
{
	mlLine aTob = mlLine(a, b);

	if(aTob.IsOnLine(point))
		return true;

	mlLine bToc = mlLine(b, c);

	if(bToc.IsOnLine(point))
		return true;

	mlLine cToa = mlLine(c, a);

	if(cToa.IsOnLine(point))
		return true;

	return false;
}

mlVector3D mlThreePlaneIntersection(const mlTriangle & triangleA, const mlTriangle & triangleB, const mlTriangle & triangleC)
{
	/*

The intersection of three planes, if no two of them are parallel, is a point. The three planes can be written as 

N1 . p = d1
N2 . p = d2

N3 . p = d3 

In the above and what follows, "." signifies the dot product and "*" is the cross product. The intersection point P is given by: 

   d1 ( N2 * N3 ) + d2 ( N3 * N1 ) + d3 ( N1 * N2 )  
P =  -------------------------------------------------------------------------  
   N1 . ( N2 * N3 )  


Note that the denominator is 0 if any two of the planes are parallel. If (N2 * N3) is zero it means they are parallel. Otherwise (N2 * N3) gives a vector that is perpendicular to both N2 and N3. The only time the dot product is zero is if the two vectors are perpendicular. So, if N1 dot (N2 * N3) is zero it means that N1 is the same as either N2 or N3 ... hence the planes are parallel and therefore there is no intersection point. 

	*/

	mlVector3D N1 = triangleA.Normal().Normalised(); mlVector3D p1 = triangleA.a;
	mlVector3D N2 = triangleB.Normal().Normalised(); mlVector3D p2 = triangleB.a;
	mlVector3D N3 = triangleC.Normal().Normalised(); mlVector3D p3 = triangleC.a;

	mlVector3D N1xN2 = mlVectorCross(N1, N2);
	mlVector3D N2xN3 = mlVectorCross(N2, N3);
	mlVector3D N3xN1 = mlVectorCross(N3, N1);

	mlFloat d1 = N1 * p1;
	mlFloat d2 = N2 * p2;
	mlFloat d3 = N3 * p3;

	mlFloat denominator = N1 * N2xN3;

	if(denominator == 0.0f)
		return mlVector3DZero;

	mlVector3D numerator = (d1 * N2xN3) + (d2 * N3xN1) + (d3 * N1xN2);
	return numerator * (1.0f / denominator);
}

mlLine mlTwoPlaneIntersection(const mlTriangle & triangleA, const mlTriangle & triangleB)
{
	/*
The intersection of two planes (if they are not parallel) is a line. 

Define the two planes with normals N as


N1 . p = d1
N2 . p = d2 

The equation of the line can be written as


p = c1 N1 + c2 N2 + u N1 * N2 
Where "*" is the cross product, "." is the dot product, and u is the parameter of the line. 

Taking the dot product of the above with each normal gives two equations with unknowns c1 and c2.


N1 . p = d1 = c1 N1 . N1 + c2 N1 . N2
N2 . p = d2 = c1 N1 . N2 + c2 N2 . N2


Solving for c1 and c2


c1 = ( d1 N2 . N2 - d2 N1 . N2 ) / determinant 
c2 = ( d2 N1 . N1 - d1 N1 . N2) / determinant 

determinant = ( N1 . N1 ) ( N2 . N2 ) - ( N1 . N2 )2 

Note that a test should first be performed to check that the planes aren't parallel or coincident (also parallel), this is most easily achieved by checking that the cross product of the two normals isn't zero. The planes are parallel if


N1 * N2 = 0 
	*/

	mlVector3D N1 = triangleA.Normal().Normalised(); mlVector3D p1 = triangleA.a;
	mlVector3D N2 = triangleB.Normal().Normalised(); mlVector3D p2 = triangleB.a;

	mlFloat d1 = N1 * p1;
	mlFloat d2 = N2 * p2;

	mlVector3D N1xN2 = mlVectorCross(N1, N2);
	mlVector3D N2xN1 = mlVectorCross(N2, N2);

	mlFloat N1dotN2 = N1 * N2;
	mlFloat N1dotN1 = N1 * N1;
	mlFloat N2dotN2 = N2 * N2;

	if(N1xN2 == mlVector3DZero)
		return mlLine();

	mlFloat determinant = (N1dotN1 * N2dotN2) - (N1dotN2 * N1dotN2);

	if(determinant == 0.0f)
		return mlLine();

	mlFloat c1 = ((d1 * N2dotN2) - (d2 * N1dotN2)) / determinant;
	mlFloat c2 = ((d2 * N1dotN1) - (d1 * N1dotN2)) / determinant;

	mlVector3D pointA = (c1 * N1) + (c2 * N2);
	mlVector3D pointB = (c1 * N1) + (c2 * N2) + N1xN2;

	return mlLine(pointA, pointB);
}

bool mlTrianglesAreCoplanar(mlTriangle & triangleA, mlTriangle & triangleB)
{
	if(triangleA.IsOnPlane(triangleB.a) && triangleA.IsOnPlane(triangleB.b) && triangleA.IsOnPlane(triangleB.c))
		return true;
	else
		return false;
}

void mlConvertTriangleToTransform(const mlTriangle & triangle, mlTransform & transform)
{
	mlVector3D vI = triangle.b - triangle.a;
	//mlVector3D vJ = triangle.c - triangle.a;
	mlVector3D vJold = triangle.c - triangle.a;
	mlVector3D vK = triangle.Normal();

	//mlVector3D vJ = mlVectorCross(vI, vK);
	mlVector3D vJ = mlVectorCross(vK, vI);

	mlVector3D vT = triangle.a;

	vI.Normalise();
	vJ.Normalise();
	vJold.Normalise();
	vK.Normalise();

	mlMatrix3x4 matrix(vI, vJ, vK, vT);

	transform = matrix;
}

mlLine mlPerpendicularBisector(const mlTriangle & pointsonplane)
{
	mlTransform transform;
	mlConvertTriangleToTransform(pointsonplane, transform);

	float fDistanceBetweenPoints = (pointsonplane.a - pointsonplane.b).Magnitude();
	float fLineLength = 10.0f;

	mlLine line;

	line.a = transform.TransformPoint(mlVector3D(fDistanceBetweenPoints * 0.5f, +fLineLength, 0.0f));
	line.b = transform.TransformPoint(mlVector3D(fDistanceBetweenPoints * 0.5f, -fLineLength, 0.0f));

	return line;
}

// This function tells you if points vP1 and vP2 are on opposite sides of the plane triPlane
bool mlRayCrossesPlane(const mlVector3D & vP1, const mlVector3D & vP2, mlTriangle & triPlane)
{
	mlVector3D vNormal = triPlane.Normal();

	mlVector3D vP1Projected = triPlane.ProjectPoint(vP1) - vP1;
	mlVector3D vP2Projected = triPlane.ProjectPoint(vP2) - vP2;

	float fDot1 = vP1Projected * vNormal;
	float fDot2 = vP2Projected * vNormal;

	if(mlFabs(fDot1) < 1.0f)
	{
		fDot1 = 0.0f;
	}

	if(mlFabs(fDot2) < 1.0f)
	{
		fDot2 = 0.0f;
	}

	float fSgn1 = mlSign(fDot1);
	float fSgn2 = mlSign(fDot2);

	if(fSgn1 == fSgn2 || fSgn1 == 0.0f || fSgn2 == 0.0f)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// This function tells you if a ball moving from vP1 to vP2 with radius fRadius will cross plane triPlane.
// If the ball at vP1 is intersecting the plane, it doesn't care as long as the ball is moving away.
// A weird situation then is if both vP1 and vP2 is intersecting, yet vP2 is further away than vP1
bool mlBallCrossesPlane(const mlVector3D & vP1, const mlVector3D & vP2, mlTriangle & triPlane, float fRadius)
{
	// Is P2 close enough so the ball crosses the plane?

	mlVector3D vP1Projected = triPlane.ProjectPoint(vP1);
	mlVector3D vP1ProjectedRay = vP1Projected - vP1;

	mlVector3D vP2Projected = triPlane.ProjectPoint(vP2);
	mlVector3D vP2ProjectedRay = vP2Projected - vP2;

	//float fLength = vP2ProjectedRay.Magnitude();

	float fLengthP1 = vP1ProjectedRay.Magnitude();
	float fLengthP2 = vP2ProjectedRay.Magnitude();

	//if(fLength < fRadius)
	if(fLengthP2 < fRadius && fLengthP2 < fLengthP1)
	{
		// Perhaps only return true if it is closer to the plane than P1 is.
		return true;
	}

	// Get the 2 sphere points, and see if they are on opposite sides of the plane

	vP2ProjectedRay.Normalise();
	vP2ProjectedRay *= fRadius;

	mlVector3D vP1ClosePoint = vP1 + vP2ProjectedRay;
	mlVector3D vP2ClosePoint = vP2 + vP2ProjectedRay;

	return mlRayCrossesPlane(vP1ClosePoint, vP2ClosePoint, triPlane);
}

// This function projects a ball onto the side of the plane the center is on.
// It returns the new center of the ball.
mlVector3D mlProjectBall(const mlVector3D & vPosition, float fRadius, mlTriangle & triPlane)
{
	mlVector3D vProjected = triPlane.ProjectPoint(vPosition);

	mlVector3D vShiftFromProjected = vProjected - vPosition;
	vShiftFromProjected.Normalise();
	vShiftFromProjected *= fRadius;

	return vProjected - vShiftFromProjected;
}

//#include "GX_RenderingTriangle.h"

mlLine mlMiddleLine(const mlLine & a, const mlLine & b)
{
	// Assume a.a <-> b.a and a.b <-> b.b

	// two cases to consider:
	// lines are parallel
	// lines are not parallel

	// First produce a coordinate system based on line a and one of the points on line b
	// using b.b is safe because of the above assumption - a.b will never coincide with b.b since the a side of the lines
	// will be the corner if there is one.

	// Next thing to consider is the length of this line.

	mlTriangle plane(a.a, a.b, b.b);

	mlLine snappedb(plane.ProjectPoint(b.a), b.b);


	//mlVector3D vSnappedBA = plane.ProjectPoint(b.a);

	mlTriangle plane2(a.a, snappedb.a, snappedb.b);

	//gxRenderingTriangle tri(gxColor(0,0,150,150), plane);
	//tri.Render();
	//gxRenderingTriangle tri2(gxColor(0,0,200,150), plane2);
	//tri2.Render();

	// make the edge lines

	mlLine edgelineA(a.a, snappedb.a);
	mlLine edgelineB(a.b, snappedb.b);

	mlTransform transform;
	mlConvertTriangleToTransform(plane, transform);

	// first the parallel test

	mlVector3D vLocalBA = transform.TransformPointInverse(snappedb.a);
	mlVector3D vLocalBB = transform.TransformPointInverse(snappedb.b);

	mlLine line;

	//if(mlFabs(vLocalBA.y - vLocalBB.y) < 0.01f)
	if(mlFabs(vLocalBA.y - vLocalBB.y) < 0.1f)
	{
		// parallel
		line.a = transform.TransformPoint(mlVector3D(0.0f, vLocalBA.y * 0.5f, 0.0f));
		line.b = transform.TransformPoint(mlVector3D(10.0f, vLocalBA.y * 0.5f, 0.0f));
	}
	else
	{
		// not parallel
		mlLine intersection = a.ShortestLineToLine(snappedb);
		// the two points of the intersection line should be identical
		mlVector3D vWalkA = a.MoveInSameDirection(intersection.a, 20.0f);
		mlVector3D vWalkB = snappedb.MoveInSameDirection(intersection.b, 20.0f);
		mlVector3D vMidpoint = vWalkA + ((vWalkB - vWalkA) * 0.5f);

		line.a = intersection.a;
		line.b = vMidpoint;
	}

	if(edgelineA.Length() > 0.01f)
	{
		mlLine intersectionA = line.ShortestLineToLine(edgelineA);
		line.a = intersectionA.a;
	}

	if(edgelineB.Length() > 0.01f)
	{
		mlLine intersectionB = line.ShortestLineToLine(edgelineB);
		line.b = intersectionB.a;
	}

	return line;
}
