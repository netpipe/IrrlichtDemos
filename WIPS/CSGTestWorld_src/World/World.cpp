
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "3DWorld.h"

#include "GX_RenderingLine.h"
#include "GX_RenderingTriangle.h"

#include "GX_Model.h"

#include "GX_ModelLibrary.h"
#include "GX_ModelUtil.h"

#include "GX_Intersection.h"

World::World()
{
	m_nID = -1;

	m_pJet = ModelMaker::MakeJetModel();
	//m_pJet = NULL;

	m_pFeedbackModel = new gxModel();
	//m_pFeedbackBuffer = new float[32000];
	m_pFeedbackBuffer = new float[932000];

	m_vMouse2DPosition = mlVector3D(300, 300, 0);

	m_trnCamera.SetTranslation(mlVector3D(50.0f, 50.0f, 300.0f));
	//LookAt(mlVector3D(250,0,250));

	m_bLeftMouseDown = false;
	m_bMiddleMouseDown = false;
	m_bRightMouseDown = false;

	m_bUpArrowKeyDown = false;
	m_bDownArrowKeyDown = false;
	m_bLeftArrowKeyDown = false;
	m_bRightArrowKeyDown = false;

	m_bUpArrowKeyWentDown = false;
	m_bDownArrowKeyWentDown = false;
	m_bLeftArrowKeyWentDown = false;
	m_bRightArrowKeyWentDown = false;

	m_bSpacebarDown = false;
	m_bSpacebarWentDown = false;

	m_bLeftMouseWentDown = false;
	m_bRightMouseWentDown = false;
	m_bMiddleMouseWentDown = false;

	m_bUpdatePickPosition = true;

	m_nWindowWidth = 512;
	m_nWindowHeight = 512;

	m_Viewport[0] = 0;
	m_Viewport[1] = 0;
	m_Viewport[2] = 512;
	m_Viewport[3] = 512;

	m_nGridSizeX = 30;
	m_nGridSizeY = 40;
	m_fGridCellWidth = 10.0f;

	m_fFieldOfView = 45.0f;

	m_bPickAppScene = false;
	m_bRenderingPickScene = false;
	m_bRenderFeedbackModel = false;
	m_nFeedbackFrustumSize = 160;

	m_bRenderMousePickSphere = true;
	m_bRenderFloorGrid = true;
	m_bMouseMovesCamera = true;
	m_bMouseOrbitsCamera = true;
	m_bMousePansCamera = true;
	m_bMouseFocussesCamera = true;
	m_bUpdateMousePosition = true;
	m_bRenderFloorBox = true;
}

World::~World()
{
	delete m_pJet;
	delete m_pFeedbackModel;
	delete [] m_pFeedbackBuffer;
}

int World::GetViewportLeft()
{
	return m_Viewport[0];
}

int World::GetViewportBottom()
{
	return m_Viewport[1];
}

int World::GetViewportWidth()
{
	return m_Viewport[2];
}

int World::GetViewportHeight()
{
	return m_Viewport[3];
}

// Other functions will be handy - Getting the transform for instance
mlTransform * World::GetCameraTransform()
{
	return &m_trnCamera;
}

void World::LoadTextures()
{
}

void World::PreRender()
{
	// At this point, the framebuffer has been cleared, nothing has been rendered yet
	// You can render your own thing here, but you need to clean up after yourself if you don't want it
	// to appear in the frame.

	// This is the place for fancy render to texture stuff
}

void World::PostRender()
{
	// At this point, the frame has been rendered (all views have been rendered)

	// This is the place for maybe rendering to texture the current framebuffer
}

void World::PreUpdate()
{
}

void World::PostUpdate()
{
}

void World::PostRenderUpdate()
{
}

