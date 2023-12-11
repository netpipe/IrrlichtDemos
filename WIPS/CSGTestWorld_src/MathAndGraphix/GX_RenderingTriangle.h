
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef GX_RENDERINGTRIANGLE_H
#define GX_RENDERINGTRIANGLE_H

#include "ML_Triangle.h"

#include "GX_Color.h"

//#include "GL/glut.h"
#include "GL/glut.h"

#include <string>

class gxRenderingTriangle : public mlTriangle
{
public:
	gxRenderingTriangle();

	gxRenderingTriangle(
		const gxColor & colorv,
		const mlVector3D & av,
		const mlVector3D & bv,
		const mlVector3D & cv);

	gxRenderingTriangle(
		const gxColor & colorv,
		const mlVector3D & av,
		const mlVector3D & bv,
		const mlVector3D & cv,
		GLuint nTextureObject,
		const mlVector2D & textureCoordAv,
		const mlVector2D & textureCoordBv,
		const mlVector2D & textureCoordCv);

	gxRenderingTriangle(
		const gxColor & colorv,
		const mlTriangle & trianglev);
	gxRenderingTriangle(
		const gxColor & colorv,
		const mlTriangle & trianglev,
		GLuint nTextureObject,
		const mlVector2D & textureCoordAv,
		const mlVector2D & textureCoordBv,
		const mlVector2D & textureCoordCv);

	gxRenderingTriangle(
		const gxColor & colorAv,
		const gxColor & colorBv,
		const gxColor & colorCv,
		const mlVector3D & av,
		const mlVector3D & bv,
		const mlVector3D & cv,
		GLuint nTextureObject,
		const mlVector2D & textureCoordAv,
		const mlVector2D & textureCoordBv,
		const mlVector2D & textureCoordCv);

	gxRenderingTriangle(
		const gxColor & colorAv,
		const gxColor & colorBv,
		const gxColor & colorCv,
		const mlVector3D & av,
		const mlVector3D & bv,
		const mlVector3D & cv);

	gxRenderingTriangle Inverse(void);

	void Render();
	void RenderAsLines();

	void RenderAsDisc();

	//gxColor		color;

	gxColor		colorA;
	gxColor		colorB;
	gxColor		colorC;

	bool		m_bTextured;
	int			m_nTextureObject;

	mlVector2D	textureCoordA;
	mlVector2D	textureCoordB;
	mlVector2D	textureCoordC;
};

#endif // GX_RENDERINGTRIANGLE_H
