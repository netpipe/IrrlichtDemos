
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_Intersection.h"

gxIntersectionResult::gxIntersectionResult() :
		utIntersectionResult(),
		triangle(gxRenderingTriangle())
{
}

gxIntersectionResult::gxIntersectionResult( utIntersectionResult result ) :
	utIntersectionResult(result.lineLengthener, result.intersectionPosition)
{
}

gxIntersectionResult::gxIntersectionResult( utIntersectionResult result, gxRenderingTriangle trianglev ) :
	utIntersectionResult(result.lineLengthener, result.intersectionPosition),
	triangle(trianglev)
{
}

gxIntersectionResult gxIntersectionResultSet::GetIntersectionResultClosestTo(const mlVector3D & point)
{
	gxIntersectionResult	bestResult	= gxIntersectionResult();
	mlFloat			bestDistance	= 999999.9f;

	for(unsigned int i = 0; i < intersectionResults.size(); i++)
	{
		mlFloat currentDistance = (intersectionResults[i].intersectionPosition - point).Magnitude();

		if(currentDistance < bestDistance)
		{
			bestResult	= intersectionResults[i];
			bestDistance	= currentDistance;
		}
	}

	return bestResult;
}

void gxPick(const mlVector3D & rayPoint, const mlVector3D & rayVector, const gxModel & model, gxIntersectionResultSet & intersectionResultSet)
{
	intersectionResultSet.intersectionResults.clear();
	
	for(unsigned int i = 0; i < model.m_triangles.size(); i++)
	{
		utIntersectionResult currentIntersectionResult = utIntersectionPosition(rayPoint, rayVector, model.m_triangles[i]);
		if(currentIntersectionResult.lineLengthener > 0.0f && model.m_triangles[i].IsInTriangle(currentIntersectionResult.intersectionPosition))
		{
			gxIntersectionResult resultToStore(currentIntersectionResult, model.m_triangles[i]);
			intersectionResultSet.intersectionResults.push_back(resultToStore);
		}
	}

	mlLine lnRay(rayPoint, rayPoint + rayVector);

	for(unsigned int i = 0; i < model.m_lines.size(); i++)
	{
		const mlLine & line = model.m_lines[i];

		mlLine lnJoiningLine = lnRay.ShortestLineToLine(line);
		
		GLdouble modelMatrix[16];
		GLdouble projMatrix[16];
		GLint viewport[4];

		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
		glGetIntegerv(GL_VIEWPORT, viewport);

		GLdouble win1x;
		GLdouble win1y;
		GLdouble win1z;

		GLdouble win2x;
		GLdouble win2y;
		GLdouble win2z;

		GLdouble obj1x = lnJoiningLine.a.x;
		GLdouble obj1y = lnJoiningLine.a.y;
		GLdouble obj1z = lnJoiningLine.a.z;

		GLdouble obj2x = lnJoiningLine.b.x;
		GLdouble obj2y = lnJoiningLine.b.y;
		GLdouble obj2z = lnJoiningLine.b.z;

		gluProject(obj1x, obj1y, obj1z, modelMatrix, projMatrix, viewport, &win1x, &win1y, &win1z);
		gluProject(obj2x, obj2y, obj2z, modelMatrix, projMatrix, viewport, &win2x, &win2y, &win2z);

		mlVector3D vScreenTween = mlVector3D(win1x,win1y,win1z) - mlVector3D(win2x,win2y,win2z);
		mlFloat fPixels = vScreenTween.Magnitude();

		mlFloat fMaxLinePixels = 5.0f;

		//mlFloat fMaxLinePixels = 10.0f; // Even this is a bit much...
		//mlFloat fMaxLinePixels = 20.0f;

		//if(lnJoiningLine.Length() < 0.1f && line.IsOnLine(lnJoiningLine.b))
		//if(lnJoiningLine.Length() < 0.5f && line.IsOnLine(lnJoiningLine.b))
		if(fPixels < fMaxLinePixels && line.ProjectionIsOnLine(lnJoiningLine.b))
		{
			intersectionResultSet.intersectionResults.push_back(gxIntersectionResult(utIntersectionResult(0.0f, lnJoiningLine.b)));
		}
	}
}