void World::Update(float fDelta)
{
	//float fSpeed = 120.0f;

	//if(m_bUpArrowKeyDown)
	//{
	//	mlVector3D vMovement = m_trnCamera.TransformVector(mlVector3D(0,fSpeed * fDelta,0));
	//	m_trnCamera.ApplyTranslation(vMovement);
	//}

	//if(m_bDownArrowKeyDown)
	//{
	//	mlVector3D vMovement = m_trnCamera.TransformVector(mlVector3D(0,-fSpeed * fDelta,0));
	//	m_trnCamera.ApplyTranslation(vMovement);
	//}

	//if(m_bLeftArrowKeyDown)
	//{
	//	mlVector3D vMovement = m_trnCamera.TransformVector(mlVector3D(-fSpeed * fDelta,0, 0));
	//	m_trnCamera.ApplyTranslation(vMovement);
	//}

	//if(m_bRightArrowKeyDown)
	//{
	//	mlVector3D vMovement = m_trnCamera.TransformVector(mlVector3D(fSpeed * fDelta,0, 0));
	//	m_trnCamera.ApplyTranslation(vMovement);
	//}

	if(m_bMouseMovesCamera)
	{
		if(m_bLeftMouseDown && m_bMouseFocussesCamera)
		{
			//RotateCamera(m_vMouse2DPositionDelta.x, m_vMouse2DPositionDelta.y);

			// Thos only reason this is here in ::Update is so the camera moves when you hold the mouse down and don't move.
			// Note that no delta is specified - if you move, both orbits will be called, but this one won't orbit the camera
			OrbitCamera(m_vMousePickPosition, 0.0f, 0.0f);
		}
	}

	UpdateSimulation(fDelta);

	m_bUpArrowKeyWentDown = false;
	m_bDownArrowKeyWentDown = false;
	m_bLeftArrowKeyWentDown = false;
	m_bRightArrowKeyWentDown = false;

	m_bSpacebarWentDown = false;
	//m_bSpacebarWentDown = m_bSpacebarDown;

	m_bLeftMouseWentDown = false;
	m_bRightMouseWentDown = false;
	m_bMiddleMouseWentDown = false;
}

void World::Render()
{
	RefreshViewport();

	Render_NoViewportSetting();
}

void World::Render_NoViewportSetting()
{
	if(m_bUpdatePickPosition)
	{
		MakeFeedbackModel();
	}

	if(m_bUpdateMousePosition)
	{
		RefreshMousePosition();
	}

	//if(m_bUpdatePickPosition)
	//{
	//	RefreshPickPosition();
	//}

	// This does nothing - multiple viewports now
	//ClearViewport();

	RebuildProjectionMatrix();

	RebuildModelviewMatrix();

	// Rendering begin

	if(m_bRenderFloorGrid)
	{
		RenderFloorGrid();
	}

	//RenderFeedbackModel();

	//m_triangle1.Render();
	//m_triangle2.Render();

	// App rendering
	m_bRenderingPickScene = false;
	RenderScene();

	// Alpha rendering should be done last

	if(m_bRenderFloorBox)
	{
		RenderFloorBox();
	}

	if(m_bUpdatePickPosition)
	{
		RefreshPickPosition();
	}

	RenderFeedbackModel();

	// Rendering end

	// This does nothing - multiple viewports now
	//SwapBuffers();
}

// User interface messages

void World::Reshape(int nX, int nY, int nWidth, int nHeight, int nWindowWidth, int nWindowHeight)
{
	m_nWindowWidth = nWindowWidth;
	m_nWindowHeight = nWindowHeight;

	m_Viewport[0] = nX;
	m_Viewport[1] = nY;
	m_Viewport[2] = nWidth;
	m_Viewport[3] = nHeight;

	RefreshViewport();
}

