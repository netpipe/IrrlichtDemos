// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_WRAP_MAP_H
#define OSRPG_WRAP_MAP_H

/// \ingroup Wrapper
///@{

/// Wrapper around map physics and graphics representations

#include <ode/ode.h>
#include <irrlicht.h>

class WrapMap {
	public:
		/// Constructor

		/// \param space Space in which to create map geom
		/// \param dev Irrlicht device
		/// \param pos Position
		/// \param map_path Path to a map
		/// \param map_name Map name (use only when you load Quake3 maps)
		WrapMap (const dSpaceID space, irr::IrrlichtDevice *dev,
				const irr::core::vector3df pos, const char *map_path,
				const char *map_name = 0);
		/// Destructor
		~WrapMap ();
		/// Create node

		/// \param dev Irrlicht device
		///
		void CreateNode (irr::IrrlichtDevice *dev);

	private:
		irr::scene::IAnimatedMesh *am;
		irr::scene::ISceneNode *sn;
		dGeomID map;
		irr::core::vector3df pos;
		irr::core::stringc path,
			name;
};

///@}

#endif // OSRPG_WRAP_MAP_H

