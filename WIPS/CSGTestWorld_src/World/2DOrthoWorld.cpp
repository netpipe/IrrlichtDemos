
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "2DOrthoWorld.h"

#include "GX_RenderingLine.h"
#include "GX_RenderingTriangle.h"

#include "GX_Model.h"

#include "GX_ModelLibrary.h"
#include "GX_ModelUtil.h"

#include "GX_Intersection.h"

// How the 2D world is intended to work
// Panning and zooming, possibly rotating
// Orthographic projection
//
// Or stick with perspective projection and constrain the camera.
// This confirms I can use orthogonal view

OrthoWorld::OrthoWorld() : World()
{
	m_nID = 2;

	m_nGridSizeX = 30;
	m_nGridSizeY = 40;
	m_fGridCellWidth = 10.0f;

	m_fOrthoSize = m_nGridSizeY * m_fGridCellWidth;

	m_trnCamera.SetTranslation(
		mlVector3D(
			(m_nGridSizeX) * m_fGridCellWidth * 0.5f,
			200.0f,
			(m_nGridSizeY) * m_fGridCellWidth * 0.5f));
	LookDown();

	m_vMouse2DPosition = mlVector3D(100, 100, 0);
}

OrthoWorld::~OrthoWorld()
{
}

void OrthoWorld::ApplyProjectionMatrix()
{
	// Assumes GL_PROJECTION matrix mode

	int nWindowWidth = m_Viewport[2];
	int nWindowHeight = m_Viewport[3];
	
	GLdouble fAspect = GLdouble(nWindowWidth) / GLdouble(nWindowHeight);

	mlFloat fSize = 80.0f;

	mlFloat fHalfOrthoSize = m_fOrthoSize * 0.5f;

	mlFloat fHalfOrthoSizeAspected = fHalfOrthoSize * fAspect;

	//gluPerspective(45.0, fAspect, 0.1f, 20000.0f);
	//glOrtho(-fSize, fSize, -fSize, fSize, 1.0f, 1000.0f);
	//glOrtho(-fSize, fSize, -fSize, fSize, 1.0f, 60.0f);

	//glOrtho(-fHalfOrthoSizeAspected, fHalfOrthoSizeAspected, -fHalfOrthoSize, fHalfOrthoSize, 1.0f, 60.0f);
	glOrtho(-fHalfOrthoSizeAspected, fHalfOrthoSizeAspected, -fHalfOrthoSize, fHalfOrthoSize, 1.0f, 1000.0f);
}

// This is the OrthoWorld customisation of the PanCamera.

void OrthoWorld::PanCamera(float fX, float fY)
{
	// get the picked position, make a camera plane at this point.
	// move the mouse position accorind to fX and fY, cast a ray to intersect the camera plane

	mlVector3D vMouse2DA;
	mlVector3D vMouse2DB;

	vMouse2DA = m_vMouse2DPosition - m_vMouse2DPositionDelta;
	vMouse2DB = m_vMouse2DPosition;

	mlVector3D vMouse3DA, vMouse3DRayA;
	mlVector3D vMouse3DB, vMouse3DRayB;

	Convert2Dto3D(vMouse2DA, vMouse3DA, vMouse3DRayA);
	Convert2Dto3D(vMouse2DB, vMouse3DB, vMouse3DRayB);

	//mlTriangle triObjectCameraPlane(
	//	m_vMousePickPosition,
	//	m_vMousePickPosition + m_trnCamera.TransformVector(mlVector3D(0,0,-20)),
	//	m_vMousePickPosition + m_trnCamera.TransformVector(mlVector3D(0,20,0)));


	////mlTriangle triObjectCameraPlane2(m_vMouseRay, m_vMousePickPosition, 20.0f);
	//mlTriangle triObjectCameraPlane2(m_trnCamera.TransformVector(mlVector3D(0,0,-1)), mlVector3D(0.0f, 1.0f, 0.0f), m_vMousePickPosition, 20.0f);

	//m_triangle1 = gxRenderingTriangle(gxColorBlue, triObjectCameraPlane);
	//m_triangle2 = gxRenderingTriangle(gxColorRed, triObjectCameraPlane2);

	mlVector3D vDelta = m_triangle2.ProjectRay(vMouse3DB, vMouse3DRayB) - m_vMousePickPosition;
	//mlVector3D vDelta = triObjectCameraPlane2.ProjectRay(vMouse3DB, m_trnCamera.TransformVector(mlVector3D(0,0,-1))) - m_vMousePickPosition;

	mlTransform transform;
	mlConvertTriangleToTransform(m_triangle2, transform);

	vDelta = transform.TransformVectorInverse(vDelta);
	//vDelta = m_trnCamera.TransformVectorInverse(vDelta);

	//float fSpeed = 0.5f;

	//mlVector3D vMovementX = m_trnCamera.TransformVector(mlVector3D(fSpeed * -fX,0, 0));
	mlVector3D vMovementX = m_trnCamera.TransformVector(mlVector3D(-vDelta.x,0, 0));
	m_trnCamera.ApplyTranslation(vMovementX);

	//mlVector3D vMovementY = m_trnCamera.TransformVector(mlVector3D(0,fSpeed * -fY,0));
	mlVector3D vMovementY = m_trnCamera.TransformVector(mlVector3D(0,-vDelta.y,0));
	m_trnCamera.ApplyTranslation(vMovementY);
}