bool World::IsPointInViewport(int nX, int nY)
{
	//return true;

	nY = m_nWindowHeight - nY;

	int nLeft = m_Viewport[0];
	int nRight = m_Viewport[0] + m_Viewport[2];

	int nTop = m_Viewport[1];
	int nBottom = m_Viewport[1] + m_Viewport[3];

	if(nX > nLeft && nX < nRight && nY > nTop && nY < nBottom)
	//if(nY > nTop && nY < nBottom)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void World::UpdateMouseCoordinates(int nX, int nY)
{
	mlVector3D vOldPosition = m_vMouse2DPosition;

	m_vMouse2DPosition.x = nX;
	m_vMouse2DPosition.y = m_nWindowHeight - nY;

	m_vMouse2DPositionDelta = m_vMouse2DPosition - vOldPosition;
}

void World::OnMouseMove(int nX, int nY)
{
	if(!IsPointInViewport(nX, nY) && !m_bLeftMouseDown && !m_bRightMouseDown)
	{
		return;
	}

	UpdateMouseCoordinates(nX, nY);

	//if(mlFabs(m_vMouse2DPositionDelta.y) > mlFabs(m_vMouse2DPositionDelta.x))
	//{
	//	RotateCamera(m_vMouse2DPositionDelta.y, 0.0f);
	//}
	//else
	//{
	//	RotateCamera(0.0f, m_vMouse2DPositionDelta.x);
	//}

	if(m_bMouseMovesCamera)
	{
		if(m_bRightMouseDown && m_bMousePansCamera)
		{
			PanCamera(m_vMouse2DPositionDelta.x, m_vMouse2DPositionDelta.y);
		}

		if(m_bLeftMouseDown && m_bMouseOrbitsCamera)
		{
			//RotateCamera(m_vMouse2DPositionDelta.x, m_vMouse2DPositionDelta.y);
			OrbitCamera(m_vMousePickPosition, m_vMouse2DPositionDelta.x, m_vMouse2DPositionDelta.y);
		}
	}
}

void World::OnPassiveMouseMove(int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		return;
	}

	UpdateMouseCoordinates(nX, nY);
}

void World::OnKeyUp(unsigned char nKey, int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		//return;
	}
	
	if(nKey == ' ')
	{
		m_bSpacebarDown = false;
	}
}

void World::OnKeyDown(unsigned char nKey, int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		return;
	}
	
	// move the camera to test key repeat option
	
	if(nKey == ' ')
	{
		m_bSpacebarDown = true;
		m_bSpacebarWentDown = true;
	}
	
	int nModifiers = glutGetModifiers();

	if(nModifiers == GLUT_ACTIVE_SHIFT)
	{
		int n = 0;
		n++;
	}

	if(nModifiers == GLUT_ACTIVE_CTRL)
	{
		int n = 0;
		n++;
	}

	if(nModifiers == GLUT_ACTIVE_ALT)
	{
		int n = 0;
		n++;
	}

	if(nModifiers == (GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL))
	{
		int n = 0;
		n++;
	}

	if(nModifiers == (GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_ALT))
	{
		int n = 0;
		n++;
	}
}

void World::OnKeyUpSpecial(int nKey, int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		//return;
	}

	if(nKey == GLUT_KEY_UP)
	{
		m_bUpArrowKeyDown = false;
	}

	if(nKey == GLUT_KEY_DOWN)
	{
		m_bDownArrowKeyDown = false;
	}

	if(nKey == GLUT_KEY_LEFT)
	{
		m_bLeftArrowKeyDown = false;
	}

	if(nKey == GLUT_KEY_RIGHT)
	{
		m_bRightArrowKeyDown = false;
	}
}

void World::OnKeyDownSpecial(int nKey, int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		return;
	}

	if(nKey == GLUT_KEY_UP)
	{
		m_bUpArrowKeyDown = true;
		m_bUpArrowKeyWentDown = true;
	}

	if(nKey == GLUT_KEY_DOWN)
	{
		m_bDownArrowKeyDown = true;
		m_bDownArrowKeyWentDown = true;
	}

	if(nKey == GLUT_KEY_LEFT)
	{
		m_bLeftArrowKeyDown = true;
		m_bLeftArrowKeyWentDown = true;
	}

	if(nKey == GLUT_KEY_RIGHT)
	{
		m_bRightArrowKeyDown = true;
		m_bRightArrowKeyWentDown = true;
	}
}

void World::OnLButtonDown(int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		return;
	}

	m_bLeftMouseDown = true;
	m_bLeftMouseWentDown = true;

	if(m_bMouseMovesCamera)
	{
		m_bUpdatePickPosition = false;
	}
}

void World::OnLButtonUp(int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		//return;
	}

	m_bLeftMouseDown = false;

	if(m_bMouseMovesCamera)
	{
		m_bUpdatePickPosition = true;
	}
}

