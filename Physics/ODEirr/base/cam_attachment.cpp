// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "cam_attachment.h"

BaseCamAttachment::BaseCamAttachment () {

}

BaseCamAttachment::~BaseCamAttachment () {

}

void BaseCamAttachment::Create (IrrlichtDevice *dev, Camera *cam,
		const c8 *model_path, const c8 *tex_path, core::vector3df pos,
		core::vector3df rot) {
	ca_mn = new MeshNode (dev->getSceneManager (), model_path);
	// This is only necessary for ms3d 
	if (tex_path)
		ca_mn->GetSceneNode ()->setMaterialTexture (0,
				dev->getVideoDriver ()->getTexture (tex_path));
	ca_mn->GetSceneNode ()->setParent (cam->GetCamera ());

	core::vector3df cam_pos = cam->GetPosition ();
	core::vector3df cam_target = cam->GetTarget ();

	cam->SetPosition (core::vector3df (0, 0, 0));
	cam->SetTarget (core::vector3df (0, 0, 1));

	core::vector3df poss = cam->GetPosition ();
	poss.X += pos.X;
	poss.Y += pos.Y;
	poss.Z += pos.Z;
	ca_mn->SetPosition (poss);
	ca_mn->SetRotation (rot);

	cam->SetPosition (cam_pos);
	cam->SetTarget (cam_target);

}

MeshNode* BaseCamAttachment::GetMeshNode () {
	return ca_mn;
}

