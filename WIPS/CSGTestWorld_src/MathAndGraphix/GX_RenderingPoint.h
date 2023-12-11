
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef GX_RENDERINGPOINT_H
#define GX_RENDERINGPOINT_H

#include "ML_Vector.h"

#include "GX_Color.h"

class gxRenderingPoint : public mlVector3D
{
public:

	gxRenderingPoint();
	gxRenderingPoint(
		const gxColor & colorv,
		const mlVector3D & pointv);

	void Render();
	void RenderAsSphere(mlFloat radius);
	void RenderAsScreenSphere(int nRadius);
	void RenderAsCube(mlFloat sideLength);
	void RenderAsSolidSphere(mlFloat radius);

	gxColor color;
};

#endif // GX_RENDERINGPOINT_H
