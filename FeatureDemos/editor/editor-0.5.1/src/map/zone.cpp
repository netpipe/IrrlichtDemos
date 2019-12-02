// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "zone.h"
using namespace irr;
using namespace core;
using namespace video;

MapZone::MapZone (Registry *reg, const char *name, const char *model_file,
		const char *script_name, const vector3df pos, const vector3df rot) {
	MapZone::reg = reg;
	node = 0;
	memset (&zone, 0, sizeof (OMFZone));

	Edit (name, model_file, script_name, pos, rot);
}

MapZone::MapZone (Registry *reg, const OMFZone z) {
	MapZone (reg, z.name, z.model_file, z.script_name, vector3df (z.pos.a,
				z.pos.b, z.pos.c), vector3df (z.rot.a, z.rot.b, z.rot.c));
}

void MapZone::Edit (const char *name, const char *model_file,
		const char *script_name, const vector3df pos, const vector3df rot) {
	SetModelFile (model_file);
	SetName (name);
	SetPosition (pos);
	SetRotation (rot);
	SetScriptName (script_name);
}

OMFZone MapZone::GetCopy () const {
	return zone;
}

char MapZone::GetType () const {
	return 2; // Zone
}

void MapZone::SetModelFile (const char *model_file) {
	if (strlen (model_file) < 4)
		return;
	// If we set the same model file, no need to change anything
	if (strcmp (zone.model_file, model_file) == 0)
		return;
	// If scene node already exists, remove it
	if (node)
		node->remove ();
	// If we create node, get free id
	else
		id = reg->GetFreeID ();
	vector3df pos (zone.pos.a, zone.pos.b, zone.pos.c);
	vector3df rot (zone.rot.a, zone.rot.b, zone.rot.c);
	// Create node
	// If we recreate node, we keep the same id
	stringc path = reg->settings->GetDataDir () + "models/" +
		model_file;
	node = reg->smgr->addAnimatedMeshSceneNode (reg->smgr->getMesh (
				path.c_str ()), 0, id, pos, rot);
	node->setMaterialType (EMT_TRANSPARENT_ADD_COLOR);
	strcpy (zone.model_file, model_file);
}

void MapZone::SetName (const char *name) {
	strcpy (zone.name, name);
	node->setName (name);
}

void MapZone::SetPosition (const irr::core::vector3df pos) {
	zone.pos.a = pos.X;
	zone.pos.b = pos.Y;
	zone.pos.c = pos.Z;
	node->setPosition (pos);
}

void MapZone::SetRotation (const irr::core::vector3df rot) {
	zone.rot.a = rot.X;
	zone.rot.b = rot.Y;
	zone.rot.c = rot.Z;
	node->setRotation (rot);
}

void MapZone::SetScriptName (const char *script_name) {
	strcpy (zone.script_name, script_name);
}

