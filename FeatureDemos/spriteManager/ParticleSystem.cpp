#include "ParticleSystem.h"
#include "SpriteManager.h"

using namespace irr;
using namespace video;

float calculate_variation(float inner, float outer, float var)
{
    float low = inner - (outer/2.0f)*var;
    float high = inner + (outer/2.0f)*var;
    float r = (rand() / (float(RAND_MAX)+1));
    return low*(1-r)+high*r;
}


ParticleSystem::ParticleSystem(SpriteManager * sprite, unsigned int buffer) : pStart(0), pLast(0), pEnd(0), active(true), emissionRate(0),
        emitCounter(0), lifetime(-1), life(0), particleLifeMin(0), particleLifeMax(0),
        direction(0), spread(0), relative(false), speedMin(0), speedMax(0), gravityMin(0),
        gravityMax(0), radialAccelerationMin(0), radialAccelerationMax(0),
        tangentialAccelerationMin(0), tangentialAccelerationMax(0),
        sizeStart(1), sizeEnd(1), sizeVariation(0), rotationMin(0), rotationMax(0),
        spinStart(0), spinEnd(0), spinVariation(0), offsetX(0),
        offsetY(0)
{
    this->sprite = sprite;
    memset(colorStart, 255, 4);
    memset(colorEnd, 255, 4);
    setBufferSize(buffer);
    tangentialX = 0;
    tangentialY = 0;
}

ParticleSystem::~ParticleSystem()
{
    if (this->sprite != 0)
    {
        this->sprite = 0;
    }

    if (pStart != 0)
        delete [] pStart;
}

void ParticleSystem::add()
{
    if (isFull()) return;

    float min,max;

    min = particleLifeMin;
    max = particleLifeMax;
    if (min == max)
        pLast->life = min;
    else
        pLast->life = (rand() / (float(RAND_MAX)+1)) * (max - min) + min;
    pLast->lifetime = pLast->life;

    pLast->position[0] = position.X;
    pLast->position[1] = position.Y;

    min = direction - spread/2.0f;
    max = direction + spread/2.0f;
    pLast->direction = (rand() / (float(RAND_MAX)+1)) * (max - min) + min;

    min = speedMin;
    max = speedMax;
    float speed = (rand() / (float(RAND_MAX)+1)) * (max - min) + min;
    pLast->speed = core::vector2df(cos(pLast->direction), sin(pLast->direction));
    pLast->speed *= speed;

    min = gravityMin;
    max = gravityMax;
    pLast->gravity = (rand() / (float(RAND_MAX)+1)) * (max - min) + min;

    min = radialAccelerationMin;
    max = radialAccelerationMax;
    pLast->radialAcceleration = (rand() / (float(RAND_MAX)+1)) * (max - min) + min;

    min = tangentialAccelerationMin;
    max = tangentialAccelerationMax;
    pLast->tangentialAcceleration = (rand() / (float(RAND_MAX)+1)) * (max - min) + min;

    pLast->sizeStart = calculate_variation(sizeStart, sizeEnd, sizeVariation);
    pLast->sizeEnd = calculate_variation(sizeEnd, sizeStart, sizeVariation);
    pLast->size = pLast->sizeStart;

    min = rotationMin;
    max = rotationMax;
    pLast->spinStart = calculate_variation(spinStart, spinEnd, spinVariation);
    pLast->spinEnd = calculate_variation(spinEnd, spinStart, spinVariation);
    pLast->rotation = (rand() / (float(RAND_MAX)+1)) * (max - min) + min;;

    pLast->color[0] = (float)colorStart[0] / 255;
    pLast->color[1] = (float)colorStart[1] / 255;
    pLast->color[2] = (float)colorStart[2] / 255;
    pLast->color[3] = (float)colorStart[3] / 255;

    pLast++;
}

void ParticleSystem::remove(particle * p)
{
    if (!isEmpty())
    {
        *p = *(--pLast);
    }
}

