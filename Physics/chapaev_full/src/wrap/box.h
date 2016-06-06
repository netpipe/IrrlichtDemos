// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_WRAP_BOX_H
#define OSRPG_WRAP_BOX_H

/// \ingroup Wrapper
///@{

/// Wrapper around box physics and graphics representations

#include "../phys/box.h"
#include "sm_entity.h"

class WrapBox: public WrapSMEntity {
	public:
		/// Constructor

		/// \param world World in which to create box body
		/// \param space Space in which to create box geom
		/// \param pos Box initial position
		/// \param mass Box mass
		/// \param sz Box size
		/// \param lvel Linear velocity threshold
		/// \param avel Angular velocity threshold
		/// \param steps Amount of disable steps
		/// \param dev Irrlicht device
		/// \param tex_path Path to a texture
		WrapBox (const dWorldID world, const dSpaceID space,
				const irr::core::vector3df pos, const float mass,
				const float sz, const float lvel, const float avel,
				const unsigned short steps, irr::IrrlichtDevice *dev,
				const irr::c8 *tex_path);
		/// Destructor
		~WrapBox ();
		/// Create node

		/// \param dev Irrlicht device
		///
		void CreateNode (irr::IrrlichtDevice *dev);

	private:
		PhysBox *pb;
		float size;
};

#endif // OSRPG_WRAP_BOX_H

