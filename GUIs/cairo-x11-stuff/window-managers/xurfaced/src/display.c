#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <display.h>

struct xurfaced_display_backend *xurfaced_display_create()
{

    XInitThreads();

    struct xurfaced_display_backend *backend = malloc(sizeof (struct xurfaced_display_backend));
    backend->display = XOpenDisplay(0);

    if (!backend->display)
    {

        fprintf(stderr, "xurfaced: can not open display\n");
        exit(EXIT_FAILURE);

    }

    backend->descriptor = XConnectionNumber(backend->display);
    backend->screen = XDefaultScreen(backend->display);
    backend->width = XDisplayWidth(backend->display, backend->screen);
    backend->height = XDisplayHeight(backend->display, backend->screen);
    backend->visual = XDefaultVisual(backend->display, 0);

    return backend;

}

void xurfaced_display_destroy(struct xurfaced_display_backend *backend)
{

    XCloseDisplay(backend->display);
    free(backend);

}

