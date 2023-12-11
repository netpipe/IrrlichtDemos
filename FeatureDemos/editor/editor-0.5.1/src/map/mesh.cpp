// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "mesh.h"
using namespace irr;
using namespace core;

MapMesh::MapMesh (Registry *reg, const char *name, const char *script_name,
		const char *model_file, const char *collision_model_file,
		const vector3df pos, const vector3df rot, const char maggun_effect,
		const float mass, bool collidable, bool optimize) {
	MapMesh::reg = reg;
	node = 0;
	memset (&mesh, 0, sizeof (OMFMesh));

	Edit (name, script_name, model_file, collision_model_file, pos, rot,
			maggun_effect, mass, collidable, optimize);
}

MapMesh::MapMesh (Registry *reg, const OMFMesh m) {
	MapMesh (reg, m.name, m.script_name, m.model_file,
			m.collision_model_file, vector3df (m.pos.a, m.pos.b, m.pos.c),
			vector3df (m.rot.a, m.rot.b, m.rot.c), m.maggun_effect,
			m.mass, m.collidable, m.optimize);
}

void MapMesh::Edit (const char *name, const char *script_name,
		const char *model_file, const char *collision_model_file,
		const vector3df pos, const vector3df rot, const char maggun_effect,
		const float mass, bool collidable, bool optimize) {
	SetModelFile (model_file, optimize);
	SetCollisionModelFile (collision_model_file);
	SetName (name);
	SetScriptName (script_name);
	SetPosition (pos);
	SetRotation (rot);
	SetMaggunEffect (maggun_effect);
	SetMass (mass);
	SetCollidable (collidable);
}

OMFMesh MapMesh::GetCopy () const {
	return mesh;
}

vector3df MapMesh::GetPosition () const {
	return node->getPosition ();
}

char MapMesh::GetType () const {
	return 0; // Mesh
}

void MapMesh::SetCollidable (bool state) {
	mesh.collidable = state;
}

void MapMesh::SetCollisionModelFile (const char *cmf) {
	strcpy (mesh.collision_model_file, cmf);
}

void MapMesh::SetMaggunEffect (const char maggun_effect) {
	mesh.maggun_effect = maggun_effect;
}

void MapMesh::SetMass (const float mass) {
	mesh.mass = mass;
}

void MapMesh::SetModelFile (const char *model_file, bool optimize) {
	if (strlen (model_file) < 4)
		return;
	// If we set the same model file and don't change
	// omptimization level, no need to change anything
	if (strcmp (mesh.model_file, model_file) == 0 &&
			mesh.optimize == optimize) {
		return;
	}
	// If scene node already exists, remove it
	if (node)
		node->remove ();
	// If we create node, get free id
	else
		id = reg->GetFreeID ();
	vector3df pos (mesh.pos.a, mesh.pos.b, mesh.pos.c);
	vector3df rot (mesh.rot.a, mesh.rot.b, mesh.rot.c);
	// Create node
	// If we recreate node, we keep the same id
	stringc path = reg->settings->GetDataDir () + "models/" +
		model_file;
	// Optimization is good only for huge meshes, like maps
	if (!optimize)
		node = reg->smgr->addAnimatedMeshSceneNode (reg->smgr->getMesh (
					path.c_str ()), 0, id, pos, rot);
	else {
		node = reg->smgr->addOctTreeSceneNode (reg->smgr->getMesh (
					path.c_str ()), 0, id);
		node->setPosition (pos);
		node->setRotation (rot);
	}
	strcpy (mesh.model_file, model_file);
	mesh.optimize = optimize;
}

void MapMesh::SetName (const char *name) {
	strcpy (mesh.name, name);
	node->setName (name);
}

void MapMesh::SetPosition (const irr::core::vector3df pos) {
	mesh.pos.a = pos.X;
	mesh.pos.b = pos.Y;
	mesh.pos.c = pos.Z;
	node->setPosition (pos);
}

void MapMesh::SetRotation (const irr::core::vector3df rot) {
	mesh.rot.a = rot.X;
	mesh.rot.b = rot.Y;
	mesh.rot.c = rot.Z;
	node->setRotation (rot);
}

void MapMesh::SetScriptName (const char *s) {
	strcpy (mesh.script_name, s);
}

