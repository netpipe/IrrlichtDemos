// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "light.h"
using namespace irr;
using namespace core;
using namespace video;

MapLight::MapLight (Registry *rg, const OMFLight l) {
	reg = rg;
	ln = 0;
	light = l;
	CreateNode ();
}

MapLight::~MapLight () {
	if (ln) {
		ln->remove ();
		ln = 0;
	}
}

void MapLight::CreateNode () {
	ln = reg->smgr->addLightSceneNode ();
	Edit (light);
}

void MapLight::Edit (const OMFLight l) {
	ln->getLightData ().Position = vector3df (l.pos.a, l.pos.b, l.pos.c);
	ln->getLightData ().Radius = l.radius;
	ln->getLightData ().DiffuseColor = SColorf (l.color.a, l.color.b,
			l.color.c, l.color.d);
	strcpy (light.name, l.name);
	light.pos = l.pos;
	light.radius = l.radius;
	light.color = l.color;
}

OMFLight MapLight::GetCopy () const {
	return light;
}

