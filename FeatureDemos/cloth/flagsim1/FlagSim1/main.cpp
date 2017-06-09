/**********************************************************************

  Flag in the wind

  February, 2003

  This simulation was created by Patrick Engel and Philipp Crocoll
  
  Contact: 
	philipp.crocoll@web.de
	www.codecolony.de

  Every comment would be appreciated.

***********************************************************************/

#include <GL\glut.h>
#include <vector>
#include <time.h>
#include <math.h>

#include "camera.h"
#include "textures.h"

#include "Flag.h"



//lighting:
GLfloat LightAmbient[]=		{ 0.5,0.5,0.5,1.0 };
GLfloat LightDiffuse[]=		{ 0.5,0.5,0.5,1.0 };
GLfloat LightPosition[]=	{ 0,0,-2,0 };
GLfloat LightPosition2[]=	{ 0,0,2,0 };

SF3dVector wind;
SF3dVector gravity;

COGLTexture FlagTexture1,FlagTexture2,FlagTexture3,
			GroundTexture;
CFlag Flag;

CCamera Camera;

bool g_bPaused = false;


void UpdateScene( float deltaTime)
{
	wind.x= 0.2+float(rand())/float(RAND_MAX)*0.1;

	
	
	Flag.UpdateVertices(deltaTime,gravity, wind);
}


void KeyDown(unsigned char key, int x, int y)
{	
	switch(key)
	{
	case 27:	//ESC
		exit(0);
		break;
	case 'a':		
		Camera.RotateY(5.0f);
		break;
	case 'd':		
		Camera.RotateY(-5.0f);
		break;
	case 'w':		
		Camera.MoveForwards( -0.15f ) ;
		break;
	case 's':		
		Camera.MoveForwards( 0.15f ) ;
		break;
	case 'x':		
		Camera.RotateX(5.0f);
		break;
	case 'y':		
		Camera.RotateX(-5.0f);
		break;
	case 'c':		
		Camera.StrafeRight(-0.05f);
		break;
	case 'v':		
		Camera.StrafeRight(0.05f);
		break;
	case 'f':
		Camera.Move(F3dVector(0.0,-0.1,0.0));
		break;
	case 'r':
		Camera.Move(F3dVector(0.0,0.1,0.0));
		break;
	case 'p':
		g_bPaused = ! g_bPaused;
		break;
	case 'l': 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'L': 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	}
}


void DrawScene(void)
{

	//Render Europe:
	glEnable(GL_LIGHTING);
	FlagTexture2.SetActive();
	Flag.Render();

	//Render Germany:
	glEnable(GL_LIGHTING);
	FlagTexture1.SetActive();
	glPushMatrix();
		glTranslatef(2.5,0.0,2.5);
		Flag.Render();
	glPopMatrix();

	//Render Portugal:
	glEnable(GL_LIGHTING);
	FlagTexture3.SetActive();
	glPushMatrix();
		glTranslatef(-2.5,0.0,2.5);
		Flag.Render();
	glPopMatrix();

	//Render ground:
	GroundTexture.SetActive();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	float HalfGroundSize = 20.0f;

	glTranslatef(0.0,-4.0,0.0);
	glBegin(GL_POLYGON);
	  
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-HalfGroundSize, 0.0, -HalfGroundSize);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(-HalfGroundSize, 0.0, +HalfGroundSize);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(+HalfGroundSize, 0.0, +HalfGroundSize);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(+HalfGroundSize, 0.0, -HalfGroundSize);
	  

	glEnd();


}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	Camera.Render();
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);		
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse);		
	glLightfv(GL_LIGHT2, GL_POSITION,LightPosition2);


	DrawScene();

	glFlush();			
	glutSwapBuffers();
}

void Reshape(int x, int y)
{
	if (y == 0 || x == 0) return;  //Nothing is visible then, so return
	//Set a new projection matrix
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	//Angle of view:40 degrees
	//Near clipping plane distance: 0.3
	//Far clipping plane distance: 20.0
	gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.3,40.0);
	glViewport(0,0,x,y);  //Use the whole window for rendering
	glMatrixMode(GL_MODELVIEW);
	
}

void Idle(void)
{

	//Do the physical calculation for one step:
	if (!g_bPaused) UpdateScene(0.05);
	//render the scene:
	Display();
}

int main (int argc, char **argv)
{
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize(600,600);
	//Create a window with rendering context and everything else we need
	glutCreateWindow("Flag in the wind");


	//load the textures:
	FlagTexture1.LoadFromFile("germany.bmp");
	FlagTexture2.LoadFromFile("europe.bmp");
	FlagTexture3.LoadFromFile("portugal.bmp");
	GroundTexture.LoadFromFile("ground.bmp");

	//init texture stuff:
	GroundTexture.SetActive();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FlagTexture1.SetActive();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FlagTexture2.SetActive();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FlagTexture3.SetActive();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//initialize the flag:
	Flag.Initialize(40,26,2.0f,50.0f,0.997f);
	//and the values affecting it:
	wind = F3dVector (0.2,0.04,0.00);
	gravity = F3dVector(0.0,-0.015,0.0005);

	//initialize camera: 
	Camera.Move(F3dVector(1.0f, 0.0f, 15.0f));


	//Enable the vertex array functionality:
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY );
	glClearColor(0.0,0.0,1.0,0.0);  //blue background = sky ;-)
	
	//Switch on solid rendering:
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE ,1);
	glEnable(GL_DEPTH_TEST);

	//Initialize lighting:
	glEnable(GL_LIGHT1);								
	glEnable(GL_LIGHT2);

	glEnable(GL_LIGHTING);

	glFrontFace(GL_CCW);   //Tell OGL which orientation shall be the front face
	glShadeModel(GL_SMOOTH);

	//initialize generation of random numbers:
	srand((unsigned)time(NULL));

	
	//Assign the two used Msg-routines
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyDown);
	glutIdleFunc(Idle);
	//Let GLUT get the msgs
	glutMainLoop();
	return 0;
}
