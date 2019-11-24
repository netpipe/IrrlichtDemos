#include "ParticleFactory.h"

using GUI::Console;

//! This is a big fat function O_O   Underdocumented at the moment
void ParticleGenerator::init(const float &X, const float &Y, const float &Z, const float &Dx, const float &Dy, const float &Dz, const std::string &spriteTexture, const float &Width, const float &Height, const float &sAlpha, const float &eAlpha, const float &dur, const float &rateOfSpawn, unsigned long int &numOfParts)
{
    x = X;  dx = Dx;
    y = Y;  dy = Dy;
    z = Z;  dz = Dz;
    width = Width; height = Height;
    texture = spriteTexture;
    duration = dur;
    startAlpha = sAlpha; endAlpha = eAlpha;
    numParticles = numOfParts;
    rate = rateOfSpawn;
    TextureFactory::getInstance()->loadTexture(texture);
    done = false;
    currentParticles = 0;
    lastTicks = 0;
    startTime = SDL_GetTicks();
}

//! Updates the position of the particles as well as creates new ones
int ParticleGenerator::tick()
{
    if (SDL_GetTicks() - lastTicks >= rate)
    {
        for (unsigned int i = 0; i < numParticles; ++i)
        {
            if (particles[currentParticles].isDead())
                continue;
            particles[currentParticles].init(x, y, z, dx, dy, dz, width, height, startAlpha, endAlpha, duration);
        }

        lastTicks = SDL_GetTicks();
    }
    if (SDL_GetTicks() - startTime > duration)
    {
        TextureFactory::getInstance()->deleteTexture(texture);
        done = true;
    }
    return 0;
}

//! Draws the particles themselves
void ParticleGenerator::draw(const float &x, const float &y, const float &z, const float &ix, const float &iy)
{
    TextureFactory::getInstance()->applyTexture(texture);
    for (std::map<unsigned long int,Particle>::iterator it = particles.begin(); it != particles.end(); ++it)
    {
        (*it).second.draw(x, y, z, ix, iy);
    }
}

ParticleFactory* ParticleFactory::instance = NULL;

ParticleFactory::ParticleFactory()
{
    console << Console::highish << Console::log << "Initializing Particle Factory" << Console::endl();
    instance = this;
    currentIterator = 0;
}

ParticleFactory::~ParticleFactory()
{

}

//! Creates a new particle generator as the specified position (There has to be a way to make the arguments smaller)
unsigned int ParticleFactory::newPart(const float &x, const float &y, const float &z, const float &dx, const float &dy, const float &dz, const std::string &spriteTexture, const float &width, const float &height, const float &startAlpha, const float &endAlpha, const float &duration, const float &rate, unsigned long int &numberOfParticles)
{
    while (PartGens[currentIterator] != NULL)
        ++currentIterator;
    ParticleGenerator *newGen = new ParticleGenerator;
    PartGens[currentIterator] = newGen;
    PartGens[currentIterator]->init(x, y, z, dx, dy, dz, spriteTexture, width, height, startAlpha, endAlpha, duration, rate, numberOfParticles);
    return currentIterator;
}

//! Should be called each cycle, as it updates every particle generator
void ParticleFactory::tick()
{
    for (std::map<unsigned long int, ParticleGenerator*>::iterator it = PartGens.begin(); it != PartGens.end(); ++it)
    {
        if ((*it).second != NULL)
            (*it).second->tick();
    }
}

//! Should be called each cycle as well, calls each particle generator to draw (expensive operation!!!)
void ParticleFactory::draw(const float &x, const float &y, const float &z, const float &ix, const float &iy)
{
    for (std::map<unsigned long int, ParticleGenerator*>::iterator it = PartGens.begin(); it != PartGens.end(); ++it)
    {
        if ((*it).second != NULL)
            (*it).second->draw(x, y, z, ix, iy);
    }
}
