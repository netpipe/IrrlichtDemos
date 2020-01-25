//https://stackoverflow.com/questions/12465552/c-and-xlib-center-window
//XDisplayWidth(dpy, sreen); XDisplayHeight(dpy, sreen);
//getenv("DISPLAY")
//putenv( mypath ); 
//xwininfo -tree -root
//sts = system(command);
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

int screen;
Display *display;
XSetWindowAttributes window_attributes;
Window mainwindow;
XEvent events;

int main(int argc, char** argv) {
    display = XOpenDisplay(NULL);screen = DefaultScreen(display);
    window_attributes.background_pixel = XWhitePixel(display, screen);
    window_attributes.border_pixel = XBlackPixel(display, screen);
    window_attributes.win_gravity = SouthWestGravity;

    mainwindow = XCreateWindow(display,
                             RootWindow(display, screen),
                             1, 1,
                             600, 400,
                             0,
                             CopyFromParent,
                             InputOutput,
                             CopyFromParent,
                             CWBackPixel|CWBorderPixel,
                             &window_attributes
                            );
    XMapWindow(display, mainwindow);
    XFlush(display);
    XSelectInput(display, mainwindow, ExposureMask|KeyPressMask|ButtonPressMask);
    while (1)  {
        XNextEvent(display, &events);
        switch  (events.type) {
            case Expose:
                // Lots of code that doesn't matter for my problem
            break;
        }
    }
    return 0;
}
