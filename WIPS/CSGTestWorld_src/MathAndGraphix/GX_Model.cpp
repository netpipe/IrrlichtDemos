
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_Model.h"

#include "UT_String.h"

#include "GX_ModelUtil.h"

//#include "GL/glut.h"
#include "GL/glut.h"

// ************************************************************************

int g_nGXModelReserveSize = 512;

gxModel::gxModel()
{
	m_correctionalTransform = mlTransformIdentity;

	m_triangles.reserve(g_nGXModelReserveSize);
	m_lines.reserve(g_nGXModelReserveSize);
	m_points.reserve(g_nGXModelReserveSize);

	m_bRenderLines = false;
	m_bRenderTriangles = true;
	m_bRenderPoints = false;

	m_bRenderTriangleIndices = false;
}

void gxModel::Render()
{
	glPushMatrix();

	mlMatrix4x4 mMatrix = mlMatrix4x4(m_correctionalTransform.GetMatrix());
	mlMatrix4x4 * pMatrix = &mMatrix;
	mlFloat * pafMatrix = reinterpret_cast<mlFloat*>(pMatrix);

	glMultMatrixf(pafMatrix);
	//glLoadMatrixf(pafMatrix);

	glEnable (GL_POLYGON_OFFSET_FILL);
	glPolygonOffset (1., 1.);

	glLineWidth(1);

	if(m_bRenderTriangles)
	{
		RenderTriangles();
	}

	if(m_bRenderLines)
	{
		RenderLines();
	}

	if(m_bRenderPoints)
	{
		RenderPoints();
	}

	if(m_bRenderTriangleIndices)
	{
		for(unsigned int triangleIndex = 0; triangleIndex < m_triangles.size(); triangleIndex++)
		{
			gxRenderingTriangle & triangle = m_triangles[triangleIndex];

			mlVector3D vCenter = triangle.Centroid();
			mlVector3D vNormal = triangle.Normal();
			vNormal.Normalise();

			gxRenderText(gxColorAqua, vCenter + vNormal * 1.5f, "%d", triangleIndex);
			gxRenderText(gxColorAqua, vCenter - vNormal * 1.5f, "%d", triangleIndex);
		}
	}

	glPopMatrix();
}

void gxModel::RenderTriangles()
{
	for(unsigned int triangleIndex = 0; triangleIndex < m_triangles.size(); triangleIndex++)
	{
		gxRenderingTriangle & triangle = m_triangles[triangleIndex];

		triangle.Render();

		// temporarily reintroduce this until fix box rendering.
		//triangle.Inverse().Render();
	}
}

void gxModel::RenderLines()
{
	for(unsigned int lineIndex = 0; lineIndex < m_lines.size(); lineIndex++)
	{
		m_lines[lineIndex].Render();
	}
}

void gxModel::RenderPoints()
{
	for(unsigned int pointIndex = 0; pointIndex < m_points.size(); pointIndex++)
	{
		m_points[pointIndex].Render();
	}
}

void gxModel::RenderAsPointSet(void)
{
	for(unsigned int triangleIndex = 0; triangleIndex < m_triangles.size(); triangleIndex++)
	{
		gxRenderingPoint pointa(m_triangles[triangleIndex].colorA, m_triangles[triangleIndex].a);
		gxRenderingPoint pointb(m_triangles[triangleIndex].colorB, m_triangles[triangleIndex].b);
		gxRenderingPoint pointc(m_triangles[triangleIndex].colorC, m_triangles[triangleIndex].c);

		pointa.Render();
		pointb.Render();
		pointc.Render();
	}
	for(unsigned int lineIndex = 0; lineIndex < m_lines.size(); lineIndex++)
	{
		gxRenderingPoint pointa(m_lines[lineIndex].color, m_lines[lineIndex].a);
		gxRenderingPoint pointb(m_lines[lineIndex].color, m_lines[lineIndex].b);

		pointa.Render();
		pointb.Render();
	}
	for(unsigned int pointIndex = 0; pointIndex < m_points.size(); pointIndex++)
	{
		m_points[pointIndex].Render();
	}
}

