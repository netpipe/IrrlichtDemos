
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "EnhancedCameraWorld.h"

#include "GX_TextureArchive.h"
#include "GX_RenderingTriangle.h"

mlTransform * g_pTheCamera = 0;

// Start off with chopping boxes


EnhancedCameraWorld::EnhancedCameraWorld() : World3D()
{
	m_bPickAppScene = true;

	m_fOrbitInterpolate = 0.2f;

	//m_fFieldOfView = 45.0f;
	m_fFieldOfView = 90.0f;

	extern int g_nGXModelReserveSize;

	g_nGXModelReserveSize = 10;

	g_pTheCamera = &m_trnCamera;

	m_trnCamera.SetTranslation(mlVector3D(162,66,35));
	LookAt(mlVector3D(144,60,45));
}

EnhancedCameraWorld::~EnhancedCameraWorld()
{
	g_pTheCamera = 0;
}

void EnhancedCameraWorld::UpdateSimulation(float fDelta)
{
	if(m_bUpArrowKeyDown || m_bRightArrowKeyWentDown)
	{
	}

	if(m_bDownArrowKeyDown || m_bLeftArrowKeyWentDown)
	{
	}
}

void EnhancedCameraWorld::PreRender()
{
	// faffing around with light
	//glEnable(GL_LIGHTING);
	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	//glLighti(0, GL_CONSTANT_ATTENUATION, 0);
	//glLighti(0, GL_LINEAR_ATTENUATION, 0);
	//glLighti(0, GL_QUADRATIC_ATTENUATION, 0);

	//glLighti(0, GL_SPOT_CUTOFF, 180);
	//glLighti(0, GL_SPOT_EXPONENT, 0);

	glEnable (GL_POLYGON_OFFSET_FILL);
	glPolygonOffset (1., 1.);
}

void EnhancedCameraWorld::RenderScene()
{
}

void EnhancedCameraWorld::OnKeyDown(unsigned char nKey, int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		return;
	}
	
	if(nKey == ' ')
	{
		m_bSpacebarDown = true;
		m_bSpacebarWentDown = true;
	}
}

void EnhancedCameraWorld::WheelUpBehaviour()
{
	float fStepSize = 5.0f;

	mlVector3D vCamera = GetCameraTransform()->GetTranslation();
	mlVector3D vPick = m_vMousePickPosition;
	float fDistance = (vPick - vCamera).Magnitude();
	fStepSize = fDistance * 0.1f;

	fStepSize = mlClamp(fStepSize, -20.0f, 20.0f);

	//mlVector3D vForward = m_trnCamera.TransformVector(mlVector3D(0,0,-fStepSize));
	mlVector3D vForward = vCamera - vPick;
	vForward.Normalise();
	vForward *= -fStepSize;

	m_trnCamera.ApplyTranslation(vForward);
}

void EnhancedCameraWorld::WheelDownBehaviour()
{
	float fStepSize = 5.0f;

	mlVector3D vCamera = GetCameraTransform()->GetTranslation();
	mlVector3D vPick = m_vMousePickPosition;
	float fDistance = (vPick - vCamera).Magnitude();
	fStepSize = fDistance * 0.1f;

	fStepSize = mlClamp(fStepSize, -20.0f, 20.0f);

	//mlVector3D vBackward = m_trnCamera.TransformVector(mlVector3D(0,0,fStepSize));
	mlVector3D vBackward = vCamera - vPick;
	vBackward.Normalise();
	vBackward *= fStepSize;

	m_trnCamera.ApplyTranslation(vBackward);
}

void EnhancedCameraWorld::OnMouseMove(int nX, int nY)
{
	if(!IsPointInViewport(nX, nY) && !m_bLeftMouseDown && !m_bRightMouseDown)
	{
		return;
	}

	UpdateMouseCoordinates(nX, nY);

	if(m_bMouseMovesCamera)
	{
		if(m_bRightMouseDown && m_bMousePansCamera)
		{
			PanCamera(m_vMouse2DPositionDelta.x, m_vMouse2DPositionDelta.y);
		}

		if(m_bLeftMouseDown && m_bMouseOrbitsCamera)
		{
			if(m_bSpacebarDown)
			{
				CustomOrbitCamera(m_vMousePickPosition, m_vMouse2DPositionDelta.x, m_vMouse2DPositionDelta.y);
			}
			else
			{
				RotateCamera(m_vMouse2DPositionDelta.x, m_vMouse2DPositionDelta.y);
			}
		}
	}
}

