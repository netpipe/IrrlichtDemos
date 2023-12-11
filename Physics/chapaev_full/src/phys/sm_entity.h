// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_PHYS_SM_ENTITY_H
#define OSRPG_PHYS_SM_ENTITY_H

/// \ingroup Physics
/// @{

/// Single-mesh physics entity

#include <ode/ode.h>
#include <irrlicht.h>

class PhysSMEntity {
	public:
		/// Destructor
		virtual ~PhysSMEntity ();
		/// Check if body needs to be disabled
		void DisableCheck ();
		/// Get body

		/// \return Body
		///
		dBodyID GetBody () const;
		/// Get geom
		
		/// \return Geom
		///
		dGeomID GetGeom () const;
		/// Set disable parameters

		/// Upon reaching these parameters body will be disabled
		/// \param lvel Linear velocity threshold
		/// \param avel Angular velocity threshold
		/// \param steps Amount of steps
		void SetDisableParams (const float lvel, const float avel,
				const unsigned short steps);
		/// Shoot entity

		/// \param pos Start position
		/// \param dir Direction vector
		/// \param force Amount of applied force
		/// \n TODO: Modify so that force acts not only through a center
		/// but also accepts force application position
		void Shoot (const irr::core::vector3df pos,
				const irr::core::vector3df dir, const float force);

	protected:
		/// Disable body
		void Disable ();

		/// Body
		dBodyID body;
		/// Geom
		dGeomID geom;
		/// Mass
		dMass m;
		/// Current amount of disable steps
		unsigned short ds_cur,
					   /// Given amount of disable steps
					   ds_given;
		/// Disable linear velocity
		float dlvel,
			  /// Disable angular velocity
			  davel;
};

///@}

#endif // OSRPG_PHYS_SM_ENTITY_H