void World::OnMButtonDown(int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		return;
	}

	m_bMiddleMouseDown = true;
	m_bMiddleMouseWentDown = true;
}

void World::OnMButtonUp(int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		//return;
	}

	m_bMiddleMouseDown = false;
}

void World::OnRButtonDown(int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		return;
	}

	m_bRightMouseDown = true;
	m_bRightMouseWentDown = true;

	m_bUpdatePickPosition = false;
}

void World::OnRButtonUp(int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		//return;
	}

	m_bRightMouseDown = false;
	m_bUpdatePickPosition = true;
}

void World::OnWheelUp(int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		return;
	}

	WheelUpBehaviour();
}

void World::WheelUpBehaviour()
{
	float fStepSize = 5.0f;

	mlVector3D vForward = m_trnCamera.TransformVector(mlVector3D(0,0,-fStepSize));

	m_trnCamera.ApplyTranslation(vForward);
}

void World::OnWheelDown(int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		return;
	}

	WheelDownBehaviour();
}

void World::WheelDownBehaviour()
{
	float fStepSize = 5.0f;

	mlVector3D vBackward = m_trnCamera.TransformVector(mlVector3D(0,0,fStepSize));

	m_trnCamera.ApplyTranslation(vBackward);
}

// OpenGL matrix operations

void World::ResetProjectionMatrix()
{
	// Sets GL_PROJECTION matrix mode

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// for left handed instead of right handed coordinate system
	//glScalef(1.0f, 1.0f, -1.0f);
}

