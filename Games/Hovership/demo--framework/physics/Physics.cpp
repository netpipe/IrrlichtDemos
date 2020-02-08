#include "Physics.h"

#include "../core/Game.h"

Game* Physics::game = 0;

dWorldID Physics::world = 0;
dSpaceID Physics::space = 0;
dJointGroupID Physics::contactgroup = 0;

Physics::Physics(Game* game_)
{
	game = game_;

	dInitODE();

	//Create a new, empty world
	world = dWorldCreate();

	//Create a space
	space = dSimpleSpaceCreate(0);

	//create contact group
	contactgroup = dJointGroupCreate(0);

	dWorldSetGravity(world, 0.0, -9.80665, 0.0);

	//Set and get the global ERP value, that controls how much error correction is performed in each time step.
	dWorldSetERP(world, 0.8);

	//Set and get the global CFM (constraint force mixing) value.
	dWorldSetCFM(world, 1e-10);

	//Add some damping. Default's no damping. That's not good, we would spin around forever. 
	const dReal linearDamping = 0.15;
	const dReal angularDamping = 0.15;
	dWorldSetDamping(world, linearDamping, angularDamping);
}

Physics::~Physics()
{
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(space);
    dWorldDestroy(world);
    dCloseODE();
}

void Physics::nearCallback(void* node, dGeomID o1, dGeomID o2)
{
	//check if they may collide
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	if(b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) return;

	//generate contact points
	const irr::u32 MAX_CONTACTS = 2048;
	dContact contact[MAX_CONTACTS];

	const irr::u32 numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
	if(numc > 0)
	{
		irr::u32 i = 0;
		for(i = 0; i < numc; ++i)
		{
			contact[i].surface.mode = dContactBounce;
			contact[i].surface.mu = dInfinity;
			contact[i].surface.bounce = 0.0;
			contact[i].surface.bounce_vel = 0.0;

			dJointID c = dJointCreateContact(world, contactgroup, &contact[i]);
			dJointAttach(c, b1, b2);
		}

		//note that player health would be altered on any collision
		game->setPlayerHealth(game->getPlayerHealth()-1);
	}
}

const void Physics::update()
{
	  const dReal simstep = 0.05;
	  const irr::u32 steps = (irr::u32) ceilf(game->getElapsed()/simstep);

	  irr::u32 i;
	  for (i = 0; i < steps; ++i)
	  {
		  //determines which pairs of geoms in a space may potentially intersect, and calls
		  //a callback function with each candidate pair.
		  dSpaceCollide(space, 0, &nearCallback);

		  //step world
		  dWorldStep(world, simstep);

		  //The contact group needs to be emptied.
		  dJointGroupEmpty(contactgroup);
	  }
}
