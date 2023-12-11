
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "FourWorldConfiguration.h"

#include <GL/glut.h>

#include "World.h"

World * g_pFourWorldConfiguration_Worlds[4] = {0,0,0,0};
float g_fFourWorldConfiguration_SplitProportionH = 0.7f;
float g_fFourWorldConfiguration_SplitProportionW = 0.4f;

void FourWorldConfiguration_update();
void FourWorldConfiguration_display();
void FourWorldConfiguration_reshape( int width, int height );
void FourWorldConfiguration_keyDown( unsigned char key, int x, int y );
void FourWorldConfiguration_keyUp( unsigned char key, int x, int y );
void FourWorldConfiguration_keyDownSpecial( int key, int x, int y );
void FourWorldConfiguration_keyUpSpecial( int key, int x, int y );
void FourWorldConfiguration_mouse(int button, int state, int x, int y);
void FourWorldConfiguration_mousemove(int x, int y );
void FourWorldConfiguration_passivemousemove(int x, int y );
void FourWorldConfiguration_mousewheel(int wheel, int direction, int x, int y);

World * GetFourWorldConfigurationWorld(int nID)
{
	return g_pFourWorldConfiguration_Worlds[nID];
}

void RunFourWorldConfiguration(
	World * pWorld1, World * pWorld2, World * pWorld3, World * pWorld4,
	char * sName,
	float fSplitProportionW, float fSplitProportionH,
	int argc, char** argv)
{
	g_pFourWorldConfiguration_Worlds[0] = pWorld1;
	g_pFourWorldConfiguration_Worlds[1] = pWorld2;
	g_pFourWorldConfiguration_Worlds[2] = pWorld3;
	g_pFourWorldConfiguration_Worlds[3] = pWorld4;

	g_fFourWorldConfiguration_SplitProportionH = fSplitProportionH;
	g_fFourWorldConfiguration_SplitProportionW = fSplitProportionW;

	glutInit( &argc, argv );
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow( sName );

	glutIdleFunc( FourWorldConfiguration_update );
	glutDisplayFunc( FourWorldConfiguration_display );
	glutReshapeFunc( FourWorldConfiguration_reshape );

	glutKeyboardFunc( FourWorldConfiguration_keyDown );
	glutKeyboardUpFunc( FourWorldConfiguration_keyUp );

	glutSpecialFunc( FourWorldConfiguration_keyDownSpecial );
	glutSpecialUpFunc( FourWorldConfiguration_keyUpSpecial );

	// Insert menu function

	glutIgnoreKeyRepeat(1);

	glutMouseFunc( FourWorldConfiguration_mouse );

	glutMotionFunc( FourWorldConfiguration_mousemove );
	glutPassiveMotionFunc( FourWorldConfiguration_passivemousemove);

//	glutMouseWheelFunc( FourWorldConfiguration_mousewheel );

	glClearColor(0,0,0,1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	pWorld1->LoadTextures();
	pWorld2->LoadTextures();
	pWorld3->LoadTextures();
	pWorld4->LoadTextures();

	glutMainLoop();
}

/////////////////////////////////////////////////////////////////////

void FourWorldConfiguration_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->PreRender();
	}

	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->Render();
	}

	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->PostRender();
	}

    glutSwapBuffers();
}

float g_fFourWorld_PrevTime = 0.0f;
float g_fFourWorld_UpdateTime = 0.0f;

void FourWorldConfiguration_update()
{
	float curr = glutGet(GLUT_ELAPSED_TIME);
	float delta = (curr-g_fFourWorld_PrevTime) / 1000.0f;
	g_fFourWorld_PrevTime = curr;

	g_fFourWorld_UpdateTime += delta;

	//float fTimeBwFrames = 0.03f;
	float fTimeBwFrames = 0.01f;

	if(g_fFourWorld_UpdateTime < fTimeBwFrames)
	{
		return;
	}

	g_fFourWorld_UpdateTime = 0.0f;

	float harddelta = 0.03f;

	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->PreUpdate();
	}

	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->Update(harddelta);
	}

	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->PostUpdate();
	}

	//prev = curr;

	FourWorldConfiguration_display();

	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->PostRenderUpdate();
	}
}

