
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ThreeWorldConfiguration.h"

#include <GL/glut.h>

#include "World.h"

World * g_pThreeWorldConfiguration_Worlds[3] = {0,0,0};
float g_fThreeWorldConfiguration_SplitProportionH = 0.7f;
float g_fThreeWorldConfiguration_SplitProportionW = 0.4f;
ThreeWorldConfigurationsEnum g_eThreeWorldConfiguration = ThreeWorldConfigurationsEnum_VerticalSplitRight;

int g_nOffscreenWindowSize = 512;
int g_nOffscreenWindowID = -1;
int g_nMainWindowID = -1;

void ThreeWorldConfiguration_update();
void ThreeWorldConfiguration_display();
void ThreeWorldConfiguration_reshape( int width, int height );
void ThreeWorldConfiguration_keyDown( unsigned char key, int x, int y );
void ThreeWorldConfiguration_keyUp( unsigned char key, int x, int y );
void ThreeWorldConfiguration_keyDownSpecial( int key, int x, int y );
void ThreeWorldConfiguration_keyUpSpecial( int key, int x, int y );
void ThreeWorldConfiguration_mouse(int button, int state, int x, int y);
void ThreeWorldConfiguration_mousemove(int x, int y );
void ThreeWorldConfiguration_passivemousemove(int x, int y );
void ThreeWorldConfiguration_mousewheel(int wheel, int direction, int x, int y);

World * GetThreeWorldConfigurationWorld(int nID)
{
	return g_pThreeWorldConfiguration_Worlds[nID];
}

void RunThreeWorldConfiguration(
	World * pWorld1, World * pWorld2, World * pWorld3,
	char * sName,
	ThreeWorldConfigurationsEnum eConfig, float fSplitProportionW, float fSplitProportionH,
	int argc, char** argv)
{
	g_pThreeWorldConfiguration_Worlds[0] = pWorld1;
	g_pThreeWorldConfiguration_Worlds[1] = pWorld2;
	g_pThreeWorldConfiguration_Worlds[2] = pWorld3;

	g_fThreeWorldConfiguration_SplitProportionH = fSplitProportionH;
	g_fThreeWorldConfiguration_SplitProportionW = fSplitProportionW;
	g_eThreeWorldConfiguration = eConfig;

	glutInit( &argc, argv );
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	g_nMainWindowID = glutCreateWindow( sName );

	glutIdleFunc( ThreeWorldConfiguration_update );
	glutDisplayFunc( ThreeWorldConfiguration_display );
	glutReshapeFunc( ThreeWorldConfiguration_reshape );

	glutKeyboardFunc( ThreeWorldConfiguration_keyDown );
	glutKeyboardUpFunc( ThreeWorldConfiguration_keyUp );

	glutSpecialFunc( ThreeWorldConfiguration_keyDownSpecial );
	glutSpecialUpFunc( ThreeWorldConfiguration_keyUpSpecial );

	// Insert menu function

	glutIgnoreKeyRepeat(1);

	glutMouseFunc( ThreeWorldConfiguration_mouse );

	glutMotionFunc( ThreeWorldConfiguration_mousemove );
	glutPassiveMotionFunc( ThreeWorldConfiguration_passivemousemove);

	glutMouseWheelFunc( ThreeWorldConfiguration_mousewheel );

    glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH | GLUT_OFFSCREEN | GLUT_STENCIL );
    glutInitWindowSize( g_nOffscreenWindowSize, g_nOffscreenWindowSize );
	g_nOffscreenWindowID = glutCreateWindow( "" );

	glutSetWindow(g_nOffscreenWindowID);

	glClearColor(0,0,0,1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc (GL_ONE, GL_ONE);

	glutSetWindow(g_nMainWindowID);

	glClearColor(0,0,0,1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	pWorld1->LoadTextures();
	pWorld2->LoadTextures();
	pWorld3->LoadTextures();

	glutMainLoop();
}

/////////////////////////////////////////////////////////////////////

void UseMainWindowContext()
{
	glutSetWindow(g_nMainWindowID);
}

void UseOffscreenContext()
{
	glutSetWindow(g_nOffscreenWindowID);
}

/////////////////////////////////////////////////////////////////////

void ThreeWorldConfiguration_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->PreRender();
	}

	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->Render();
	}

	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->PostRender();
	}

    glutSwapBuffers();
}

float g_fThreeWorld_PrevTime = 0.0f;
float g_fThreeWorld_UpdateTime = 0.0f;

void ThreeWorldConfiguration_update()
{
	float curr = glutGet(GLUT_ELAPSED_TIME);
	float delta = (curr-g_fThreeWorld_PrevTime) / 1000.0f;
	g_fThreeWorld_PrevTime = curr;

	g_fThreeWorld_UpdateTime += delta;

	//float fTimeBwFrames = 0.03f;
	float fTimeBwFrames = 0.01f;

	if(g_fThreeWorld_UpdateTime < fTimeBwFrames)
	{
		return;
	}

	g_fThreeWorld_UpdateTime = 0.0f;

	float harddelta = 0.03f;

	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->PreUpdate();
	}

	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->Update(harddelta);
	}

	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->PostUpdate();
	}

	//prev = curr;

	ThreeWorldConfiguration_display();

	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->PostRenderUpdate();
	}
}

