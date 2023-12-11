// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_PHYS_SPHERE_H
#define OSRPG_PHYS_SPHERE_H

/// \ingroup Physics
///@{

/// Sphere physics representation

#include "sm_entity.h"

class PhysSphere: public PhysSMEntity {
	public:
		/// Constructor

		/// \param world World in which to create sphere body
		/// \param space Space in which to create sphere geom
		/// \param pos Initial sphere position
		/// \param mass Sphere mass
		/// \param rad Sphere radius
		PhysSphere (const dWorldID world, const dSpaceID space,
				const irr::core::vector3df pos, const float mass,
				const float rad);
};

///@}

#endif // OSRPG_PHYS_SPHERE_H