void OrthoWorld::RotateCamera(float fHeading, float fPitch)
{
	float fRotationSpeed = 0.008f;
	//float fRotationSpeed = 0.016f;

	m_trnCamera.ApplyRotation(mlQuaternion(mlVector3D(0,1,0), fRotationSpeed * -fHeading));

	mlVector3D vSide = m_trnCamera.TransformVector(mlVector3D(1,0,0));

	m_trnCamera.ApplyRotation(mlQuaternion(vSide, fRotationSpeed * fPitch));
}

void OrthoWorld::OrbitCamera(mlVector3D & vCenter, float fHeading, float fPitch)
{
	//mlVector3D vToPoint = vCenter - m_trnCamera.GetTranslation();
	mlVector3D vToPoint = m_trnCamera.TransformPointInverse(vCenter);

	//float fRadius = vToPoint.Magnitude();

	m_trnCamera.SetTranslation(vCenter);

	RotateCamera(fHeading, 0.0f);

	//mlVector3D vPushBack = m_trnCamera.TransformVector(mlVector3D(0, 0, -fRadius));

	vToPoint.x *= 0.9f;
	vToPoint.y *= 0.9f;

	//float fDesiredDistance = -40.0f;
	float fDesiredDistance = -200.0f;

	vToPoint.z = vToPoint.z - ((vToPoint.z - fDesiredDistance) * 0.05f);

	mlVector3D vPushBack = m_trnCamera.TransformVector(vToPoint);

	m_trnCamera.SetTranslation(vCenter - vPushBack);

	m_trnCamera.Normalise();
}

void OrthoWorld::LookAt(mlVector3D vFocalPoint)
{
	mlVector3D vForward = vFocalPoint - m_trnCamera.GetTranslation();

	vForward.Normalise();

	mlQuaternion rotLookAt = mlQuaternionFromDirection(vForward, mlVector3D(0.0f, 1.0f, 0.0f));

	m_trnCamera.SetRotation(rotLookAt);
}

void OrthoWorld::LookDown()
{
	mlMatrix3x3 mLookDown;
	//mLookDown.I = mlVector3D(1.0f, 0.0f, 0.0f);
	//mLookDown.J = mlVector3D(0.0f, 0.0f, -1.0f);
	//mLookDown.K = mlVector3D(0.0f, 1.0f, 0.0f);

	mLookDown.I = mlVector3D(0.0f, 1.0f, 0.0f);
	mLookDown.J = mlVector3D(0.0f, 0.0f, -1.0f);
	mLookDown.K = mlVector3D(1.0f, 0.0f, 0.0f);

	mlQuaternion qRot = mlQuaternionFromRotationMatrix(mLookDown);

	m_trnCamera.SetRotation(qRot);
}

void OrthoWorld::WheelUpBehaviour()
{
	float fStepSize = 5.0f;

	m_fOrthoSize -= fStepSize;

	m_fOrthoSize = mlClamp(m_fOrthoSize, 10.0f, 500.0f);
}

void OrthoWorld::WheelDownBehaviour()
{
	float fStepSize = 5.0f;

	m_fOrthoSize += fStepSize;

	m_fOrthoSize = mlClamp(m_fOrthoSize, 10.0f, 500.0f);
}

