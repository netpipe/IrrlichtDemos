
//http://irrlicht.sourceforge.net/forum/viewtopic.php?f=5&t=50533
//main.cpp

#include "mainwindow.h"
#include <QApplication>
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindowFrom((void*)w.ptr_gfx->winId());
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    SDL_RenderFillRect(render, NULL);
    SDL_RenderPresent(render);

    return a.exec();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    SDL_Quit();

    return a.exec();
}
