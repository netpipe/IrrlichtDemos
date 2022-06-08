#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <iostream>
#include "consolemgr.h"
//http://compsci.ca/v3/viewtopic.php?t=39585
//g++ sdl_console.cpp -lSDL2 -lSDL2_image
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;
const int CONSOLEW = 40;
const int CONSOLEH = 20;

enum Keys {
    KEY_DEFAULT,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_TOTAL
};

bool graphicConsole = false;
bool init();
bool loadMedia();
void run();
void close();
SDL_Surface* loadImage(string path);

//SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
char gKeyPress[KEY_TOTAL][3];
char gBackColor[3];
SDL_Surface* gImage = NULL;
TMT* virtualConsole;
ConsoleMgr consoleMgr;
void setColor(char* c, char r, char g, char b) { c[0] = r; c[1] = g; c[2] = b; }
void setColor(char* dest, char* src) { setColor(dest, src[0], src[1], src[2]); }
void callback(tmt_msg_t m, TMT* vt, const void* a, void* p);
int initConsoleMgr();
void closeConsoleMgr();
SDL_Surface *ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
Uint32 ReadPixel(SDL_Surface *surface, int x, int y);
int main(int argc, char* args[]) {
    printf("HI\n");
    if (init()) {
        if (loadMedia()) {
            run();
        }
    }

    close();

    return 0;
}

bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized; SDL_Error: " << SDL_GetError() << endl;
        success = false;
    }
    else {
        freopen("CON", "wt", stdout);
        freopen("CON", "wt", stderr);
        printf("HI\n");
        // gWindow = SDL_CreateWindow("My SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
        gScreenSurface = SDL_SetVideoMode(WIDTH, HEIGHT, 0, 0);
        if (/* gWindow */ gScreenSurface == NULL) {
            cout << "Window could not be created; SDL_Error: " << SDL_GetError() << endl;
            success = false;
        }
        else {
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)) {
                cout << "SDL_image could not initialize; SDL_image Error: " << IMG_GetError() << endl;
                success = false;
            }
            else {
                if (TTF_Init() < 0) {
                    fprintf(stderr, "Couldn't initialize TTF: %s\n", SDL_GetError());
                    close();
                    return(EXIT_FAILURE);
                }
                if (initConsoleMgr() == EXIT_FAILURE)
                    success = false;
                else
                    ;// gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;

    setColor(gKeyPress[KEY_DEFAULT], 0x00, 0x00, 0x00);
    setColor(gKeyPress[KEY_UP], 0xff, 0xff, 0xff);
    setColor(gKeyPress[KEY_DOWN], 0xff, 0xff, 0x00);
    setColor(gKeyPress[KEY_LEFT], 0x00, 0xff, 0xff);
    setColor(gKeyPress[KEY_RIGHT], 0xff, 0x00, 0xff);

    string imgPath = "./hello.bmp";

    gImage = loadImage(imgPath);
    if (gImage == NULL) {
        success = false;
    }

    return success;
}

void run() {
    bool running = true;
    SDL_Event e;
    setColor(gBackColor, gKeyPress[KEY_DEFAULT]);
    char str[10] = {0,0,0,0,0,0,0,0,0,0};
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_F1:
                    graphicConsole = !graphicConsole;
                    break;
                case SDLK_UP:
                    tmt_write(virtualConsole, TMT_KEY_UP, 0);
                    break;
                case SDLK_DOWN:
                    tmt_write(virtualConsole, TMT_KEY_DOWN, 0);
                    break;
                case SDLK_LEFT:
                    tmt_write(virtualConsole, TMT_KEY_LEFT, 0);
                    break;
                case SDLK_RIGHT:
                    tmt_write(virtualConsole, TMT_KEY_RIGHT, 0);
                    break;
                default:
                     str[0] = e.key.keysym.sym;
                    tmt_write(virtualConsole, str, 0);
                    setColor(gBackColor, gKeyPress[KEY_DEFAULT]);
                }
            }
        }
        renderCursor(&consoleMgr);
        SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, gBackColor[0], gBackColor[1], gBackColor[2]));

        SDL_Rect rect;
        rect.x = 450;
        rect.y = 350;
        rect.w = 300;
        rect.h = 200;
        if(consoleMgr.surface&& graphicConsole)
            SDL_BlitSurface(consoleMgr.surface, NULL, gScreenSurface, &consoleMgr.surface->clip_rect);
        // SDL_BlitScaled(gImage, NULL, gScreenSurface, &rect);
        gImage = ScaleSurface(gImage, rect.w, rect.h);
        SDL_BlitSurface(gImage, NULL, gScreenSurface, &rect);
        // SDL_UpdateWindowSurface(gWindow);
        SDL_Flip(gScreenSurface);
    }
}

