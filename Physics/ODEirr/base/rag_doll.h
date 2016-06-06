// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Wrapper around physics and graphics rag doll representations
#ifndef BASE_RAG_DOLL_H
#define BASE_RAG_DOLL_H

#include "../settings.h"
#include "../help.h"
#include "../physics/rag_doll.h"
#include "../node/mesh_node.h"

class BaseRagDoll {
	public:
		/// Constructor
		BaseRagDoll ();
		/// Destructor
		~BaseRagDoll ();
		/// Create rag doll

		/// \param world World in which we create rag doll
		/// \param space Space in which rag doll will be affected by collision
		/// \param x Starting X rag doll position
		/// \param y Starting Y rag doll position
		/// \param z Starting Z rag doll position
		/// \param smgr Scene manager
		void Create (dWorldID world, dSpaceID space, float x, float y,
				float z, ISceneManager *smgr);
		/// Create rag doll node
		void CreateNode (ISceneManager *smgr);
		/// Update rag doll

		/// Update rag doll node position and rotation
		///
		void Update (bool run);
	
	private:
		void Destroy ();

		PhysicsRagDoll *phys_rd;
		MeshNode *leg_node[4];
		MeshNode *hand_node[4];
		MeshNode *body_node;
};

#endif // BASE_RAG_DOLL_H

