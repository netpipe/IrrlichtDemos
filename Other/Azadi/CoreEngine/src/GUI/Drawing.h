#ifndef DRAWING_HPP_05112006
#define DRAWING_HPP_05112006

#include <string>

#include "../Texture/TextureFactory.h"
#include "../Fonts/Font.h"
#include "Console.h"
#include "../Scripting/Headers/Drawing.h"

class Drawing : public azDrawing
{
public:
	Drawing();
	~Drawing(){};

	static Drawing* getInstance() { return instance; }

	virtual void drawTexturedRectWithColor(const int &x, const int &y, const int &w, const int &h, const std::string &textureName, const float &r, const float &g, const float &b);
	void drawTexturedRect(const int &x, const int &y, const int &w, const int &h, const std::string &textureName);

    void toggleAnisotropicFiltering(const float &amount);

	void clearScreen(const float &r, const float &g, const float &b, const float &i);
	inline void flipBuffers()
    {
        unsigned long lastTicks = SDL_GetTicks();
        frame++;
        last_frame_length = lastTicks - last_frame_time;
        last_frame_time = lastTicks;
        running_fps_value += last_frame_length;
        number_fps_values++;
        if (number_fps_values==10)
        {
            number_fps_values = 0;
            fps = 10000.0 / running_fps_value;
            running_fps_value = 0;
        }

        SDL_GL_SwapBuffers();
    }

	inline unsigned long int getFrame() { return frame; }
	unsigned int getLastFrameLength() { return last_frame_length; }
	inline double getFps() {return fps;}
	std::string fixText(std::string raw, unsigned int length);

	void setScreenResolution(const unsigned int &width, const unsigned int &height);
	void setFov(const float &newfov);
	void setAspect(const float &newaspect);

	inline void prepare2D()
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1500.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_CULL_FACE);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);
    }

	inline void prepare3D()
    {
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        glViewport(0,0,screenWidth,screenHeight);
        glFrustum(-fov * aspect, fov * aspect, -fov, fov, 1.0, 1500.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_CULL_FACE);
    }

	void init();
private:
	unsigned long int frame;
	unsigned int last_frame_time;
	unsigned int last_frame_length;
	std::string font_path;
	double fps;
	char number_fps_values;
	unsigned running_fps_value;
	static Drawing *instance;

	unsigned int screenWidth;
	unsigned int screenHeight;
	float fov;
	float aspect;
};

#endif
