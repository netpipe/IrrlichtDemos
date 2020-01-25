#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#include <display.h>
#include <window.h>

static void xurfaced_window_init_root(struct xurfaced_display_backend *backend)
{

    backend->root = XRootWindow(backend->display, backend->screen);
    backend->atom_net[0] = XInternAtom(backend->display, "_NET_SUPPORTED", 0);
    backend->atom_net[1] = XInternAtom(backend->display, "_NET_WM_NAME", 0);
    backend->atom_net[2] = XInternAtom(backend->display, "_NET_WM_STATE", 0);
    backend->atom_net[3] = XInternAtom(backend->display, "_NET_WM_STATE_FULLSCREEN", 0);

    XSelectInput(backend->display, backend->root, SubstructureRedirectMask | SubstructureNotifyMask | StructureNotifyMask | PropertyChangeMask);
    XChangeProperty(backend->display, backend->root, backend->atom_net[0], XA_ATOM, 32, PropModeReplace, (unsigned char *)backend->atom_net, 4);
    XGrabKey(backend->display, XKeysymToKeycode(backend->display, XK_c), Mod1Mask | ShiftMask, backend->root, 1, GrabModeAsync, GrabModeAsync);
    XGrabKey(backend->display, XKeysymToKeycode(backend->display, XK_Escape), Mod1Mask, backend->root, 1, GrabModeAsync, GrabModeAsync);
    XGrabKey(backend->display, XKeysymToKeycode(backend->display, XK_Tab), Mod1Mask, backend->root, 1, GrabModeAsync, GrabModeAsync);
    XCompositeRedirectSubwindows(backend->display, backend->root, CompositeRedirectAutomatic);
    XSync(backend->display, 0);

}

static void xurfaced_window_init_main(struct xurfaced_display_backend *backend)
{

    XRenderPictureAttributes pa;
    pa.subwindow_mode = IncludeInferiors;

    backend->main = XCreateSimpleWindow(backend->display, backend->root, 0, 0, backend->width, backend->height, 0, XBlackPixel(backend->display, backend->screen), XBlackPixel(backend->display, backend->screen));
    backend->mainPicture = XRenderCreatePicture(backend->display, backend->main, XRenderFindVisualFormat(backend->display, backend->visual), CPSubwindowMode, &pa); 

    //XShapeSelectInput(backend->display, backend->main, ShapeNotifyMask);
    XSelectInput(backend->display, backend->main, ButtonPressMask | KeyPressMask);
    XMapWindow(backend->display, backend->main);
    XSync(backend->display, 0);

}

void xurfaced_window_init(struct xurfaced_display_backend *backend)
{

    xurfaced_window_init_root(backend);
    xurfaced_window_init_main(backend);

}

void xurfaced_window_destroy(struct xurfaced_display_backend *backend)
{

    XUnmapWindow(backend->display, backend->main);
    XUngrabKeyboard(backend->display, CurrentTime);
    XSync(backend->display, 0);

}

