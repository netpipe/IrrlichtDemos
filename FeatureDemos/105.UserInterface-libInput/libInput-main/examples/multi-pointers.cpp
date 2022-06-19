#include <cassert>
#include <cstdio>
#include <chrono>
#include <thread>
#include <utility>
#include <csignal>
#include <cstring>
#include <iostream>
#include <sstream>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/shape.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include <InputController.h>
#include "../src/x11/X11InputController.h"

#include <xdo.h>

#define OVERLAY_WIDTH 800
#define OVERLAY_HEIGHT 600

using namespace std;
using namespace std::chrono;

xdo_t* xdo{nullptr};
int overlay_width = 0;
int overlay_height = 0;
Window overlay_win;
Window focused_win;

void HideGlobalCursor(Display* d) {
    XFixesHideCursor(d, DefaultRootWindow(d));
    XFlush(d);
}
void ShowGlobalCursor(Display* d) {
    XFixesShowCursor(d, DefaultRootWindow(d));
    XFlush(d);
}

volatile bool exit_loop = false;

void sig_int_handler(int signum, siginfo_t *info, void *ptr)
{
    exit_loop = true;
}

void SetupSignalHandlers()
{
    static struct sigaction _sigact;

    memset(&_sigact, 0, sizeof(_sigact));
    _sigact.sa_sigaction = sig_int_handler;
    _sigact.sa_flags = SA_SIGINFO;

    sigaction(SIGINT, &_sigact, NULL);
}

void AllowInputPassthrough(Display* d, Window w)
{
    XserverRegion region = XFixesCreateRegion (d, NULL, 0);

    XFixesSetWindowShapeRegion (d, w, ShapeBounding, 0, 0, 0);
    XFixesSetWindowShapeRegion (d, w, ShapeInput, 0, 0, region);

    XFixesDestroyRegion (d, region);
}

Window CreateOverlayWindow(Display* d, Window parent, cairo_surface_t** overlay_surface) {
    XSetWindowAttributes attrs;
    attrs.override_redirect = true;
    int screen = DefaultScreen(d);
    int screen_width = DisplayWidth(d, screen);
    int screen_height = DisplayHeight(d, screen);

    overlay_width = screen_width;
    overlay_height = screen_height;

    XVisualInfo vinfo;
    if (!XMatchVisualInfo(d, DefaultScreen(d), 32, TrueColor, &vinfo)) {
        printf("No visual found supporting 32 bit color, terminating\n");
        exit(EXIT_FAILURE);
    }
    // these next three lines add 32 bit depth, remove if you dont need and change the flags below
    attrs.colormap = XCreateColormap(d, parent, vinfo.visual, AllocNone);
    attrs.background_pixel = 0;
    attrs.border_pixel = 0;
#if 1
    Window overlay = XCreateWindow(
        d, parent,
        0, 0, overlay_width, overlay_height, 0,
        vinfo.depth, InputOutput,
        vinfo.visual,
        CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel, &attrs
    );
#endif
    //Window overlay = XCompositeGetOverlayWindow(d, parent);

    printf("Created overlay window, width: %d, height: %d\n", overlay_width, overlay_height);
    cairo_surface_t* surf = cairo_xlib_surface_create(d, overlay,
                                  vinfo.visual,
                                  overlay_width, overlay_height);

    *overlay_surface = surf;
    overlay_win = overlay; // For testing, remove later
    return overlay;
}

void DrawCursor(cairo_t* cr, int x, int y, cairo_surface_t* cursor) {
    cairo_set_source_surface(cr, cursor, x, y);
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
    cairo_paint(cr);
}

void Clear(cairo_t *cr) {
    cairo_set_source_rgba(cr, 0.0f, 0.0f, 0.0f, 0.0f);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
}

void SendFakeMouseButton(Display* d, int x, int y, int button, bool pressed) {
    XTestFakeMotionEvent(d, XDefaultScreen(d), x, y, CurrentTime);
    XTestFakeButtonEvent(d, button, pressed, CurrentTime);
}

