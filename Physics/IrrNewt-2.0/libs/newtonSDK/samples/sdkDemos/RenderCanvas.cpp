#include <toolbox_stdafx.h>

#include "RenderCanvas.h"
#include "MainFrame.h"
#include "DGCamera.h"
#include "PhysicsUtils.h"
#include "HiResTimer.h"
#include "OpenGlUtil.h"
#include "MousePick.h"
#include <DebugDisplay.h>

BEGIN_EVENT_TABLE(RenderCanvas, wxGLCanvas)
	EVT_KEY_DOWN(RenderCanvas::OnKeyDown)
	EVT_KEY_UP(RenderCanvas::OnKeyUp)	
	EVT_MOUSE_EVENTS (RenderCanvas::OnMouseEvent)

	EVT_SIZE(RenderCanvas::OnSize)
	EVT_PAINT(RenderCanvas::OnPaint)
	EVT_IDLE(RenderCanvas::OnIdle)

	EVT_ERASE_BACKGROUND(RenderCanvas::OnEraseBackground)
//	EVT_MOUSE_EVENTS(RenderCanvas::OnMouse)
END_EVENT_TABLE()


CHiResTimer timer;

RenderCanvas::RenderCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, int* attributes)
	: wxGLCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, _T("GLRenderCanvas"), attributes)
{
	// init open gl
//	m_dragging0 = false;
//	m_dragging1 = false;
	m_mousePos.x = 0;
	m_mousePos.y = 0;
	m_showCollision = false;
	m_glIsInit = false;
//	m_camera = new DGCamera(0.1f, 90.0f, 1000.0f, 1.0f, 0.1f);
	m_camera = new DGCamera();
}


RenderCanvas::~RenderCanvas(void)
{	
	delete m_camera;
}


void RenderCanvas::InitGl ()
{
	// Our shading model--Goraud (smooth). 
	glShadeModel (GL_SMOOTH);

	// Culling. 
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);
	glEnable (GL_CULL_FACE);
	
//	glEnable(GL_DITHER);

	// z buffer test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

	// Set the clear color. 
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Change to the projection matrix and set our viewing volume.
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity( );

	// set on ambient directional light
	GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 0.0 };
	GLfloat lightAmbientColor[] = { 0.7f, 0.7f, 0.7f, 0.0 };
	GLfloat lightPosition[] = { 500.0f, 200.0f, 500.0f, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// init the camera
	m_camera->ResetDefaultCamera ();
}


void RenderCanvas::ResetProjectionMode()
{
	_ASSERTE (m_camera);
	if (GetContext()) {
		int w, h;

		GetClientSize(&w, &h);
		SetCurrent();
		glViewport(0, 0, (GLint) w, (GLint) h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

//		GLint viewport[4]; 
//		glGetIntegerv(GL_VIEWPORT, viewport); 

		gluPerspective(m_camera->m_FocalLength, GLfloat (w) /GLfloat(h), m_camera->m_NearPlane, m_camera->m_DepthOfView);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	}
}

void RenderCanvas::OnSize(wxSizeEvent& event)
{
	// this is also necessary to update the context on some platforms
	wxGLCanvas::OnSize(event);
	// Reset the OpenGL view aspect
	ResetProjectionMode();
}






void RenderCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// Do nothing, to avoid flashing on MSW
}


void RenderCanvas::OnKeyDown( wxKeyEvent &event )
{
	int keyCode = event.GetKeyCode();
	if (keyCode == WXK_ESCAPE)  {
		// send a display refresh event in case the runtime update is stopped bu the user.
		wxMenuEvent exitEvent (wxEVT_COMMAND_MENU_SELECTED, wxID_EXIT);
		GetParent()->GetEventHandler()->ProcessEvent(exitEvent);
	}

	if (!event.GetModifiers()) {
		KeyDown(keyCode);	
	}
}

void RenderCanvas::OnKeyUp( wxKeyEvent &event )
{
	if (!event.GetModifiers()) {
		int keyCode = event.GetKeyCode();
		KeyUp(keyCode);
	}
}

