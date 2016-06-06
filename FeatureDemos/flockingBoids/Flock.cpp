#include "Flock.h"
#include <ISceneManager.h>
#include "core/Configuration.h"
#include "core/Demo.h"
#include "scene/BoidSceneNode.h"


Flock::Flock(const Demo* const demo, const irr::u32 numBoids, const irr::core::vector3df& target, const irr::f32 borders[4]) :
	demo(demo),
	target(target),
	scatterFlockModifier(demo->getConfiguration()->getScatterFlockModifier()),
	seekCenterOfMass(demo->getConfiguration()->getSeekCenterOfMass()),
	distanceToOtherBoids(demo->getConfiguration()->getDistanceToOtherBoids()),
	matchVelocity(demo->getConfiguration()->getMatchVelocity()),
	tendencyTowardsPlace(demo->getConfiguration()->getTendencyTowardsPlace()),
	tendencyAvoidPlace(demo->getConfiguration()->getTendencyAvoidPlace()),
	mimimumAboveGround(demo->getConfiguration()->getMimimumAboveGround()),
	speedLimit(demo->getConfiguration()->getSpeedLimit())
{
	//copy borders
	memcpy(this->borders, borders, sizeof(irr::f32)*4);

	irr::f32 radius = 20.0f;
	irr::u32 polyCountX = 16;
	irr::u32 polyCountY = 16;

	for (irr::u32 i = 0; i < numBoids; ++i)
	{
		irr::scene::BoidSceneNode* boid = new irr::scene::BoidSceneNode(
			radius, polyCountX, polyCountY, target, demo->getSelector(), this->borders,
			demo->getSceneManager()->getRootSceneNode(), demo->getSceneManager(), i);

		boids.push_back(boid);
	}
}

Flock::~Flock()
{
	for (irr::u32 i = 0; i < boids.size(); ++i)
	{
		boids[i]->remove();
	}
}

void Flock::update(const irr::f32 deltaTime, const bool scatterFlock) const
{
	//update every boid
	const irr::u32 numBoids = this->boids.size();
	irr::u32 current;
	for (current = 0; current < numBoids; ++current)
	{
		//apply rules to boid
		this->boids[current]->applyRules(
			this->boids,

			//rules in flock
			this->distanceToOtherBoids,
			this->seekCenterOfMass,
			this->matchVelocity,

			//seek and avoid targets
			this->target,
			this->tendencyTowardsPlace,
			this->mimimumAboveGround,
			this->tendencyAvoidPlace,

			//scattering the flock
			scatterFlock, this->scatterFlockModifier,

			//timing
			deltaTime, this->speedLimit
#ifdef _SOUND
			, demo->getSoundEngine(), demo->getConfiguration()->isSoundEnabled()
#endif
		);
	}
}

irr::scene::BoidSceneNode* const Flock::addBoid()
{
/*
	Boid* const boid = new Boid(this->demo, this, this->boids.size());
	this->boids.push_back(boid);
*/

	irr::f32 radius = 20;
	irr::u32 polyCountX = 16;
	irr::u32 polyCountY = 16;

	irr::scene::BoidSceneNode* boid = new irr::scene::BoidSceneNode(
		radius, polyCountX, polyCountY, target, demo->getSelector(), this->borders,
		demo->getSceneManager()->getRootSceneNode(), demo->getSceneManager(), this->boids.size());

	boids.push_back(boid);


	return boid;
}

void Flock::removeBoid(irr::scene::BoidSceneNode* const boid)
{
	if (this->boids.size() > 2) // there should be at least 2 boids in the flock
	{
		irr::u32 i;
		for(i = 0; i < this->boids.size(); ++i)
		{
			if (*this->boids[i] == *boid)
			{
				boid->remove();
				this->boids.erase(i);

				break;
			}
		}
	}
}
