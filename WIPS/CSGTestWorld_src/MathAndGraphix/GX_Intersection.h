
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef GX_INTERSECTION_H
#define GX_INTERSECTION_H

#include <vector>

#include "UT_Intersection.h"

#include "GX_RenderingTriangle.h"
#include "GX_Model.h"

class gxIntersectionResult : public utIntersectionResult
{
public:

	gxIntersectionResult();

	gxIntersectionResult( utIntersectionResult result );
	gxIntersectionResult( utIntersectionResult result, gxRenderingTriangle trianglev );

	gxRenderingTriangle	triangle;
};

class gxIntersectionResultSet
{
public:

	gxIntersectionResultSet() {}

	gxIntersectionResult GetIntersectionResultClosestTo(const mlVector3D & point);

	typedef std::vector<gxIntersectionResult> gxIntersectionResultVector;

	gxIntersectionResultVector intersectionResults;
};

void gxPick(const mlVector3D & rayPoint, const mlVector3D & rayVector, const gxModel & model, gxIntersectionResultSet & results);

#endif // GX_INTERSECTION_H
