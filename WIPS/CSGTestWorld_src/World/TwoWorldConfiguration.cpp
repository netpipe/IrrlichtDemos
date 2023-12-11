
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "TwoWorldConfiguration.h"

#include <GL/glut.h>

#include "World.h"

World * g_pTwoWorldConfiguration_Worlds[2] = {0,0};
float g_fTwoWorldConfiguration_SplitProportionH = 0.7f;
float g_fTwoWorldConfiguration_SplitProportionW = 0.4f;

void TwoWorldConfiguration_update();
void TwoWorldConfiguration_display();
void TwoWorldConfiguration_VerticalSplit_reshape( int width, int height );
void TwoWorldConfiguration_HorizontalSplit_reshape( int width, int height );
void TwoWorldConfiguration_keyDown( unsigned char key, int x, int y );
void TwoWorldConfiguration_keyUp( unsigned char key, int x, int y );
void TwoWorldConfiguration_keyDownSpecial( int key, int x, int y );
void TwoWorldConfiguration_keyUpSpecial( int key, int x, int y );
void TwoWorldConfiguration_mouse(int button, int state, int x, int y);
void TwoWorldConfiguration_mousemove(int x, int y );
void TwoWorldConfiguration_passivemousemove(int x, int y );
void TwoWorldConfiguration_mousewheel(int wheel, int direction, int x, int y);

World * GetTwoWorldConfigurationWorld(int nID)
{
	return g_pTwoWorldConfiguration_Worlds[nID];
}

void RunTwoWorldConfiguration_VerticalSplit(
	World * pWorld1, World * pWorld2,
	char * sName,
	float fSplitProportionW,
	int argc, char** argv)
{
	g_pTwoWorldConfiguration_Worlds[0] = pWorld1;
	g_pTwoWorldConfiguration_Worlds[1] = pWorld2;

	g_fTwoWorldConfiguration_SplitProportionW = fSplitProportionW;

	glutInit( &argc, argv );
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);

	// Lets try both, see if it makes a difference
	// This makes no difference to whether stencils work or not - it seems that openglut just accepts them and does nothing with them.
	//glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	glutCreateWindow( sName );

	glutIdleFunc( TwoWorldConfiguration_update );
	glutDisplayFunc( TwoWorldConfiguration_display );
	glutReshapeFunc( TwoWorldConfiguration_VerticalSplit_reshape );

	glutKeyboardFunc( TwoWorldConfiguration_keyDown );
	glutKeyboardUpFunc( TwoWorldConfiguration_keyUp );

	glutSpecialFunc( TwoWorldConfiguration_keyDownSpecial );
	glutSpecialUpFunc( TwoWorldConfiguration_keyUpSpecial );

	// Insert menu function

	glutIgnoreKeyRepeat(1);

	glutMouseFunc( TwoWorldConfiguration_mouse );

	glutMotionFunc( TwoWorldConfiguration_mousemove );
	glutPassiveMotionFunc( TwoWorldConfiguration_passivemousemove);

	glutMouseWheelFunc( TwoWorldConfiguration_mousewheel );

	glClearColor(0,0,0,1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	pWorld1->LoadTextures();
	pWorld2->LoadTextures();

	glutMainLoop();
}

void RunTwoWorldConfiguration_HorizontalSplit(
	World * pWorld1, World * pWorld2,
	char * sName,
	float fSplitProportionH,
	int argc, char** argv)
{
	g_pTwoWorldConfiguration_Worlds[0] = pWorld1;
	g_pTwoWorldConfiguration_Worlds[1] = pWorld2;

	g_fTwoWorldConfiguration_SplitProportionH = fSplitProportionH;

	glutInit( &argc, argv );
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);

	// Lets try both, see if it makes a difference
	// This makes no difference to whether stencils work or not - it seems that openglut just accepts them and does nothing with them.
	//glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	glutCreateWindow( sName );

	glutIdleFunc( TwoWorldConfiguration_update );
	glutDisplayFunc( TwoWorldConfiguration_display );
	glutReshapeFunc( TwoWorldConfiguration_HorizontalSplit_reshape );

	glutKeyboardFunc( TwoWorldConfiguration_keyDown );
	glutKeyboardUpFunc( TwoWorldConfiguration_keyUp );

	glutSpecialFunc( TwoWorldConfiguration_keyDownSpecial );
	glutSpecialUpFunc( TwoWorldConfiguration_keyUpSpecial );

	// Insert menu function

	glutIgnoreKeyRepeat(1);

	glutMouseFunc( TwoWorldConfiguration_mouse );

	glutMotionFunc( TwoWorldConfiguration_mousemove );
	glutPassiveMotionFunc( TwoWorldConfiguration_passivemousemove);

	glutMouseWheelFunc( TwoWorldConfiguration_mousewheel );

	glClearColor(0,0,0,1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	pWorld1->LoadTextures();
	pWorld2->LoadTextures();

	glutMainLoop();
}

/////////////////////////////////////////////////////////////////////

void TwoWorldConfiguration_display()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->PreRender();
	}

	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->Render();
	}

	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->PostRender();
	}

    glutSwapBuffers();
}

