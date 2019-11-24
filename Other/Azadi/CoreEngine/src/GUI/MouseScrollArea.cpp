#include "MouseScrollArea.h"

GUI::MouseScrollArea::MouseScrollArea()
{
    x = y = w = h = 0;
}

GUI::MouseScrollArea::MouseScrollArea(const unsigned int &X, const unsigned int &Y, const unsigned int &W, const unsigned int &H)
{
    x = X; y = Y;
    w = W; h = H;
}

GUI::MouseScrollArea::~MouseScrollArea()
{

}

void GUI::MouseScrollArea::parseInput(Input &input)
{
    unsigned int mouseX = input.getMouseX();
    unsigned int mouseY = input.getMouseY();
    bool scrolledUp = input.getMouseScroll(1);
    bool scrolledDown = input.getMouseScroll(2);
    wheelScrolledDown = wheelScrolledUp = false;
    if (scrolledUp || scrolledDown)
    {
        if (((mouseX > x) && (mouseX < x+w)) && ((mouseY > y) && (mouseY < y+h)))
        {
            wheelScrolledUp = scrolledUp;
            wheelScrolledDown = scrolledDown;
        }
    }
}

void GUI::MouseScrollArea::setDimensions(const unsigned int &X, const unsigned int &Y, const unsigned int &W, const unsigned int &H)
{
    x = X; y = Y;
    w = W; h = H;
}
