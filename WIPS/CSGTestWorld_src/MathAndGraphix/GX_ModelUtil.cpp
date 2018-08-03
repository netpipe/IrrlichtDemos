
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_ModelUtil.h"

#include "GX_Model.h"

//#include "GL/glut.h"
#include "GL/glut.h"

#include <stdarg.h>

void gxMakeModelFromFeedbackBuffer(
	gxModel *	feedbackModel,
	GLfloat *	feedbackBuffer,
	GLint		size,
	GLdouble *	modelMatrix,
	GLdouble *	projMatrix,
	GLint *		viewport)
{
	typedef std::vector<mlVector3D> VertexVector;

	int i = 0;
	while(i < size)
	{
		if(feedbackBuffer[i] == GL_POLYGON_TOKEN)
		{
			VertexVector vertices;

			i++;

			int numberOfVertices = int(feedbackBuffer[i]);

			i++;

			for(int j = 0; j < numberOfVertices; j++)
			{
				double coordinatesX , coordinatesY, coordinatesZ;

				gluUnProject(
					GLdouble(feedbackBuffer[i+0]), GLdouble(feedbackBuffer[i+1]), GLdouble(feedbackBuffer[i+2]),
					modelMatrix, projMatrix, viewport,
					&coordinatesX,
					&coordinatesY,
					&coordinatesZ);

				vertices.push_back(mlVector3D(coordinatesX, coordinatesY, coordinatesZ));

				i += 3;
			}

			for(U32 vertexIndex = 0; vertexIndex < vertices.size()-2; vertexIndex++)
			{
				feedbackModel->m_triangles.push_back(gxRenderingTriangle(gxColor(128, 0, 0), mlTriangle(vertices[vertexIndex+0], vertices[vertexIndex+1], vertices[vertexIndex+2])));
				//feedbackModel->m_triangles.push_back(gxRenderingTriangle(gxColor(128, 0, 0), mlTriangle(vertices[vertexIndex+2], vertices[vertexIndex+1], vertices[vertexIndex+0])));
			}
		}
		else if(feedbackBuffer[i] == GL_POINT_TOKEN)
		{
			i += 4;
		}
		else if(feedbackBuffer[i] == GL_LINE_TOKEN)
		{
			// Why did I have this commented out?? Did it not work?
			// It is necessary to capture the lines in a glut sphere.

			double coordinatesX1, coordinatesY1, coordinatesZ1;
			double coordinatesX2, coordinatesY2, coordinatesZ2;

			gluUnProject(
				GLdouble(feedbackBuffer[i+1]), GLdouble(feedbackBuffer[i+2]), GLdouble(feedbackBuffer[i+3]),
				modelMatrix, projMatrix, viewport,
				&coordinatesX1,
				&coordinatesY1,
				&coordinatesZ1);

			gluUnProject(
				GLdouble(feedbackBuffer[i+4]), GLdouble(feedbackBuffer[i+5]), GLdouble(feedbackBuffer[i+6]),
				modelMatrix, projMatrix, viewport,
				&coordinatesX2,
				&coordinatesY2,
				&coordinatesZ2);

			mlLine line(mlVector3D(coordinatesX1, coordinatesY1, coordinatesZ1), mlVector3D(coordinatesX2, coordinatesY2, coordinatesZ2));

			feedbackModel->m_lines.push_back(gxRenderingLine(gxColor(0, 0, 128), line));

			i += 7;
		}
		else if(feedbackBuffer[i] == GL_LINE_RESET_TOKEN)
		{
			// Is this the "last" line in a glBegin / glEnd block?

			double coordinatesX1, coordinatesY1, coordinatesZ1;
			double coordinatesX2, coordinatesY2, coordinatesZ2;

			gluUnProject(
				GLdouble(feedbackBuffer[i+1]), GLdouble(feedbackBuffer[i+2]), GLdouble(feedbackBuffer[i+3]),
				modelMatrix, projMatrix, viewport,
				&coordinatesX1,
				&coordinatesY1,
				&coordinatesZ1);

			gluUnProject(
				GLdouble(feedbackBuffer[i+4]), GLdouble(feedbackBuffer[i+5]), GLdouble(feedbackBuffer[i+6]),
				modelMatrix, projMatrix, viewport,
				&coordinatesX2,
				&coordinatesY2,
				&coordinatesZ2);

			mlLine line(mlVector3D(coordinatesX1, coordinatesY1, coordinatesZ1), mlVector3D(coordinatesX2, coordinatesY2, coordinatesZ2));

			feedbackModel->m_lines.push_back(gxRenderingLine(gxColor(0, 128, 0), line));

			i += 7;
		}
		else if(feedbackBuffer[i] == GL_PASS_THROUGH_TOKEN)
		{
			i += 2;
		}
		else if(feedbackBuffer[i] == GL_BITMAP_TOKEN)
		{
			i += 4;
		}
		else if(feedbackBuffer[i] == GL_DRAW_PIXEL_TOKEN)
		{
			i += 4;
		}
		else if(feedbackBuffer[i] == GL_COPY_PIXEL_TOKEN)
		{
			i += 4;
		}
		else
		{
			assert(false);
			break;
		}
	}
}

