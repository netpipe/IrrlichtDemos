// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Camera
#ifndef CAMERA_H
#define CAMERA_H

#include "settings.h"
#include "help.h"

class Camera {
	public:
		/// Constructor
		Camera ();
		/// Create camera

		/// \param smgr Scene manager
		/// \param parent Parent scene node
		void Create (ISceneManager *smgr, ISceneNode *parent = 0);
		/// Save position in private members

		/// Used when changing screen parameters
		///
		void SavePosition ();
		/// Set position

		/// \param pos New position
		///
		void SetPosition (core::vector3df pos);
		/// Set target

		/// \param target New target
		///
		void SetTarget (core::vector3df target);
		/// Print current camera position and target
		void PrintCurrentPos ();
		/// Get camera force vector
		core::vector3df GetForceVec ();
		/// Get camera position
		core::vector3df GetPosition ();
		/// Get camera target
		core::vector3df GetTarget ();
		/// Get camera
		ICameraSceneNode* GetCamera ();
		/// Restore previous position 
		void RestorePos ();
		/// Get XZ proportion
		float GetXZProportion ();

	private:
		core::vector3df pos,
			 target;
		ICameraSceneNode *cam;
};

#endif // CAMERA_H

