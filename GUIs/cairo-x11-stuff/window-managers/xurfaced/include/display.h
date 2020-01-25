#ifndef XURFACED_DISPLAY_H
#define XURFACED_DISPLAY_H

struct xurfaced_display_backend
{

    Display *display;
    int descriptor;
    int screen;
    int width;
    int height;
    Visual *visual;
    Window root;
    Window main;
    Picture mainPicture;
    Atom atom_net[4];

};

extern struct xurfaced_display_backend *xurfaced_display_create();
extern void xurfaced_display_destroy(struct xurfaced_display_backend *backend);

#endif

