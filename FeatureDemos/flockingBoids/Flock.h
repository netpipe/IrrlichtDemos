#ifndef FLOCK_H_
#define FLOCK_H_

#include <irrArray.h>
#include <vector3d.h>

namespace irr
{
	namespace scene
	{
		class BoidSceneNode;
	}
};

class Demo;
class Boid;

class Flock
{

public:

	Flock(const Demo* const demo, const irr::u32 numBoids, const irr::core::vector3df& target, const irr::f32 borders[4]);

	~Flock();

	///Applies the rules for flocking to the boids. See http://www.vergenet.net/~conrad/boids/pseudocode.html
	void update(const irr::f32 deltaTime, const bool scatterFlock) const;

	///Adds a boid to the flock.
	irr::scene::BoidSceneNode* const addBoid();

	///Removes a boid from the flock.
	void removeBoid(irr::scene::BoidSceneNode* const boid);

	//setter

	///Sets the target of the flock
	inline void setTarget(const irr::core::vector3df& target) {	this->target = target; }

	//getter

	inline const irr::core::vector3df& getTarget() const { return this->target; }

	inline const irr::core::array<irr::scene::BoidSceneNode*>& getBoids() const { return this->boids; }

private:

	///Pointer to the demo;
	const Demo* const demo;

	irr::f32 borders[4];

	///Array holding pointers to all the boids.
	irr::core::array<irr::scene::BoidSceneNode*> boids;

	///The target vector of the flock.
	irr::core::vector3df target;


	irr::f32 scatterFlockModifier;

	//rule1
	irr::f32 seekCenterOfMass;

	//rule2
	irr::f32 distanceToOtherBoids;

	//rule3
	irr::f32 matchVelocity;

	//seek target
	irr::f32 tendencyTowardsPlace;

	irr::f32 tendencyAvoidPlace;

	irr::f32 mimimumAboveGround;

	///Maximum speed of the boids.
	irr::f32 speedLimit;
};

#endif /* FLOCK_H_ */
