
//
// A simple XLib only MessageBox() style replacement 
//

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include "MessageBox.h"

void 
MessageBox( const char * caption, const char * message )
{
	// Open the display
	Display *dpy = XOpenDisplay(0);

	// Get black/white
	int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
	int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

	// Create the window
	Window w = 
		XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
												200, 100, 0, blackColor, whiteColor);

	// Select events
	XSelectInput(dpy, w, KeyPressMask | ButtonPressMask | StructureNotifyMask |
												ExposureMask );

  // register to receive a message, when the window is about to be destroyed
  Atom wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
  XSetWMProtocols( dpy, w, &wm_delete_window, 1);

	// set window name
	XStoreName( dpy, w, caption );

	// Create a gc
	GC gc = XCreateGC(dpy, w, 0, 0);

	// load font
	XFontStruct * fs;
	fs = XLoadQueryFont( dpy, "8x13" );

	// set font in gc
	XSetFont( dpy, gc, fs->fid );

	// check the message text size
	int dir, ascent, descent;
	XCharStruct cs;
	XTextExtents( fs, message, strlen(message), &dir, &ascent, &descent, &cs );

	// try to center the window
	int xpos = (DisplayWidth( dpy, 0 ) - cs.width+16)/ 2;
	int ypos = (DisplayHeight( dpy, 0 ) - 100) / 2;

	// move it, and map it
	XMoveResizeWindow( dpy, w, xpos, ypos, cs.width+16, 100 );	

	//
	// Set size hints so that wm will not allow resizing, and
	// so that it will obey our window position given earlier.
	//
	XSizeHints * shints = XAllocSizeHints();

	shints->flags = PPosition | PMinSize | PMaxSize | PBaseSize;

	shints->x = shints->y = 0;
	shints->min_width = cs.width+16;
	shints->max_width = cs.width+16;
	shints->min_height = 100;
	shints->max_height = 100;
	shints->base_width = cs.width+16;
	shints->base_height = 100;

	XSetWMNormalHints( dpy, w, shints );
	XFree( shints );

	XMapWindow(dpy, w);
	
	//
	// process events since this is a modal dialog
	//

	while(1)
	{
		XEvent e;
		XNextEvent(dpy, &e);

		if (e.type == KeyPress || e.type == ButtonPress)
			break;
		if( e.type == ClientMessage && e.xclient.data.l[0] == wm_delete_window )
			break;

		if( e.type == Expose || e.type == ConfigureNotify )
		{
			// draw dialog text
			XSetBackground( dpy, gc, whiteColor );
			XSetForeground( dpy, gc, blackColor );
			XDrawString( dpy, w, gc, 8, 26+cs.ascent, message, strlen(message) );

			XSetBackground( dpy, gc, blackColor );
			XSetForeground( dpy, gc, whiteColor );
			XDrawImageString( dpy, w, gc, 8, 100-cs.descent-4, "[OK]", 4 );
		}
	}

	// remove window
	XUnmapWindow( dpy, w );
	XDestroyWindow( dpy, w );
}

#ifdef _TEST_
#include <stdio.h>
int main( int argc, char **argv )
{
	if( argc < 3 )
	{
		printf( "Usage: %s caption message\n", argv[0] );
		return 0;
	}

	MessageBox(argv[1], (const char*)(argv[2]));
	return 0;
}
#endif
