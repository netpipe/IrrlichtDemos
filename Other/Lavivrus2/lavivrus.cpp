#include "Lavivrus.h"
#include "CSystems.h"
#include "3ds.h"

object* test3ds;
string filename;

void init(){
test3ds=new object;
	SDL_Init(SDL_INIT_VIDEO);
	surface = SDL_SetVideoMode(800, 600, 0, SDL_OPENGL | SDL_GL_DOUBLEBUFFER );	//| SDL_FULLSCREEN
	glViewport(0, 0, 800, 600);
	glClearColor(0.0f, 0.0f,0.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity( );
	glShadeModel( GL_SMOOTH );
	gluPerspective( 75.0f, (4/3), 1.0f, 100.0f );  // 75, 4/3, 1.0f, 1000.0f
	glFrustum (-1.0, 1.0, -1.0, 1.0, 0.0f, 20.0);  // left right bottom top near far

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity( );
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
//  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

//	glDisable( GL_LIGHTING );//disable lighting
//	glDisable( GL_BLEND );
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
	glEnable (GL_COLOR_MATERIAL);

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 5.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
Load3dsObject(test3ds,"oldChevy-Truck.3ds");
CreateList();
//DispMods();
  }

float mn;




void Render(){
	mn++;
	//glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT | GL_STENCIL_BUFFER_BIT);
	if ( mn > 360){mn=0;};
	GLfloat light_position[] = { 1.0, 10.0, 6.0, 1.0 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glPushMatrix();


	DrawObject(test3ds);

	//gluLookAt (0.0,10.0,10.0,  0.0,0.0,0.0, 0.0,1.0,0.0); // camera , lookat , up
	GLfloat xtrans = xpos;
	GLfloat ztrans = zpos;
	GLfloat ytrans = -walkbias - 0.25f- jump;
	if(jump > 0.5f){jump -= 0.02;};
	GLfloat sceneroty = 360.0f - yrot; /* 360 Degree Angle For Player Direction */

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glRotatef( sceneroty, 0.0f, 1.0f , 0.0f );
	glTranslatef( xtrans, ytrans, ztrans );

	glCallList(ListID);




  glPushMatrix();
	glTranslatef( xtrans, 0, ztrans );
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glPopMatrix();

	//glScalef(7,1,7);
	glLoadIdentity();
	glRotatef( lookupdown, 1.0f, 0.0f , 0.0f );


    SDL_GL_SwapBuffers();
}



int main(int argc, char *argv[]){

	init();
	SDL_ShowCursor (SDL_DISABLE);
	int isActive = TRUE;
//	SDL_EnableKeyRepeat( 30, SDL_DEFAULT_REPEAT_INTERVAL );

	while ( !done )
	{
	  while ( SDL_PollEvent( &event ) )
		{
		switch( event.type ){
		case SDL_ACTIVEEVENT:
			if ( event.active.gain == 0 )
				isActive = FALSE;
			    else
				isActive = TRUE;
			    break;

		case SDL_MOUSEMOTION:
		if (event.motion.yrel!=0)
		{
		if(lookupdown+event.motion.yrel < 80 && event.motion.yrel+lookupdown > -80)
		{	lookupdown += (float)(event.motion.yrel);	};
		};

		if(event.motion.xrel!=0)
		{
			if(yrot - event.motion.xrel > 359 || yrot - event.motion.xrel < -359){yrot = 0;}
			yrot -= (float)(event.motion.xrel);
		};
		break;

		case SDL_KEYDOWN:
		KeyDown( &event.key.keysym );
		break;
		case SDL_KEYUP:
		KeyUp( &event.key.keysym );
		break;
			}
		}
		if ( isActive ){
			KeyBoardControl();
			Render();

		}
	SDL_Delay(30);
	}
SDL_Quit();
}
