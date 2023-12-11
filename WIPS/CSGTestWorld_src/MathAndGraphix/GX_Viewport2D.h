
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef GX_VIEWPORT2D_H
#define GX_VIEWPORT2D_H

#include "ML_Vector.h"

class gxViewport2D  
{
public:
	 gxViewport2D();
	~gxViewport2D();

	mlVector2D RealToScreen(const mlVector2D & realPoint);
	mlVector2D ScreenToReal(const mlVector2D & screenPoint);

	mlVector2D Normalise(const mlVector2D & realPoint);

	void SetScreenViewport(const mlVector2D & bottomLeft, const mlVector2D & topRight);
	void SetRealViewport(const mlVector2D & bottomLeft, const mlVector2D & topRight);

	mlVector2D	GetScreenBottomLeft();
	void		SetScreenBottomLeft(const mlVector2D & position);

	mlVector2D	GetScreenTopRight();
	void		SetScreenTopRight(const mlVector2D & position);

	mlVector2D	GetRealBottomLeft();
	void		SetRealBottomLeft(const mlVector2D & position);

	mlVector2D	GetRealTopRight();
	void		SetRealTopRight(const mlVector2D & position);

	bool		GetWrapOption();
	void		SetWrapOption(bool option);

private:

	mlVector2D	m_screenBottomLeft;
	mlVector2D	m_screenTopRight;

	mlVector2D	m_realBottomLeft;
	mlVector2D	m_realTopRight;

	bool		m_wrap;
};

inline void gxViewport2D::SetScreenViewport(const mlVector2D & bottomLeft, const mlVector2D & topRight)
{
	m_screenBottomLeft = bottomLeft;
	m_screenTopRight = topRight;
}

inline void gxViewport2D::SetRealViewport(const mlVector2D & bottomLeft, const mlVector2D & topRight)
{
	m_realBottomLeft = bottomLeft;
	m_realTopRight = topRight;
}

inline mlVector2D gxViewport2D::GetScreenBottomLeft()
{
	return m_screenBottomLeft;
}

inline void gxViewport2D::SetScreenBottomLeft(const mlVector2D & position)
{
	m_screenBottomLeft = position;
}

inline mlVector2D gxViewport2D::GetScreenTopRight()
{
	return m_screenTopRight;
}

inline void gxViewport2D::SetScreenTopRight(const mlVector2D & position)
{
	m_screenTopRight = position;
}

inline mlVector2D gxViewport2D::GetRealBottomLeft()
{
	return m_realBottomLeft;
}

inline void gxViewport2D::SetRealBottomLeft(const mlVector2D & position)
{
	m_realBottomLeft = position;
}

inline mlVector2D gxViewport2D::GetRealTopRight()
{
	return m_realTopRight;
}

inline void gxViewport2D::SetRealTopRight(const mlVector2D & position)
{
	m_realTopRight = position;
}

inline bool gxViewport2D::GetWrapOption()
{
	return m_wrap;
}

inline void gxViewport2D::SetWrapOption(bool option)
{
	m_wrap = option;
}

#endif // GX_VIEWPORT2D_H
