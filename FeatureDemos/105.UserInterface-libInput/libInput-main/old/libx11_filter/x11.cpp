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
#include <X11/extensions/XInput2.h>

int screen;
Display *display;
XSetWindowAttributes window_attributes;
Window mainwindow;
XEvent event;
int root_x, root_y; //<--two
unsigned int mask; //<--three

int main(int argc, char** argv) {
    display = XOpenDisplay(NULL);
    screen = DefaultScreen(display);
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


    XGrabPointer(display, mainwindow, False, ButtonPressMask, GrabModeAsync,
                 GrabModeAsync, None, None, CurrentTime);

    int button;
    int x=-1,y=-1;

    XMapWindow(display, mainwindow);
    XFlush(display);
    // How to set a mask for the desired input device (keyboard, mouse)?
    /* Check for XInput extension */
    int opcode, first_event, error;
    if (!XQueryExtension(display, "XInputExtension", &opcode, &first_event, &error)) {
        printf("XInputExtension not available\n");
        return -1;
    }

    int major = 2, minor = 0;
    if (XIQueryVersion(display, &major, &minor) == BadRequest) {
        printf("XI2 not available, the X server supports %d.%d\n", major, minor);
        return -1;
    }

    
    int ndevices;
    XIDeviceInfo *devices, *device;

    devices = XIQueryDevice(display, XIAllDevices, &ndevices);

    for (int i = 0; i < ndevices; i++) {
        device = &devices[i];
        printf("Device %s (id: %d) is a ", device->name, device->deviceid);

        switch(device->use) {
        case XIMasterPointer: printf("master pointer\n"); break;
        case XIMasterKeyboard: printf("master keyboard\n"); break;
        case XISlavePointer: printf("slave pointer\n"); break;
        case XISlaveKeyboard: printf("slave keyboard\n"); break;
        case XIFloatingSlave: printf("floating slave\n"); break;
        }

        printf("Device is attached to/paired with %d\n", device->attachment);
    }

    XIFreeDeviceInfo(devices); 

    XIEventMask eventmask;
    unsigned char mask[1] = {0};
    eventmask.deviceid = 15;
    eventmask.mask_len = sizeof(mask);
    eventmask.mask = mask;
    XISetMask(mask, XI_ButtonPress);
    XISetMask(mask, XI_Motion);
    XISetMask(mask, XI_KeyPress);

    XIEventMask eventmask2;
    unsigned char mask2[1] = {0};
    eventmask2.deviceid = 13;
    eventmask2.mask_len = sizeof(mask2);
    eventmask2.mask = mask2;
    XISetMask(mask2, XI_ButtonPress);
    XISetMask(mask2, XI_Motion);
    XISetMask(mask2, XI_KeyPress);

    XISelectEvents(display, mainwindow, &eventmask, 1); 
//	XISelectEvents(display, mainwindow, &eventmask2, 1); 
    //XSelectInput(display, mainwindow, ExposureMask|KeyPressMask|ButtonPressMask);
    
    while (1)  {
        XNextEvent(display, &event);
        //switch  (events.type) {

      //   XQueryPointer(display, DefaultRootWindow(display), &mainwindow, &mainwindow, &root_x, &root_y, &root_x, &root_y, &mask); //<--four
      // printf("Mouse coordinates (X: %d, Y: %d)\n", root_x, root_y);
        if (event.xcookie.type == GenericEvent && 
            event.xcookie.extension == opcode &&
            XGetEventData(display, &event.xcookie)) {
            XIDeviceEvent* xievent = (XIDeviceEvent*)event.xcookie.data; 
            switch (event.xcookie.evtype) {
            case XI_ButtonPress:
                printf("Button Pressed! %d\n", xievent->detail);
                break;
            case XI_Motion:
                printf("Motion detected! %p\n", event.xcookie.data);
                break;
            case XI_KeyPress:
                printf("Key Pressed! %d\n", xievent->detail);
                break;
            }
            XFreeEventData(display, &event.xcookie);
        }
        
        switch(event.type){
        case ButtonPress:
            switch(event.xbutton.button){
            case Button1:
                x=event.xbutton.x;
                y=event.xbutton.y;
                button=Button1;
            printf("button1\n");
                break;

            case Button3:
                x=event.xbutton.x;
                y=event.xbutton.y;
                button=Button3;
            printf("button3\n");
                break;
            default:
                break;
            }
        }

        if (event.type == KeyPress)
        {
            printf( "KeyPress: %x\n", event.xkey.keycode );
            if ( event.xkey.keycode == 0x09 )
                break;
        }
        else if (event.type == KeyRelease)
        {
            printf( "KeyRelease: %x\n", event.xkey.keycode );
        }
        
    }
    XCloseDisplay(display);
    return 0;
}
