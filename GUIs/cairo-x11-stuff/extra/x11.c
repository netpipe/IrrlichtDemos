#include <SDL.h>

#include <X11/Xlib.h>

#include <stdio.h>

// clang -lSDL2 -lX11 -I/usr/include/SDL2 -Weverything x11.c -o x11

int main(void)
{
    Display *x11_d;
    int x11_s;
    Window x11_w;
    SDL_Window *w;
    SDL_Renderer *r;

    x11_d = XOpenDisplay(NULL);

    if(!x11_d) {
        fprintf(stderr, "couldn't open display\n");
        return 1;
    }

    x11_s = DefaultScreen(x11_d);
    x11_w = RootWindow(x11_d, x11_s);

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "couldn't initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    w = SDL_CreateWindowFrom((void *)x11_w);

    XCloseDisplay(x11_d);

    if(!w) {
        fprintf(stderr, "couldn't attach to the root X11 window: %s\n", SDL_GetError());
        return 1;
    }

    r = SDL_CreateRenderer(w, -1, 0);

    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderClear(r);
    SDL_RenderPresent(r);

    SDL_Delay(5700);

    SDL_Quit();
    return 0;
}
