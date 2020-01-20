// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "place_tag.h"
#include <string.h>

MapPlaceTag::MapPlaceTag (const OMFPlaceTag p) {
	Edit (p);
}

MapPlaceTag::~MapPlaceTag () {

}

void MapPlaceTag::Edit (const OMFPlaceTag p) {
	strcpy (pt.name, p.name);
	pt.pos = p.pos;
}

OMFPlaceTag MapPlaceTag::GetCopy () const {
	return pt;
}

