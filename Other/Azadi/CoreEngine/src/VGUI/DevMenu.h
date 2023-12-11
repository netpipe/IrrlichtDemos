#ifndef DEVMENU_H
#define DEVMENU_H

#include "../GUI/Menu.h"
#include "../GUI/Button.h"
#include "../Texture/TextureFactory.h"

class DevMenu : public GUI::Menu
{
    public:
        DevMenu();
        ~DevMenu();

        void parseInput(Input &input);

        void draw(const int &x, const int &y);
};

#endif
