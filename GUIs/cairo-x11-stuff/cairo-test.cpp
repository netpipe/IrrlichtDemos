/*
	Grabs a screenshot of the root window.

	Usage	: ./scr_tool <display> <output file>
	Example	: ./scr_tool :0 /path/to/output.png

	Author: S Bozdag <selcuk.bozdag@gmail.com>
*/

#include <assert.h>
#include <stdio.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>

int main(int argc, char** argv) {
    Display *disp;
    Window root;
    cairo_surface_t *surface;
    int scr;
    /* The only checkpoint only concerns about the number of parameters, see "Usage" */
    if( argc != 3) {
    	fprintf(stderr, "Wrong number of parameters given \n");
    	fprintf(stderr, "Usage: ./ahenk_import <display> <output file> \n");
    	return 1;
    }
    /* try to connect to display, exit if it's NULL */
    disp = XOpenDisplay( argv[1] );
    if( disp == NULL ){
    	fprintf(stderr, "Given display cannot be found, exiting: %s\n" , argv[1]);
    	return 1;
    }
    scr = DefaultScreen(disp);
    root = DefaultRootWindow(disp);
    /* get the root surface on given display */
    surface = cairo_xlib_surface_create(disp, root, DefaultVisual(disp, scr),
                                                    DisplayWidth(disp, scr),
                                                    DisplayHeight(disp, scr));
    /* right now, the tool only outputs PNG images */
    cairo_surface_write_to_png( surface, argv[2] );
    /* free the memory*/
    cairo_surface_destroy(surface);
    /* return with success */
    return 0;
}
