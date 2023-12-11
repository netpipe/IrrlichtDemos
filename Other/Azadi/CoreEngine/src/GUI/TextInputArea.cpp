#include "TextInputArea.h"

GUI::TextInputArea::TextInputArea()
{
    x = y = w = h = cursorPosition = 0;
    pointSize = 0;
    font = "";
    drawBorder = false;
    TextList = glGenLists(1);
    recompileText = true;
}

GUI::TextInputArea::TextInputArea(const int &X, const int &Y, const int &W, const int &H, const std::string &fontName, const unsigned int &pSize, const bool &Border)
{
    cursorPosition = 0;
    x = X; y = Y;
    w = W; h = H;
    drawBorder = Border;
    font = fontName;
    Font::getInstance()->loadFont(font);
    pointSize = pSize;
    Font::getInstance()->genPointSize(font, pointSize);
    TextList = glGenLists(1);
    recompileText = true;
}

void GUI::TextInputArea::parseInput(Input &input)
{
    int mousex = input.getMouseX();
    int mousey = input.getMouseY();
    bool clicked = input.getMouseDown(1);

    if (clicked)
    {
        if (mousex > x && mousex < x + w && mousey > y && mousey < y + h)
        {
            Focus = true;
            input.setKeyCatcher(this);
            input.getMouseClick(1);
        }
        else
        {
            Focus = false;
        }
    }
}

GUI::TextInputArea& GUI::TextInputArea::setX(const int &X)
{
    x = X;
    return *this;
}

GUI::TextInputArea& GUI::TextInputArea::setY(const int &Y)
{
    y = Y;
    return *this;
}

GUI::TextInputArea& GUI::TextInputArea::setW(const int &W)
{
    w = W;
    return *this;
}

GUI::TextInputArea& GUI::TextInputArea::setH(const int &H)
{
    h = H;
    return *this;
}

GUI::TextInputArea& GUI::TextInputArea::setFont(const std::string &fontName)
{
    font = fontName;
    Font::getInstance()->loadFont(fontName);
    return *this;
}

GUI::TextInputArea& GUI::TextInputArea::setPointSize(const unsigned int &size)
{
    pointSize = size;
    Font::getInstance()->genPointSize(font, size);
    return *this;
}

void GUI::TextInputArea::keyPressed(const SDLKey &sym, const short &ch)
{
    if (sym == SDLK_RETURN)
    {
        textBuffer = enterBuffer;
        enterBuffer.clear();
        return;
    }

    if (sym == SDLK_BACKSPACE)
    {
        if (cursorPosition > 0)
            enterBuffer.erase(cursorPosition--, 1);

        return;
    }

    if (sym == SDLK_DELETE)
    {
        if (cursorPosition < enterBuffer.size())
            enterBuffer.erase(cursorPosition, 1);

        return;
    }

    if (sym == SDLK_LEFT)
    {
        if (cursorPosition > 0)
            --cursorPosition;

        return;
    }

    if (sym == SDLK_RIGHT)
    {
        if (cursorPosition < enterBuffer.size())
            ++cursorPosition;

        return;
    }

    if (ch)
        enterBuffer.insert(++cursorPosition, 1, ch);
}

void GUI::TextInputArea::draw(const int &sw, const int &sh)
{
    float textEditPosition = Font::getInstance()->textWidth(textBuffer.substr(0, cursorPosition), font, pointSize);

    if (drawBorder)
    {
        glDisable(GL_TEXTURE_2D);
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);

        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        {
            glVertex2f(x + 1    , y + 1);
            glVertex2f(x + 1 + w, y + 1);
            glVertex2f(x + 1 + w, y + 1 + h);
            glVertex2f(x + 1    , y + 1 + h);
        }
        glEnd();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        {
            glVertex2f(x, y);
            glVertex2f(x + w, y);
            glVertex2f(x + w, y + h);
            glVertex2f(x, y + h);
        }
        glEnd();

        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        glEnable(GL_TEXTURE_2D);
    }

    glTranslatef(x, y, 0.0f);
    if (recompileText)
    {
        glDeleteLists(TextList, 1);
        glNewList(TextList, GL_COMPILE_AND_EXECUTE);
        Font::getInstance()->drawText(textBuffer, font, pointSize, w);
        glEndList();
    }
    else
        glCallList(TextList);

    unsigned int cursorblinktime = 0;
    cursorblinktime += Drawing::getInstance()->getLastFrameLength();

    if ( (cursorblinktime / 400) & 1)
    {
        glTranslatef(textEditPosition, 0.0f, 0.0f);
        Font::getInstance()->drawText("_", font, pointSize, 0);
    }
}
