// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Wrapper around graphics camera attachment representation
#ifndef BASE_CAM_ATTACHMENT_H
#define BASE_CAM_ATTACHMENT_H

#include "../settings.h"
#include "../camera.h"
#include "../node/mesh_node.h"

class BaseCamAttachment {
	public:
		/// Constructor
		BaseCamAttachment ();
		/// Destructor
		~BaseCamAttachment ();
		/// Create hand and weapon

		/// \param dev Irrlicht device
		/// \param cam Camera (not irr's)
		/// \param model_path Path to a model
		/// \param tex_path Path to a texture (this is necessary only for ms3d models
		/// since ms3d models are loaded without texture by default (bug?)
		/// \param pos Position
		/// \param rot Rotation
		void Create (IrrlichtDevice *dev, Camera *cam, const c8 *model_path,
				const c8 *tex_path, core::vector3df pos, core::vector3df rot);
		/// Get mesh node
		MeshNode* GetMeshNode ();

	private:
		MeshNode *ca_mn;
};

#endif // BASE_CAM_ATTACHMENT_H

