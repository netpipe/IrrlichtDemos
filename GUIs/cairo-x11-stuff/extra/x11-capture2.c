//https://forums.freebsd.org/threads/make-a-desktop-screenshot-with-only-libx11-dev.61755/

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>  
#include <time.h>  
int main( int argc, char *argv[])
{

    ////////////////////////////////////////////////////////
    if ( argc == 2)
      if ( strcmp( argv[1] , "" ) !=  0 ) 
      {

   Display *d;
   Window w;
   XEvent e;
   char *msg = "";
   char cmdi[ PATH_MAX ]; 
   int s;

      strncpy( cmdi , "" , PATH_MAX);
      strncat( cmdi , strtimenow() , PATH_MAX - strlen( cmdi ) - 1);
      strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) - 1);
      strncat( cmdi , argv[ 1 ], PATH_MAX - strlen( cmdi ) - 1);
      strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) - 1);

   int WINWIDTH = strlen( cmdi ) * 9;

   d = XOpenDisplay(NULL);
   if (d == NULL) {
      fprintf(stderr, "Cannot open display\n");
      exit(1);
   }

   s = DefaultScreen(d);
   w = XCreateSimpleWindow(d, RootWindow(d, s), 0, 0, WINWIDTH , 20, 1, BlackPixel(d, s), WhitePixel(d, s));
   XStoreName( d , w , "nxmessage");
   XSelectInput(d, w, ExposureMask | KeyPressMask);
   XMapWindow(d, w);
   XMoveResizeWindow(d, w , 2 , 2 ,  WINWIDTH  , 20 );
   XRaiseWindow(d , w );
   {
      XNextEvent(d, &e);
      if (e.type == Expose) 
      {
    {
           XClearWindow(d, w);
           XDrawString(d, w, DefaultGC(d, s), 5, 10, cmdi , strlen( cmdi ));
           XFlush( d );
      usleep( 10000e2 );
    }
      }
   }
         XCloseDisplay(d);
         return 0;
      }
   return 0;
}
