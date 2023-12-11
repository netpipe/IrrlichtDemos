// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_WRAP_SM_ENTITY_H
#define OSRPG_WRAP_SM_ENTITY_H

/// \defgroup Wrapper
///@{

/// Wrapper around single-mesh physics and graphics representations

#include "../phys/sm_entity.h"

class WrapSMEntity {
	public:
		/// Get user-data pointer

		/// \return User-data pointer
		///
		void* GetData ();
		/// Get position
		
		/// \return Position
		///
		const dReal* GetPosition () const;
		/// Update entity graphics representation and check for disabling
		void Update ();
		/// Shoot

		/// \param pos Start position
		/// \param dir Direction vector
		/// \param force Amount of applied force
		void Shoot (const irr::core::vector3df pos,
				const irr::core::vector3df dir, const float force);
		/// Is entity enabled?

		/// \return Enable state
		///
		bool IsEnabled () const;
		/// Set texture

		/// \param dev Irrlicht device
		///
		void SetTexture (irr::IrrlichtDevice *dev);
		/// Set user-data pointer

		/// \param data User-data pointer
		///
		void SetData (void *data);

	protected:
		/// Graphics representation
		irr::scene::ISceneNode *sn;
		/// Physics representation
		PhysSMEntity *pe;
		/// Stored path to a texture
		irr::core::stringc tex_path;
};

///@}

#endif // OSRPG_WRAP_SM_ENTITY_H

