
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ML_Vector.h"
#include "ML_Transform.h"

#include "GX_Color.h"

class mlDisc
{
public:
	mlDisc() :
		m_vNormal(mlVector3D(0,1,0)),
		m_vCenter(mlVector3D(0,0,0)),
		m_fRadius(1.0f)
	{
	}

	mlDisc(mlFloat i_fRadius, const mlVector3D & i_vCenter, const mlVector3D & i_vNormal) :
		m_vNormal(i_vNormal),
		m_vCenter(i_vCenter),
		m_fRadius(i_fRadius)
	{
	}

	mlDisc(const mlVector3D & i_vPointA, const mlVector3D & i_vPointB, const mlVector3D & i_vPointC);
	
	void Render(const gxColor & color, bool bRenderSpokes);

	mlVector3D	m_vNormal;
	mlVector3D	m_vCenter;
	mlFloat		m_fRadius;
};

mlDisc MakeTouchingDisc1(
	mlTransform & i_tVehicleTransform,
	mlDisc & i_oTouchDisc);

mlDisc MakeTouchingDisc2(
	mlTransform & i_tVehicleTransform,
	mlDisc & i_oTouchDisc);
