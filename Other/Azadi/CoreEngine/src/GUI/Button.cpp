#include "Button.h"

GUI::Button::Button()
{
    x = y = w = h = 0;
    pressed = held = hovering = false;
    normalTextureName = pressedTextureName = hoveringTextureName = "";

    texture_factory = TextureFactory::getInstance();
}

GUI::Button::~Button()
{

}

GUI::Button& GUI::Button::setX(const int &X)
{
    x = X;
    return *this;
}

GUI::Button& GUI::Button::setY(const int &Y)
{
    y = Y;
    return *this;
}

GUI::Button& GUI::Button::setW(const int &W)
{
    w = W;
    return *this;
}

GUI::Button& GUI::Button::setH(const int &H)
{
    h = H;
    return *this;
}

GUI::Button& GUI::Button::setTexture(const std::string &textureName)
{
    texture_factory->loadTexture(textureName.c_str());
    normalTextureName = textureName;
    return *this;
}

GUI::Button& GUI::Button::setPressedTexture(const std::string &textureName)
{
    texture_factory->loadTexture(textureName.c_str());
    pressedTextureName = textureName;
    return *this;
}

GUI::Button& GUI::Button::setHoverTexture(const std::string &textureName)
{
    texture_factory->loadTexture(textureName.c_str());
    hoveringTextureName = textureName;
    return *this;
}

void GUI::Button::parseInput(Input &input)
{
    int mousex = input.getMouseX();
    int mousey = input.getMouseY();

    if (!input.getMouseDown(1))
    {
        held = false;
    }

    if ((mousex > x) && (mousex < (x+w)) && (mousey > y) && (mousey < (y+h)))
    {
        if (input.getMouseDown(1))
        {
            pressed = true;
            held = true;
        }
        else
            hovering = true;
    }
    else
    {
        pressed = false;
        held = false;
        hovering = false;
    }

    if (pressed)
        input.getMouseClick(1);
}

void GUI::Button::draw()
{
    if (pressed && pressedTextureName != "")
        texture_factory->applyTexture(pressedTextureName);
    else if (hovering && hoveringTextureName != "")
        texture_factory->applyTexture(hoveringTextureName);
    else
        texture_factory->applyTexture(normalTextureName);

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(x, y);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(x+w, y);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(x+w, y+h);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(x, y+h);
    }
    glEnd();
}

bool GUI::Button::isPressed()
{
    return pressed;
}

bool GUI::Button::isHeld()
{
    return held;
}

bool GUI::Button::isHoveredOver()
{
    return hovering;
}
