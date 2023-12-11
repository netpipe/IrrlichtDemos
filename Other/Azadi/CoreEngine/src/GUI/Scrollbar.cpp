#include "Scrollbar.h"

GUI::Scrollbar::Scrollbar(const int &X, const int &Y, const int &W, const int &H, const int &Direction,
    const std::string &btex, const std::string &stex)
{
    x = X;
    y = Y;
    w = W;
    h = H;
    direction = Direction;

    texture_factory = TextureFactory::getInstance();

    texture_factory->loadTexture(btex);
    barTexName = btex;
    texture_factory->loadTexture(stex);
    scrollTexName = stex;
    sX = sY = sW = sH = 0;
    minVisPixels = 8;

    sHeld = false;
}

GUI::Scrollbar::~Scrollbar()
{

}

GUI::Scrollbar& GUI::Scrollbar::setX(const int &X)
{
    x = X;
    return *this;
}

GUI::Scrollbar& GUI::Scrollbar::setY(const int &Y)
{
    y = Y;
    return *this;
}

GUI::Scrollbar& GUI::Scrollbar::setW(const int &W)
{
    w = W;
    return *this;
}

GUI::Scrollbar& GUI::Scrollbar::setH(const int &H)
{
    h = H;
    return *this;
}

GUI::Scrollbar& GUI::Scrollbar::setDirection(const int &dir)
{
    direction = dir;
    return *this;
}

GUI::Scrollbar& GUI::Scrollbar::setBarTexture(const std::string &texName)
{
    texture_factory->loadTexture(texName);
    barTexName = texName;
    return *this;
}

GUI::Scrollbar& GUI::Scrollbar::setSliderTexture(const std::string &texName)
{
    texture_factory->loadTexture(texName);
    scrollTexName = texName;
    return *this;
}

GUI::Scrollbar& GUI::Scrollbar::setBarMin(const int &bmin)
{
    min = bmin;
    return *this;
}

GUI::Scrollbar& GUI::Scrollbar::setBarMax(const int &bmax)
{
    max = bmax;
    return *this;
}

int GUI::Scrollbar::getBarMin()
{
    return min;
}

int GUI::Scrollbar::getBarMax()
{
    return max;
}

GUI::Scrollbar& GUI::Scrollbar::setCurrentValue(const int &value)
{
    cValue = value;
    return *this;
}

int GUI::Scrollbar::getCurrentValue()
{
    return cValue;
}

GUI::Scrollbar& GUI::Scrollbar::incCurrentValue()
{
    cValue++;
    if (cValue > max)
        cValue = max;
    return *this;
}

GUI::Scrollbar& GUI::Scrollbar::decCurrentValue()
{
    cValue--;
    if (cValue < min)
        cValue = min;
    return *this;
}

GUI::Scrollbar& GUI::Scrollbar::setMinVisiblePixels(const int &minVis)
{
    minVisPixels = minVis;
    return *this;
}

void GUI::Scrollbar::updateSlider()
{
    int tmax = max;
    if (min < 0)
        tmax += abs(min);

    if (tmax == 0)
        tmax = 1;

    if (direction == 0)
    {
        sW = w / tmax;
        if (sW < minVisPixels)
            sW = minVisPixels;

        float calc = (float(w) - sW) / tmax;

        sH = h;

        sX = int(calc * cValue)+x;
        sY = y;
    }
    else
    {
        sH = h / tmax;
        if (sH < minVisPixels)
            sH = minVisPixels;

        float calc = (float(h) - sH) / tmax;

        sW = w;

        sY = int(calc * cValue)+y;
        sX = x;
    }
}

void GUI::Scrollbar::parseInput(Input &input)
{
    int mouseX, mouseY;
    mouseX = input.getMouseX();
    mouseY = input.getMouseY();
    if (mouseX > x && mouseX < x+w)
    {
        if (mouseY > y && mouseY < y+h)
        {
            if (input.getMouseDown(1))
            {
                if (direction == 0)
                {
                    if ((mouseX < sX || mouseX > sX + sW) && !sHeld)
                    {
                        int tmax = max;
                        if (min < 0)
                            tmax += abs(min);
                        else if (min > 0)
                            tmax -= min;

                        cValue = int(((float(mouseX-x)/w)*tmax) + min);
                        input.getMouseClick(1);
                    }
                    else
                    {
                        sHeld = true;
                    }
                }
                else
                {
                    if ((mouseY < sY || mouseY > sY + sH) && !sHeld)
                    {
                        int tmax = max;
                        if (min < 0)
                            tmax += abs(min);
                        else if (min > 0)
                            tmax -= min;

                        cValue = int(((float(mouseY-y)/h)*tmax) + min);
                        input.getMouseClick(1);
                    }
                    else
                    {
                        sHeld = true;
                    }
                }
            }
        }
    }

    if (!input.getMouseDown(1))
        sHeld = false;

    if (sHeld)
    {
        if (direction == 0)
        {
            int tmax = max;
            if (min < 0)
                tmax += abs(min);
            else if (min > 0)
                tmax -= min;

            if (mouseX <  x)
                cValue = min;
            else if (mouseX > x+w)
                cValue = max;
            else
                cValue = int(((float(mouseX-x)/w)*tmax) + min);
        }
        else
        {
            int tmax = max;
            if (min < 0)
                tmax += abs(min);
            else if (min > 0)
                tmax -= min;

            if (mouseY <  y)
                cValue = min;
            else if (mouseY > y+h)
                cValue = max;
            else
                cValue = int(((float(mouseY-y)/h)*tmax) + min);
        }
    }
    updateSlider();
}

void GUI::Scrollbar::draw()
{
    if (barTexName != "")
        texture_factory->applyTexture(barTexName);
    else
        glBindTexture(GL_TEXTURE_2D, 0);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(x, y);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(x+w, y);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(x+w, y+h);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(x, y+h);
    }
    glEnd();

    if (scrollTexName != "")
        texture_factory->applyTexture(scrollTexName);
    else
        glBindTexture(GL_TEXTURE_2D, 0);

    glColor3f(1.0f, (scrollTexName != "")?1.0f:0.0f, (scrollTexName != "")?1.0f:0.0f);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(sX, sY);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(sX+sW, sY);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(sX+sW, sY+sH);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(sX, sY+sH);
    }
    glEnd();
}
