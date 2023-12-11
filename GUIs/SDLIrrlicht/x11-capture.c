
//https://stackoverflow.com/questions/10284858/taking-screenshot-with-libx11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

int main(void) {
XImage* pic;
Display* dpl;
unsigned int buffer_size;

dpl = XOpenDisplay("127.0.0.1:0.0");

pic = XGetImage(dpl, RootWindow(dpl, DefaultScreen(dpl)), 10, 10, 201, 201,
        AllPlanes, ZPixmap);
}

dpl = XOpenDisplay(NULL);
assert(dpl);
