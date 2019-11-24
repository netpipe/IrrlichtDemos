#include "TextArea.h"

GUI::TextArea::TextArea(const unsigned int &X, const unsigned int &Y, const unsigned int &W, const unsigned int &H, const std::string &fontName, const float &point, const bool &multiLine)
{
    x = X; y = Y;
    w = W; h = H;
    font = fontName;
    pointSize = point;
    multiline = multiLine;
    redraw = true;
    textList = glGenLists(1);
}

void GUI::TextArea::draw()
{
    if (redraw)
    {
        glDeleteLists(textList, 1);
        glNewList(textList, GL_COMPILE_AND_EXECUTE);
        glTranslatef(x, y, 0.0f);
        Font::getInstance()->drawText(textBuffer, font, pointSize, w);

        glEndList();
    }
    else
        glCallList(textList);
}

void GUI::TextArea::resize(const unsigned int &X, const unsigned int &Y, const unsigned int &W, const unsigned int &H)
{
    (X >= 0)?x = X: x = x;
    (Y >= 0)?y = Y: y = y;
    (W >= 0)?w = W: w = w;
    (H >= 0)?h = H: h = h;
}
