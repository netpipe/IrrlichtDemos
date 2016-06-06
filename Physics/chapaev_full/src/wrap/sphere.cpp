// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "sphere.h"
using namespace irr;
using namespace core;
using namespace scene;

WrapSphere::WrapSphere (const dWorldID world, const dSpaceID space,
		const vector3df pos, const float mass, const float rad,
		const float lvel, const float avel, const unsigned short steps,
		IrrlichtDevice *dev, const c8 *tex_path) {
	radius = rad;
	WrapSphere::tex_path = stringc (tex_path);
	CreateNode (dev);
	ps = new PhysSphere (world, space, pos, mass, rad);
	pe = ps;
	ps->SetDisableParams (lvel, avel, steps);
}

WrapSphere::~WrapSphere () {
	delete ps;
	pe = 0;
	ps = 0;
}

void WrapSphere::CreateNode (IrrlichtDevice *dev) {
	sn = dev->getSceneManager ()->addSphereSceneNode (radius);
	SetTexture (dev);
}

