/*

Pole - header file

*/

#ifndef __Pole_h
#define __Pole_h

#include <irrlicht.h>

using namespace irr;

#include <Init.h>
#include <SceneNodeAnimatorTA.h>
#include "CollisionObjectAABBMesh.h"
#include <Einstein.h>


class cPole {
public:
	
	// constructor
	cPole();
	
	// destructor
	~cPole();
	
	// Initialization
	void Initialize();
	
	// add a new pole
	void addPole(TA::StaticObject* addPole, core::vector3df pos);
	
	// get amount of poles
	u32 getPoleCount();
	
	// checking cables for collision
	u32 checkForCollision(TA::Physics &physics, core::line3d<f32> cable);
	
	// calculate the angular velocity to let the rocket rotate around the pole
	core::vector3df calculateAngularVelocity(core::vector3df rocketLookAt, core::vector3df poleLookAt, f32 radius);
	
	// return pole position
	const core::vector3df getPolePosition(u32 i);
	
	// return polePosition array
	const core::array<core::vector3df> getPolePosition();
	
	
private:
	
	cEinstein einstein;
	
	core::array<TA::StaticObject*> poleArray;
	core::array<core::vector3df> polePosition;
	
};

#endif
	