void World::ResetModelviewMatrix()
{
	// Sets GL_MODELVIEW matrix mode

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void World::RebuildProjectionMatrix()
{
	ResetProjectionMatrix();
	ApplyProjectionMatrix();
}

void World::RebuildProjectionMatrixForPicking()
{
	ResetProjectionMatrix();
	ApplyPickMatrix();
	ApplyProjectionMatrix();
}

void World::ApplyProjectionMatrix()
{
	// Assumes GL_PROJECTION matrix mode

	int nViewportWidth = m_Viewport[2];
	int nViewportHeight = m_Viewport[3];

	// This uses the viewport, so calls to Render_NoViewportSetting are still actually using the viewport
	// This was causing disturbing problems in release build
	// Setting m_Viewport in the constructor corrects this, but it is not an ultimate solution.
	// I hope that the output on the laptop will be fixed as well. Debug works smoothly and well, why shouldn't release??
	
	GLdouble fAspect = GLdouble(nViewportWidth) / GLdouble(nViewportHeight);

	//gluPerspective(45.0, fAspect, 0.1f, 20000.0f);
	gluPerspective(m_fFieldOfView, fAspect, 0.1f, 20000.0f);
}

void World::ApplyPickMatrix()
{
	// Assumes GL_PROJECTION matrix mode

	//gluPickMatrix(m_vMouse2DPosition.x, m_vMouse2DPosition.y, 2, 2, m_Viewport);
	//gluPickMatrix(m_vMouse2DPosition.x, m_vMouse2DPosition.y, 5, 5, m_Viewport);
	//gluPickMatrix(m_vMouse2DPosition.x, m_vMouse2DPosition.y, 160, 160, m_Viewport);
	gluPickMatrix(m_vMouse2DPosition.x, m_vMouse2DPosition.y, m_nFeedbackFrustumSize, m_nFeedbackFrustumSize, m_Viewport);
}

void World::RebuildModelviewMatrix()
{
	ResetModelviewMatrix();

	mlTransform transform = m_trnCamera;

	transform.Invert();

	mlMatrix4x4 matCameraTransform(transform.GetMatrix());

	mlFloat * pCameraTransform = reinterpret_cast<mlFloat*>(&matCameraTransform);

	glLoadMatrixf(pCameraTransform);
}

// OpenGL rendering

void World::ClearViewport()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void World::SwapBuffers()
{
    //glutSwapBuffers();
}

void World::RenderFloorBox()
{
	glPushMatrix();
		
		float fGridSizeX = m_fGridCellWidth * m_nGridSizeX;
		float fGridSizeY = m_fGridCellWidth * m_nGridSizeY;
		float fRatio = (float)m_nGridSizeY / (float)m_nGridSizeX;
		float fBoxSize = mlMax(fGridSizeX, fGridSizeY) * 1.5f;

		glScalef(1.0f, 0.05f, 1.0f);
		glTranslatef(fGridSizeX * 0.5f,-fBoxSize * 0.5f,fGridSizeY * 0.5f);
		//glScalef(1.0f, 1.0f, fRatio);

		// If the alpha value is too high, z fighting is intensified

		glColor4f(0.3f, 0.3f, 0.8f, 1.0f);
		//glColor3f(0.3f, 0.3f, 0.8f);
		glutWireCube(fBoxSize);
		//glutSolidCube(fBoxSize);

		//glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
		glColor4f(0.3f, 0.3f, 0.3f, 0.0f);
		glutSolidCube(fBoxSize);

	glPopMatrix();
}

void World::RenderFloorGrid()
{
	//glDisable(GL_DEPTH_TEST);
	//glDepthFunc(GL_NOTEQUAL);
	glPushMatrix();
		
		//glTranslatef(-150,0,-150);
		//glTranslatef(0,-0.2,0);

		glColor3f(0.6f, 0.1f, 0.9f);

		glBegin(GL_LINES);

		mlFloat fWidth = m_fGridCellWidth;
		int nSizeX = m_nGridSizeX;
		int nSizeY = m_nGridSizeY;

		float fExtentX = nSizeX * fWidth;
		int nNumberOfLinesX = nSizeX + 1;

		float fExtentY = nSizeY * fWidth;
		int nNumberOfLinesY = nSizeY + 1;

		for(int i = 0; i < nNumberOfLinesX; i++)
		{
			glVertex3f(i * fWidth, 0, 0);
			glVertex3f(i * fWidth, 0, fExtentY);
		}

		for(int i = 0; i < nNumberOfLinesY; i++)
		{
			glVertex3f(0, 0, i * fWidth);
			glVertex3f(fExtentX, 0, i * fWidth);
		}

		glEnd();

	glPopMatrix();
	//glDepthFunc(GL_LESS);
	//glDepthFunc(GL_LEQUAL);
	//glEnable(GL_DEPTH_TEST);

}

void World::RenderScene()
{
	// Assumes GL_MODELVIEW matrix mode

	//m_pJet->Render();
	//glutSolidCube(30.0);
}

void World::UpdateSimulation(float fDelta)
{
}

void World::RenderFeedbackModel()
{
	// Assumes GL_MODELVIEW matrix mode

	//glPushMatrix();

	//m_pFeedbackModel->Render();
	//glTranslatef(40,40,40);

	if(m_bRenderFeedbackModel)
	{
		m_pFeedbackModel->RenderAsWireframe();
	}

	if(m_bRenderMousePickSphere)
	{
		//gxRenderingPoint mousepos(gxColorGreen, m_vMouse3DPosition);
		gxRenderingPoint mousepos(gxColorGreen, m_vMousePickPosition);
		//mousepos.RenderAsSphere(0.3f);
		//mousepos.RenderAsSphere(1.0f);
		mousepos.RenderAsScreenSphere(15);
		//mousepos.RenderAsSphere(5.0f);
	}

	//glPopMatrix();
}

// OpenGL viewport

void World::RefreshViewport()
{
	glViewport(m_Viewport[0], m_Viewport[1], m_Viewport[2], m_Viewport[3]);
	//glScissor(m_Viewport[0], m_Viewport[1], m_Viewport[2], m_Viewport[3]);
	//glGetIntegerv(GL_VIEWPORT, m_Viewport);
}

// OpenGL picking

void World::MakeFeedbackModel()
{
	return;

	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];

	glFeedbackBuffer(932000, GL_3D, m_pFeedbackBuffer);

	RebuildProjectionMatrixForPicking();

	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	RebuildModelviewMatrix();

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

	glRenderMode(GL_FEEDBACK);

	RenderFloorBox();
	RenderFloorGrid();

	// Don't render the scene when picking
	// Actually, yes, render the scene when picking
	// Lets make this specifiable by the app with a bool!! Genius.
	if(m_bPickAppScene)
	{
		m_bRenderingPickScene = true;
		RenderScene();
	}

	GLint feedbackBufferSize = glRenderMode(GL_RENDER);

	m_pFeedbackModel->m_triangles.clear();
	m_pFeedbackModel->m_lines.clear();
	gxMakeModelFromFeedbackBuffer(m_pFeedbackModel, m_pFeedbackBuffer, feedbackBufferSize, modelMatrix, projMatrix, m_Viewport);
}

