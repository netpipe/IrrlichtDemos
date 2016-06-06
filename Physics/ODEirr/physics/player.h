// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Physics representation of a player
#ifndef PHYSICS_PLAYER_H
#define PHYSICS_PLAYER_H

#include "../help.h"
#include "../settings.h"

//#define DEBUG

class PhysicsPlayer {
	public:
		/// Constructor
		PhysicsPlayer ();
		/// Destructor
		~PhysicsPlayer ();
		/// Create player

		/// \param world World in which we create player
		/// \param space Space in which player will be affected by collision
		/// \param x Starting X box position
		/// \param y Starting Y box position
		/// \param z Starting Z box position
		/// \param mass Mass of a player
		void CreatePlayer (dWorldID world, dSpaceID space, float x, float y,
				float z, float mass);
		/// Destroy player
		void DestroyPlayer ();
		/// Get player position
		core::vector3df GetPosition ();
		/// Shoot the player

		/// \param force_vec Force vector
		/// \param force Amount of applied force
		void Shoot (core::vector3df force_vec, float force);

		/// Player body
		dBodyID player_body;
		/// Player geom
		dGeomID player_geom;
		/// Player ray

		/// Ray tells us how high we are from below object so that we can keep
		/// constant hight above below objects. This is necessary to
		/// climp up stairs
		dGeomID player_ray;

	private:
		dMass m;
};

#endif // PHYSICS_PLAYER_H