void close() {
    SDL_FreeSurface(gImage);
    gImage = NULL;

    // SDL_DestroyWindow(gWindow);
    // gWindow = NULL;

    SDL_Quit();
}

SDL_Surface* loadImage(string path) {
    SDL_Surface* optimizedImage = NULL;
    SDL_Surface* newImage = SDL_LoadBMP(path.c_str());
    if (newImage == NULL) {
        cout << "Unable to load image " << path << "; SDL_image Error: " << IMG_GetError() << endl;
    }
    else {
        optimizedImage = SDL_ConvertSurface(newImage, gScreenSurface->format, 0 /* NULL */);
        if (optimizedImage == NULL) {
            cout << "Unable to optimize image; SDL Error: " << SDL_GetError() << endl;
        }
        SDL_FreeSurface(newImage);
    }

    return optimizedImage;
}

int initConsoleMgr()
{
    virtualConsole = tmt_open(CONSOLEH, CONSOLEW, callback, NULL, NULL);
    initTmt(&consoleMgr, virtualConsole);
    if (!initFont(&consoleMgr, "consola.ttf", 18)) return EXIT_FAILURE;
    setConsoleRect(&consoleMgr, { 0,0,WIDTH ,HEIGHT });
    setScreenRect(&consoleMgr, { 0,0,WIDTH ,HEIGHT });

    if (!virtualConsole) {
        closeFont(&consoleMgr);
        return EXIT_FAILURE;
    }
    consoleMgr.surface = createConsoleSurface(WIDTH, HEIGHT);
    tmt_write(virtualConsole, "\033[1mhello,\033[0m world (in bold!)", 0);
    tmt_write(virtualConsole, TMT_KEY_DOWN, 0);
    tmt_write(virtualConsole, TMT_KEY_LEFT, 0);
    tmt_write(virtualConsole, TMT_KEY_LEFT, 0);
    tmt_write(virtualConsole, "\033[5D", 0);
    tmt_write(virtualConsole, "\033[35mQWERTYUIO", 0);
    if (!consoleMgr.surface) {
        closeConsoleMgr();
        return EXIT_FAILURE;
    }
}
void closeConsoleMgr() {
    closeFont(&consoleMgr);
    SDL_FreeSurface(consoleMgr.surface);
    tmt_close(virtualConsole);
}
void callback(tmt_msg_t m, TMT* vt, const void* a, void* p)
{
    /* grab a pointer to the virtual screen */
    const TMTSCREEN* s = tmt_screen(vt);
    const TMTPOINT* c = tmt_cursor(vt);

    switch (m) {
    case TMT_MSG_BELL:
        /* the terminal is requesting that we ring the bell/flash the
         * screen/do whatever ^G is supposed to do; a is NULL
         */
        printf("bing!\n");
        break;

    case TMT_MSG_UPDATE:
        /* the screen image changed; a is a pointer to the TMTSCREEN */
        renderConsole(&consoleMgr);
        /* let tmt know we've redrawn the screen */
        tmt_clean(vt);
        break;

    case TMT_MSG_ANSWER:
        /* the terminal has a response to give to the program; a is a
         * pointer to a string */
        printf("terminal answered %s\n", (const char*)a);
        break;

    case TMT_MSG_MOVED:
        /* the cursor moved; a is a pointer to the cursor's TMTPOINT */
        printf("cursor is now at %zd,%zd\n", c->r, c->c);
        break;
    }
}

SDL_Surface *ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height) {
    if(!Surface || !Width || !Height)
        return 0;

    SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,
        Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);

    double    _stretch_factor_x = (static_cast<double>(Width)  / static_cast<double>(Surface->w)),
        _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(Surface->h));

    for(Sint32 y = 0; y < Surface->h; y++)
        for(Sint32 x = 0; x < Surface->w; x++)
            for(Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y)
                for(Sint32 o_x = 0; o_x < _stretch_factor_x; ++o_x)
                    DrawPixel(_ret, static_cast<Sint32>(_stretch_factor_x * x) + o_x,
                        static_cast<Sint32>(_stretch_factor_y * y) + o_y, ReadPixel(Surface, x, y));

    return _ret;
}

Uint32 ReadPixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;
    }
}

void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
