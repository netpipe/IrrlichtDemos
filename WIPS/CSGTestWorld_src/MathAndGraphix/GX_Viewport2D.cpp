
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_Viewport2D.h"

#include "ML_Maths.h"

gxViewport2D::gxViewport2D()
{
	m_screenBottomLeft	= mlVector2DZero;
	m_screenTopRight	= mlVector2DZero;

	m_realBottomLeft	= mlVector2DZero;
	m_realTopRight		= mlVector2DZero;

	m_wrap = false;
}

gxViewport2D::~gxViewport2D()
{
}

mlVector2D gxViewport2D::RealToScreen(const mlVector2D & realPoint)
{
	mlVector2D pointToMap = realPoint;

	if(m_wrap)
		pointToMap = Normalise(pointToMap);

	mlVector2D realRange = mlVector2D(
		(m_realTopRight.x - m_realBottomLeft.x),
		(m_realTopRight.y - m_realBottomLeft.y));

	mlVector2D proportion = mlVector2D(
		(pointToMap.x - m_realBottomLeft.x) / realRange.x,
		(pointToMap.y - m_realBottomLeft.y) / realRange.y);

	mlVector2D screenRange = mlVector2D(
		(m_screenTopRight.x - m_screenBottomLeft.x),
		(m_screenBottomLeft.y - m_screenTopRight.y));

	mlVector2D screenOffset = mlVector2D(m_screenBottomLeft.x, m_screenBottomLeft.y);

	mlVector2D mappedVector = mlVector2D(
		screenOffset.x + proportion.x * screenRange.x,
		screenOffset.y - proportion.y * screenRange.y);

	return mappedVector;
}

mlVector2D gxViewport2D::ScreenToReal(const mlVector2D & screenPoint)
{
	mlVector2D realRange = mlVector2D(
		(m_realTopRight.x - m_realBottomLeft.x),
		(m_realTopRight.y - m_realBottomLeft.y));

	mlVector2D screenRange = mlVector2D(
		(m_screenTopRight.x - m_screenBottomLeft.x),
		(m_screenBottomLeft.y - m_screenTopRight.y));

	mlVector2D realPixelWidth = mlVector2D(realRange.x / screenRange.x, realRange.y / screenRange.y);

	mlVector2D mappedVector = mlVector2D(
		(screenPoint.x - m_screenBottomLeft.x) * realPixelWidth.x + m_realBottomLeft.x,
		m_realTopRight.y - (screenPoint.y - m_screenTopRight.y) * realPixelWidth.y);

	return mappedVector;
}

mlVector2D gxViewport2D::Normalise(const mlVector2D & realPoint)
{
	mlVector2D normalisedPoint = realPoint;

	mlVector2D realRange = mlVector2D(
		(m_realTopRight.x - m_realBottomLeft.x),
		(m_realTopRight.y - m_realBottomLeft.y));

	normalisedPoint = mlVector2D(realPoint.x - m_realBottomLeft.x, realPoint.y - m_realBottomLeft.y);

	normalisedPoint = mlVector2D(mlFmodUniform(normalisedPoint.x, realRange.x), mlFmodUniform(normalisedPoint.y, realRange.y));

	normalisedPoint = mlVector2D(normalisedPoint.x + m_realBottomLeft.x, normalisedPoint.y + m_realBottomLeft.y);

	return normalisedPoint;
}
