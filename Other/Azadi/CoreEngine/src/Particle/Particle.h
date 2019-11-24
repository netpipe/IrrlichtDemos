#include "../vector.h"

#include <GL/glew.h>
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

class Particle
{
	public:
		inline bool isDead()
        {
            return (color[3] <= endAlpha);
        }

		void init(const float &x, const float &y, const float &z, const float &dx, const float &dy, const float &dz, const float &width, const float &height, const float &startAlpha, const float &endAlpha, const float &rate);
		void draw(const float &x, const float &y, const float &z, const float &ix, const float &iy);
		void update();
	private:
		GLfloat position[3];
		GLfloat direction[3];
		GLfloat color[4];
		GLfloat rate;
		GLfloat endAlpha;
};
