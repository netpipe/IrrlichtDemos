#include "Window.h"

const int windowTitleBarHeight = 20;
const int windowDragBoxPixels = 20;

GUI::Window::Window()
{
    x = y = w = h = 0;
    resizeable = false;
    isResizing = isDragging = false;
}

GUI::Window::Window(const unsigned int &X, const unsigned int &Y, const unsigned int &W, const unsigned int &H, const std::string &Texture, const bool &reSizeable)
{
    x = X; y = Y;
    w = W; h = H;
    texture = Texture;
    TextureFactory::getInstance()->loadTexture(texture);
    resizeable = reSizeable;
    isResizing = isDragging = false;
}

GUI::Window& GUI::Window::setX(const int &X)
{
    x = X;
    return *this;
}

GUI::Window& GUI::Window::setY(const int &Y)
{
    y = Y;
    return *this;
}

GUI::Window& GUI::Window::setW(const int &W)
{
    w = W;
    return *this;
}

GUI::Window& GUI::Window::setH(const int &H)
{
    h = H;
    return *this;
}

GUI::Window& GUI::Window::setResizeable(const bool &flag)
{
    resizeable = flag;
    return *this;
}

void GUI::Window::setVisible(const bool &flag)
{
    visible = flag;
}

void GUI::Window::parseInput(Input &input)
{
    bool clicked = input.getMouseDown(1);
    if (!clicked)
        return;

    if (!input.getMouseDown(1))
    {
        isDragging = false;
        isResizing = false;
    }

    int mousex = input.getMouseX();
    int mousey = input.getMouseY();

    if (!isDragging && !isResizing)
    {
        if (clicked)
        {
            if (mousex > x && mousex < x + w && mousey > y && mousey < y + h)
            {
                hasFocus = true;
                if (input.getMouseDown(1))
                {
                    if (mousey - y < windowTitleBarHeight)
                        isDragging = true;
                    else if (((x + w) - mousex < windowDragBoxPixels) &&  ((y + h) - mousey < windowDragBoxPixels))
                        isResizing = true;

                    mouseStartDragX = mousex - x;
                    mouseStartDragY = mousey - y;

                    mouseStartResizeX = (x + w) - mousex;
                    mouseStartResizeY = (y + h) - mousey;
                }
                else
                    input.getMouseClick(1);
            }
            else
                hasFocus = false;
        }
        return;
    }

    if (isDragging)
    {
        x = mousex - mouseStartDragX;
        y = mousey - mouseStartDragY;
    }
    else if (isResizing && resizeable)
    {
        w = (mousex - x) + mouseStartResizeX;
        h = (mousey - y) + mouseStartResizeY;
    }
}

