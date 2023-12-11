
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_RenderingLine.h"

//#include "GL/glut.h"
#include "GL/glut.h"

gxRenderingLine::gxRenderingLine() : mlLine(), color(), m_fLineWidth(2.0f)
{
}

gxRenderingLine::gxRenderingLine(
		const gxColor & colorv,
		const mlLine & linev) :
			mlLine(linev),
			color(colorv),
			m_fLineWidth(2.0f)
{
}

gxRenderingLine::gxRenderingLine(
		const gxColor & colorv,
		const mlLine & linev,
		mlFloat fLineWidth) :
			mlLine(linev),
			color(colorv),
			m_fLineWidth(fLineWidth)
{
}

gxRenderingLine::gxRenderingLine(
		const gxColor & colorv,
		const mlVector3D & av,
		const mlVector3D & bv) :
			mlLine(av, bv),
			color(colorv),
			m_fLineWidth(2.0f)
{
}

gxRenderingLine::gxRenderingLine(
		const gxColor & colorv,
		const mlVector3D & av,
		const mlVector3D & bv,
		mlFloat fLineWidth) :
			mlLine(av, bv),
			color(colorv),
			m_fLineWidth(fLineWidth)
{
}

gxRenderingLine gxRenderingLine::Inverse(void)
{
	return gxRenderingLine(color, b, a);
}

void gxRenderingLine::Render()
{
	glLineWidth(m_fLineWidth);
	//glLineWidth(5.0f);

	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_LINES);

	glColor4ub(
		color.red,
		color.green,
		color.blue,
		color.alpha);

	glVertex3f(
		a.x,
		a.y,
		a.z);
	glVertex3f(
		b.x,
		b.y,
		b.z);

	glEnd();

	glLineWidth(1.0f);
}
