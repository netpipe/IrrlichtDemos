
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef ORTHOWORLD_H
#define ORTHOWORLD_H

#include "ML_Transform.h"

#include "GX_RenderingTriangle.h"

#include "GL/glut.h"

#include "World.h"

class OrthoWorld : public World
{
public:
	OrthoWorld();
	virtual ~OrthoWorld();

	//void Update(float fDelta);
	//void Render();

	virtual void WheelUpBehaviour();
	virtual void WheelDownBehaviour();

	void ApplyProjectionMatrix();

	void PanCamera(float fX, float fY);
	void RotateCamera(float fHeading, float fPitch);
	void OrbitCamera(mlVector3D & vCenter, float fHeading, float fPitch);
	void LookAt(mlVector3D vFocalPoint);
	void LookDown();

	//virtual void RenderFloorBox();
	//virtual void RenderFloorGrid();

	float m_fOrthoSize;
	//int m_nGridSize;
	//float m_fGridCellWidth;
};

#endif // ORTHOWORLD_H