void ThreeWorldConfiguration_reshape ( int width, int height )
{
	int nHeight1 = (int)((float)height * g_fThreeWorldConfiguration_SplitProportionH);
	int nHeight2 = (int)((float)height * (1.0f - g_fThreeWorldConfiguration_SplitProportionH));

	int nWidth1 = (int)((float)width * g_fThreeWorldConfiguration_SplitProportionW);
	int nWidth2 = (int)((float)width * (1.0f - g_fThreeWorldConfiguration_SplitProportionW));

	// Arrange the worlds like left to right, top to bottom, like so:
	// 1 2
	// 3 4

	switch(g_eThreeWorldConfiguration)
	{
	case ThreeWorldConfigurationsEnum_VerticalSplitLeft:

		g_pThreeWorldConfiguration_Worlds[0]->Reshape(
			nWidth1, 0,
			nWidth2, height,
			width, height);

		g_pThreeWorldConfiguration_Worlds[1]->Reshape(
			0, nHeight2,
			nWidth1, nHeight1,
			width, height);

		g_pThreeWorldConfiguration_Worlds[2]->Reshape(
			0, 0,
			nWidth1, nHeight2,
			width, height);

		break;

	case ThreeWorldConfigurationsEnum_VerticalSplitRight:

		g_pThreeWorldConfiguration_Worlds[0]->Reshape(
			0, 0,
			nWidth1, height,
			width, height);

		g_pThreeWorldConfiguration_Worlds[1]->Reshape(
			nWidth1, nHeight2,
			nWidth2, nHeight1,
			width, height);

		g_pThreeWorldConfiguration_Worlds[2]->Reshape(
			nWidth1, 0,
			nWidth2, nHeight2,
			width, height);

		break;

	case ThreeWorldConfigurationsEnum_HorizontalSplitTop:

		g_pThreeWorldConfiguration_Worlds[0]->Reshape(
			0, 0,
			width, nHeight2,
			width, height);

		g_pThreeWorldConfiguration_Worlds[1]->Reshape(
			0, nHeight2,
			nWidth1, nHeight1,
			width, height);

		g_pThreeWorldConfiguration_Worlds[2]->Reshape(
			nWidth1, nHeight2,
			nWidth2, nHeight1,
			width, height);

		break;

	case ThreeWorldConfigurationsEnum_HorizontalSplitBottom:

		g_pThreeWorldConfiguration_Worlds[0]->Reshape(
			0, nHeight2,
			width, nHeight1,
			width, height);

		g_pThreeWorldConfiguration_Worlds[1]->Reshape(
			0, 0,
			nWidth1, nHeight2,
			width, height);

		g_pThreeWorldConfiguration_Worlds[2]->Reshape(
			nWidth1, 0,
			nWidth2, nHeight2,
			width, height);

		break;

	default:
		break;
	}

	glutPostRedisplay();
}

void ThreeWorldConfiguration_keyDown ( unsigned char key, int x, int y )
{
	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->OnKeyDown(key, x, y);
	}

	if(key == 27) // Escape
	{
		glutLeaveMainLoop();
	}
}

void ThreeWorldConfiguration_keyUp ( unsigned char key, int x, int y )
{
	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->OnKeyUp(key, x, y);
	}
}


void ThreeWorldConfiguration_keyDownSpecial ( int key, int x, int y )
{
	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->OnKeyDownSpecial(key, x, y);
	}
}

void ThreeWorldConfiguration_keyUpSpecial ( int key, int x, int y )
{
	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->OnKeyUpSpecial(key, x, y);
	}
}

void ThreeWorldConfiguration_mousemove(int x, int y)
{
	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->OnMouseMove(x, y);
	}
}

void ThreeWorldConfiguration_passivemousemove(int x, int y)
{
	for(int i = 0; i < 3; i++)
	{
		g_pThreeWorldConfiguration_Worlds[i]->OnPassiveMouseMove(x, y);
	}
}

void ThreeWorldConfiguration_mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			for(int i = 0; i < 3; i++)
			{
				g_pThreeWorldConfiguration_Worlds[i]->OnLButtonDown(x, y);
			}
		}

		if(state == GLUT_UP)
		{
			for(int i = 0; i < 3; i++)
			{
				g_pThreeWorldConfiguration_Worlds[i]->OnLButtonUp(x, y);
			}
		}
	}

	if(button == GLUT_MIDDLE_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			for(int i = 0; i < 3; i++)
			{
				g_pThreeWorldConfiguration_Worlds[i]->OnMButtonDown(x, y);
			}
		}

		if(state == GLUT_UP)
		{
			for(int i = 0; i < 3; i++)
			{
				g_pThreeWorldConfiguration_Worlds[i]->OnMButtonUp(x, y);
			}
		}
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			for(int i = 0; i < 3; i++)
			{
				g_pThreeWorldConfiguration_Worlds[i]->OnRButtonDown(x, y);
			}
		}

		if(state == GLUT_UP)
		{
			for(int i = 0; i < 3; i++)
			{
				g_pThreeWorldConfiguration_Worlds[i]->OnRButtonUp(x, y);
			}
		}
	}
}

void ThreeWorldConfiguration_mousewheel(int wheel, int direction, int x, int y)
{
	if(direction == 1)
	{
		for(int i = 0; i < 3; i++)
		{
			g_pThreeWorldConfiguration_Worlds[i]->OnWheelUp(x, y);
		}
	}
	else
	{
		for(int i = 0; i < 3; i++)
		{
			g_pThreeWorldConfiguration_Worlds[i]->OnWheelDown(x, y);
		}
	}
}
