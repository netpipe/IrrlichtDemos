#include "Drawing.h"

#include <SDL/SDL.h>
#include <GL/glew.h>
#ifndef __APPLE__
	#include <GL/gl.h>
#else
	#include <OpenGL/gl.h>
#endif

#include <string>

Drawing* Drawing::instance = NULL;

Drawing::Drawing()
{
#ifndef NDEBUG
	console << GUI::Console::normal << GUI::Console::normal << "Initializing Drawing subsystem" << GUI::Console::endl();
#endif
    font_path = "font.png";
    frame = 0;
    last_frame_time = 0;
    last_frame_length = 1;
    fps = 0;
    number_fps_values = 0;
    running_fps_value = 0;

    screenWidth = 0;
    screenHeight = 0;
    fov = 0;
    aspect = 0;

	TextureFactory::getInstance()->loadTexture(font_path);

	frame = 0;

	instance = this;
}

void Drawing::init()
{

}

void Drawing::setScreenResolution(const unsigned int &width, const unsigned int &height)
{
    screenWidth = width;
    screenHeight = height;
}

void Drawing::setFov(const float &newfov)
{
    fov = newfov;
}

void Drawing::setAspect(const float &newaspect)
{
    aspect = newaspect;
}

void Drawing::drawTexturedRectWithColor(const int &x, const int &y, const int &w, const int &h, const std::string &textureName, const float &r, const float &g, const float &b)
{
    TextureFactory::getInstance()->applyTexture(textureName);
    glColor3f(r, g, b);
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

void Drawing::drawTexturedRect(const int &x, const int &y, const int &w, const int &h, const std::string &textureName)
{
    TextureFactory::getInstance()->applyTexture(textureName);
    glColor3f(1.0f, 1.0f, 1.0f);
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

void Drawing::clearScreen(const float &r, const float &g, const float &b, const float &i)
{
    glClearColor(r, g, b, i);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

std::string Drawing::fixText(std::string raw, unsigned int length)
{
	if (raw.length() <= length)
		return raw;

	return raw.substr(0,length-3) + "...";
}

void Drawing::toggleAnisotropicFiltering(const float &amount)
{
    if (amount == 1.0)
        TextureFactory::getInstance()->enableAnisotropicFilter(false);
    else
        TextureFactory::getInstance()->enableAnisotropicFilter(true);
    TextureFactory::getInstance()->setAnisotropicFilteringLevel(amount);
    TextureFactory::getInstance()->reloadTextures();
}
