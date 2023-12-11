// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "item.h"
using namespace irr;
using namespace video;

MapItem::~MapItem () {
	if (node) {
		node->remove ();
		reg->AddFreedID (id);
	}
}

unsigned int MapItem::GetID () const {
	return id;
}

void MapItem::Select (bool state) {
	node->setMaterialFlag (EMF_LIGHTING, !state);
}

