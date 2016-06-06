// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "mesh_node.h"

MeshNode::MeshNode (ISceneManager *smgr, const c8 *model_path) {
	// Get the mesh
	//cout << "\n\n1\n";
	mesh = smgr->getMesh (model_path);
	// Create scene node for the mesh
	//cout << "\n\n2\n";
	node = smgr->addAnimatedMeshSceneNode (mesh);
	// Textures must be seen when dynamic lighting is off
	//cout << "\n\n3\n";
	node->setMaterialFlag (EMF_LIGHTING, false);
}

void MeshNode::SetPosition (core::vector3df pos) {
	node->setPosition (pos);
}

void MeshNode::SetRotation (core::vector3df rot) {
	node->setRotation (rot);
}

core::vector3df MeshNode::GetRotation () {
	return node->getRotation ();
}

core::vector3df MeshNode::GetPosition () {
	return node->getPosition ();
}

IAnimatedMesh* MeshNode::GetAnimatedMesh () {
	return mesh;
}

ISceneNode* MeshNode::GetSceneNode () {
	return node;
}