void World::RefreshPickPosition()
{
	{
		//m_vMouse2DPosition
		//float fDepth = 0.0f;
		float fDepth;
		glReadPixels(m_vMouse2DPosition.x, m_vMouse2DPosition.y, 1,1, GL_DEPTH_COMPONENT, GL_FLOAT, &fDepth);
		if(fDepth == 1.0f)
			fDepth = 0.1f;

		GLdouble modelMatrix[16];
		GLdouble projMatrix[16];

		RebuildProjectionMatrix();
		RebuildModelviewMatrix();

		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

		GLdouble winX = m_vMouse2DPosition.x;
		GLdouble winY = m_vMouse2DPosition.y;
		GLdouble winZ = fDepth;
		//GLdouble winZ = 0.01f;

		GLdouble objX, objY, objZ;

		gluUnProject(winX, winY, winZ, modelMatrix, projMatrix, m_Viewport, &objX, &objY, &objZ);

		m_vMousePickPosition = mlVector3D(objX, objY, objZ);
	}

	//m_vMousePickPosition = m_vMouse3DPosition + m_vMouseRay * 50.0f;

	//return;

	//gxIntersectionResultSet results;
	//gxPick(m_vMouse3DPosition, m_vMouseRay, *m_pFeedbackModel, results);
	//m_vMousePickPosition = results.GetIntersectionResultClosestTo(m_vMouse3DPosition).intersectionPosition;

	// This exposes a silly bug to do with infinite rays

	mlTriangle triObjectCameraPlane(
		m_vMousePickPosition,
		m_vMousePickPosition + m_trnCamera.TransformVector(mlVector3D(0,0,-20)),
		m_vMousePickPosition + m_trnCamera.TransformVector(mlVector3D(0,20,0)));


	//mlTriangle triObjectCameraPlane2(m_vMouseRay, m_vMousePickPosition, 20.0f);
	//mlTriangle triObjectCameraPlane2(m_trnCamera.TransformVector(mlVector3D(0,0,-1)), mlVector3D(0.0f, 1.0f, 0.0f), m_vMousePickPosition, 20.0f);
	mlTriangle triObjectCameraPlane2(
		m_vMousePickPosition,
		m_vMousePickPosition + m_trnCamera.TransformVector(mlVector3D(20,0,0)),
		m_vMousePickPosition + m_trnCamera.TransformVector(mlVector3D(0,20,0)));

	m_triangle1 = gxRenderingTriangle(gxColorBlue, triObjectCameraPlane);
	m_triangle2 = gxRenderingTriangle(gxColorRed, triObjectCameraPlane2);
}

void World::RefreshMousePosition()
{
	Convert2Dto3D(m_vMouse2DPosition, m_vMouse3DPosition, m_vMouseRay);
}

void World::Convert2Dto3D(mlVector3D & v2DPosition, mlVector3D & v3DPosition, mlVector3D & v3DRay)
{
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];

	RebuildProjectionMatrix();
	RebuildModelviewMatrix();

	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

	GLdouble winx = v2DPosition.x;
	GLdouble winy = v2DPosition.y;
	GLdouble winz = 0.01;

	GLdouble objx, objy, objz;

	gluUnProject(
		winx, winy, winz,
		modelMatrix, projMatrix, m_Viewport,
		&objx, &objy, &objz);

	v3DPosition = mlVector3D(objx, objy, objz);

	winz += 0.1;

	gluUnProject(
		winx, winy, winz,
		modelMatrix, projMatrix, m_Viewport,
		&objx, &objy, &objz);

	v3DRay = mlVector3D(objx, objy, objz) - v3DPosition;
	v3DRay.Normalise();
}

