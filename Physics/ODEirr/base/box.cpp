// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "box.h"

BaseBox::BaseBox () {
	// Create physics representation (almost)
	phys_box = new PhysicsBox[1];
}

BaseBox::~BaseBox () {
	Destroy ();
}

void BaseBox::Destroy () {
	phys_box->DestroyBox ();
}

void BaseBox::Create (dWorldID world, dSpaceID space, float x, float y,
		float z, float size, float mass, IrrlichtDevice *dev, c8* tex_path) {
	BaseBox::size = size;
	BaseBox::mass = mass;

	// Create graphics representation
	CreateNode (dev, tex_path);
	// Complete physics representation creation
	// We need to do it that ugly, because collision depends on a mesh, which
	// we can get only after scene node creation
	// Well, not for the box, but for rag doll :)
	// Now we just keep all in sync
	phys_box->CreateBox (world, space, x, y, z, size, mass);
	phys_box->SetDisableParams (0.05, 0.05, 100);
}

int BaseBox::GetDisSteps () {
	return phys_box->GetDisSteps ();
}

void BaseBox::SetTexture (IrrlichtDevice *dev, c8 *tex_path) {
	box_node->setMaterialTexture (0,
			dev->getVideoDriver ()->getTexture (tex_path));
}

void BaseBox::CreateNode (IrrlichtDevice *dev, c8 *tex_path) {
	box_node = dev->getSceneManager ()->addCubeSceneNode (size);
	box_node->setMaterialTexture (0,
			dev->getVideoDriver ()->getTexture (tex_path));
	box_node->setMaterialFlag (EMF_LIGHTING, false);
}

void BaseBox::Update (bool run) {
	phys_box->DisableCheck (run);
	UpdateObject (phys_box->box_geom, box_node);
}

void BaseBox::Shoot (core::vector3df cam_pos, core::vector3df cam_target,
		float force) {
	phys_box->Shoot (cam_pos, cam_target, force);
}

bool BaseBox::IsEnabled () {
	return dBodyIsEnabled (phys_box->box_body);
}

