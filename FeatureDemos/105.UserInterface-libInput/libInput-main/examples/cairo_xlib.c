#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cairo.h>
#include <cairo-xlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*! Cairo graphics and X11/Xlib motion example.
 * @author Bernhard R. Fischer, 2048R/5C5FFD47 <bf@abenteuerland.at>.
 * @version 2014110801
 * Compile with gcc -Wall $(pkg-config --cflags --libs cairo x11) -o cairo_xlib cairo_xlib.c
 */

/*! Check for Xlib Mouse/Keypress events. All other events are discarded. 
 * @param sfc Pointer to Xlib surface.
 * @param block If block is set to 0, this function always returns immediately
 * and does not block. if set to a non-zero value, the function will block
 * until the next event is received.
 * @return The function returns 0 if no event occured (and block is set). A
 * positive value indicates that a key was pressed and the X11 key symbol as
 * defined in <X11/keysymdef.h> is returned. A negative value indicates a mouse
 * button event. -1 is button 1 (left button), -2 is the middle button, and -3
 * the right button.
 */
int cairo_check_event(cairo_surface_t *sfc, int block)
{
   char keybuf[8];
   KeySym key;
   XEvent e;

   for (;;)
   {
      if (block || XPending(cairo_xlib_surface_get_display(sfc)))
         XNextEvent(cairo_xlib_surface_get_display(sfc), &e);
      else 
         return 0;

      switch (e.type)
      {
         case ButtonPress:
            return -e.xbutton.button;
         case KeyPress:
            XLookupString(&e.xkey, keybuf, sizeof(keybuf), &key, NULL);
            return key;
         default:
            fprintf(stderr, "Dropping unhandled XEevent.type = %d.\n", e.type);
      }
   }
}


static void fullscreen(Display* dpy, Window win)
{
  Atom atoms[2] = { XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False), None };
  XChangeProperty(dpy, win, XInternAtom(dpy, "_NET_WM_STATE", False),
                  XA_ATOM, 32, PropModeReplace, (unsigned char*) atoms, 1);
}


/*! Open an X11 window and create a cairo surface base on that window. If x and
 * y are set to 0 the function opens a full screen window and stores to window
 * dimensions to x and y.
 * @param x Pointer to width of window.
 * @param y Pointer to height of window.
 * @return Returns a pointer to a valid Xlib cairo surface. The function does
 * not return on error (exit(3)).
 */
cairo_surface_t *cairo_create_x11_surface(int *x, int *y)
{
   Display *dsp;
   Drawable da;
   Screen *scr;
   int screen;
   cairo_surface_t *sfc;

   if ((dsp = XOpenDisplay(NULL)) == NULL)
      exit(1);
   screen = DefaultScreen(dsp);
   scr = DefaultScreenOfDisplay(dsp);
   if (!*x || !*y)
   {
      *x = WidthOfScreen(scr), *y = HeightOfScreen(scr);
      da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, *x, *y, 0, 0, 0);
      fullscreen (dsp, da);
   }
   else
      da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, *x, *y, 0, 0, 0);
   XSelectInput(dsp, da, ButtonPressMask | KeyPressMask);
   XMapWindow(dsp, da);

   sfc = cairo_xlib_surface_create(dsp, da, DefaultVisual(dsp, screen), *x, *y);
   cairo_xlib_surface_set_size(sfc, *x, *y);

   return sfc;
}


/*! Destroy cairo Xlib surface and close X connection.
 */
void cairo_close_x11_surface(cairo_surface_t *sfc)
{
   Display *dsp = cairo_xlib_surface_get_display(sfc);

   cairo_surface_destroy(sfc);
   XCloseDisplay(dsp);
}


static void turn(double v, double max, double *diff)
{
   if (v <= 0 || v >= max)
      *diff *= -1.0;
}


int main(int argc, char **argv)
{
   cairo_surface_t *sfc;
   cairo_t *ctx;
   int x, y;
   struct timespec ts = {0, 5000000};

   double x0 = 20, y0 = 20, x1 = 200, y1 = 400, x2 = 450, y2 = 100;
   double dx0 = 1, dx1 = 1.5, dx2 = 2;
   double dy0 = 2, dy1 = 1.5, dy2 = 1;
   int running;

   x = y = 0;
   sfc = cairo_create_x11_surface(&x, &y);
   ctx = cairo_create(sfc);

   for (running = 1; running;)
   {
      cairo_push_group(ctx);
      cairo_set_source_rgb(ctx, 1, 1, 1);
      cairo_paint(ctx);
      cairo_move_to(ctx, x0, y0);
      cairo_line_to(ctx, x1, y1);
      cairo_line_to(ctx, x2, y2);
      cairo_line_to(ctx, x0, y0);
      cairo_set_source_rgb(ctx, 0, 0, 1);
      cairo_fill_preserve(ctx);
      cairo_set_line_width(ctx, 5);
      cairo_set_source_rgb(ctx, 1, 1, 0);
      cairo_stroke(ctx);
      cairo_set_source_rgb(ctx, 0, 0, 0);
      cairo_move_to(ctx, x0, y0);
      cairo_show_text(ctx, "P0");
      cairo_move_to(ctx, x1, y1);
      cairo_show_text(ctx, "P1");
      cairo_move_to(ctx, x2, y2);
      cairo_show_text(ctx, "P2");
      cairo_pop_group_to_source(ctx);
      cairo_paint(ctx);
      cairo_surface_flush(sfc);

      x0 += dx0;
      y0 += dy0;
      x1 += dx1;
      y1 += dy1;
      x2 += dx2;
      y2 += dy2;
      turn(x0, x, &dx0);
      turn(x1, x, &dx1);
      turn(x2, x, &dx2);
      turn(y0, y, &dy0);
      turn(y1, y, &dy1);
      turn(y2, y, &dy2);

      switch (cairo_check_event(sfc, 0))
      {
         case 0xff53:   // right cursor
            dx0 *= 2.0;
            dy0 *= 2.0;
            break;

         case 0xff51:   // left cursor
            dx0 /= 2.0;
            dy0 /= 2.0;
            break;

         case 0xff1b:   // Esc
         case -1:       // left mouse button
            running = 0;
            break;
      }

      nanosleep(&ts, NULL);
   }

   cairo_destroy(ctx);
   cairo_close_x11_surface(sfc);

   return 0;
}

