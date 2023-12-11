// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_WRAP_SPHERE_H
#define OSRPG_WRAP_SPHERE_H

/// \ingroup Wrapper
///@{

/// Wrapper around sphere physics and graphics representations

#include "../phys/sphere.h"
#include "sm_entity.h"

class WrapSphere: public WrapSMEntity {
	public:
		/// Constructor

		/// \param world World in which to create sphere body
		/// \param space Space in which to create sphere geom
		/// \param pos Sphere initial position
		/// \param mass Sphere mass
		/// \param rad Sphere radius
		/// \param lvel Linear velocity threshold
		/// \param avel Angular velocity threshold
		/// \param steps Amount of disable steps
		/// \param dev Irrlicht device
		/// \param tex_path Path to a texture
		WrapSphere (const dWorldID world, const dSpaceID space,
				const irr::core::vector3df pos, const float mass,
				const float rad, const float lvel, const float avel,
				const unsigned short steps, irr::IrrlichtDevice *dev,
				const irr::c8 *tex_path);
		/// Destructor
		~WrapSphere ();
		/// Create node

		/// \param dev Irrlicht device
		///
		void CreateNode (irr::IrrlichtDevice *dev);

	private:
		PhysSphere *ps;
		float radius;
};

///@}

#endif // OSRPG_WRAP_SPHERE_H

