// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// \file basic.h
/// \brief Basic physics objects and functions
///
/// All at global scope, which is not good at all, I think
///
#ifndef PHYSICS_BASIC_H
#define PHYSICS_BASIC_H

#include "../settings.h"

/// Main world
extern dWorldID world;
/// Main collision space
extern dSpaceID space;
/// Contact joints group
extern dJointGroupID contact_group;
/// Geom which represents map
extern dGeomID map;

/// Create main world
void CreateWorld ();
/// Destroy main world
void DestroyWorld ();

#endif // PHYSICS_BASIC_H