void gxTransformModel(mlTransform & transform, gxModel * model)
{
	for(U32 iTriangle = 0; iTriangle < model->m_triangles.size(); iTriangle++)
	{
		gxRenderingTriangle & triangle = model->m_triangles[iTriangle];

		triangle.a = transform.TransformPoint(triangle.a);
		triangle.b = transform.TransformPoint(triangle.b);
		triangle.c = transform.TransformPoint(triangle.c);
	}
	for(U32 iLine = 0; iLine < model->m_lines.size(); iLine++)
	{
		gxRenderingLine & line = model->m_lines[iLine];

		line.a = transform.TransformPoint(line.a);
		line.b = transform.TransformPoint(line.b);
	}
	for(U32 iPoint = 0; iPoint < model->m_points.size(); iPoint++)
	{
		gxRenderingPoint & point = model->m_points[iPoint];

		mlVector3D transformedPoint = transform.TransformPoint(point);

		point.Set(transformedPoint.x, transformedPoint.y, transformedPoint.z);
	}
}

void gxRenderLine(const gxColor & col, const mlVector3D & vA, const mlVector3D & vB)
{
	glBegin(GL_LINES);

	glColor4ub(col.red, col.green, col.blue, col.alpha);

	glVertex3f(vA.x, vA.y, vA.z);
	glVertex3f(vB.x, vB.y, vB.z);

	glEnd();
}

void gxRenderTriangle(const gxColor & col, const mlVector3D & vA, const mlVector3D & vB, const mlVector3D & vC)
{
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLES);

	mlTriangle tri(vA, vB, vC);

	mlVector3D vNormal = tri.Normal();
	vNormal.Normalise();
	glNormal3f(vNormal.x, vNormal.y, vNormal.z);

	glColor4ub(col.red, col.green, col.blue, col.alpha);

	glVertex3f(vA.x, vA.y, vA.z);
	glVertex3f(vB.x, vB.y, vB.z);
	glVertex3f(vC.x, vC.y, vC.z);

	glEnd();
}

void gxRenderQuad(const gxColor & col, const mlVector3D & vA, const mlVector3D & vB, const mlVector3D & vC, const mlVector3D & vD)
{
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

	glColor4ub(col.red, col.green, col.blue, col.alpha);

	glVertex3f(vA.x, vA.y, vA.z);
	glVertex3f(vB.x, vB.y, vB.z);
	glVertex3f(vC.x, vC.y, vC.z);
	glVertex3f(vD.x, vD.y, vD.z);

	glEnd();
}

void gxRenderQuadFromCorner(const gxColor & col, const mlVector3D & vMainCorner, float fWidth, float fLength)
{
	mlVector3D vCorner1 = vMainCorner;
	mlVector3D vCorner2 = vMainCorner + mlVector3D(fWidth, 0, 0);
	mlVector3D vCorner3 = vMainCorner + mlVector3D(fWidth, 0, fLength);
	mlVector3D vCorner4 = vMainCorner + mlVector3D(0, 0, fLength);

	gxRenderQuad(col, vCorner1, vCorner2, vCorner3, vCorner4);

	//glLineWidth(25.0f);

	gxRenderLine(gxColorBlack, vCorner1, vCorner2);
	gxRenderLine(gxColorBlack, vCorner2, vCorner3);
	gxRenderLine(gxColorBlack, vCorner3, vCorner4);
	gxRenderLine(gxColorBlack, vCorner4, vCorner1);

	//glLineWidth(1.0f);
}

//void gxRenderArrow(const gxColor & col, const mlVector3D & vStart, const ml

void gxSimpleGlutBitmapString( void *font, const char *s )
{
  const char *p = s;

  while( *p != '\0' )  {
    glutBitmapCharacter( font, *p );
    p++;
  }
}

void gxRenderText(const gxColor & col, const mlVector3D & vPosition, const char format[], ...)
{
	const unsigned int utFormatStringMax = 4000;
	char text[utFormatStringMax];

	{
		assert(format);

		va_list argList;
		va_start(argList, format);

//		U32 charCount = vsprintf(text, format, argList);

//		text[utFormatStringMax - 1] = '\0';

//		assert(charCount <= utFormatStringMax - 1);

		va_end(argList);
	}

	glColor4ub(col.red, col.green, col.blue, col.alpha);

	glRasterPos3d(vPosition.x, vPosition.y, vPosition.z);

	//glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);
	//glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char *)text);

	//glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char *)text);
	//SimpleGlutBitmapString(GLUT_BITMAP_9_BY_15, text);
}

mlVector3D gxGetAAQuadCorner(int nRow, int nCol, float fCellWidth, float fBoardHeight)
{
	mlVector3D vCorner(fCellWidth * nRow, fBoardHeight, fCellWidth * nCol);

	return vCorner;
}

void gxRenderAAQuad(const gxColor & col, int nRow, int nCol, float fCellWidth, float fBoardHeight, float fPieceWidth)
{
	mlVector3D vCorner = gxGetAAQuadCorner(nRow, nCol, fCellWidth, fBoardHeight);

	float fBorderOffset = (fCellWidth - fPieceWidth) * 0.5f;
	mlVector3D vBorderOffset(fBorderOffset, 0, fBorderOffset);

	vCorner += vBorderOffset;

	gxRenderQuadFromCorner(col, vCorner, fPieceWidth, fPieceWidth);
}

void gxRenderAACellLine(const gxColor & col, int nRow, int nCol, float fCellWidth, float fBoardHeight, const mlVector3D & vA, const mlVector3D & vB)
{
	mlVector3D vCorner = gxGetAAQuadCorner(nRow, nCol, fCellWidth, fBoardHeight);

	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_BLEND);

	//GLdouble fRange = 0;
	//GLdouble fGranu = 0;

	//glGetDoublev(GL_LINE_WIDTH_RANGE, &fRange);
	//glGetDoublev(GL_LINE_WIDTH_GRANULARITY, &fGranu);

	//glLineWidth(5.0f);

	gxRenderLine(col, vCorner + vA, vCorner + vB);

	//glLineWidth(1.0f);
}