void RenderCanvas::OnMouseEvent(wxMouseEvent &event)
{
	dMOUSE_POINT mouse;
	mouse.x = event.GetX();
	mouse.y = event.GetY();

	int mouseLeftKey = event.LeftIsDown();
	NewtonFrame* parent = (NewtonFrame*) GetParent();
	if (parent->m_world && !MousePick (parent->m_world, mouse, mouseLeftKey, 0.125f, 1.0f)) {
			// we are not in mouse pick mode, then we are in camera tracking mode
/*
			if (mouseLeftKey) {
				// when click left mouse button the first time, we reset the camera
				// convert the mouse x position to delta yaw angle
				if (mouse1.x > (mouse0.x + 1)) {
					yawAngle += 1.0f * 3.141592f / 180.0f;
					if (yawAngle > (360.0f * 3.141592f / 180.0f)) {
						yawAngle -= (360.0f * 3.141592f / 180.0f);
					}
				} else if (mouse1.x < (mouse0.x - 1)) {
					yawAngle -= 1.0f * 3.141592f / 180.0f;
					if (yawAngle < 0.0f) {
						yawAngle += (360.0f * 3.141592f / 180.0f);
					}
				}

				if (mouse1.y > (mouse0.y + 1)) {
					rollAngle += 1.0f * 3.141592f / 180.0f;
					if (rollAngle > (80.0f * 3.141592f / 180.0f)) {
						rollAngle = 80.0f * 3.141592f / 180.0f;
					}
				} else if (mouse1.y < (mouse0.y - 1)) {
					rollAngle -= 1.0f * 3.141592f / 180.0f;
					if (rollAngle < -(80.0f * 3.141592f / 180.0f)) {
						rollAngle = -80.0f * 3.141592f / 180.0f;
					}
				}
				dMatrix cameraDirMat (dRollMatrix(rollAngle) * dYawMatrix(yawAngle));
				cameraDir = cameraDirMat.m_front;
			}

		}
*/
	}
	m_mousePos = mouse;
}



void RenderCanvas::OnIdle(wxIdleEvent &event)
{
/*
	// this method is recommended by the WxWidegt doc, but it is very buggy and it appears to mess with the timer.
	{
		// send a paint update for viewing the next frame 
		//	wxPaintEvent paintEvent;
		//	GetEventHandler()->ProcessEvent(paintEvent);

		// make sure we send a request for the next update
		//event.RequestMore ();
	}

	// this is much elegant and it works on all platform
	Refresh(false);
*/
	wxClientDC dc(this);
	RenderFrame ();
	event.RequestMore(); // render continuously, not only once on idle
}




void RenderCanvas::OnPaint (wxPaintEvent& WXUNUSED(event) )
{
	// must always be here
	wxPaintDC dc(this);
	RenderFrame ();
}


void RenderCanvas::RenderFrame ()
{
	SetCurrent();
	if (GetContext()) {

		// Initialize OpenGL
		if (!m_glIsInit)
		{
			InitGl();
			ResetProjectionMode();
			m_glIsInit = true;
		}


		// clear the screen
	//	glClearColor( 0.3f, 0.4f, 0.6f, 1.0f );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// submit all the geometry for rendering
	//	DrawScene ();

		NewtonFrame* parent = (NewtonFrame*) GetParent();


		// get the time step
		dFloat timeStep = timer.GetElapsedSeconds();
	//	dFloat visualTimeStep = timeStep;
		timeStep = 1.0f/60.0f;

		// update the Newton physics world
	//	if (!re_entrantUpdate) {
	//		re_entrantUpdate = 1;
	//		if (!simulationStopped) {
				if (parent->m_world) {
					NewtonUpdate (parent->m_world, timeStep);
	//				NewtonCollisionUpdate (demo.m_world);
				}
	//		}
	//		re_entrantUpdate = 0;
	//	}

		// read the control 	
		//	demo.ApplyControl ();

		// apply the Camera after the controls are read
		dVector target (cameraEyepoint + cameraDir);
		m_camera->Position (cameraEyepoint);
		m_camera->TargetPosition(target);
		m_camera->Apply();

	//static int xxx;
	//xxx ++;
	//dTrace (("%d\n", xxx));

		// render the scene
		glEnable (GL_LIGHTING);
		glPushMatrix();	
	//	if (!hideGeometry) {
			parent->Render ();
	//	}
		glPopMatrix();

		if (m_showCollision) {
			if (parent->m_world) {
				DebugRenderWorldCollision (parent->m_world);
			}
		}
		DebugRenderDebugInfo ();

	/*
	//	if (!simulationStopped) {
	//		int profileFlags = 0;
	//		for (int i = 0; i < int (sizeof (showProfiler) / sizeof (showProfiler[0])); i ++) {
	//			profileFlags |=  showProfiler[i] ? (1 << i) : 0;
	//		}

			//profileFlags = 1;
	//		if (profileFlags) {
	//			profiler.Render (demo.m_world, profileFlags);
	//		}
	//	}

	//	if (showfps) {
	//		dVector color (1.0, 1.0f, 1.0f);
	//		Print (color, 4,  4, "KiloBytes used (%7d)     FPS (%3d)", NewtonGetMemoryUsed() / 1024, int (1.0f / visualTimeStep + 0.5));
	//	}
	*/



		// draw everything and swap the display buffer
		glFlush();

		// Swap
		SwapBuffers();

	//		if (specialKeyOn) {
	//			specialKeyOn = 0;
	//			for (int i = 0; i < KeyCode_lastSpecialKey; i ++) {
	//				keybuffer[i] = 0;
	//			}
	//		}
	}
}
