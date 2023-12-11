// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Wrapper around physics and graphics map representations
#ifndef BASE_MAP_H
#define BASE_MAP_H

#include "../settings.h"
#include "../help.h"
#include "../physics/basic.h"

class BaseMap {
	public:
		/// Constructor
		BaseMap ();
		/// Destructor
		~BaseMap ();
		/// Create map

		/// We don't need world here, because we create only geom (collision).
		/// We don't create body for the map not to be affected by gravity.
		/// \param space Space in which map will be affected by collision
		/// \param x X map position
		/// \param y Y map position
		/// \param z Z map position
		/// \param dev Irrlicht device
		void Create (dSpaceID space, float x, float y, float z,
				IrrlichtDevice *dev);
		/// Create map node

		/// \param dev Irrlicht device
		///
		void CreateNode (IrrlichtDevice *dev);

	private:
		void Destroy ();

		IAnimatedMesh *map_mesh;
		ISceneNode *map_node;
};

#endif // BASE_MAP_H

