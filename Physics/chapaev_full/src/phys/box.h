// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_PHYS_BOX_H
#define OSRPG_PHYS_BOX_H

/// \ingroup Physics
///@{

/// Box physics representation

#include "sm_entity.h"

class PhysBox: public PhysSMEntity {
	public:
		/// Constructor

		/// \param world World in which to create box body
		/// \param space Space in which to create box geom
		/// \param pos Initial box position
		/// \param mass Box mass
		/// \param size Box size
		PhysBox (const dWorldID world, const dSpaceID space,
				const irr::core::vector3df pos, const float mass,
				const float size);
};

///@}

#endif // OSRPG_PHYS_BOX_H

