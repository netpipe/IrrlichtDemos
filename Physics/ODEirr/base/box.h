// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Wrapper around physics and graphics box representations
#ifndef BASE_BOX_H
#define BASE_BOX_H

#include "../physics/box.h"

class BaseBox {
	public:
		/// Constructor
		BaseBox ();
		/// Destructor
		~BaseBox ();
		/// Create box

		/// \param world World in which we create box
		/// \param space Space in which box will be affected by collision
		/// \param x Starting X box position
		/// \param y Starting Y box position
		/// \param z Starting Z box position
		/// \param size Size of a box
		/// \param mass Mass of a box
		/// \param dev Irrlicht device
		/// \param tex_path Path to a texture
		void Create (dWorldID world, dSpaceID space, float x, float y,
				float z, float size, float mass, IrrlichtDevice *dev,
				c8* tex_path);
		/// Create box node

		/// We use separate node creation function to be able
		/// to change screen parameters without application restart:
		/// we rebuild all Irrlicht related things, but keep physics
		/// intact
		/// \param dev Irrlicht device
		/// \param tex_path Path to a texture
		void CreateNode (IrrlichtDevice *dev, c8* tex_path);
		/// Update box

		/// Update box node position and rotation
		///
		void Update (bool run);
		/// Shoot the box

		/// We apply force in the direction of a camera
		/// \param cam_pos Camera position
		/// \param cam_target Camera target
		/// \param force Amount of applied force
		void Shoot (core::vector3df cam_pos, core::vector3df cam_target, float force);
		/// Check if the box is enabled
		bool IsEnabled ();
		/// Get disable steps
		int GetDisSteps ();
		/// Set box texture

		/// \param dev Irrlicht device
		/// \param tex_path Path to a texture
		void SetTexture (IrrlichtDevice *dev, c8 *tex_path);

	private:
		void Destroy ();

		ISceneNode *box_node;
		PhysicsBox *phys_box;
		float size,
			  mass;
};

#endif // BASE_BOX_H

