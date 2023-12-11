#include "GLSL.h"
#include "../Fonts/Font.h"

GLSLEditor::GLSLEditor(const unsigned int &X, const unsigned int &Y, const unsigned int &W, const unsigned int &H)
{
    x = X; y = Y;
    w = W; h = H;
    mouseHitX = 0;
    mouseHitY = 0;
    mouseHeld = resizing = false;
    currentLine = textBuffer.begin();
}

GLSLEditor::~GLSLEditor()
{

}

void GLSLEditor::tick(Input &input)
{
    if (Focus || Visible)
    {
        if (!Focus)
        {
            if (input.getMouseClick(1))
            {
                unsigned int tempx = input.getMouseX();
                unsigned int tempy = input.getMouseY();
                if (tempx > x && tempx < x + w)
                    if (tempy > y && tempy < y + h)
                    {
                        Focus = true;
                        mouseHeld = true;
                    }
            }
        }
        else
        {
            input.setKeyCatcher(this);
            if (!input.getMouseDown(1))
            {
                mouseHeld = false;
                resizing = false;
            }

            if (mouseHeld)
            {
                if (resizing)
                {
                    w = input.getMouseX() - x + mouseHitX;
                    h = input.getMouseY() - y + mouseHitY;
                    if (w < 100)
                        w = 100;
                    if (h < 100)
                        h = 100;
                }
                else if (highlighting)
                {

                }
            }

            if (input.getMouseClick(1))
            {
                unsigned int tempx = input.getMouseX();
                unsigned int tempy = input.getMouseY();
                if (tempx > x && tempx < x + w)
                    if (tempy > y && tempy < y + h)
                    {
                        mouseHitX = x + w - tempx;
                        mouseHitY = y + h - tempy;
                        if (mouseHitX < 50 && mouseHitY < 50)
                            resizing = true;
                    }
            }
        }
    }
}

void GLSLEditor::keyPressed(const SDLKey &keysym, const short &ch)
{
    if (keysym == SDLK_BACKSPACE)
    {
        // Are you at the start of the line?
        if (cursorPos == 0)
        {
            // Does more text exist on the current line?
            if ((*currentLine).size())
            {
                // Is the current character a space?
                if ((*currentLine)[cursorPos] == ' ')
                {
                    (*currentLine--).erase(cursorPos, 1);
                    cursorPos = (*currentLine).size();
                }
                else
                {
                    unsigned int location = (*currentLine).find(" ");
                    // Are there any more spaces on this line?
                    if (location != (*currentLine).npos)
                    {
                        std::string temp = (*currentLine).substr(0, location);
                        // Would adding the word terminated by a space to the previous line overflow the line width?
                        if (Font::getInstance()->textWidth((*(currentLine-1)) + (*currentLine).substr(0, location), fontname, pointSize) <= lineMaxWidth)
                        {
                            (*(currentLine-1)).append(temp);
                            (*currentLine--).erase(0, location +1);
                            cursorPos = (*currentLine).size();
                        }
                        else
                            cursorPos = (*--currentLine).size();
                    }
                    else
                    {
                        // Would adding this whole slab of text to the previous line overflow the line width?
                        if (Font::getInstance()->textWidth((*(currentLine-1)) + (*currentLine), fontname, pointSize) <= lineMaxWidth)
                        {
                            (*(currentLine-1)).append((*currentLine));
                            cursorPos = (*(currentLine-1)).size();
                            textBuffer.erase(currentLine--);
                        }
                        else
                            cursorPos = (*(--currentLine)).size();
                    }
                }
            }
            else
                cursorPos = (*(--currentLine)).size();
        }
        else if (cursorPos > 0)
        {
            // Are we at the end of the line? And are there more lines to follow?
            if (cursorPos == (*currentLine).size() && currentLine != textBuffer.end())
            {
                // Is the first character of the next line a space? And will adding this space to the current line overflow?
                if ((*(currentLine + 1))[0] == ' ' && Font::getInstance()->textWidth((*currentLine) + " ", fontname, pointSize) <= lineMaxWidth)
                {
                    (*(currentLine + 1)).erase(0);
                    (*currentLine).append(" ");
                }
                else
                {
                    unsigned int location = (*(currentLine + 1)).find(" ");
                    if (location != (*(currentLine + 1)).npos)
                    {
                        std::string temp = (*(currentLine + 1)).substr(0, location);
                        if (Font::getInstance()->textWidth((*currentLine) + temp, fontname, pointSize) <= lineMaxWidth)
                        {
                            (*currentLine).append(temp);
                            (*(currentLine + 1)).erase(0, location);
                            // If by erasing the word, the line becomes empty, erase the line
                            if ((*(currentLine + 1)).size() == 0)
                                textBuffer.erase(currentLine+1);
                        }
                        else
                            (*currentLine).erase(--cursorPos, 1);
                    }
                    else
                        (*currentLine).erase(--cursorPos, 1);
                }
            }
            else
                (*currentLine).erase(--cursorPos, 1);
        }
    }
    else if (keysym == SDLK_DELETE)
    {
        if (cursorPos == (*currentLine).size())
        {
            if (currentLine != textBuffer.end())
            {
                unsigned int location = (*(currentLine+1)).find(" ");
                if (location != (*(currentLine+1)).npos)
                {
                    if (location == 0)
                        (*(currentLine+1)).erase(0);
                    else
                    {
                        std::string temp = (*(currentLine+1)).substr(0, location);
                        if (Font::getInstance()->textWidth((*currentLine) + temp, fontname, pointSize) < lineMaxWidth)
                        {
                            (*currentLine).append(temp);
                            (*(currentLine+1)).erase(0, location);
                            if ((*(currentLine+1)).size() == 0)
                                textBuffer.erase((currentLine + 1));
                        }
                        else
                            (*(currentLine+1)).erase(0);
                    }
                }
                else
                    (*(currentLine+1)).erase(0);
            }
            // else do nothing.
        }
        else if ((*currentLine).size() > cursorPos)
            (*currentLine).erase(cursorPos, 1);
    }
    else if (keysym == SDLK_LEFT)
    {
        if (cursorPos == 0 && currentLine != textBuffer.begin())
        {
            cursorPos = (*(currentLine-1)).size(); --currentLine;
        }
        if (cursorPos > 0)
            --cursorPos;
    }
    else if (keysym == SDLK_RIGHT)
    {
        if (cursorPos == (*currentLine).size() && currentLine != textBuffer.end())
        {
            cursorPos = 0; --currentLine;
        }
        if (cursorPos < (*currentLine).size())
            ++cursorPos;
    }
    else if (keysym == SDLK_UP)
    {
        if (currentLine != textBuffer.begin())
        {
            if ((*--currentLine).size() < cursorPos)
                cursorPos = (*currentLine).size();
        }
    }
    else if (keysym == SDLK_DOWN)
    {
        if (currentLine != textBuffer.end())
        {
            if ((*++currentLine).size() < cursorPos)
                cursorPos = (*currentLine).size();
        }
    }
    else if (ch)
    {
        if (Font::getInstance()->textWidth((*currentLine) + char(ch), fontname, pointSize) > lineMaxWidth)
        {
            ++currentLine;
            cursorPos = 0;
        }
        (*currentLine).insert(cursorPos++, 1, ch);
    }
}

void GLSLEditor::draw()
{

}