void gxModel::RenderAsSpheres(mlFloat radius)
{
	for(unsigned int triangleIndex = 0; triangleIndex < m_triangles.size(); triangleIndex++)
	{
		gxRenderingPoint pointa(m_triangles[triangleIndex].colorA, m_triangles[triangleIndex].a);
		gxRenderingPoint pointb(m_triangles[triangleIndex].colorB, m_triangles[triangleIndex].a);
		gxRenderingPoint pointc(m_triangles[triangleIndex].colorC, m_triangles[triangleIndex].a);

		pointa.RenderAsSphere(radius);
		pointb.RenderAsSphere(radius);
		pointc.RenderAsSphere(radius);
	}
	for(unsigned int lineIndex = 0; lineIndex < m_lines.size(); lineIndex++)
	{
		gxRenderingPoint pointa(m_lines[lineIndex].color, m_lines[lineIndex].a);
		gxRenderingPoint pointb(m_lines[lineIndex].color, m_lines[lineIndex].b);

		pointa.RenderAsSphere(radius);
		pointb.RenderAsSphere(radius);
	}
	for(unsigned int pointIndex = 0; pointIndex < m_points.size(); pointIndex++)
	{
		m_points[pointIndex].RenderAsSphere(radius);
	}
}

void gxModel::RenderAsWireframe(void)
{
	glPushMatrix();

	mlMatrix4x4 mMatrix = mlMatrix4x4(m_correctionalTransform.GetMatrix());
	mlMatrix4x4 * pMatrix = &mMatrix;
	mlFloat * pafMatrix = reinterpret_cast<mlFloat*>(pMatrix);

	glMultMatrixf(pafMatrix);
	//glLoadMatrixf(pafMatrix);

	for(unsigned int triangleIndex = 0; triangleIndex < m_triangles.size(); triangleIndex++)
	{
		gxRenderingTriangle & triangle = m_triangles[triangleIndex];

		triangle.Render();

		gxRenderingLine linea(triangle.colorA, triangle.a, triangle.b);
		gxRenderingLine lineb(triangle.colorB, triangle.b, triangle.c);
		gxRenderingLine linec(triangle.colorC, triangle.c, triangle.a);

		//glDisable(GL_DEPTH_TEST);
		linea.Render();
		lineb.Render();
		linec.Render();
		//glEnable(GL_DEPTH_TEST);
	}

	RenderLines();

	RenderPoints();

	glPopMatrix();
}

void gxModel::RenderAsWireframe(const gxColor & col, float fWidth)
{
	glPushMatrix();

	mlMatrix4x4 mMatrix = mlMatrix4x4(m_correctionalTransform.GetMatrix());
	mlMatrix4x4 * pMatrix = &mMatrix;
	mlFloat * pafMatrix = reinterpret_cast<mlFloat*>(pMatrix);

	glMultMatrixf(pafMatrix);
	//glLoadMatrixf(pafMatrix);

	for(unsigned int triangleIndex = 0; triangleIndex < m_triangles.size(); triangleIndex++)
	{
		gxRenderingTriangle & triangle = m_triangles[triangleIndex];

		//triangle.colorA = col;
		//triangle.colorB = col;
		//triangle.colorC = col;

		//triangle.Render();

		gxRenderingLine linea(col, triangle.a, triangle.b, fWidth);
		gxRenderingLine lineb(col, triangle.b, triangle.c, fWidth);
		gxRenderingLine linec(col, triangle.c, triangle.a, fWidth);

		linea.Render();
		lineb.Render();
		linec.Render();
	}

    //glPolygonMode(GL_FRONT, GL_FILL);
    //glPolygonMode(GL_BACK, GL_FILL);

	//RenderLines();

	//RenderPoints();

	glPopMatrix();
}

void gxModel::RenderConnectedPoints(int startIndex)
{
	int pointIndex = startIndex;

	for(U32 nPointsVisited = 0; nPointsVisited < m_points.size(); nPointsVisited++)
	{
		int previousPointIndex = (pointIndex - 1 + m_points.size()) % m_points.size();

		if(nPointsVisited != 0)
		{
			gxRenderingPoint currentPoint = m_points[pointIndex];
			gxRenderingPoint previousPoint = m_points[previousPointIndex];

			gxRenderingLine line(currentPoint.color, mlLine(currentPoint, previousPoint));

			line.Render();
		}

		pointIndex = (pointIndex + 1) % m_points.size();
	}
}

mlVector3D gxModel::Centroid()
{
	if(m_triangles.size() == 0)
		return mlVector3DZero;

	mlVector3D centroidTotal = mlVector3DZero;

	for(U32 triangleIndex = 0; triangleIndex < m_triangles.size(); triangleIndex++)
	{
		gxRenderingTriangle & triangle = m_triangles[triangleIndex];

		centroidTotal += triangle.Centroid();
	}

	mlVector3D centroidAverage = centroidTotal * (1.0f / mlFloat(m_triangles.size()));

	return centroidAverage;
}

void gxModel::Clear()
{
	m_points.clear();
	m_lines.clear();
	m_triangles.clear();
}