void SendMouseButtonWindow(Display* d, Window w, int x, int y, int button, bool pressed) {
    XButtonEvent btn_event;
    btn_event.type = pressed ? ButtonPress : ButtonRelease;
    btn_event.display = d;
    btn_event.time = CurrentTime;
    btn_event.root = DefaultRootWindow(d);
    btn_event.window = overlay_win; // src window
    btn_event.button = button;

    unsigned int state = 0;
    state |= (button == Button1 ? Button1Mask : 0);
    state |= (button == Button2 ? Button2Mask : 0);
    state |= (button == Button3 ? Button3Mask : 0);
    btn_event.state = state; // state prior to the event

    btn_event.x_root = x;
    btn_event.y_root = y;
    //btn_event.x = x;
    //btn_event.y = y;
    btn_event.subwindow = None;

    Window child;
    XTranslateCoordinates(d, overlay_win, w,
                          x, y, &btn_event.x, &btn_event.y, &child);

    //printf("Win id: %d, press: %d, child: %d, src(x): %d, src(y): %d, trans(x): %d, trans(y): %d\n", w, pressed, child, x, y, btn_event.x, btn_event.y);

    btn_event.same_screen = True;

    Bool propagate = False;
    int ret = 0;
    // see https://tronche.com/gui/x/xlib/events/mask.html for available event masks
    ret = XSendEvent(d, w, propagate, ButtonPressMask, (XEvent *)&btn_event);
    XFlush(d);
    if (w == focused_win) {
       printf("Focused window: \n");
       printf("Win id: %d, press: %d, child: %d, src(x): %d, src(y): %d, trans(x): %d, trans(y): %d\n", w, pressed, child, x, y, btn_event.x, btn_event.y);
    }

    //printf("XSendEvent for button %d ret: %d\n", button, ret);
}

void SendMouseButtonDownWindow(Display* d, Window w, int x, int y, int button) {
    if (w == focused_win)
        printf("Sending mouse down to at x: %d, y:%d, button: %d, win: %d\n", x, y, button, w);
    SendMouseButtonWindow(d, w, x, y, button, true);
}

void SendMouseButtonUpWindow(Display* d, Window w, int x, int y, int button) {
    if (w == focused_win)
        printf("Sending mouse up to at x: %d, y:%d, button: %d, win: %d\n", x, y, button, w);
    SendMouseButtonWindow(d, w, x, y, button, false);
}

bool WindowUnderOverlay(Display* d, Window overlay, Window window) {
    XWindowAttributes oa;
    XWindowAttributes wa;

    XGetWindowAttributes(d, overlay, &oa);
    XGetWindowAttributes(d, window, &wa);

    //printf("Overlay id: %d, x: %d, y:%d, width: %d, height: %d, state: %d\n", overlay, oa.x, oa.y, oa.width, oa.height, oa.map_state);
    //printf("Window id: %d, x: %d, y:%d, width: %d, height: %d, state: %d\n", window, wa.x, wa.y, wa.width, wa.height, wa.map_state);

    return true;

    if (wa.x < oa.x)
        return false;

    if (wa.y < oa.y)
        return false;

    return true;
}

void SendMouseDown(Display* d, int x, int y, int button) {
#if 1
    SendFakeMouseButton(d, x, y, button, true);
#else
    Window root = DefaultRootWindow(d);
    Window root_return;
    Window parent_return;
    Window* children;
    unsigned int n_children;

    printf("Sending mouse down. x:%d, y: %d, button: %d\n", x, y, button);

    Window focused;
    int revert_to = RevertToParent;

    XGetInputFocus(d, &focused, &revert_to);

    printf("Focused: %d, revert to: %d\n", focused, revert_to);

    focused_win = focused;
    SendMouseButtonDownWindow(d, focused, x, y, button);
    int ret = XQueryTree(d, root, &root_return, &parent_return, &children, &n_children);

    for (int i = 0; i < n_children; i++) {
        Window window = children[i];
        if (window == overlay_win)
            continue;

        if (!WindowUnderOverlay(d, overlay_win, window)) {
            continue;
        }

        SendMouseButtonDownWindow(d, window, x, y, button);
        //std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // from xdotool
    }
#endif
}

