// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "box.h"
using namespace irr;
using namespace core;
using namespace scene;

WrapBox::WrapBox (const dWorldID world, const dSpaceID space,
		const vector3df pos, const float mass, const float sz,
		const float lvel, const float avel,	const unsigned short steps,
		irr::IrrlichtDevice *dev, const c8 *tex_path) {
	size = sz;
	WrapBox::tex_path = stringc (tex_path);
	// We create node before physics, since when using arbitrary shape
	// we build ODE trimesh from node's IMesh
	CreateNode (dev);
	pb = new PhysBox (world, space, pos, mass, sz);
	// We use this ugly way so that we don't need to override Update,
	// Shoot and other functions for each class derived from WrapSMEntity
	pe = pb;
	pb->SetDisableParams (lvel, avel, steps);
}

WrapBox::~WrapBox () {
	delete pb;
	pe = 0;
	pb = 0;
}

void WrapBox::CreateNode (IrrlichtDevice *dev) {
	sn = dev->getSceneManager ()->addCubeSceneNode (size);
	SetTexture (dev);
}

