#include "Flock.h"
#include <ISceneManager.h>
//#include "Configuration.h"
//#include "Demo.h"
#include "BoidSceneNode.h"


Flock::Flock( const irr::core::vector3df& target, const irr::f32 borders[4])// :
//	paused(false), target(target),
/*
	scatterFlockModifier(->getConfiguration()->getScatterFlockModifier()),
	seekCenterOfMass(demo->getConfiguration()->getSeekCenterOfMass()),
	distanceToOtherBoids(demo->getConfiguration()->getDistanceToOtherBoids()),
	matchVelocity(demo->getConfiguration()->getMatchVelocity()),
	tendencyTowardsPlace(demo->getConfiguration()->getTendencyTowardsPlace()),
	tendencyAvoidPlace(demo->getConfiguration()->getTendencyAvoidPlace()),
	mimimumAboveGround(demo->getConfiguration()->getMimimumAboveGround()),
	speedLimit(demo->getConfiguration()->getSpeedLimit())
*/
{
	memcpy(this->borders, borders, sizeof(irr::f32)*4);

	    scatterFlockModifier=(2.0f);
	seekCenterOfMass=(30.0f);
	distanceToOtherBoids=(20.0f);
	matchVelocity=(80.0f);
	tendencyTowardsPlace=(30.0f);
	tendencyAvoidPlace=(15.0f);
	mimimumAboveGround=(200.0f);
	speedLimit=(10.0f);
}

Flock::~Flock()
{
	for (irr::u32 i = 0; i < boids.size(); ++i)
	{
		boids[i]->remove();
		boids[i]->drop();
	}
}

void Flock::update(irr::scene::ITriangleSelector* const selector, const irr::f32 deltaTime, const bool scatterFlock) const
{
	if (this->paused)
		return;

	//update every boid
	const irr::u32 numBoids = this->boids.size();
	//const irr::f32 deltaTime3 = deltaTime;

#ifdef _SOUND
	irrklang::ISoundEngine* const soundEngine = this->demo->getSoundEngine();
	const bool soundEnabled = this->demo->getConfiguration()->isSoundEnabled();
#endif

	irr::u32 current;
	for (current = 0; current < numBoids; ++current)
	{
		//apply rules to boid
		this->boids[current]->applyRules(
			selector,
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
			, soundEngine, soundEnabled
#endif
		);
	}
}

irr::scene::BoidSceneNode* const Flock::addBoid(irr::scene::IMesh* const boidMesh)
{
	irr::scene::BoidSceneNode* const boid = new irr::scene::BoidSceneNode(boidMesh,
		this->target, this->borders,
		smgr, this->boids.size());

	this->boids.push_back(boid);

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
				boid->drop(); //?
				this->boids.erase(i);

				break;
			}
		}
	}
}
