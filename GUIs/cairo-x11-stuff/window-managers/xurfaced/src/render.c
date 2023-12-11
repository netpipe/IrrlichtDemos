#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <animation.h>
#include <display.h>
#include <menu.h>
#include <render.h>

static cairo_t *xurfacedRenderCairo;
static cairo_surface_t *xurfacedRenderCairoXlib;
static cairo_surface_t *xurfacedRenderCairoBackgroundImage;
static cairo_pattern_t *xurfacedRenderCairoBackgroundPattern;
unsigned int xurfacedRenderText;
unsigned int xurfacedRenderBackgroundTop;
unsigned int xurfacedRenderBackgroundBottom;
double boxheight;
double btextheight;
double stextheight;
double middle;
double r;
double g;
double b;
double a;

static double xurfaced_render_convert(unsigned int value)
{

    return (double)(value & 0xFF) / (double)0xFF;

}

static void xurfaced_render_blit_background(int width, int height)
{

    cairo_rectangle(xurfacedRenderCairo, 0, 0, width, height);
    cairo_set_source(xurfacedRenderCairo, xurfacedRenderCairoBackgroundPattern);
    cairo_fill_preserve(xurfacedRenderCairo);
    cairo_set_source_surface(xurfacedRenderCairo, xurfacedRenderCairoBackgroundImage, 0, 0);
    cairo_fill_preserve(xurfacedRenderCairo);
    cairo_set_source_rgba(xurfacedRenderCairo, 0.0, 0.0, 0.0, 0.8);
    cairo_fill(xurfacedRenderCairo);

}

