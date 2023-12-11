#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrender.h>
#include <animation.h>
#include <config.h>
#include <client.h>
#include <display.h>
#include <menu.h>
#include <xurfaced.h>
#include <event.h>

static void xurfaced_event_configurerequest(struct xurfaced *xurfaced, XConfigureRequestEvent *event)
{

    XWindowChanges wc;
    wc.x = 0;
    wc.y = 0;
    wc.width = xurfaced->backend->width;
    wc.height = xurfaced->backend->height;
    wc.border_width = 0;
    wc.sibling = event->above;
    wc.stack_mode = event->detail;

    XConfigureWindow(xurfaced->backend->display, event->window, event->value_mask, &wc);

    XConfigureEvent ce;
    ce.type = ConfigureNotify;
    ce.display = xurfaced->backend->display;
    ce.event = event->window;
    ce.window = event->window;
    ce.x = 0;
    ce.y = 0;
    ce.width = xurfaced->backend->width;
    ce.height = xurfaced->backend->height;
    ce.border_width = 0;
    ce.above = 0;
    ce.override_redirect = 0;

    XSendEvent(xurfaced->backend->display, event->window, 0, StructureNotifyMask, (XEvent *)&ce);

}

static void xurfaced_event_destroywindow(struct xurfaced *xurfaced, XDestroyWindowEvent *event)
{

    struct xurfaced_client *client = xurfaced_client_list_find(xurfaced->clients, event->window);

    if (!client)
    {

        XSync(xurfaced->backend->display, 0);

        return;

    }

    xurfaced_client_list_remove(xurfaced->clients, client);
    xurfaced_client_destroy(client);

    xurfaced->clients->current = xurfaced->clients->head;

    xurfaced->paused = 0;

    XRaiseWindow(xurfaced->backend->display, xurfaced->backend->main);
    XSetInputFocus(xurfaced->backend->display, xurfaced->backend->main, RevertToParent, CurrentTime);
    XSync(xurfaced->backend->display, 0);

}

static void xurfaced_event_expose(struct xurfaced *xurfaced, XExposeEvent *event)
{

    if (event->count)
        return;

}

static void xurfaced_event_maprequest(struct xurfaced *xurfaced, XMapRequestEvent *event)
{

    struct xurfaced_client *client = xurfaced_client_create(event->window);

    if (!client)
        return;

    xurfaced_client_list_add(xurfaced->clients, client);
    xurfaced->clients->current = client;

    XSelectInput(xurfaced->backend->display, client->window, StructureNotifyMask | PropertyChangeMask);
    XRaiseWindow(xurfaced->backend->display, client->window);
    XMoveResizeWindow(xurfaced->backend->display, client->window, 0, 0, xurfaced->backend->width, xurfaced->backend->height);
    XMapWindow(xurfaced->backend->display, client->window);
    XSetInputFocus(xurfaced->backend->display, client->window, RevertToParent, CurrentTime);
    XSync(xurfaced->backend->display, 0);

    XWindowAttributes wa;
    XGetWindowAttributes(xurfaced->backend->display, client->window, &wa);

    XRenderPictFormat *format = XRenderFindVisualFormat(xurfaced->backend->display, wa.visual);

    XRenderPictureAttributes pa;
    pa.subwindow_mode = IncludeInferiors;

    client->picture = XRenderCreatePicture(xurfaced->backend->display, client->window, format, CPSubwindowMode, &pa);

    xurfaced->paused = 1;

}

static void xurfaced_event_unmap(struct xurfaced *xurfaced, XUnmapEvent *event)
{

}

static void xurfaced_event_buttonpress(struct xurfaced *xurfaced, XButtonEvent *event)
{

    switch (event->button)
    {

        case 1:

            xurfaced_menu_activate(xurfaced->menu);

            break;

        case 4:

            xurfaced_menu_previous(xurfaced->menu, 1);

            break;

        case 5:

            xurfaced_menu_next(xurfaced->menu, 1);

            break;

    }

    XSync(xurfaced->backend->display, 0);

}

static void xurfaced_event_keypress(struct xurfaced *xurfaced, XKeyPressedEvent *event)
{

    KeySym key = XLookupKeysym(event, 0);

    char skey[32];

    sprintf(skey, "%d", (unsigned int)key);
    xurfaced_config_write(xurfaced->config.key, skey);

    char path[128];

    if (event->state & Mod1Mask & ShiftMask)
        sprintf(path, "%s/key-mod1-shift-%u", xurfaced->config.base, (unsigned int)key);
    else if (event->state & Mod1Mask)
        sprintf(path, "%s/key-mod1-%u", xurfaced->config.base, (unsigned int)key);
    else
        sprintf(path, "%s/key-%u", xurfaced->config.base, (unsigned int)key);

    struct stat info;

    if (stat(path, &info) != -1)
        xurfaced_execute(path, 0);

    int status;

    wait(&status);

    switch (key)
    {

        case XK_c:

            if (!(event->state & Mod1Mask))
                break;

            if (!xurfaced->clients->current)
                break;

    		XKillClient(xurfaced->backend->display, xurfaced->clients->current->window);

            break;

        case XK_Up:

            xurfaced_menu_previous(xurfaced->menu, 1);

            break;

        case XK_Page_Up:

            xurfaced_menu_previous(xurfaced->menu, 6);

            break;

        case XK_Down:

            xurfaced_menu_next(xurfaced->menu, 1);

            break;

        case XK_Page_Down:

            xurfaced_menu_next(xurfaced->menu, 6);

            break;

        case XK_Tab:

            if (!(event->state & Mod1Mask))
                break;

            if (!xurfaced->clients->current)
                break;

            xurfaced->clients->current = xurfaced->clients->current->next;

            XRaiseWindow(xurfaced->backend->display, xurfaced->clients->current->window);
            XSetInputFocus(xurfaced->backend->display, xurfaced->clients->current->window, RevertToParent, CurrentTime);

            break;

        case XK_Escape:

            if (!(event->state & Mod1Mask))
                break;

            xurfaced->paused = 0;

            XRaiseWindow(xurfaced->backend->display, xurfaced->backend->main);
            XSetInputFocus(xurfaced->backend->display, xurfaced->backend->main, RevertToParent, CurrentTime);

            break;

        case XK_Return:

            xurfaced_menu_activate(xurfaced->menu);

            break;

    }

    XSync(xurfaced->backend->display, 0);

}

void xurfaced_event_handler(struct xurfaced *xurfaced)
{

    XEvent event;
    XNextEvent(xurfaced->backend->display, &event);

    switch (event.type)
    {

        case ButtonPress:

            xurfaced_event_buttonpress(xurfaced, &event.xbutton);

            break;

        case Expose:

            xurfaced_event_expose(xurfaced, &event.xexpose);

            break;

        case KeyPress:

            xurfaced_event_keypress(xurfaced, &event.xkey);

            break;

        case MapRequest:

            xurfaced_event_maprequest(xurfaced, &event.xmaprequest);

            break;

        case ConfigureRequest:

            xurfaced_event_configurerequest(xurfaced, &event.xconfigurerequest);

            break;

        case UnmapNotify:

            xurfaced_event_unmap(xurfaced, &event.xunmap);

            break;

        case DestroyNotify:

            xurfaced_event_destroywindow(xurfaced, &event.xdestroywindow);

            break;

    }

}