void EnhancedCameraWorld::CustomOrbitCamera(mlVector3D & vCenter, float fHeading, float fPitch)
{
	// If the proposed angle is to acute, do nuzzing
	if(false)
	{
		mlVector3D vGlobalDown(0,-1,0);
		mlVector3D vGlobalUp(0,1,0);
		mlVector3D vForward = vCenter - m_trnCamera.GetTranslation();
		mlVector3D vCameraForward = m_trnCamera.TransformVector(mlVector3D(0,0,-1));

		float fNewAngleToDown = 0.0f;
		{
			// Angle between two vectors...
			mlVector3D xyNorm = vGlobalDown.Normalised();
			mlVector3D xzNorm = vForward.Normalised();
			float fDot = xyNorm * xzNorm;
			float fAngle = mlArcCos(fDot);
			fNewAngleToDown = fAngle;
		}

		float fNewAngleToUp = 0.0f;
		{
			// Angle between two vectors...
			mlVector3D xyNorm = vGlobalUp.Normalised();
			mlVector3D xzNorm = vForward.Normalised();
			float fDot = xyNorm * xzNorm;
			float fAngle = mlArcCos(fDot);
			fNewAngleToUp = fAngle;
		}

		float fCurrentAngleToDown = 0.0f;
		{
			// Angle between two vectors...
			mlVector3D xyNorm = vGlobalDown.Normalised();
			mlVector3D xzNorm = vCameraForward.Normalised();
			float fDot = xyNorm * xzNorm;
			float fAngle = mlArcCos(fDot);
			fCurrentAngleToDown = fAngle;
		}

		float fCurrentAngleToUp = 0.0f;
		{
			// Angle between two vectors...
			mlVector3D xyNorm = vGlobalUp.Normalised();
			mlVector3D xzNorm = vCameraForward.Normalised();
			float fDot = xyNorm * xzNorm;
			float fAngle = mlArcCos(fDot);
			fCurrentAngleToUp = fAngle;
		}

		float fPitchAngleLimit = 10.0f;

		if(mlFabs(mlRadiansToDegrees(fNewAngleToDown)) < fPitchAngleLimit && mlFabs(fNewAngleToDown) < mlFabs(fCurrentAngleToDown))
		{
			return;
		}

		if(mlFabs(mlRadiansToDegrees(fNewAngleToUp)) < fPitchAngleLimit && mlFabs(fNewAngleToUp) < mlFabs(fCurrentAngleToUp))
		{
			return;
		}
	}

	// Make a quaternion to represent vector to the focal point (Y is up)
	mlQuaternion rotToPoint;
	{
		mlVector3D vForward = vCenter - m_trnCamera.GetTranslation();

		vForward.Normalise();

		rotToPoint = mlQuaternionFromDirection(vForward, mlVector3D(0.0f, 1.0f, 0.0f));
		rotToPoint.Normalise();
	}

	//// Get the orbital radius
	//float fRadius = (vCenter - m_trnCamera.GetTranslation()).Magnitude();

	//// Move the camera to the center, maintaining the current rotation
	//m_trnCamera.SetTranslation(vCenter);

	// Rotate the quaternion according to the specified heading and pitch change.
	// (Y is up)
	mlQuaternion rotToPointChanged = rotToPoint;
	{
		// This is a magic number which maps pixels of mouse motion to radians of rotation
		float fRotationSpeed = 0.008f;
		//float fRotationSpeed = 0.016f;

		rotToPointChanged = rotToPointChanged * mlQuaternion(mlVector3D(0,1,0), fRotationSpeed * -fHeading);

		// For some reason, you musn't use the local rotation vector...
		//mlVector3D vSide = rotToPoint.TransformVector(mlVector3D(1,0,0));
		mlVector3D vSide = mlVector3D(1,0,0);

		rotToPointChanged = rotToPointChanged * mlQuaternion(vSide, fRotationSpeed * fPitch);

		rotToPointChanged.Normalise();
	}

	// If the proposed angle is to acute, do nuzzing
	//if(false)
	{
		mlVector3D vGlobalDown(0,-1,0);
		mlVector3D vGlobalUp(0,1,0);
		mlVector3D vLookForward = vCenter - m_trnCamera.GetTranslation();
		mlVector3D vLookMoveForward = rotToPointChanged.TransformVector(mlVector3D(0,0,-1));

		float fNewAngleToDown = 0.0f;
		{
			// Handy function! Angle between two vectors...
			mlVector3D xyNorm = vGlobalDown.Normalised();
			mlVector3D xzNorm = vLookMoveForward.Normalised();
			float fDot = xyNorm * xzNorm;
			float fAngle = mlArcCos(fDot);
			fNewAngleToDown = fAngle;
		}

		float fNewAngleToUp = 0.0f;
		{
			// Handy function! Angle between two vectors...
			mlVector3D xyNorm = vGlobalUp.Normalised();
			mlVector3D xzNorm = vLookMoveForward.Normalised();
			float fDot = xyNorm * xzNorm;
			float fAngle = mlArcCos(fDot);
			fNewAngleToUp = fAngle;
		}

		float fCurrentAngleToDown = 0.0f;
		{
			// Handy function! Angle between two vectors...
			mlVector3D xyNorm = vGlobalDown.Normalised();
			mlVector3D xzNorm = vLookForward.Normalised();
			float fDot = xyNorm * xzNorm;
			float fAngle = mlArcCos(fDot);
			fCurrentAngleToDown = fAngle;
		}

		float fCurrentAngleToUp = 0.0f;
		{
			// Handy function! Angle between two vectors...
			mlVector3D xyNorm = vGlobalUp.Normalised();
			mlVector3D xzNorm = vLookForward.Normalised();
			float fDot = xyNorm * xzNorm;
			float fAngle = mlArcCos(fDot);
			fCurrentAngleToUp = fAngle;
		}

		float fPitchAngleLimit = 10.0f;

		if(mlFabs(mlRadiansToDegrees(fNewAngleToDown)) < fPitchAngleLimit && mlFabs(fNewAngleToDown) < mlFabs(fCurrentAngleToDown))
		{
			return;
		}

		if(mlFabs(mlRadiansToDegrees(fNewAngleToUp)) < fPitchAngleLimit && mlFabs(fNewAngleToUp) < mlFabs(fCurrentAngleToUp))
		{
			return;
		}
	}

	// Get the orbital radius
	float fRadius = (vCenter - m_trnCamera.GetTranslation()).Magnitude();

	// Move the camera to the center, maintaining the current rotation
	m_trnCamera.SetTranslation(vCenter);

	// Now the rotation quaternion is ready, and may be used to make the push back vector
	mlVector3D vPushBack = rotToPointChanged.TransformVector(mlVector3D(0, 0, -fRadius));

	// Push back the camera. Note that the rotation still hasn't altered.
	// The "topoint" rotation was used to make the push back vector (just a reminder)
	m_trnCamera.SetTranslation(vCenter - vPushBack);

	// Now we are free to modify the rotation of the camera
	// We can either make it point to the point,
	// Or lerp is to that rotation.
	// However, I want to lock in the fact that the camera has locked on the center
	// so repeated adjustments don't need to be made (using the m_fOrbitInterpolate variable)
	{
		mlVector3D vForward = vCenter - m_trnCamera.GetTranslation();

		vForward.Normalise();

		mlQuaternion rotNewLookAt = mlQuaternionFromDirection(vForward, mlVector3D(0.0f, 1.0f, 0.0f));

		mlQuaternion rotCurrent = m_trnCamera.GetRotation();

		// Need relationship between rotCurrent and rotToPoint

		mlQuaternion rotLookAtInv = rotToPoint;
		rotLookAtInv.Invert();
		//mlQuaternion rotCurrentInv = rotCurrent;
		//rotCurrentInv.Invert();

		//mlQuaternion rotDiff = rotCurrent * rotLookAtInv;
		mlQuaternion rotDiff = rotLookAtInv * rotCurrent;
		// rotLookAt * rotX = rotCurrent;
		// rotX = rotCurrent * rotLookAtInv;

		mlQuaternion rotNewCurrent = rotNewLookAt * rotDiff;

		//float fRate = 0.2f;

		//if(mlFabs(fHeading) + mlFabs(fPitch) > 0.0f)
		//{
		//	fRate = 1.0f;
		//}


		//mlQuaternion rotResult = mlQuaternionInterpolate(fRate, rotOrig, rotLookAt);
		//mlQuaternion rotResult = mlQuaternionInterpolateCubic(fRate, rotOrig, rotLookAt, rotToPoint);

		//mlQuaternion rotResult = mlQuaternionInterpolate(m_fOrbitInterpolate, rotOrig, rotLookAt);
		mlQuaternion rotResult = mlQuaternionInterpolate(0.2f, rotNewCurrent, rotNewLookAt);

		m_trnCamera.SetRotation(rotResult);
	}

	// Another idea: get the rotation that rotates the old lookat rotation to the old rotation
	// Use that to change the current lookat rotation to provide a starting point for the
	// lerping.

	m_trnCamera.Normalise();
}

void EnhancedCameraWorld::Update(float fDelta)
{
	if(m_bMouseMovesCamera)
	{
		if(m_bLeftMouseDown && m_bMouseFocussesCamera)
		{
			if(m_bSpacebarDown)
			{
				m_fOrbitInterpolate += 0.05f;
				if(m_fOrbitInterpolate > 1.0f) m_fOrbitInterpolate = 1.0f;

				CustomOrbitCamera(m_vMousePickPosition, 0.0f, 0.0f);
			}
		}
	}

	UpdateSimulation(fDelta);

	m_bUpArrowKeyWentDown = false;
	m_bDownArrowKeyWentDown = false;
	m_bLeftArrowKeyWentDown = false;
	m_bRightArrowKeyWentDown = false;

	m_bSpacebarWentDown = false;

	m_bLeftMouseWentDown = false;
	m_bRightMouseWentDown = false;
	m_bMiddleMouseWentDown = false;
}

