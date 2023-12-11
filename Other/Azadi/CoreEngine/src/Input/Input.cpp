#include "Input.h"
#include "../GUI/Console.h"
#include "../Azadi.h"

Input::Input()
{
    capturedKeys = false;
    keysp = 0;
}

Input::~Input()
{

}

void Input::get()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            if (capturedKeys)
            {
                keyCatcher->keyPressed(event.key.keysym.sym, event.key.keysym.unicode);
                continue;
            }
            keys[event.key.keysym.sym] = true;
            ++keysp;
        }
        else if (event.type == SDL_KEYUP)
        {
            keys[event.key.keysym.sym] = false;
            --keysp;
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            mouse[0] = event.motion.x;
            mouse[1] = event.motion.y;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            mouseButton[event.button.button] = true;
            ++mousep;
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            mouseButton[event.button.button] = false;
            --mousep;
        }
    }
}

void Input::flush()
{
    for (std::map<int, bool>::iterator it = keys.begin(); it != keys.end(); ++it)
        it->second = false;
}
