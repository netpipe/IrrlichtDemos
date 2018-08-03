
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_RenderingPoint.h"

//#include "GL/glut.h"
#include "GL/glut.h"

gxRenderingPoint::gxRenderingPoint() : mlVector3D(mlVector3DZero), color()
{
}

gxRenderingPoint::gxRenderingPoint(
	const gxColor & colorv,
	const mlVector3D & pointv) : mlVector3D(pointv), color(colorv)
{
}

void gxRenderingPoint::Render()
{
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_POINTS);

	glColor4ub(
		color.red,
		color.green,
		color.blue,
		color.alpha);

	glVertex3f(
		x,
		y,
		z);

	glEnd();
}

void gxRenderingPoint::RenderAsSphere(mlFloat radius)
{
	glLineWidth(1.0f);

	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glTranslatef(x, y, z);

	glColor4ub(
		color.red,
		color.green,
		color.blue,
		color.alpha);

	glutWireSphere(radius, 15, 15);

	glPopMatrix();
}

void gxRenderingPoint::RenderAsScreenSphere(int nRadius)
{
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];

	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble winx;
	GLdouble winy;
	GLdouble winz;

	GLdouble objx = x;
	GLdouble objy = y;
	GLdouble objz = z;

	gluProject(objx, objy, objz, modelMatrix, projMatrix, viewport, &winx, &winy, &winz);

	winx += nRadius;

	gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

	mlVector3D vSurfacePosition(objx, objy, objz);

	float fRadius = (vSurfacePosition - (*this)).Magnitude();

	RenderAsSphere(fRadius);
}

void gxRenderingPoint::RenderAsCube(mlFloat sideLength)
{
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glTranslatef(x, y, z);

	glColor4ub(
		color.red,
		color.green,
		color.blue,
		color.alpha);

	glutWireCube(sideLength);

	glPopMatrix();
}

void gxRenderingPoint::RenderAsSolidSphere(mlFloat radius)
{
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glTranslatef(x, y, z);

	glColor4ub(
		color.red,
		color.green,
		color.blue,
		color.alpha);

	glutSolidSphere(radius, 15, 15);

	glPopMatrix();
}
