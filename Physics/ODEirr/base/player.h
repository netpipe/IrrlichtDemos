// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Wrapper around physics player representation
#ifndef BASE_PLAYER_H
#define BASE_PLAYER_H

#include "../settings.h"
#include "../physics/player.h"
//#include "../node/mesh_node.h"

class BasePlayer {
	public:
		/// Constructor
		BasePlayer ();
		/// Destructor
		~BasePlayer ();
		/// Create player

		/// \param world World in which we create player
		/// \param space Space in which player will be affected by collision
		/// \param x Starting X player position
		/// \param y Starting Y player position
		/// \param z Starting Z player position
		/// \param mass Mass of a player
		// \param smgr Scene manager
		void Create (dWorldID world, dSpaceID space, float x, float y,
				float z, float mass/*, ISceneManager *smgr*/);
		// Create player node
		//void CreateNode (ISceneManager *smgr);
		/// Update player

		/// Update player node position and rotation
		///
		void Update (bool run);
		/// Get physics player
		PhysicsPlayer *GetPhysicsPlayer ();
		// Get scene node
		//ISceneNode* GetSceneNode ();
		/// Get position
		core::vector3df GetPosition ();
		/// Shoot the player

		/// \param force_vec Force vector
		/// \param force Amount of applied force
		void Shoot (core::vector3df force_vec, float force);

	private:
		void Destroy ();

		PhysicsPlayer *phys_pl;
		//MeshNode *capsule_node;
};

#endif // BASE_PLAYER_H