// OpenGL camera

void World::PanCamera(float fX, float fY)
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

void World::RotateCamera(float fHeading, float fPitch)
{
	// This is a magic number which maps pixels of mouse motion to radians of rotation
	float fRotationSpeed = 0.008f;
	//float fRotationSpeed = 0.016f;

	m_trnCamera.ApplyRotation(mlQuaternion(mlVector3D(0,1,0), fRotationSpeed * -fHeading));
	//m_trnCamera.ApplyRotation(mlQuaternion(mlVector3D(0,1,0), mlDegrees(-fHeading)));

	mlVector3D vSide = m_trnCamera.TransformVector(mlVector3D(1,0,0));

	m_trnCamera.ApplyRotation(mlQuaternion(vSide, fRotationSpeed * fPitch));
	//m_trnCamera.ApplyRotation(mlQuaternion(vSide, mlDegrees(fPitch)));
}

void World::OrbitCamera(mlVector3D & vCenter, float fHeading, float fPitch)
{
	//mlVector3D vToPoint = vCenter - m_trnCamera.GetTranslation();
	mlVector3D vToPoint = m_trnCamera.TransformPointInverse(vCenter);

	//float fRadius = vToPoint.Magnitude();

	m_trnCamera.SetTranslation(vCenter);

	RotateCamera(fHeading, fPitch);

	//mlVector3D vPushBack = m_trnCamera.TransformVector(mlVector3D(0, 0, -fRadius));

	vToPoint.x *= 0.9f;
	vToPoint.y *= 0.9f;

	//vToPoint.x *= 0.0f;
	//vToPoint.y *= 0.0f;

	//float fDesiredDistance = -100.0f;

	//vToPoint.z = vToPoint.z - ((vToPoint.z - fDesiredDistance) * 0.05f);
	//vToPoint.z = vToPoint.z - ((vToPoint.z - fRadius) * 0.05f);
	//m_trnCamera.SetTranslation(vToPoint);

	mlVector3D vPushBack = m_trnCamera.TransformVector(vToPoint);

	m_trnCamera.SetTranslation(vCenter - vPushBack);

	m_trnCamera.Normalise();
}

void World::LookAt(mlVector3D vFocalPoint)
{
	mlVector3D vForward = vFocalPoint - m_trnCamera.GetTranslation();

	vForward.Normalise();

	mlQuaternion rotLookAt = mlQuaternionFromDirection(vForward, mlVector3D(0.0f, 1.0f, 0.0f));

	m_trnCamera.SetRotation(rotLookAt);
}

void World::LookDown()
{
	mlMatrix3x3 mLookDown;
	mLookDown.I = mlVector3D(0.0f, 1.0f, 0.0f);
	mLookDown.J = mlVector3D(0.0f, 0.0f, -1.0f);
	mLookDown.K = mlVector3D(1.0f, 0.0f, 0.0f);

	mlQuaternion qRot = mlQuaternionFromRotationMatrix(mLookDown);

	m_trnCamera.SetRotation(qRot);
}

void World::LookDown_Alternate()
{
	mlMatrix3x3 mLookDown;
	mLookDown.I = mlVector3D(1.0f, 0.0f, 0.0f);
	mLookDown.J = mlVector3D(0.0f, 0.0f, -1.0f);
	mLookDown.K = mlVector3D(0.0f, 1.0f, 0.0f);

	mlQuaternion qRot = mlQuaternionFromRotationMatrix(mLookDown);

	m_trnCamera.SetRotation(qRot);
}

// Text rendering

void World::SimpleGlutBitmapString( void *font, const char *s )
{
  const char *p = s;

  while( *p != '\0' )  {
    glutBitmapCharacter( font, *p );
    p++;
  }
}

void World::RenderText(const mlVector3D & vPosition, char * text)
{
   glRasterPos3d(vPosition.x, vPosition.y, vPosition.z);
   
   glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);
   //glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char *)text);
   //SimpleGlutBitmapString(GLUT_BITMAP_9_BY_15, text);
}
