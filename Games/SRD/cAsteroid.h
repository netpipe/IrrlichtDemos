#ifndef ASTEROID_H
#define ASTEROID_H

#include <irrlicht.h>
#include "cObject.h"

using namespace irr;
using namespace core;
using namespace video;

class cAsteroid : public cObject
{
	public:
		cAsteroid();



		void GetExplosionFrames();

		int health;				// how much dmg this asteroid can take before breaking appart

		int type;				// 

		int id;					// 

		int fragments;			// how many fragments this asteroid breaks into

		bool active;			// active or inactive
		int countdownTimer;		// Time till this asteroid is deleted

		rect<int> explosionFrames[30];

		rect<int> asteroidFrames[20];

		int currentFrame;				// Current frame

		int frameTimer;						// Controlls FPS, update frame if this number has reached a certain amount
};

#endif

cAsteroid::cAsteroid()
{
	int counter = 0;

	for(int y = 0; y < 3; y++)
		for(int x = 0; x < 10; x++)
		{
			explosionFrames[counter] = rect<int>(x*100, y*100, (x*100)+100, (y*100)+100);
			counter++;
		}

	counter = 0;

	for(int a = 0; a < 2; a++)
		for(int b = 0; b < 10; b++)
		{
			asteroidFrames[counter] = rect<int>(b*100, a*100, (b*100)+100, (a*100)+100);
			counter++;
		}
}

