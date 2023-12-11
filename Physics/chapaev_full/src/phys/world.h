// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_PHYS_WORLD_H
#define OSRPG_PHYS_WORLD_H

/// \defgroup Physics
/// @{

/// World physics representation

#include <ode/ode.h>

struct PhysWorld {
	/// Create world
	
	/// \param gr Gravity (must be negative to make things fall)
	///
	void Create (float gr);
	/// Destory world
	void Destroy ();
	
	/// Main world
	dWorldID world;
	/// Main space
	dSpaceID space;
	/// Contact group (used for contact joints)
	dJointGroupID contact_group;
};

/// @}
#endif // OSRPG_PHYS_WORLD_H

