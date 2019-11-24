#ifndef MAINMENU_H
#define MAINMENU_H

#include <GL/glew.h>
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#include <string>

#include "../GUI/Menu.h"
#include "../GUI/Button.h"
#include "../Texture/TextureFactory.h"

class MainMenu : public GUI::Menu
{
    public:
        void init(const int &x, const int &y, const int &w, const int &h);
        void parseInput(Input &input);
        void draw(const int &sw, const int &sh);
        inline int clickedButton() { return buttonNumber; };
    private:
        std::string texture;
        int buttonNumber;

        GUI::Button StartGame;
        GUI::Button ExitGame;

        GLint menuList;
        bool redraw;
};

#endif
