#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

using namespace cv;
using namespace std;

#define ACTIVE_WINDOWS "_NET_CLIENT_LIST"

#define WINDOW_TITLE "Wolf3d Screenshot"
#define WINDOW_DOXBOX "DOSBox"

// Find the DOSBox Window so we can do cool stuff with it!
bool findDOSBoxWindow(Display *display, Window &window)
{
    bool found = false;
    Window rootWindow = RootWindow(display, DefaultScreen(display));
    Atom atom = XInternAtom(display, ACTIVE_WINDOWS, true);
    Atom actualType;
    int format;
    unsigned long numItems;
    unsigned long bytesAfter;
    
    unsigned char *data = '\0';
    Window *list;    
    char *windowName;

    int status = XGetWindowProperty(display, rootWindow, atom, 0L, (~0L), false,
        AnyPropertyType, &actualType, &format, &numItems, &bytesAfter, &data);
    list = (Window *)data;
    
    if (status >= Success && numItems) {
        for (int i = 0; i < numItems; ++i) {
            status = XFetchName(display, list[i], &windowName);
            if (status >= Success) {
                string windowNameStr(windowName);
                if (windowNameStr.find(WINDOW_DOXBOX) == 0) {
                    window = list[i];
                    found = true;
                    break;
                }
            }
        }
    }

    XFree(windowName);
    XFree(data);

    return found;
}

void millisleep(int ms)
{
    usleep(ms * 1000);
}

int main(int argc, char *argv[])
{
    Display *display = XOpenDisplay(NULL);
    Window rootWindow = RootWindow(display, DefaultScreen(display));    
    Window DOSBoxWindow;

    XWindowAttributes DOSBoxWindowAttributes;
    if (findDOSBoxWindow(display, DOSBoxWindow) == false) {
        printf("Error: Cannot find DOSBox window. Exiting program.");
        return 0;
    }

    XKeyEvent event;
    event.type = KeyPress;      
    event.display = display;
    event.send_event = False;
    event.window = DOSBoxWindow;
    event.root = rootWindow;
    event.time = CurrentTime;
    event.same_screen = True;
    event.keycode = XKeysymToKeycode(display, XK_Up);
    XSendEvent(display, DOSBoxWindow, True, KeyPressMask, (XEvent *)&event);
    XFlush(display);

    millisleep(100);

    event.type = KeyRelease;      
    XSendEvent(display, DOSBoxWindow, True, KeyReleaseMask, (XEvent *)&event);
    XFlush(display);
}