void FourWorldConfiguration_reshape ( int width, int height )
{
	int nHeight1 = (int)((float)height * g_fFourWorldConfiguration_SplitProportionH);
	int nHeight2 = (int)((float)height * (1.0f - g_fFourWorldConfiguration_SplitProportionH));

	int nWidth1 = (int)((float)width * g_fFourWorldConfiguration_SplitProportionW);
	int nWidth2 = (int)((float)width * (1.0f - g_fFourWorldConfiguration_SplitProportionW));

	// Arrange the worlds like left to right, top to bottom, like so:
	// 1 2
	// 3 4

	g_pFourWorldConfiguration_Worlds[0]->Reshape(
		0, nHeight2,
		nWidth1, nHeight1,
		width, height);

	g_pFourWorldConfiguration_Worlds[1]->Reshape(
		nWidth1, nHeight2,
		nWidth2, nHeight1,
		width, height);

	g_pFourWorldConfiguration_Worlds[2]->Reshape(
		0, 0,
		nWidth1, nHeight2,
		width, height);

	g_pFourWorldConfiguration_Worlds[3]->Reshape(
		nWidth1, 0,
		nWidth2, nHeight2,
		width, height);

	glutPostRedisplay();
}

void FourWorldConfiguration_keyDown ( unsigned char key, int x, int y )
{
	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->OnKeyDown(key, x, y);
	}

	if(key == 27) // Escape
	{
		//glutLeaveMainLoop();
		//triewd  glutExit();
		glutDestroyWindow(glutGetWindow());
	}
}

void FourWorldConfiguration_keyUp ( unsigned char key, int x, int y )
{
	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->OnKeyUp(key, x, y);
	}
}


void FourWorldConfiguration_keyDownSpecial ( int key, int x, int y )
{
	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->OnKeyDownSpecial(key, x, y);
	}
}

void FourWorldConfiguration_keyUpSpecial ( int key, int x, int y )
{
	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->OnKeyUpSpecial(key, x, y);
	}
}

void FourWorldConfiguration_mousemove(int x, int y)
{
	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->OnMouseMove(x, y);
	}
}

void FourWorldConfiguration_passivemousemove(int x, int y)
{
	for(int i = 0; i < 4; i++)
	{
		g_pFourWorldConfiguration_Worlds[i]->OnPassiveMouseMove(x, y);
	}
}

void FourWorldConfiguration_mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			for(int i = 0; i < 4; i++)
			{
				g_pFourWorldConfiguration_Worlds[i]->OnLButtonDown(x, y);
			}
		}

		if(state == GLUT_UP)
		{
			for(int i = 0; i < 4; i++)
			{
				g_pFourWorldConfiguration_Worlds[i]->OnLButtonUp(x, y);
			}
		}
	}

	if(button == GLUT_MIDDLE_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			for(int i = 0; i < 4; i++)
			{
				g_pFourWorldConfiguration_Worlds[i]->OnMButtonDown(x, y);
			}
		}

		if(state == GLUT_UP)
		{
			for(int i = 0; i < 4; i++)
			{
				g_pFourWorldConfiguration_Worlds[i]->OnMButtonUp(x, y);
			}
		}
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			for(int i = 0; i < 4; i++)
			{
				g_pFourWorldConfiguration_Worlds[i]->OnRButtonDown(x, y);
			}
		}

		if(state == GLUT_UP)
		{
			for(int i = 0; i < 4; i++)
			{
				g_pFourWorldConfiguration_Worlds[i]->OnRButtonUp(x, y);
			}
		}
	}
}

void FourWorldConfiguration_mousewheel(int wheel, int direction, int x, int y)
{
	if(direction == 1)
	{
		for(int i = 0; i < 4; i++)
		{
			g_pFourWorldConfiguration_Worlds[i]->OnWheelUp(x, y);
		}
	}
	else
	{
		for(int i = 0; i < 4; i++)
		{
			g_pFourWorldConfiguration_Worlds[i]->OnWheelDown(x, y);
		}
	}
}
