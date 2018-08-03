
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ML_HermiteCurveRenderer.h"

#include "ML_HermiteCurvePoint.h"

#include "ML_HermiteCurveCalculator.h"

#include "ML_Transform.h"

#include "GX_RenderingLine.h"
#include "GX_RenderingPoint.h"

//#include "GL/glut.h"
#include "GL/glut.h"

void mlHermiteCurveRenderer::RenderSegment(
	mlHermiteCurvePoint * i_poPointA,
	mlHermiteCurvePoint * i_poPointB,
	mlFloat fStepSize)
{
	int nVertex = 0;
	mlVector3D vPrevPts[3];

	for(mlFloat fSplineTime = 0.0f; fSplineTime < 1.0f; fSplineTime += fStepSize)
	{
		vPrevPts[2] = vPrevPts[1];
		vPrevPts[1] = vPrevPts[0];
		vPrevPts[0] = mlHermiteCurveCalculator::CalculatePoint(i_poPointA, i_poPointB, fSplineTime);

		if(nVertex > 0)
		{
			gxRenderingLine(gxColorGreen, vPrevPts[0], vPrevPts[1], 1.0f).Render();
		}

		nVertex++;
	}
}
