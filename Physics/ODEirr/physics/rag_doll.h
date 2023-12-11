// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Physics representation of a rag doll constructed of parallelepipeds

#ifndef PHYSICS_RAG_DOLL_H
#define PHYSICS_RAG_DOLL_H

#include "../help.h"

//#define DEBUG

class PhysicsRagDoll {
	public:
		/// Constructor
		PhysicsRagDoll ();
		/// Destructor
		~PhysicsRagDoll ();
		/// Create rag doll

		/// \param world World in which we create rag doll
		/// \param space Space in which rag doll will be affected by collision
		/// \param x Starting X position
		/// \param y Starting Y position
		/// \param z Starting Z position
		/// \param mesh Body mesh
		void CreateRagDoll (dWorldID world, dSpaceID space, float x, float y,
				float z, IAnimatedMesh* mesh);
		/// Destroy rag doll
		void DestroyRagDoll ();
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
		//void Reanimate ();
		
		/// Rag doll leg bodies
		dBodyID leg_body[4];
		/// Rag doll leg geoms
		dGeomID leg_geom[4];
		/// Rag doll knee joints
		dJointID knee_joint[2];

		/// Rag doll body body (that's not a mistake)
		dBodyID body_body;
		/// Rag doll body geom
		dGeomID body_geom;
		/// Rag doll hip joints
		dJointID hip_joint[2];
		/// Rag doll shoulder joints
		dJointID shoulder_joint[2];

		/// Rag doll hand bodies
		dBodyID hand_body[4];
		/// Rag doll hand geoms
		dGeomID hand_geom[4];
		/// Rag doll elbow joints
		dJointID elbow_joint[2];

		/*
		dBodyID head_body;
		dGeomID head_geom;
		dJointID neck_joint;
		*/

	private:
		void CreateLegs (dWorldID world, dSpaceID space);
		void CreateBody (dWorldID world, dSpaceID space, IAnimatedMesh *mesh);
		void CreateHands (dWorldID world, dSpaceID space);
		//void CreateHead (dWorldID world, dSpaceID space);
		void DisableRagDoll ();
		//void CreateRagDollSpace (dSpaceID space);

		int dis_steps_cur,
			dis_steps_given;
		float dis_avel,
			  dis_lvel;
		dMass m;
		dSpaceID rag_doll_space;
};


#endif // PHYSICS_RAG_DOLL_H

