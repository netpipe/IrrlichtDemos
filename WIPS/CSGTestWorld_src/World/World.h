
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef WORLD_H
#define WORLD_H

#include "ML_Transform.h"

#include "GX_RenderingTriangle.h"

//#include "GL/glut.h"
#include "GL/glut.h"

class gxModel;

class World
{
public:
	World();
	virtual ~World();

	virtual void LoadTextures();

	int m_nID;

	virtual void Update(float fDelta);
	virtual void Render();

	void Render_NoViewportSetting();

	virtual void PreRender();
	virtual void PostRender();

	virtual void PreUpdate();
	virtual void PostUpdate();

	virtual void PostRenderUpdate();

	virtual void OnMouseMove(int nX, int nY);
	virtual void OnPassiveMouseMove(int nX, int nY);

	virtual void OnKeyDown(unsigned char nKey, int nX, int nY);
	virtual void OnKeyUp(unsigned char nKey, int nX, int nY);

	virtual void OnKeyUpSpecial(int nKey, int nX, int nY);
	virtual void OnKeyDownSpecial(int nKey, int nX, int nY);

	virtual void OnLButtonDown(int nX, int nY);
	virtual void OnLButtonUp(int nX, int nY);

	virtual void OnMButtonDown(int nX, int nY);
	virtual void OnMButtonUp(int nX, int nY);

	virtual void OnRButtonDown(int nX, int nY);
	virtual void OnRButtonUp(int nX, int nY);

	virtual void OnWheelUp(int nX, int nY);
	virtual void OnWheelDown(int nX, int nY);

	virtual void Reshape(int nX, int nY, int nWidth, int nHeight, int nWindowWidth, int nWindowHeight);

	bool IsPointInViewport(int nX, int nY);

	// Accessor functions for the viewport
	int GetViewportWidth();
	int GetViewportHeight();
	int GetViewportLeft();
	int GetViewportBottom();

	// Other functions will be handy - Getting the transform for instance
	//void GetCameraTransform(mlTransform & trnCamera);
	mlTransform * GetCameraTransform();

	// and more as the need arises...

	virtual void PanCamera(float fX, float fY);
	virtual void RotateCamera(float fHeading, float fPitch);
	virtual void OrbitCamera(mlVector3D & vCenter, float fHeading, float fPitch);
	virtual void LookAt(mlVector3D vFocalPoint);
	virtual void LookDown();
	virtual void LookDown_Alternate();

	virtual void WheelUpBehaviour();
	virtual void WheelDownBehaviour();

protected:

	virtual void UpdateSimulation(float fDelta);
	virtual void RenderScene();

//private:

	void ResetModelviewMatrix();
	void RebuildModelviewMatrix();

	void ResetProjectionMatrix();
	void RebuildProjectionMatrix();
	void RebuildProjectionMatrixForPicking();
	virtual void ApplyProjectionMatrix();
	void ApplyPickMatrix();

	void ClearViewport();
	void SwapBuffers();

	void RenderFeedbackModel();

	void RefreshViewport();
	void MakeFeedbackModel();

	void RefreshPickPosition();
	void RefreshMousePosition();

	void Convert2Dto3D(mlVector3D & v2DPosition, mlVector3D & v3DPosition, mlVector3D & v3DRay);

	// camera

	void UpdateMouseCoordinates(int nX, int nY);

	virtual void RenderFloorBox();
	virtual void RenderFloorGrid();

	// I've been making everything virtual...
	void SimpleGlutBitmapString( void *font, const char *s );
	void RenderText(const mlVector3D & vPosition, char * text);

	mlTransform m_trnCamera;

	gxModel * m_pJet;

	float * m_pFeedbackBuffer;
	gxModel * m_pFeedbackModel;

	mlVector3D m_vMouse2DPosition;
	mlVector3D m_vMouse3DPosition;
	mlVector3D m_vMouseRay;
	mlVector3D m_vMousePickPosition;

	mlVector3D m_vMouse2DPositionDelta;

	GLint m_Viewport[4];

	int m_nWindowWidth;
	int m_nWindowHeight;

	bool m_bUpdatePickPosition;
	int m_nGridSizeX;
	int m_nGridSizeY;
	float m_fGridCellWidth;

	float m_fFieldOfView;

	bool m_bPickAppScene;

	// UI state variables
	bool m_bLeftMouseDown;
	bool m_bMiddleMouseDown;
	bool m_bRightMouseDown;

	bool m_bUpArrowKeyDown;
	bool m_bDownArrowKeyDown;
	bool m_bLeftArrowKeyDown;
	bool m_bRightArrowKeyDown;

	bool m_bUpArrowKeyWentDown;
	bool m_bDownArrowKeyWentDown;
	bool m_bLeftArrowKeyWentDown;
	bool m_bRightArrowKeyWentDown;

	bool m_bSpacebarDown;
	bool m_bSpacebarWentDown;

	bool m_bLeftMouseWentDown;
	bool m_bRightMouseWentDown;
	bool m_bMiddleMouseWentDown;

	gxRenderingTriangle m_triangle1;
	gxRenderingTriangle m_triangle2;

	bool m_bRenderingPickScene;
	bool m_bRenderFeedbackModel;
	int m_nFeedbackFrustumSize;

	bool m_bRenderMousePickSphere;

	bool m_bRenderFloorGrid;

	bool m_bMouseMovesCamera;
	bool m_bMouseOrbitsCamera;
	bool m_bMousePansCamera;
	bool m_bMouseFocussesCamera;

	bool m_bUpdateMousePosition;
	bool m_bRenderFloorBox;
};

#endif // WORLD_H
