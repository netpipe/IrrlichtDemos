
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_RenderingTriangle.h"

#include "GX_Color.h"

#include "ML_Disc.h"

#include "GL/glut.h"

gxRenderingTriangle::gxRenderingTriangle() :
	mlTriangle(),
	colorA(),
	colorB(),
	colorC(),
	m_bTextured(false),
	m_nTextureObject(0),
	textureCoordA(mlVector2DZero),
	textureCoordB(mlVector2DZero),
	textureCoordC(mlVector2DZero)
{
}

gxRenderingTriangle::gxRenderingTriangle(
	const gxColor & colorv,
	const mlVector3D & av,
	const mlVector3D & bv,
	const mlVector3D & cv) :
		mlTriangle(av,bv,cv),
		colorA(colorv),
		colorB(colorv),
		colorC(colorv),
		m_bTextured(false),
		m_nTextureObject(0),
		textureCoordA(mlVector2DZero),
		textureCoordB(mlVector2DZero),
		textureCoordC(mlVector2DZero)
{
}

gxRenderingTriangle::gxRenderingTriangle(
	const gxColor & colorv,
	const mlVector3D & av,
	const mlVector3D & bv,
	const mlVector3D & cv,
	GLuint nTextureObject,
	const mlVector2D & textureCoordAv,
	const mlVector2D & textureCoordBv,
	const mlVector2D & textureCoordCv) :
		mlTriangle(av,bv,cv),
		colorA(colorv),
		colorB(colorv),
		colorC(colorv),
		m_bTextured(true),
		m_nTextureObject(nTextureObject),
		textureCoordA(textureCoordAv),
		textureCoordB(textureCoordBv),
		textureCoordC(textureCoordCv)
{
}

gxRenderingTriangle::gxRenderingTriangle(
	const gxColor & colorv,
	const mlTriangle & trianglev) :
		mlTriangle(trianglev),
		colorA(colorv),
		colorB(colorv),
		colorC(colorv),
		m_bTextured(false),
		m_nTextureObject(0),
		textureCoordA(mlVector2DZero),
		textureCoordB(mlVector2DZero),
		textureCoordC(mlVector2DZero)
{
}

gxRenderingTriangle::gxRenderingTriangle(
		const gxColor & colorv,
		const mlTriangle & trianglev,
		GLuint nTextureObject,
		const mlVector2D & textureCoordAv,
		const mlVector2D & textureCoordBv,
		const mlVector2D & textureCoordCv) :
			mlTriangle(trianglev),
			colorA(colorv),
			colorB(colorv),
			colorC(colorv),
			m_bTextured(true),
			m_nTextureObject(nTextureObject),
			textureCoordA(textureCoordAv),
			textureCoordB(textureCoordBv),
			textureCoordC(textureCoordCv)
{
}

gxRenderingTriangle::gxRenderingTriangle(
	const gxColor & colorAv,
	const gxColor & colorBv,
	const gxColor & colorCv,
	const mlVector3D & av,
	const mlVector3D & bv,
	const mlVector3D & cv,
	GLuint nTextureObject,
	const mlVector2D & textureCoordAv,
	const mlVector2D & textureCoordBv,
	const mlVector2D & textureCoordCv) :
		mlTriangle(av,bv,cv),
		colorA(colorAv),
		colorB(colorBv),
		colorC(colorCv),
		m_bTextured(true),
		m_nTextureObject(nTextureObject),
		textureCoordA(textureCoordAv),
		textureCoordB(textureCoordBv),
		textureCoordC(textureCoordCv)
{
}

gxRenderingTriangle::gxRenderingTriangle(
	const gxColor & colorAv,
	const gxColor & colorBv,
	const gxColor & colorCv,
	const mlVector3D & av,
	const mlVector3D & bv,
	const mlVector3D & cv) :
		mlTriangle(av,bv,cv),
		colorA(colorAv),
		colorB(colorBv),
		colorC(colorCv),
		m_bTextured(false),
		m_nTextureObject(0),
		textureCoordA(mlVector2DZero),
		textureCoordB(mlVector2DZero),
		textureCoordC(mlVector2DZero)
{
}

gxRenderingTriangle gxRenderingTriangle::Inverse(void)
{
	return gxRenderingTriangle(colorC, colorB, colorA, c, b, a, m_nTextureObject, textureCoordC, textureCoordB, textureCoordA);
}

void gxRenderingTriangle::Render()
{
	GLint nRenderMode = GL_RENDER;
	glGetIntegerv(GL_RENDER_MODE, &nRenderMode);

	bool bRenderWithTexture = (m_bTextured) && (nRenderMode == GL_RENDER);

	if(bRenderWithTexture)
	{
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, m_nTextureObject);

		glBegin(GL_TRIANGLES);

		mlVector3D vNormal = Normal();
		vNormal.Normalise();
		glNormal3f(vNormal.x, vNormal.y, vNormal.z);

		glColor4ub(colorA.red, colorA.green, colorA.blue, colorA.alpha);

		glTexCoord2f(textureCoordA.x, textureCoordA.y);
		glVertex3f(a.x, a.y, a.z);

		glColor4ub(colorB.red, colorB.green, colorB.blue, colorB.alpha);

		glTexCoord2f(textureCoordB.x, textureCoordB.y);
		glVertex3f(b.x, b.y, b.z);

		glColor4ub(colorC.red, colorC.green, colorC.blue, colorC.alpha);

		glTexCoord2f(textureCoordC.x, textureCoordC.y);
		glVertex3f(c.x, c.y, c.z);

		glEnd();

		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_TRIANGLES);

		mlVector3D vNormal = Normal();
		vNormal.Normalise();
		glNormal3f(vNormal.x, vNormal.y, vNormal.z);

		glColor4ub(colorA.red, colorA.green, colorA.blue, colorA.alpha);

		glVertex3f(a.x, a.y, a.z);

		glColor4ub(colorB.red, colorB.green, colorB.blue, colorB.alpha);

		glVertex3f(b.x, b.y, b.z);

		glColor4ub(colorC.red, colorC.green, colorC.blue, colorC.alpha);

		glVertex3f(c.x, c.y, c.z);

		glEnd();
	}
}

void gxRenderingTriangle::RenderAsLines()
{
	glBegin(GL_LINES);

	glColor4ub(colorA.red, colorA.green, colorA.blue, colorA.alpha);

	glVertex3f(
		a.x,
		a.y,
		a.z);

	glColor4ub(colorB.red, colorB.green, colorB.blue, colorB.alpha);

	glVertex3f(
		b.x,
		b.y,
		b.z);

	glVertex3f(
		b.x,
		b.y,
		b.z);

	glColor4ub(colorC.red, colorC.green, colorC.blue, colorC.alpha);

	glVertex3f(
		c.x,
		c.y,
		c.z);

	glVertex3f(
		c.x,
		c.y,
		c.z);

	glColor4ub(colorA.red, colorA.green, colorA.blue, colorA.alpha);

	glVertex3f(
		a.x,
		a.y,
		a.z);

	glEnd();
}

void gxRenderingTriangle::RenderAsDisc()
{
	mlDisc disc(a, b, c);

	disc.Render(gxColorPurple, true);
}
