
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef GX_MODELUTIL_H
#define GX_MODELUTIL_H

#include "ML_Maths.h"

#include "GX_Color.h"

//#include "GL/glut.h"
#include "GL/glut.h"

#include "ML_Transform.h"

class gxModel;

void gxMakeModelFromFeedbackBuffer(
	gxModel *	feedbackModel,
	GLfloat *	feedbackBuffer,
	GLint		size,
	GLdouble *	modelMatrix,
	GLdouble *	projMatrix,
	GLint *		viewport);

void gxTransformModel(mlTransform & transform, gxModel * model);

void gxRenderLine(const gxColor & col, const mlVector3D & vA, const mlVector3D & vB);
void gxRenderTriangle(const gxColor & col, const mlVector3D & vA, const mlVector3D & vB, const mlVector3D & vC);
void gxRenderQuad(const gxColor & col, const mlVector3D & vA, const mlVector3D & vB, const mlVector3D & vC, const mlVector3D & vD);

void gxRenderQuadFromCorner(const gxColor & col, const mlVector3D & vMainCorner, float fWidth, float fLength);

//void gxRenderTexturedTriangle(GLuint nTextureID, const mlVector3D & vA, const mlVector3D & vB, const mlVector3D & vC);
//void gxRenderTexturedQuad(GLuint nTextureID, const mlVector3D & vA, const mlVector3D & vB, const mlVector3D & vC, const mlVector3D & vD);

void gxSimpleGlutBitmapString( void *font, const char *s );

void gxRenderText(const gxColor & col, const mlVector3D & vPosition, const char format[], ...);

void gxRenderAAQuad(const gxColor & col, int nRow, int nCol, float fCellWidth, float fBoardHeight, float fPieceWidth);
void gxRenderAACellLine(const gxColor & col, int nRow, int nCol, float fCellWidth, float fBoardHeight, const mlVector3D & vA, const mlVector3D & vB);

mlVector3D gxGetAAQuadCorner(int nRow, int nCol, float fCellWidth, float fBoardHeight);

#endif // GX_MODELUTIL_H
