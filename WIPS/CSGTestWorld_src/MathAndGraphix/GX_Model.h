
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef GX_MODEL_H
#define GX_MODEL_H

#include "GX_RenderingTriangle.h"
#include "GX_RenderingLine.h"
#include "GX_RenderingPoint.h"

#include "ML_Transform.h"

#include <vector>

#include "UT_String.h"

class gxModel
{
public:

	gxModel();
	virtual ~gxModel() {}

	virtual void Render();

	void	Serialise(istream & stream);
	void	Serialise(ostream & stream);

	void RenderTriangles();
	void RenderLines();
	void RenderPoints();

	void RenderAsPointSet(void);
	void RenderAsWireframe(void);
	void RenderAsWireframe(const gxColor & col, float fWidth = 4.0f);
	void RenderConnectedPoints(int startIndex = 0);
	void RenderAsSpheres(mlFloat radius);

	mlVector3D Centroid();

	void Clear();

	void AddModel(gxModel * model);

	//void AddRenderingTriangle(const mlTriangle & tTriangle, const mlVector3D & vPolytopeCentroid, gxModel * pModel, const gxColor & color);
	
	typedef std::vector<gxRenderingTriangle> TriangleVector;
	typedef std::vector<gxRenderingLine> LineVector;
	typedef std::vector<gxRenderingPoint> PointVector;

	TriangleVector	m_triangles;
	LineVector		m_lines;
	PointVector		m_points;

	mlTransform		m_correctionalTransform;

	bool m_bRenderLines;
	bool m_bRenderTriangles;
	bool m_bRenderPoints;

	bool m_bRenderTriangleIndices;
};

// **********************************************************************

class gxStreamerModel : public gxModel
{
public:

	gxStreamerModel(int nPoints = 100, gxColor color = gxColor(255, 255, 255));
	virtual ~gxStreamerModel() {}

	void AddPoint(const mlVector3D & point, mlFloat distance = 2.0f);
	void Render();

	void AddPointAndRender(const mlVector3D & point, mlFloat distance = 2.0f);

	void BunchUpStreamerAtPoint(const mlVector3D & point);
	void Resample(mlFloat distance);

private:

	int m_index;
};

#endif // GX_MODEL_H
