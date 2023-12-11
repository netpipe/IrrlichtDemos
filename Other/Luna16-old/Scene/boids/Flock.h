#ifndef FLOCK_H_
#define FLOCK_H_

#include <irrArray.h>
#include <vector3d.h>
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
namespace irr
{
	namespace scene
	{
		class BoidSceneNode;
		class IMesh;
		class ITriangleSelector;
	}
};

class Demo;

class Flock
{

public:

	Flock( const irr::core::vector3df& target, const irr::f32 borders[4]);

	~Flock();

	///Applies the rules for flocking to the boids. See http://www.vergenet.net/~conrad/boids/pseudocode.html
	void update(irr::scene::ITriangleSelector* const selector, const irr::f32 deltaTime, const bool scatterFlock) const;

	///Adds a boid to the flock.
	irr::scene::BoidSceneNode* const addBoid(irr::scene::IMesh* const boidMesh);

	///Removes a boid from the flock.
	void removeBoid(irr::scene::BoidSceneNode* const boid);

	//setter

	inline void setPaused(const bool paused) { this->paused = paused; }

	///Sets the target of the flock
	inline void setTarget(const irr::core::vector3df& target) {	this->target = target; }// + irr::core::vector3df(0.0f, this->mimimumAboveGround*2, 0.0f); }

	//getter

	inline bool isPaused() const { return this->paused; }

	inline const irr::core::vector3df& getTarget() const { return this->target; }

	inline const irr::core::array<irr::scene::BoidSceneNode*>& getBoids() const { return this->boids; }


//my mods
//    irr::scene::IMetaTriangleSelector* selector;
/*
	irr::ITimer* timer;
	irr::u32 then;
	irr::u32 now;
	irr::f32 elapsed;

	inline void tick()
	{
		//compute time since last frame
		this->now = this->timer->getTime();
		this->elapsed = ((irr::f32)(this->now - this->then))*0.001f;
		this->then = this->now;
	}
	*/
    IrrlichtDevice *device;
    ISceneManager *smgr;
	inline void registerScene(ISceneManager &scene){smgr = &scene;}
//end of mod


private:


	bool paused;

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