void gxModel::AddModel(gxModel * model)
{
	for(unsigned int triangleIndex = 0; triangleIndex < model->m_triangles.size(); triangleIndex++)
	{
		m_triangles.push_back(model->m_triangles[triangleIndex]);
	}
	for(unsigned int lineIndex = 0; lineIndex < model->m_lines.size(); lineIndex++)
	{
		m_lines.push_back(model->m_lines[lineIndex]);
	}
	for(unsigned int pointIndex = 0; pointIndex < model->m_points.size(); pointIndex++)
	{
		m_points.push_back(model->m_points[pointIndex]);
	}
}

void gxModel::Serialise(istream & stream)
{
	utVerifyTokenInStream(stream, '\n', "Model");

	utVerifyTokenInStream(stream, '\n', "Triangles");
	utVerifyTokenInStream(stream, ' ', "NumberOfTriangles");
	int nTriangles = utGetIntInStream(stream);

	for(int iTriangle = 0; iTriangle < nTriangles; iTriangle++)
	{
		utVerifyTokenInStream(stream, '\n', "Triangle");

		gxRenderingTriangle triangle;

		utVerifyTokenInStream(stream, ' ', "Color");
		triangle.colorA.red = utGetIntInStream(stream);
		triangle.colorA.green = utGetIntInStream(stream);
		triangle.colorA.blue = utGetIntInStream(stream);
		triangle.colorA.alpha = utGetIntInStream(stream);

		triangle.colorB = triangle.colorA;
		triangle.colorC = triangle.colorA;

		utVerifyTokenInStream(stream, ' ', "PointA");
		triangle.a.x = utGetFloatInStream(stream);
		triangle.a.y = utGetFloatInStream(stream);
		triangle.a.z = utGetFloatInStream(stream);
		utVerifyTokenInStream(stream, ' ', "PointB");
		triangle.b.x = utGetFloatInStream(stream);
		triangle.b.y = utGetFloatInStream(stream);
		triangle.b.z = utGetFloatInStream(stream);
		utVerifyTokenInStream(stream, ' ', "PointC");
		triangle.c.x = utGetFloatInStream(stream);
		triangle.c.y = utGetFloatInStream(stream);
		triangle.c.z = utGetFloatInStream(stream);

		m_triangles.push_back(triangle);

		utVerifyTokenInStream(stream, '\n', "TriangleEnd");
	}

	utVerifyTokenInStream(stream, '\n', "TrianglesEnd");

	utVerifyTokenInStream(stream, '\n', "Lines");
	utVerifyTokenInStream(stream, ' ', "NumberOfLines");
	int nLines = utGetIntInStream(stream);

	for(int iLine = 0; iLine < nLines; iLine++)
	{
		utVerifyTokenInStream(stream, '\n', "Line");

		gxRenderingLine line;

		utVerifyTokenInStream(stream, ' ', "Color");
		line.color.red = utGetIntInStream(stream);
		line.color.green = utGetIntInStream(stream);
		line.color.blue = utGetIntInStream(stream);
		line.color.alpha = utGetIntInStream(stream);

		utVerifyTokenInStream(stream, ' ', "PointA");
		line.a.x = utGetFloatInStream(stream);
		line.a.y = utGetFloatInStream(stream);
		line.a.z = utGetFloatInStream(stream);
		utVerifyTokenInStream(stream, ' ', "PointB");
		line.b.x = utGetFloatInStream(stream);
		line.b.y = utGetFloatInStream(stream);
		line.b.z = utGetFloatInStream(stream);

		m_lines.push_back(line);

		utVerifyTokenInStream(stream, '\n', "LineEnd");
	}

	utVerifyTokenInStream(stream, '\n', "LinesEnd");

	utVerifyTokenInStream(stream, '\n', "Points");

	utVerifyTokenInStream(stream, ' ', "NumberOfPoints");
	int nPoints = utGetIntInStream(stream);

	for(int iPoint = 0; iPoint < nPoints; iPoint++)
	{
		utVerifyTokenInStream(stream, '\n', "Point");

		gxRenderingPoint point;

		utVerifyTokenInStream(stream, ' ', "Color");
		point.color.red = utGetIntInStream(stream);
		point.color.green = utGetIntInStream(stream);
		point.color.blue = utGetIntInStream(stream);
		point.color.alpha = utGetIntInStream(stream);

		utVerifyTokenInStream(stream, ' ', "Coordinates");
		point.x = utGetFloatInStream(stream);
		point.y = utGetFloatInStream(stream);
		point.z = utGetFloatInStream(stream);

		m_points.push_back(point);

		utVerifyTokenInStream(stream, '\n', "PointEnd");
	}

	utVerifyTokenInStream(stream, '\n', "PointsEnd");

	utVerifyTokenInStream(stream, '\n', "ModelEnd");
}

