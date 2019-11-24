#include "Pane.h"

GUI::Pane::Pane(const int &X, const int &Y, const int &W, const int &H, const std::string &texName)
{
    x = X; y = Y; w = W; h = H;
    textureName = texName;
    TextureFactory::getInstance()->loadTexture(textureName);
}

GUI::Pane& GUI::Pane::setX(const int &X)
{
    x = X;
    return *this;
}

GUI::Pane& GUI::Pane::setY(const int &Y)
{
    y = Y;
    return *this;
}

GUI::Pane& GUI::Pane::setW(const int &W)
{
    w = W;
    return *this;
}

GUI::Pane& GUI::Pane::setH(const int &H)
{
    h = H;
    return *this;
}

GUI::Pane& GUI::Pane::setTexture(const std::string &texName)
{
    TextureFactory::getInstance()->deleteTexture(textureName);
    TextureFactory::getInstance()->loadTexture(texName);
    textureName = texName;
    return *this;
}

void GUI::Pane::draw()
{
    TextureFactory::getInstance()->applyTexture(textureName);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(x , y);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(x + w, y);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(x + w, y + h);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(x, y+h);
    }
    glEnd();
}

