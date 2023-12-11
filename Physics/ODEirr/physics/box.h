// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Physics represenation of a box
#ifndef PHYSICS_BOX_H
#define PHYSICS_BOX_H

#include "../help.h"

//#define DEBUG

class PhysicsBox {
	public:
		/// Constructor
		PhysicsBox ();
		/// Destructor
		~PhysicsBox ();
		/// Create physics box

		/// \param world World in which we create box
		/// \param space Space in which box will be affected by collision
		/// \param x Starting X box position
		/// \param y Starting Y box position
		/// \param z Starting Z box position
		/// \param size Size of a box
		/// \param mass Mass of a box
		void CreateBox (dWorldID world, dSpaceID space, float x, float y,
				float z, float size, float mass);
		/// Destroy the box
		void DestroyBox ();
		/// Set disable parameters

		/// Set paramaters when the box should be disabled not to consume
		/// unnecessary CPU power
		/// \param lvel Linear velocity threshold
		/// \param avel Angular velocity threshold
		/// \param steps When linear and angular velocities satisfy disable
		/// thresholds for this amount of steps, we will disable body
		void SetDisableParams (float lvel, float avel, int steps);
		/// Check if we need to disable the body

		/// \param run Do we simulate physics?
		///
		void DisableCheck (bool run);
		/// Reset disable steps
		void ResetDisSteps ();
		/// Get disable steps
		int GetDisSteps ();
		/// Shoot the box

		/// We apply force in the direction of a camera
		/// \param cam_pos Camera position
		/// \param cam_target Camera target
		/// \param force Amount of applied force
		void Shoot (core::vector3df cam_pos, core::vector3df cam_target,
				float force);

		/// Box body
		dBodyID box_body;
		/// Box geom
		dGeomID box_geom;
	
	private:
		void DisableBox ();

		int dis_steps_cur,
			dis_steps_given;
		float dis_lvel,
			  dis_avel,
			  size,
			  mass;
		dMass m;
};

#endif // PHYSICS_BOX_H

