
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef GX_RENDERINGLINE_H
#define GX_RENDERINGLINE_H

#include "ML_Vector.h"

#include "ML_Line.h"

#include "GX_Color.h"

class gxRenderingLine : public mlLine
{
public:

	gxRenderingLine();
	gxRenderingLine(
		const gxColor & colorv,
		const mlLine & linev);
	gxRenderingLine(
		const gxColor & colorv,
		const mlLine & linev,
		mlFloat fLineWidth);
	gxRenderingLine(
		const gxColor & colorv,
		const mlVector3D & av,
		const mlVector3D & bv);
	gxRenderingLine(
		const gxColor & colorv,
		const mlVector3D & av,
		const mlVector3D & bv,
		mlFloat fLineWidth);

	gxRenderingLine Inverse(void);

	void Render();

	gxColor color;
	
	mlFloat m_fLineWidth;
};

#endif // GX_RENDERINGLINE_H
