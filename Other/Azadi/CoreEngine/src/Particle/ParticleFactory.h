#ifndef __PARTICLEFACTORY_H__
#define __PARTICLEFACTORY_H__

#include "Particle.h"
#include "../GUI/Console.h"
#include "../Scripting/ScriptEngine.h"
#include "../Texture/TextureFactory.h"
#include <map>

#ifndef __APPLE__
    #include <GL/glew.h>
    #include <GL/gl.h>
#else
    #include <GL/glew.h>
    #include <OpenGL/gl.h>
#endif

//! Generates particles (to do->use sprites as well as points)
class ParticleGenerator
{
	/*!
		This is the particle generator. It differs from the factory that it's the one that generates the particles itself,
		whereas the particle factory generates generators.
	*/
	public:
		void init(const float &x, const float &y, const float &z, const float &dx, const float &dy, const float &dz, const std::string &spriteTexture, const float &width, const float &height, const float &startAlpha, const float &endAlpha, const float &duration, const float &rate, unsigned long int &numberOfParticles);
		int tick();	// Updates particle positions, returns  1 if all the particles are dead
		void draw(const float &x, const float &y, const float &z, const float &ix, const float &iy);
		inline bool isDone() { return done; }
		void stop();
	private:
		std::map<unsigned long int,Particle> particles;
		GLfloat width;
		GLfloat height;
		GLfloat x, y, z, dx, dy, dz;
		GLfloat startAlpha, endAlpha;
		GLfloat duration;
		float rate;
		std::string texture;
		unsigned long int numParticles;
		unsigned long int currentParticles;
		bool done;
		unsigned long int lastTicks;
		unsigned long int startTime;
};

//!	The particle factory that generates particle generators
class ParticleFactory
{
	public:
		ParticleFactory();
		~ParticleFactory();
		unsigned int newPart(const float &x, const float &y, const float &z, const float &dx, const float &dy, const float &dz, const std::string &spriteTexture, const float &width, const float &height, const float &startAlpha, const float &endAlpha, const float &duration, const float &rate, unsigned long int &numberOfParticles);
		void tick();
		void draw(const float &x, const float &y, const float &z, const float &ix, const float &iy);

		static ParticleFactory* getInstance() { return instance; }
	private:
        static ParticleFactory *instance;

        unsigned long int currentIterator;
		std::map<unsigned long int, ParticleGenerator*> PartGens;
};

#endif
