// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "light.h"
using namespace irr;
using namespace core;
using namespace video;

MapLight::MapLight (Registry *reg, const char *name, const vector3df pos,
		const float rad, const SColorf col) {
	MapLight::reg = reg;
	ln = 0;
	node = 0;
	memset (&light, 0, sizeof (OMFLight));

	// Create light and sphere scene nodes
	ln = reg->smgr->addLightSceneNode (0, pos, col, rad);
	id = reg->GetFreeID ();
	node = reg->smgr->addSphereSceneNode (1.0f, 16, 0, id, pos);
	node->setMaterialFlag (EMF_LIGHTING, false);
	node->setMaterialType (EMT_TRANSPARENT_ADD_COLOR);
	//node->setMaterialType (EMT_SOLID);
	//node->setMaterialFlag (EMF_WIREFRAME, true);
	// Texture node with a "light" texture
	Select (false);

	Edit (name, pos, rad, col);
}

MapLight::MapLight (Registry *reg, const OMFLight l) {
	MapLight (reg, l.name, vector3df (l.pos.a, l.pos.b, l.pos.c), l.radius,
			SColorf (l.color.a, l.color.b, l.color.c, l.color.d));
}

MapLight::~MapLight () {
	if (ln)
		ln->remove ();
}

void MapLight::Edit (const char *name, const vector3df pos,
		const float rad, const SColorf col) {
	SetName (name);
	SetPosition (pos);
	SetRadius (rad);
	SetColor (col);
}

OMFLight MapLight::GetCopy () const {
	return light;
}

char MapLight::GetType () const {
	return 1; // Light
}

void MapLight::Select (bool state) {
	stringc path;
	if (state) {
		path= reg->settings->GetDataDir () + "textures/light.png";
	}
	else {
		path= reg->settings->GetDataDir () + "textures/particlewhite.bmp";
	}
	node->setMaterialTexture (0, reg->drv->getTexture (path.c_str ()));
}

void MapLight::SetName (const char *name) {
	strcpy (light.name, name);
	node->setName (name);
}

void MapLight::SetPosition (const vector3df pos) {
	light.pos.a = pos.X;
	light.pos.b = pos.Y;
	light.pos.c = pos.Z;
	node->setPosition (pos);
	ln->setPosition (pos);
}

void MapLight::SetRadius (const float rad) {
	if (light.radius == rad)
		return;
	node->setScale (vector3df (rad, rad, rad));
	ln->getLightData ().Radius = rad;
	light.radius = rad;
}

void MapLight::SetColor (const SColorf col) {
	light.color.a = col.r;
	light.color.b = col.g;
	light.color.c = col.b;
	light.color.d = col.a;
	ln->getLightData ().DiffuseColor = col;
}