void gxModel::Serialise(ostream & stream)
{
	stream << "Model\n";

	stream << "Triangles\n";
	stream << "NumberOfTriangles " << m_triangles.size() << "\n";
	for(U32 iTriangle = 0; iTriangle < m_triangles.size(); iTriangle++)
	{
		stream << "Triangle\n";

		gxRenderingTriangle & triangle = m_triangles[iTriangle];

		stream << "Color " << triangle.colorA.red << " " << triangle.colorA.green << " " << triangle.colorA.blue << " " << triangle.colorA.alpha << "\n";
		stream << "PointA " << triangle.a.x << " " << triangle.a.y << " " << triangle.a.z << "\n";
		stream << "PointB " << triangle.b.x << " " << triangle.b.y << " " << triangle.b.z << "\n";
		stream << "PointC " << triangle.c.x << " " << triangle.c.y << " " << triangle.c.z << "\n";

		stream << "TriangleEnd\n";
	}
	stream << "TrianglesEnd\n";

	stream << "Lines\n";
	stream << "NumberOfLines " << m_lines.size() << "\n";
	for(U32 iLine = 0; iLine < m_lines.size(); iLine++)
	{
		stream << "Line\n";

		gxRenderingLine & line = m_lines[iLine];

		stream << "Color " << line.color.red << " " << line.color.green << " " << line.color.blue << " " << line.color.alpha << "\n";
		stream << "PointA " << line.a.x << " " << line.a.y << " " << line.a.z << "\n";
		stream << "PointB " << line.b.x << " " << line.b.y << " " << line.b.z << "\n";

		stream << "LineEnd\n";
	}
	stream << "LinesEnd\n";

	stream << "Points\n";
	stream << "NumberOfPoints " << m_points.size() << "\n";
	for(U32 iPoint = 0; iPoint < m_points.size(); iPoint++)
	{
		stream << "Point\n";

		gxRenderingPoint & point = m_points[iPoint];

		stream << "Color " << point.color.red << " " << point.color.green << " " << point.color.blue << " " << point.color.alpha << "\n";
		stream << "Coordinates " << point.x << " " << point.y << " " << point.z << "\n";

		stream << "PointEnd\n";
	}
	stream << "PointsEnd\n";

	stream << "ModelEnd\n";
}

// **********************************************************************

gxStreamerModel::gxStreamerModel(int nPoints, gxColor color) : gxModel()
{
	for(int i = 0; i < nPoints; i++)
	{
		m_points.push_back(gxRenderingPoint(color, mlVector3DZero));
	}

	m_index = 0;
}

void gxStreamerModel::AddPoint(const mlVector3D & point, mlFloat distance)
{
	int previousIndex = (m_index - 1 + m_points.size()) % m_points.size();

	mlVector3D previousPoint = m_points[previousIndex];

	if((point - previousPoint).MagnitudeSquared() > (distance * distance))
	{
		m_points[m_index].Set(point.x, point.y, point.z);
		m_index = (m_index + 1) % m_points.size();
	}
}

void gxStreamerModel::Render()
{
	RenderConnectedPoints(m_index);
}

void gxStreamerModel::AddPointAndRender(const mlVector3D & point, mlFloat distance)
{
	AddPoint(point, distance);
	Render();
}

void gxStreamerModel::BunchUpStreamerAtPoint(const mlVector3D & point)
{
	for(U32 i = 0; i < m_points.size(); i++)
	{
		m_points[i].Set(point.x, point.y, point.z);
	}
}

void gxStreamerModel::Resample(mlFloat distance)
{
	PointVector newPointSet;
	newPointSet.resize(m_points.size());
	{for(U32 iPoint = 0; iPoint < m_points.size(); iPoint++)
	{
		newPointSet[iPoint] = m_points[m_index];
	}}

	int pointSetCursor = m_index;

	int pointIndex = m_index;

	int newPointIndex = 0;

	newPointSet[newPointIndex] = m_points[pointSetCursor];

	for(U32 nPointsVisited = 0; nPointsVisited < m_points.size(); nPointsVisited++)
	{
		//int previousPointIndex = (pointIndex - 1 + m_points.size()) % m_points.size();

		mlVector3D cursorPosition = m_points[pointSetCursor];
		mlVector3D seekPosition = m_points[pointIndex];
		mlFloat distanceToCursor = (seekPosition - cursorPosition).Magnitude();
		if(distanceToCursor >= distance)
		{
			pointSetCursor = pointIndex;
			newPointIndex++;
			newPointSet[newPointIndex] = m_points[pointSetCursor];
		}

		pointIndex = (pointIndex + 1) % m_points.size();
	}

	for(U32 iPoint = 0; iPoint < m_points.size(); iPoint++)
	{
		m_points[iPoint] = newPointSet[iPoint];
	}

	m_index = 0;
}
