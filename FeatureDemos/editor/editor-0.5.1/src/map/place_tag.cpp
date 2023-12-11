// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "place_tag.h"
using namespace irr;
using namespace core;
using namespace video;

MapPlaceTag::MapPlaceTag (Registry *reg, const char *name, const vector3df pos) {
	MapPlaceTag::reg = reg;
	node = 0;
	memset (&pt, 0, sizeof (OMFPlaceTag));

	id = reg->GetFreeID ();
	node = reg->smgr->addCubeSceneNode (5.0f, 0, id, pos);
	node->setMaterialType (EMT_TRANSPARENT_ADD_COLOR);

	Edit (name, pos);
}

MapPlaceTag::MapPlaceTag (Registry *reg, const OMFPlaceTag pt) {
	MapPlaceTag (reg, pt.name, vector3df (pt.pos.a, pt.pos.b, pt.pos.c));
}

MapPlaceTag::~MapPlaceTag () {

}

void MapPlaceTag::Edit (const char *name, const vector3df pos) {
	SetName (name);
	SetPosition (pos);
}

OMFPlaceTag MapPlaceTag::GetCopy () const {
	return pt;
}

char MapPlaceTag::GetType () const {
	return 3; // PlaceTag
}

void MapPlaceTag::SetName (const char *name) {
	strcpy (pt.name, name);
	node->setName (name);
}

void MapPlaceTag::SetPosition (const vector3df pos) {
	pt.pos.a = pos.X;
	pt.pos.b = pos.Y;
	pt.pos.c = pos.Z;
	node->setPosition (pos);
}