static void xurfaced_render_blit_notification(int width, int height)
{

    cairo_font_extents_t fe;

    cairo_rectangle(xurfacedRenderCairo, 0, 0, width, boxheight);
    cairo_set_source_rgba(xurfacedRenderCairo, 0.0, 0.0, 0.0, 0.8);
    cairo_fill(xurfacedRenderCairo);
    cairo_select_font_face(xurfacedRenderCairo, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(xurfacedRenderCairo, stextheight);
    cairo_font_extents(xurfacedRenderCairo, &fe);
    cairo_set_source_rgba(xurfacedRenderCairo, r, g, b, a);
    cairo_move_to(xurfacedRenderCairo, width / 16.0, boxheight / 2.0 + fe.height / 2.0 - fe.descent);
    cairo_show_text(xurfacedRenderCairo, "xurfaced");

}

static void xurfaced_render_blit_menu(unsigned int height, struct xurfaced_menu *menu)
{

    struct xurfaced_menu_option *current = menu->opts->current;

    double offset = menu->animationProperties.translationY + current->animationProperties.translationY;
    double distance = fabs(middle - offset);

    if (distance > 5.0)
    {

        if (middle - offset < 0)
            menu->animationProperties.translationY -= (distance) / 5.0;
        else
            menu->animationProperties.translationY += (distance) / 5.0;

    }

    struct xurfaced_menu_option *start = menu->opts->current;
    struct xurfaced_menu_option *stop = menu->opts->current;

    unsigned int i;

    for (i = 0; i < 8; i++)
    {

        if (start != menu->opts->head)
            start = start->prev;

        if (stop != menu->opts->head->prev)
            stop = stop->next;

    }
    
    struct xurfaced_menu_option *option = start;

    do
    {

        if (option == current)
            option->animationProperties.alpha += 1.0;
        else
            option->animationProperties.alpha -= 0.05;

        if (option->animationProperties.alpha >= 1.0)
            option->animationProperties.alpha = 1.0;

        if (option->animationProperties.alpha <= 0.3)
            option->animationProperties.alpha = 0.3;

        cairo_set_source_rgba(xurfacedRenderCairo, r, g, b, option->animationProperties.alpha);

        cairo_select_font_face(xurfacedRenderCairo, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(xurfacedRenderCairo, btextheight);
        cairo_move_to(xurfacedRenderCairo, menu->animationProperties.translationX, menu->animationProperties.translationY + option->animationProperties.translationY);
        cairo_show_text(xurfacedRenderCairo, option->name);

        if (option == current)
        {

            cairo_select_font_face(xurfacedRenderCairo, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(xurfacedRenderCairo, stextheight);
            cairo_move_to(xurfacedRenderCairo, menu->animationProperties.translationX, menu->animationProperties.translationY + option->animationProperties.translationY + btextheight);
            cairo_show_text(xurfacedRenderCairo, option->description);

        }

        option = option->next;

    }
    while (option != stop->next);

}

void xurfaced_render_prep(struct xurfaced_display_backend *backend, struct xurfaced_menu *menu)
{

    cairo_push_group(xurfacedRenderCairo);
    xurfaced_render_blit_background(backend->width, backend->height);
    xurfaced_render_blit_menu(backend->height, menu);
    xurfaced_render_blit_notification(backend->width, backend->height);
    cairo_pop_group_to_source(xurfacedRenderCairo);

}

void xurfaced_render_blit(struct xurfaced_display_backend *backend)
{

    XLockDisplay(backend->display);
    cairo_paint(xurfacedRenderCairo);
    XUnlockDisplay(backend->display);

}

void xurfaced_render_init(struct xurfaced_display_backend *backend)
{

    xurfacedRenderCairoXlib = cairo_xlib_surface_create(backend->display, backend->main, backend->visual, backend->width, backend->height);
    xurfacedRenderCairo = cairo_create(xurfacedRenderCairoXlib);
    xurfacedRenderCairoBackgroundPattern = cairo_pattern_create_linear(0.0, 0.0, 0.0, backend->height);
    xurfacedRenderCairoBackgroundImage = cairo_image_surface_create_from_png("/home/jfu/.xurfaced/back.png");

    cairo_set_line_cap(xurfacedRenderCairo, CAIRO_LINE_CAP_ROUND);

    xurfacedRenderText = 0xFFFFFFFF;
    xurfacedRenderBackgroundTop = 0x441155FF;
    xurfacedRenderBackgroundBottom = 0x001133FF;

    boxheight = backend->height / 24.0;
    btextheight = backend->height / 32.0;
    stextheight = backend->height / 56.0;
    middle = backend->height / 4.0 + backend->height / 8.0;
    r = xurfaced_render_convert(xurfacedRenderText >> XURFACED_RENDER_COLOR_BITS_RED);
    g = xurfaced_render_convert(xurfacedRenderText >> XURFACED_RENDER_COLOR_BITS_GREEN);
    b = xurfaced_render_convert(xurfacedRenderText >> XURFACED_RENDER_COLOR_BITS_BLUE);
    a = xurfaced_render_convert(xurfacedRenderText >> XURFACED_RENDER_COLOR_BITS_ALPHA);

    double rt = xurfaced_render_convert(xurfacedRenderBackgroundTop >> XURFACED_RENDER_COLOR_BITS_RED);
    double gt = xurfaced_render_convert(xurfacedRenderBackgroundTop >> XURFACED_RENDER_COLOR_BITS_GREEN);
    double bt = xurfaced_render_convert(xurfacedRenderBackgroundTop >> XURFACED_RENDER_COLOR_BITS_BLUE);
    double rb = xurfaced_render_convert(xurfacedRenderBackgroundBottom >> XURFACED_RENDER_COLOR_BITS_RED);
    double gb = xurfaced_render_convert(xurfacedRenderBackgroundBottom >> XURFACED_RENDER_COLOR_BITS_GREEN);
    double bb = xurfaced_render_convert(xurfacedRenderBackgroundBottom >> XURFACED_RENDER_COLOR_BITS_BLUE);

    cairo_pattern_add_color_stop_rgb(xurfacedRenderCairoBackgroundPattern, 0.0, rt, gt, bt);
    cairo_pattern_add_color_stop_rgb(xurfacedRenderCairoBackgroundPattern, 1.0, rb, gb, bb);

}

void xurfaced_render_destroy(struct xurfaced_display_backend *backend)
{

    cairo_destroy(xurfacedRenderCairo);
    cairo_surface_destroy(xurfacedRenderCairoBackgroundImage);
    cairo_surface_destroy(xurfacedRenderCairoXlib);

}

