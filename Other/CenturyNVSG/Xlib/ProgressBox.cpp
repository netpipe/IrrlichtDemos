
//
// A simple XLib only Progress window replacement 
//

#include "ProgressBox.h"
#include <X11/Xutil.h>
#include <string.h>

ProgressBox::ProgressBox()
  : m_display( XOpenDisplay(0) )
  , m_window(0)
  , m_gc(0)
  , m_rangeLow(0)
  , m_rangeHigh(100)
  , m_step(1)
  , m_curStep(0)
  , m_displayed(false)
{
}

ProgressBox::~ProgressBox()
{
  if( m_display && m_window )
  {
    XDestroyWindow( m_display, m_window );
  }

  if( m_display )
  {
    XCloseDisplay( m_display );
  }
}

void
ProgressBox::Create()
{
  const int width = 166;
  const int height = 50;

	// Get black/white
	int blackColor = BlackPixel(m_display, DefaultScreen(m_display));
	int whiteColor = WhitePixel(m_display, DefaultScreen(m_display));

	// Create the window
	m_window = 
		XCreateSimpleWindow(m_display, DefaultRootWindow(m_display), 0, 0, 
												width, height, 0, blackColor, whiteColor);

	// Select events
	XSelectInput(m_display, m_window, StructureNotifyMask | ExposureMask );

  // register to receive a message, when the window is about to be destroyed
  Atom wm_delete_window = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols( m_display, m_window, &wm_delete_window, 1);

	// set window name
	XStoreName( m_display, m_window, "Progress" );

	// Create a gc
	m_gc = XCreateGC(m_display, m_window, 0, 0);

	// load font - not neede for the moment
	XFontStruct * fs;
	fs = XLoadQueryFont( m_display, "8x13" );

	// set font in gc
	XSetFont( m_display, m_gc, fs->fid );

	// try to center the window
	int xpos = (DisplayWidth( m_display, 0 ) - width)/ 2;
	int ypos = (DisplayHeight( m_display, 0 ) - height) / 2;

	// move it, and map it
	XMoveResizeWindow( m_display, m_window, xpos, ypos, width, height );	

	//
	// Set size hints so that wm will not allow resizing, and
	// so that it will obey our window position given earlier.
	//
	XSizeHints * shints = XAllocSizeHints();

	shints->flags = PPosition | PMinSize | PMaxSize | PBaseSize;

	shints->x = shints->y = 0;
	shints->min_width = width;
	shints->max_width = width;
	shints->min_height = height;
	shints->max_height = height;
	shints->base_width = width;
	shints->base_height = height;

	XSetWMNormalHints( m_display, m_window, shints );
	XFree( shints );
}

void
ProgressBox::SetRange( int low, int high )
{
  m_rangeLow = low;
  m_rangeHigh = high;
}

void
ProgressBox::SetStep( int step )
{
  m_step = step;
}

void
ProgressBox::StepIt( void )
{
  if( !m_displayed )
  {
  }

  m_curStep += m_step;

  // Throw an expose event into the queue, to have the window redrawn

  XExposeEvent expose;
  expose.type = Expose;
  expose.display = m_display;
  expose.window = m_window;
  expose.count = 0;

  XSendEvent( m_display, m_window, True, ExposureMask, (XEvent*)&expose );

	processMessages();
}

void
ProgressBox::processMessages()
{
  XEvent e;

  // we only want events for our window
	while( XCheckWindowEvent( m_display, m_window, ~0, &e ) )
	{
    if( e.type == Expose || e.type == ConfigureNotify || e.type == MapNotify )
    {
			XSetForeground( m_display, m_gc, 
												BlackPixel(m_display, DefaultScreen(m_display)) );

      int width = (int)
								(((float)m_curStep / (float)(m_rangeHigh - m_rangeLow))*150.f);
      XFillRectangle( m_display, m_window, m_gc, 8, 10, width, 20 );
    }
	}
}

void
ProgressBox::ShowWindow( bool tf )
{
  if( tf )
  {
    if( m_display && m_window )
    {
      XMapWindow( m_display, m_window );
    }
  }
  else
  {
    if( m_display && m_window )
    {
      XUnmapWindow( m_display, m_window );
    }
  }
}

#ifdef _TEST_
#include <stdio.h>
#include <unistd.h>
int main( int argc, char **argv )
{
  ProgressBox pb;

  pb.Create();
  pb.SetRange( 0, 10 );
  pb.SetStep(1);
  pb.ShowWindow( true );

  int i = 0;

  while( i < 10 )
  {
    pb.StepIt();
    sleep(1);
    i++;
  }

	return 0;
}
#endif