void ParticleSystem::setSprite(SpriteManager * image, int sx, int sy, int sw, int sh)
{
    if (this->sprite != 0)
    {
        this->sprite = 0;
    }

    this->sprite = image;
    this->sx = sx;
    this->sy = sy;
    this->sw = sw;
    this->sh = sh;
}

void ParticleSystem::setBufferSize(unsigned int size)
{
    // delete previous data
    delete [] pStart;

    pLast = pStart = new particle[size];

    pEnd = pStart + size;
}

void ParticleSystem::setEmissionRate(int rate)
{
    emissionRate = rate;
}

void ParticleSystem::setLifetime(float life)
{
    this->life = lifetime = life;
}

void ParticleSystem::setParticleLife(float min, float max)
{
    particleLifeMin = min;
    if (max == 0)
        particleLifeMax = min;
    else
        particleLifeMax = max;
}

void ParticleSystem::setPosition(float x, float y)
{
    position = core::vector2df(x, y);
}

void ParticleSystem::setDirection(float direction)
{
    this->direction = direction;
}

void ParticleSystem::setSpread(float spread)
{
    this->spread = spread;
}

void ParticleSystem::setRelativeDirection(bool relative)
{
    this->relative = relative;
}

void ParticleSystem::setSpeed(float speed)
{
    speedMin = speedMax = speed;
}

void ParticleSystem::setSpeed(float min, float max)
{
    speedMin = min;
    speedMax = max;
}

void ParticleSystem::setGravity(float gravity)
{
    gravityMin = gravityMax = gravity;
}

void ParticleSystem::setGravity(float min, float max)
{
    gravityMin = min;
    gravityMax = max;
}

void ParticleSystem::setRadialAcceleration(float acceleration)
{
    radialAccelerationMin = radialAccelerationMax = acceleration;
}

void ParticleSystem::setRadialAcceleration(float min, float max)
{
    radialAccelerationMin = min;
    radialAccelerationMax = max;
}

void ParticleSystem::setTangentialAcceleration(float acceleration)
{
    tangentialAccelerationMin = tangentialAccelerationMax = acceleration;
}

void ParticleSystem::setTangentialAcceleration(float min, float max)
{
    tangentialAccelerationMin = min;
    tangentialAccelerationMax = max;
}

void ParticleSystem::setSize(float size)
{
    sizeStart = size;
    sizeEnd = size;
}

void ParticleSystem::setSize(float start, float end)
{
    sizeStart = start;
    sizeEnd = end;
}

void ParticleSystem::setSize(float start, float end, float variation)
{
    sizeStart = start;
    sizeEnd = end;
    sizeVariation = variation;
}

void ParticleSystem::setSizeVariation(float variation)
{
    sizeVariation = variation;
}

void ParticleSystem::setRotation(float rotation)
{
    rotationMin = rotationMax = rotation;
}

void ParticleSystem::setRotation(float min, float max)
{
    rotationMin = min;
    rotationMax = max;
}

void ParticleSystem::setSpin(float spin)
{
    spinStart = spin;
    spinEnd = spin;
}

void ParticleSystem::setSpin(float start, float end)
{
    spinStart = start;
    spinEnd = end;
}

void ParticleSystem::setSpin(float start, float end, float variation)
{
    spinStart = start;
    spinEnd = end;
    spinVariation = variation;
}

void ParticleSystem::setSpinVariation(float variation)
{
    spinVariation = variation;
}

void ParticleSystem::setColor(unsigned char * color)
{
    memcpy(colorStart, color, 4);
    memcpy(colorEnd, color, 4);
}

void ParticleSystem::setColor(unsigned char * start, unsigned char * end)
{
    memcpy(colorStart, start, 4);
    memcpy(colorEnd, end, 4);
}

void ParticleSystem::setOffset(float x, float y)
{
    offsetX = x;
    offsetY = y;
}

float ParticleSystem::getX() const
{
    return position.X;
}

