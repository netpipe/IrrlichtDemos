// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "rag_doll.h"

BaseRagDoll::BaseRagDoll () {

}

BaseRagDoll::~BaseRagDoll () {
	Destroy ();
}

void BaseRagDoll::Create (dWorldID world, dSpaceID space, float x, float y,
		float z, ISceneManager *smgr) {
	phys_rd = new PhysicsRagDoll ();
	CreateNode (smgr);
	phys_rd->CreateRagDoll (world, space, x, y, z,
			body_node->GetAnimatedMesh ());
	phys_rd->SetDisableParams (0.05, 0.05, 100);
}

void BaseRagDoll::Destroy () {
	phys_rd->DestroyRagDoll ();
}

void BaseRagDoll::CreateNode (ISceneManager *smgr) {
	for (int i = 0; i < 4; i++) {
		leg_node[i] = new MeshNode (smgr, "_data/models/paral.obj");
		hand_node[i] = new MeshNode (smgr, "_data/models/paral.obj");
	}
	body_node = new MeshNode (smgr, "_data/models/body.obj");
}

void BaseRagDoll::Update (bool run) {
	phys_rd->DisableCheck (run);
	for (int i = 0; i < 4; i++) {
		UpdateObject (phys_rd->hand_geom[i], hand_node[i]->GetSceneNode ());
		UpdateObject (phys_rd->leg_geom[i], leg_node[i]->GetSceneNode ());
	}
	UpdateObject (phys_rd->body_geom, body_node->GetSceneNode ());
}

