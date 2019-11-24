#include "MainMenu.h"

void MainMenu::init(const int &X, const int &Y, const int &W, const int &H)
{
    x = X; y = Y;
    w = W; h = H;
    texture = "MainMenu.png";
    buttonNumber = -1;

    TextureFactory::getInstance()->loadTexture(texture);

    StartGame.setX(x + 50).setY(y + 100).setW(200).setH(40).setTexture("Continue.png");
    ExitGame.setX(x + 50).setY(y + 170).setW(200).setH(40).setTexture("Exit-Game.png");
    redraw = true;
    menuList = glGenLists(1);
}

void MainMenu::parseInput(Input &input)
{
    StartGame.parseInput(input);
    ExitGame.parseInput(input);

    if (StartGame.isPressed())
        buttonNumber = 0;
    else if (ExitGame.isPressed())
        buttonNumber = 1;
}

void MainMenu::draw(const int &sw, const int &sh)
{
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, sw, sh, 0.0f, -1.0f, 2000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    TextureFactory::getInstance()->applyTexture(texture);
    if (redraw)
    {
        glDeleteLists(menuList, 1);
        glNewList(menuList, GL_COMPILE_AND_EXECUTE);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        {
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(x    , y);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(x + w, y);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(x + w, y + h);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(x    , y + h);
        }
        glEnd();
        glEndList();

        redraw = false;
    }
    else
        glCallList(menuList);

    StartGame.draw();
    ExitGame.draw();

    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}
