// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "sm_entity.h"
#include "../help.h"
using namespace irr;
using namespace scene;
using namespace video;

void* WrapSMEntity::GetData () {
	return dBodyGetData (pe->GetBody ());
}

const dReal* WrapSMEntity::GetPosition () const {
	return dBodyGetPosition (pe->GetBody ());
}

void WrapSMEntity::Update () {
	pe->DisableCheck ();
	UpdateObject (pe->GetGeom (), sn);
}

void WrapSMEntity::Shoot (const irr::core::vector3df pos,
		const irr::core::vector3df dir, const float force) {
	pe->Shoot (pos, dir, force);
}

bool WrapSMEntity::IsEnabled () const {
	return dBodyIsEnabled (pe->GetBody ());
}

void WrapSMEntity::SetTexture (IrrlichtDevice *dev) {
	sn->setMaterialTexture (0,
			dev->getVideoDriver ()->getTexture (tex_path.c_str ()));
	// TODO: add bool param to decide if we use static or dynamic light
	sn->setMaterialFlag (EMF_LIGHTING, false);
}

void WrapSMEntity::SetData (void *data) {
	dBodySetData (pe->GetBody (), data);
}

