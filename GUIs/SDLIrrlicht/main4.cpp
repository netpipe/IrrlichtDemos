
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_syswm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

// clang -lSDL2 -lX11 -I/usr/include/SDL2 -Weverything main4.cpp -o main
//gcc -lSDL -lGL -lX11 main4.cpp -o main -lXxf86vm

int main()
{
	Window win;			/* pointer to the newly created window.      */
	Display *g_display;
	int g_screen;
	unsigned int display_width, display_height;	/* height and width of the X display.        */
	unsigned int width, height;	/* height and width for the new window.      */
	unsigned int win_x, win_y;	/* location of the window's top-left corner. */
	unsigned int win_border_width; /* width of window's border.                */

	char *display_name = getenv("DISPLAY");  /* address of the X display.      */
	char variable[64];
	g_display = XOpenDisplay(display_name);

	if (g_display == NULL) 
	{
		cerr<<"couldn't connect to XServer"<<endl;
		return -1;
	}

	/* get the geometry of the default screen for our display. */
	g_screen = DefaultScreen(g_display);
	display_width = DisplayWidth(g_display, g_screen);
	display_height = DisplayHeight(g_display, g_screen);

	/* make the new window occupy 1/9 of the screen's size. */
	width = 800;//(display_width);
	height = 600;//screenRect.h;
	//screenRect.w=400;//width;
	/* the window should be placed at the top-left corner of the screen. */
	win_x = 0;
	win_y = display_height-height;

  	/* the window's border shall be 2 pixels wide. */
	win_border_width = 2;

	win = XCreateSimpleWindow(g_display, RootWindow(g_display, g_screen),
                  win_x, win_y, width, height, win_border_width, BlackPixel(g_display, g_screen), WhitePixel(g_display,g_screen));


	Atom dock, win_type;
	//win_type=XInternAtom(g_display,"_NET_WM_WINDOW_TYPE",False);
	//dock=XInternAtom(g_display,"_NET_WM_WINDOW_TYPE_DOCK",False);
	XChangeProperty(g_display, win,  win_type, XA_ATOM, 32,  PropModeReplace,
		  (unsigned char *) &dock,1);
	sprintf(variable, "SDL_WINDOWID=0x%lx", win);
	putenv(variable);
	XMapWindow(g_display, win);
	XSync(g_display, False);
	cout<<"created initial X window: "<<variable<<endl;
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) { return false; } 
	
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	//Create Window 
	if( SDL_SetVideoMode( 800, 600, 32, SDL_OPENGL |
		   SDL_NOFRAME | SDL_HWPALETTE | SDL_GL_DOUBLEBUFFER /**/) == NULL ) 
	{ 
		return false;
	} 
	//Initialize OpenGL 
	//if( init_gl() == false ) 
	//{
	//	return false; 
	//} 
	//Set caption SDL_WM_SetCaption( "OpenGL Test", NULL ); return true; }
}
