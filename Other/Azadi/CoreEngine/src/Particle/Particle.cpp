#include "Particle.h"

//! Initializes the particle itself
void Particle::init(const float &x, const float &y, const float &z, const float &dx, const float &dy, const float &dz, const float &width, const float &height, const float &startAlpha, const float &eAlpha, const float &decayRate)
{
    position[0] = x; position[1] = y; position[2] = z;
    direction[0] = dx; direction[1] = dy; direction[2] = dz;
    color[0] = color[1] = color[2] = 1.0f;
    color[3] = startAlpha;
    rate = decayRate;
    endAlpha = eAlpha;
}

//! Draws the particle
void Particle::draw(const float &x, const float &y, const float &z, const float &ix, const float &iy)
{
    if (color[3] > endAlpha)
    {
        glColor4f(color[0], color[1], color[2], color[3]);
        glVertex3f(position[0], position[1], position[2]);

    }
}

//! Updates the particle's position and color
void Particle::update()
{
    if (color[3] <= endAlpha)
        return;

    color[3] -= rate;
    position[0] += direction[0];
    position[1] += direction[1];
    position[2] += direction[2];

    return;
}
