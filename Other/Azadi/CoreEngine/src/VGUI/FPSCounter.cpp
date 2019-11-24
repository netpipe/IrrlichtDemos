#include "FPSCounter.h"

FPSCounter::FPSCounter(const int &X, const int &Y)
{
    x = X; y = Y;

    cbg = new GUI::Pane(x, y, 220, 30, "FPSCounter.png");
    tar = new GUI::TextArea(x + 85, y + 21, 220, 30, "teen bold.ttf", 14);
    Font::getInstance()->genPointSize("teen bold.ttf", 14);
}

FPSCounter::~FPSCounter()
{

}

void FPSCounter::draw()
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    std::stringstream temp;
    temp << Drawing::getInstance()->getFps();
    (*tar) = temp.str();

    glPushMatrix();
    glLoadIdentity();
    cbg->draw();
    glLoadIdentity();
    tar->draw();
    glPopMatrix();
    glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
}
