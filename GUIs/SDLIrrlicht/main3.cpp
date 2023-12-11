#include <SDL.h>

#include <X11/Xlib.h>

#include <stdio.h>

// clang -lSDL2 -lX11 -I/usr/include/SDL2 -Weverything main3.cpp -o main

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



    if(!w) {
        fprintf(stderr, "couldn't attach to the root X11 window: %s\n", SDL_GetError());
        return 1;
    }

    r = SDL_CreateRenderer(w, -1, 0);

    while (true) {
        // Clear the window (the background color is set to white)
       // XClearWindow(dsp, win);

        // Loop through all pixels of the 800*600 window
      //  for (int j = 0; j < 600; j++) {
      //      for (int i = 0; i < 800; i++) {
                // Read the color from the pixels array (always blue for now)
       //         long int color = pixels[i + 800*j];
                // Set the foreground color for drawing
     //           XSetForeground(dsp, gc, color);
                // Draw the pixel
    //            XDrawPoint(dsp, win, gc, i, j);
    //        }
   //     }
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderClear(r);
    SDL_RenderPresent(r);

        // Flush the output buffer
   //     XFlush();
    SDL_Delay(5700);
}

    XCloseDisplay(x11_d);






    SDL_Quit();
    return 0;
}
