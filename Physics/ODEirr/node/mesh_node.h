// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Constructs scene node from a loaded mesh
#ifndef NODE_MESH_NODE_H
#define NODE_MESH_NODE_H

#include "../globals.h"
#include "../help.h"

class MeshNode {
	public:
		/// Constructor

		/// \param smgr Scene manager
		/// \param model_path Path to a model
		MeshNode (ISceneManager *smgr, const c8 *model_path);
		/// Set node position

		/// \param pos New node position
		///
		void SetPosition (core::vector3df pos);
		/// Set node rotation in Euler angles

		/// \param rot New node rotation in Euler angles
		///
		void SetRotation (core::vector3df rot);
		/// Get node rotation in Euler angles
		core::vector3df GetRotation ();
		/// Get node position
		core::vector3df GetPosition ();
		/// Get mesh

		/// We'll need this for ODE to create proper collision
		///
		IAnimatedMesh* GetAnimatedMesh ();
		/// Get scene node

		/// We'll need this in ObjectUpdate (help.h)
		///
		ISceneNode* GetSceneNode ();
	
	private:
		IAnimatedMesh *mesh;
		IAnimatedMeshSceneNode *node;
};

#endif // NODE_MESH_NODE_H

