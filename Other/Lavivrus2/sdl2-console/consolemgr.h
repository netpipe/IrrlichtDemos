#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <chrono>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "tmt.h"
#ifdef __cplusplus
}
#endif

typedef struct {
    TMT* terminal;
    TTF_Font* font;
    SDL_Surface* surface;
    SDL_Rect screenRect;
    SDL_Rect consoleRect;
    bool cursor;
    int charactW;
    int charactH;
    std::chrono::high_resolution_clock::time_point time;
} ConsoleMgr;

void initTmt(ConsoleMgr* mgr, TMT* tmt);
bool initFont(ConsoleMgr* mgr, const char* fontName, int fontSize);
void setFontStyle(ConsoleMgr* mgr, int renderstyle);
void setConsoleRect(ConsoleMgr* mgr, SDL_Rect rect);
void setScreenRect(ConsoleMgr* mgr, SDL_Rect rect);
void closeFont(ConsoleMgr* mgr);

SDL_Surface* createConsoleSurface(int width, int height);
SDL_Surface* renderConsole(ConsoleMgr* mgr);
void renderCursor(ConsoleMgr* mgr);
