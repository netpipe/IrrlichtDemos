#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <ode/ode.h>

class Game;

class Physics
{

private:

	static Game* game;
	
	static dWorldID world;

	static dSpaceID space;

	static dJointGroupID contactgroup;

	static void nearCallback(void* node, dGeomID o1, dGeomID o2);

public:

	Physics(Game* game);

	~Physics();

	static const void update();

	///\return The current ode world.
	static dWorldID getWorld() { return world; }

	///\return The default space. We only have this one.
	static dSpaceID getSpace() { return space; }
};

#endif /*PHYSICS_H_*/