float ParticleSystem::getY() const
{
    return position.Y;
}

float ParticleSystem::getDirection() const
{
    return direction;
}

float ParticleSystem::getSpread() const
{
    return spread;
}

float ParticleSystem::getOffsetX() const
{
    return offsetX;
}

float ParticleSystem::getOffsetY() const
{
    return offsetY;
}

int ParticleSystem::count() const
{
    return (int)(pLast - pStart);
}

void ParticleSystem::start()
{
    active = true;
}

void ParticleSystem::stop()
{
    active = false;
    life = lifetime;
    emitCounter = 0;
}

void ParticleSystem::pause()
{
    active = false;
}

void ParticleSystem::reset()
{
    pLast = pStart;
    life = lifetime;
    emitCounter = 0;
}

bool ParticleSystem::isActive() const
{
    return active;
}

bool ParticleSystem::isEmpty() const
{
    return pStart == pLast;
}

bool ParticleSystem::isFull() const
{
    return pLast == pEnd;
}

void ParticleSystem::draw(float x, float y, float angle, float sx, float sy, float ox, float oy) const
{
    if (sprite == 0) return;

    particle * p = pStart;
    while (p != pLast)
    {
        Sprite *s = sprite->addSprite(this->sx, this->sy, sw, sh);
        s->setPosition(x + p->position[0], y + p->position[1]);
        s->setAngle(p->rotation);
        s->setScale(p->size, p->size);
        s->setAnchor(sw>>1,sh>>1);
        s->setColor(SColor(p->color[3]*255,p->color[2]*255,p->color[1]*255,p->color[0]*255));
        p++;
    }
}

void ParticleSystem::update(float dt)
{
    // Traverse all particles and update.
    particle * p = pStart;

    // Make some more particles.
    if (active)
    {
        float rate = 1.0f / emissionRate; // the amount of time between each particle emit
        emitCounter += dt;
        while (emitCounter > rate)
        {
            add();
            emitCounter -= rate;
        }
        /*int particles = (int)(emissionRate * dt);
        for(int i = 0; i != particles; i++)
        	add();*/

        life -= dt;
        if (lifetime != -1 && life < 0)
            stop();
    }

    while (p != pLast)
    {
        // Decrease lifespan.
        p->life -= dt;

        if (p->life > 0)
        {

            // Temp variables.
            core::vector2df radial, tangential, gravity(0, p->gravity);
            core::vector2df ppos(p->position[0], p->position[1]);

            // Get vector from particle center to particle.
            radial = ppos - position;
            radial.normalize();
            tangential = radial;

            // Resize radial acceleration.
            radial *= p->radialAcceleration;

            // Calculate tangential acceleration.
            {
                float a = tangentialX;
                tangential.set(-tangentialY, a);
            }

            // Resize tangential.
            tangential *= p->tangentialAcceleration;

            // Update position.
            p->speed += (radial+tangential+gravity)*dt;

            // Modify position.
            ppos += p->speed * dt;

            p->position[0] = ppos.X;
            p->position[1] = ppos.Y;

            const float t = p->life / p->lifetime;

            // Change size.
            p->size = p->sizeEnd - ((p->sizeEnd - p->sizeStart) * t);

            // Rotate.
            p->rotation += (p->spinStart*(1-t) + p->spinEnd*t)*dt;

            // Update color.
            p->color[0] = (float)(colorEnd[0]*(1.0f-t) + colorStart[0] * t)/255.0f;
            p->color[1] = (float)(colorEnd[1]*(1.0f-t) + colorStart[1] * t)/255.0f;
            p->color[2] = (float)(colorEnd[2]*(1.0f-t) + colorStart[2] * t)/255.0f;
            p->color[3] = (float)(colorEnd[3]*(1.0f-t) + colorStart[3] * t)/255.0f;

            // Next particle.
            p++;
        }
        else
        {
            remove(p);

            if (p >= pLast)
                return;
        } // else
    } // while
}