float g_fTwoWorld_PrevTime = 0.0f;
float g_fTwoWorld_UpdateTime = 0.0f;

void TwoWorldConfiguration_update()
{
	float curr = glutGet(GLUT_ELAPSED_TIME);
	float delta = (curr-g_fTwoWorld_PrevTime) / 1000.0f;
	g_fTwoWorld_PrevTime = curr;

	g_fTwoWorld_UpdateTime += delta;

	//float fTimeBwFrames = 0.03f;
	float fTimeBwFrames = 0.01f;

	if(g_fTwoWorld_UpdateTime < fTimeBwFrames)
	{
		return;
	}

	g_fTwoWorld_UpdateTime = 0.0f;

	float harddelta = 0.03f;

	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->PreUpdate();
	}

	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->Update(harddelta);
	}

	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->PostUpdate();
	}

	//prev = curr;

	TwoWorldConfiguration_display();

	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->PostRenderUpdate();
	}
}

void TwoWorldConfiguration_VerticalSplit_reshape ( int width, int height )
{
	int nWidth1 = (int)((float)width * g_fTwoWorldConfiguration_SplitProportionW);
	int nWidth2 = (int)((float)width * (1.0f - g_fTwoWorldConfiguration_SplitProportionW));

	// Want world 1 on the left, world 2 on the right

	g_pTwoWorldConfiguration_Worlds[0]->Reshape(
		0, 0,
		nWidth1, height,
		width, height);

	g_pTwoWorldConfiguration_Worlds[1]->Reshape(
		nWidth1, 0,
		nWidth2, height,
		width, height);

	glutPostRedisplay();
}

void TwoWorldConfiguration_HorizontalSplit_reshape ( int width, int height )
{
	int nHeight1 = (int)((float)height * g_fTwoWorldConfiguration_SplitProportionH);
	int nHeight2 = (int)((float)height * (1.0f - g_fTwoWorldConfiguration_SplitProportionH));

	// Want world 1 on the top, world 2 on the bottom

	g_pTwoWorldConfiguration_Worlds[0]->Reshape(
		0, nHeight2,
		width, nHeight1,
		width, height);

	g_pTwoWorldConfiguration_Worlds[1]->Reshape(
		0, 0,
		width, nHeight2,
		width, height);

	glutPostRedisplay();
}

void TwoWorldConfiguration_keyDown ( unsigned char key, int x, int y )
{
	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->OnKeyDown(key, x, y);
	}

	if(key == 27) // Escape
	{
		glutLeaveMainLoop();
	}
}

void TwoWorldConfiguration_keyUp ( unsigned char key, int x, int y )
{
	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->OnKeyUp(key, x, y);
	}
}


void TwoWorldConfiguration_keyDownSpecial ( int key, int x, int y )
{
	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->OnKeyDownSpecial(key, x, y);
	}
}

void TwoWorldConfiguration_keyUpSpecial ( int key, int x, int y )
{
	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->OnKeyUpSpecial(key, x, y);
	}
}

void TwoWorldConfiguration_mousemove(int x, int y)
{
	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->OnMouseMove(x, y);
	}
}

void TwoWorldConfiguration_passivemousemove(int x, int y)
{
	for(int i = 0; i < 2; i++)
	{
		g_pTwoWorldConfiguration_Worlds[i]->OnPassiveMouseMove(x, y);
	}
}

void TwoWorldConfiguration_mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			for(int i = 0; i < 2; i++)
			{
				g_pTwoWorldConfiguration_Worlds[i]->OnLButtonDown(x, y);
			}
		}

		if(state == GLUT_UP)
		{
			for(int i = 0; i < 2; i++)
			{
				g_pTwoWorldConfiguration_Worlds[i]->OnLButtonUp(x, y);
			}
		}
	}

	if(button == GLUT_MIDDLE_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			for(int i = 0; i < 2; i++)
			{
				g_pTwoWorldConfiguration_Worlds[i]->OnMButtonDown(x, y);
			}
		}

		if(state == GLUT_UP)
		{
			for(int i = 0; i < 2; i++)
			{
				g_pTwoWorldConfiguration_Worlds[i]->OnMButtonUp(x, y);
			}
		}
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			for(int i = 0; i < 2; i++)
			{
				g_pTwoWorldConfiguration_Worlds[i]->OnRButtonDown(x, y);
			}
		}

		if(state == GLUT_UP)
		{
			for(int i = 0; i < 2; i++)
			{
				g_pTwoWorldConfiguration_Worlds[i]->OnRButtonUp(x, y);
			}
		}
	}
}

void TwoWorldConfiguration_mousewheel(int wheel, int direction, int x, int y)
{
	if(direction == 1)
	{
		for(int i = 0; i < 2; i++)
		{
			g_pTwoWorldConfiguration_Worlds[i]->OnWheelUp(x, y);
		}
	}
	else
	{
		for(int i = 0; i < 2; i++)
		{
			g_pTwoWorldConfiguration_Worlds[i]->OnWheelDown(x, y);
		}
	}
}