void SendMouseUp(Display* d, int x, int y, int button) {
#if 1
    SendFakeMouseButton(d, x, y, button, false);
#else
    Window root = DefaultRootWindow(d);
    Window root_return;
    Window parent_return;
    Window* children;
    unsigned int n_children;

    printf("Sending mouse up. x:%d, y: %d, button: %d\n", x, y, button);
    Window focused;
    int revert_to = RevertToParent;

    XGetInputFocus(d, &focused, &revert_to);

    printf("Focused: %d, revert to: %d\n", focused, revert_to);

    SendMouseButtonUpWindow(d, focused, x, y, button);

    int ret = XQueryTree(d, root, &root_return, &parent_return, &children, &n_children);

    for (int i = 0; i < n_children; i++) {
        Window window = children[i];
        if (window == overlay_win)
            continue;

        if (!WindowUnderOverlay(d, overlay_win, window)) {
            continue;
        }

        SendMouseButtonUpWindow(d, window, x, y, button);
    }
#endif
}

void SendMouseClick(Display* d, int x, int y, int button) {
    Window root = DefaultRootWindow(d);
    Window root_return;
    Window parent_return;
    Window* children;
    unsigned int n_children;

    int ret = XQueryTree(d, root, &root_return, &parent_return, &children, &n_children);

    for (int i = 0; i < n_children; i++) {
        Window window = children[i];
        SendMouseButtonDownWindow(d, window, x, y, button);
        std::this_thread::sleep_for(std::chrono::microseconds(12)); // from xdotool
        SendMouseButtonUpWindow(d, window, x, y, button);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

MousePtr SelectPointer(InputControllerPtr input) {
    MousePtr pointer{nullptr};
    int device_id{-1};

    printf("Select the pointer device:\n");
    do {
        printf("Enter the device id\n");
        cin >> device_id;
        pointer = input->getMouseById(device_id);
        if (pointer) {
            printf("Selected pointer with device id: %d\n", pointer->deviceId());
        } else {
            printf("Mouse with device id: %d not found!\n", device_id);
        }
    } while(!pointer);

    return pointer;
}

KeyboardPtr SelectKeyboard(InputControllerPtr input) {
    KeyboardPtr keyboard{nullptr};
    int device_id{-1};

    printf("Select the Keyboard device:\n");
    do {
        printf("Enter the device id\n");
        cin >> device_id;
        keyboard = input->getKeyboardById(device_id);
        if (keyboard) {
            printf("Selected keyboard with device id: %d\n", keyboard->deviceId());
        } else {
            printf("Keyboard with device id: %d not found!\n", device_id);
        }
    } while(!keyboard);

    return keyboard;
}

int main() {
    Display* d = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(d);
    int default_screen = XDefaultScreen(d);
    InputControllerPtr input{nullptr};

    SetupSignalHandlers();

    #if USE_X11
        input = InputControllerPtr{new X11InputController(d, &root)};
    #endif

    if (!input) {
        printf("ERROR: failed to initialize input controller\n");
        exit(1);
    }

    input->enumerate();

    auto mice = input->getMice();
    for (const auto mouse : mice) {
        printf("Found a mouse %s with device id: %d\n", mouse->name(), mouse->deviceId());
    }

    auto keyboards = input->getKeyboards();
    for (const auto kb : keyboards) {
        printf("Found a keyboard %s with device id: %d\n", kb->name(), kb->deviceId());
    }

    bool input_valid = false;
    char choice;
    int device_id;
    int pointers_selected = 0;
    int drag_drop_count_threshold = 50; // button should be pressed for threshold count (500ms approx)
    int pointer1_drag_drop_count = 0;
    int pointer2_drag_drop_count = 0;

    MousePtr pointer1 = SelectPointer(input);
    MousePtr pointer2 = SelectPointer(input);
    KeyboardPtr keyboard1 = SelectKeyboard(input);

    X11InputControllerPtr x11_input = dynamic_pointer_cast<X11InputController>(input);

    // detach devices from Xserver so we can control them manually
    x11_input->detachDevice(pointer1);
    x11_input->detachDevice(pointer2);
    x11_input->detachDevice(keyboard1);

    printf("Pointer1: %s\n", pointer1->name());
    printf("Pointer1: %s\n", pointer1->name());
    printf("Keyboard1: %s\n", keyboard1->name());

    printf("Default root window: %d\n", root);

    // create xdo instance
    xdo = xdo_new_with_opened_display(d, NULL, 0);

    printf("xdo version: %s\n", xdo_version());

    cairo_surface_t *overlay_surface;

    XCompositeRedirectSubwindows(d, root, CompositeRedirectAutomatic);
    Window overlay = CreateOverlayWindow(d, root, &overlay_surface);

    printf("Id of overlay window: %d\n", overlay);

    // Enable pass through clicks
    AllowInputPassthrough(d, overlay);

    // Make overlay window visible
    XMapWindow(d, overlay);

    cairo_t* cr = cairo_create(overlay_surface);

    cairo_surface_t *cursor1 = cairo_image_surface_create_from_png("cursor1.png");;
    cairo_surface_t *cursor2 = cairo_image_surface_create_from_png("cursor2.png");;

    Clear(cr);
    DrawCursor(cr, 10, 10, cursor1);
    DrawCursor(cr, 10, 100, cursor2);

    XFlush(d);

    MouseState last_state1;
    MouseState last_state2;
    bool last_keys_state1[256] = {false};

    printf("Hiding Global cursor\n");
    HideGlobalCursor(d);

    double pointer1_maxX = pointer1->MaximumX();
    double pointer1_maxY = pointer1->MaximumY();
    double pointer2_maxX = pointer2->MaximumX();
    double pointer2_maxY = pointer2->MaximumY();

    double scale_x1 = pointer1_maxX > 0.0f ? overlay_width / pointer1_maxX : 1.0f;
    double scale_y1 = pointer1_maxY > 0.0f ? overlay_height / pointer1_maxY : 1.0f;
    double scale_x2 = pointer2_maxX > 0.0f ? overlay_width / pointer2_maxX : 1.0f;
    double scale_y2 = pointer2_maxY > 0.0f ? overlay_height / pointer2_maxY : 1.0f;

    while (!exit_loop) {
        auto state1 = pointer1->getMouseState();
        auto state2 = pointer2->getMouseState();
        int x1 = state1->x * scale_x1;
        int y1 = state1->y * scale_y1;
        int x2 = state2->x  * scale_x2;
        int y2 = state2->y * scale_y2;

        //printf("Cursor1\traw_x: %d, raw_y: %d, x: %d, y: %d\n", state1->x, state1->y, x1, y1);
        //printf("Cursor2\traw_x: %d, raw_y: %d, x: %d, y: %d\n", state2->x, state2->y, x2, y2);

        // to support drag and drop
        if (last_state1.btn1_pressed) {
            if (pointer1_drag_drop_count == drag_drop_count_threshold) {
                xdo_move_mouse(xdo, x1, y1, DefaultScreen(d));
            } else {
                pointer1_drag_drop_count++;
            }
        }

        if (last_state2.btn1_pressed) {
            if (pointer2_drag_drop_count == drag_drop_count_threshold) {
                xdo_move_mouse(xdo, x2, y2, DefaultScreen(d));
            } else {
                pointer2_drag_drop_count++;
            }
        }

        // check if button pressed
        if (last_state1.btn1_pressed && !state1->btn1_pressed) {
            pointer1_drag_drop_count = 0;
            // send mouse button up event
            SendMouseUp(d, x1, y1, Button1);
            last_state1.btn1_pressed = false;
        } else if (state1->btn1_pressed && !last_state1.btn1_pressed) {
            // send mouse down event
            SendMouseDown(d, x1, y1, Button1);
            last_state1.btn1_pressed = true;
        }

        if (last_state1.btn2_pressed && !state1->btn2_pressed) {
            // send mouse button up event
            SendMouseUp(d, x1, y1, Button2);
            last_state1.btn2_pressed = false;
        } else if (state1->btn2_pressed && !last_state1.btn2_pressed) {
            // send mouse down event
            SendMouseDown(d, x1, y1, Button2);
            last_state1.btn2_pressed = true;
        }

        if (last_state1.btn3_pressed && !state1->btn3_pressed) {
            // send mouse button up event
            SendMouseUp(d, x1, y1, Button3);
            last_state1.btn3_pressed = false;
        } else if (state1->btn3_pressed && !last_state1.btn3_pressed) {
            // send mouse down event
            SendMouseDown(d, x1, y1, Button3);
            last_state1.btn3_pressed = true;
        }

        // Cursor2
        if (last_state2.btn1_pressed && !state2->btn1_pressed) {
            pointer2_drag_drop_count = 0;
            // send mouse button up event
            SendMouseUp(d, x2, y2, Button1);
            last_state2.btn1_pressed = false;
        } else if (state2->btn1_pressed && !last_state2.btn1_pressed) {
            // send mouse down event
            SendMouseDown(d, x2, y2, Button1);
            last_state2.btn1_pressed = true;
        }

        if (last_state2.btn2_pressed && !state2->btn2_pressed) {
            // send mouse button up event
            SendMouseUp(d, x2, y2, Button2);
            last_state2.btn2_pressed = false;
        } else if (state2->btn2_pressed && !last_state2.btn2_pressed) {
            // send mouse down event
            SendMouseDown(d, x2, y2, Button2);
            last_state2.btn2_pressed = true;
        }

        if (last_state2.btn3_pressed && !state2->btn3_pressed) {
            // send mouse button up event
            SendMouseUp(d, x2, y2, Button3);
            last_state2.btn3_pressed = false;
        } else if (state2->btn3_pressed && !last_state2.btn3_pressed) {
            // send mouse down event
            SendMouseDown(d, x2, y2, Button3);
            last_state2.btn3_pressed = true;
            //XDestroyWindow(d, focused_win); // test to destory focused window
        }
        // Read keyboard
        auto kb_state = keyboard1->getKeyboardState();
        bool* keys_state = kb_state->keys_pressed;
        for (int i = 0; i < 256; i++) {
            ostringstream key_sequence;
            char key = XKeycodeToKeysym(d, i, 0);
            key_sequence << key;
            if (!last_keys_state1[i] && keys_state[i]) {
                xdo_send_keysequence_window_down(xdo, CURRENTWINDOW, key_sequence.str().c_str(), 0);
                last_keys_state1[i] = true;
            } else if (last_keys_state1[i] && !keys_state[i]) {
                xdo_send_keysequence_window_up(xdo, CURRENTWINDOW, key_sequence.str().c_str(), 0);
                last_keys_state1[i] = false;
            }
        }

        // clear overlay window
        Clear(cr);
        // draw cursors
        DrawCursor(cr, x1, y1, cursor1);
        DrawCursor(cr, x2, y2, cursor2);

XRaiseWindow(d, overlay_win); 

        // delay loop
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 100Hz
    }

    ShowGlobalCursor(d);

    x11_input->attachDevice(pointer1);
    x11_input->attachDevice(pointer2);
    x11_input->attachDevice(keyboard1);

    cairo_destroy(cr);
    cairo_surface_destroy(overlay_surface);
    cairo_surface_destroy(cursor1);
    cairo_surface_destroy(cursor2);

    XUnmapWindow(d, overlay);
    XCloseDisplay(d);
    return 0;
}
