
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef ENHANCEDCAMERAWORLD_H
#define ENHANCEDCAMERAWORLD_H

#include "3DWorld.h"

class EnhancedCameraWorld : public World3D
{
public:
	EnhancedCameraWorld();
	virtual ~EnhancedCameraWorld();

	virtual void UpdateSimulation(float fDelta);
	virtual void RenderScene();

	virtual void PreRender();
	virtual void Update(float fDelta);

	void CustomOrbitCamera(mlVector3D & vCenter, float fHeading, float fPitch);

	virtual void OnMouseMove(int nX, int nY);
	virtual void WheelUpBehaviour();
	virtual void WheelDownBehaviour();

	virtual void OnKeyDown(unsigned char nKey, int nX, int nY);

	float m_fOrbitInterpolate;

private:
	
};

#endif // ENHANCEDCAMERAWORLD_H

