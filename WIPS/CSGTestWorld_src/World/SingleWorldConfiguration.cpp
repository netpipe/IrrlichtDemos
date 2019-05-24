
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "SingleWorldConfiguration.h"

#include <GL/glut.h>

#include "World.h"

World * g_pSingleWorldConfiguration_World = 0;

int g_nSingleWorldWidth = 640;
int g_nSingleWorldHeight = 480;

int g_nSingleWorldTop = 10;
int g_nSingleWorldLeft = 10;

void SingleWorldConfiguration_update();
void SingleWorldConfiguration_display();
void SingleWorldConfiguration_reshape( int width, int height );
void SingleWorldConfiguration_keyDown( unsigned char key, int x, int y );
void SingleWorldConfiguration_keyUp( unsigned char key, int x, int y );
void SingleWorldConfiguration_keyDownSpecial( int key, int x, int y );
void SingleWorldConfiguration_keyUpSpecial( int key, int x, int y );
void SingleWorldConfiguration_mouse(int button, int state, int x, int y);
void SingleWorldConfiguration_mousemove(int x, int y );
void SingleWorldConfiguration_passivemousemove(int x, int y );
void SingleWorldConfiguration_mousewheel(int wheel, int direction, int x, int y);

World * GetSingleWorldConfigurationWorld()
{
	return g_pSingleWorldConfiguration_World;
}

void SingleWorld_SetInitialWindowDimensions(int nTop, int nLeft, int nWidth, int nHeight)
{
	g_nSingleWorldWidth = nWidth;
	g_nSingleWorldHeight = nHeight;

	g_nSingleWorldTop = nTop;
	g_nSingleWorldLeft = nLeft;
}

void RunSingleWorldConfiguration(
	World * pYourWorld,
	char * sName,
	int argc, char** argv)
{
	g_pSingleWorldConfiguration_World = pYourWorld;

	glutInit( &argc, argv );
    glutInitWindowSize(g_nSingleWorldWidth,g_nSingleWorldHeight);
    glutInitWindowPosition(g_nSingleWorldLeft,g_nSingleWorldTop);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow( sName );

	glutIdleFunc( SingleWorldConfiguration_update );
	glutDisplayFunc( SingleWorldConfiguration_display );
	glutReshapeFunc( SingleWorldConfiguration_reshape );

	glutKeyboardFunc( SingleWorldConfiguration_keyDown );
	glutKeyboardUpFunc( SingleWorldConfiguration_keyUp );

	glutSpecialFunc( SingleWorldConfiguration_keyDownSpecial );
	glutSpecialUpFunc( SingleWorldConfiguration_keyUpSpecial );

	// Insert menu function

	glutIgnoreKeyRepeat(1);

	glutMouseFunc( SingleWorldConfiguration_mouse );

	glutMotionFunc( SingleWorldConfiguration_mousemove );
	glutPassiveMotionFunc( SingleWorldConfiguration_passivemousemove);

	//glutMouseWheelFunc( SingleWorldConfiguration_mousewheel );

	glClearColor(0,0,0,1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	pYourWorld->LoadTextures();

	glutMainLoop();
}

/////////////////////////////////////////////////////////////////////

void SingleWorldConfiguration_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_pSingleWorldConfiguration_World->PreRender();

	g_pSingleWorldConfiguration_World->Render();

	g_pSingleWorldConfiguration_World->PostRender();

    glutSwapBuffers();
}

float g_fSingleWorld_PrevTime = 0.0f;
float g_fSingleWorld_UpdateTime = 0.0f;

void SingleWorldConfiguration_update()
{
	float curr = glutGet(GLUT_ELAPSED_TIME);
	float delta = (curr-g_fSingleWorld_PrevTime) / 1000.0f;
	g_fSingleWorld_PrevTime = curr;

	g_fSingleWorld_UpdateTime += delta;

	//float fTimeBwFrames = 0.03f;
	float fTimeBwFrames = 0.01f;

	if(g_fSingleWorld_UpdateTime < fTimeBwFrames)
	{
		return;
	}

	g_fSingleWorld_UpdateTime = 0.0f;

	float harddelta = 0.03f;

	g_pSingleWorldConfiguration_World->PreUpdate();

	g_pSingleWorldConfiguration_World->Update(harddelta);

	g_pSingleWorldConfiguration_World->PostUpdate();

	SingleWorldConfiguration_display();

	g_pSingleWorldConfiguration_World->PostRenderUpdate();
}

void SingleWorldConfiguration_reshape ( int width, int height )
{
	g_pSingleWorldConfiguration_World->Reshape(
		0, 0,
		width, height,
		width, height);

	glutPostRedisplay();
}

void SingleWorldConfiguration_keyDown ( unsigned char key, int x, int y )
{
	g_pSingleWorldConfiguration_World->OnKeyDown(key, x, y);

	if(key == 27) // Escape
	{
		glutLeaveMainLoop();
	}
}

void SingleWorldConfiguration_keyUp ( unsigned char key, int x, int y )
{
	g_pSingleWorldConfiguration_World->OnKeyUp(key, x, y);
}


void SingleWorldConfiguration_keyDownSpecial ( int key, int x, int y )
{
	g_pSingleWorldConfiguration_World->OnKeyDownSpecial(key, x, y);
}

void SingleWorldConfiguration_keyUpSpecial ( int key, int x, int y )
{
	g_pSingleWorldConfiguration_World->OnKeyUpSpecial(key, x, y);
}

void SingleWorldConfiguration_mousemove(int x, int y)
{
	g_pSingleWorldConfiguration_World->OnMouseMove(x, y);
}

void SingleWorldConfiguration_passivemousemove(int x, int y)
{
	g_pSingleWorldConfiguration_World->OnPassiveMouseMove(x, y);
}

void SingleWorldConfiguration_mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			g_pSingleWorldConfiguration_World->OnLButtonDown(x, y);
		}

		if(state == GLUT_UP)
		{
			g_pSingleWorldConfiguration_World->OnLButtonUp(x, y);
		}
	}

	if(button == GLUT_MIDDLE_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			g_pSingleWorldConfiguration_World->OnMButtonDown(x, y);
		}

		if(state == GLUT_UP)
		{
			g_pSingleWorldConfiguration_World->OnMButtonUp(x, y);
		}
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			g_pSingleWorldConfiguration_World->OnRButtonDown(x, y);
		}

		if(state == GLUT_UP)
		{
			g_pSingleWorldConfiguration_World->OnRButtonUp(x, y);
		}
	}
}

void SingleWorldConfiguration_mousewheel(int wheel, int direction, int x, int y)
{
	if(direction == 1)
	{
		g_pSingleWorldConfiguration_World->OnWheelUp(x, y);
	}
	else
	{
		g_pSingleWorldConfiguration_World->OnWheelDown(x, y);
	}
}
