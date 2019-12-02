// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "joint_ball_socket.h"
using namespace irr;
using namespace core;
using namespace video;

MapJointBallSocket::MapJointBallSocket (Registry *reg, MapMesh *m1,
		MapMesh *m2, vector3df anchor, bool constrain_x, bool constrain_y,
		bool constrain_z, short x_low, short x_high, short y_low,
		short y_high, short z_low, short z_high) {
	// The outer function must verify that m1 & m2 have unique names
	MapJointBallSocket::reg = reg;
	MapJointBallSocket::m1 = m1;
	MapJointBallSocket::m2 = m2;
	selected = false;
	node = 0;
	memset (&joint, 0, sizeof (OMFJointBallSocket));
	// Make lines look good
	mat.MaterialType = EMT_SOLID;
	mat.Lighting = false;
	m.makeIdentity ();

	id = reg->GetFreeID ();
	node = reg->smgr->addSphereSceneNode (5.0f, 10, 0, id, anchor);
	node->setMaterialFlag (EMF_WIREFRAME, true);
	stringc path = reg->settings->GetDataDir () + "textures/light.png";
	node->setMaterialTexture (0, reg->drv->getTexture (path.c_str ()));
	Edit (m1, m2, anchor, constrain_x, constrain_y, constrain_z, x_low,
			x_high, y_low, y_high, z_low, z_high);
}

void MapJointBallSocket::DrawAddons () {
	if (selected) {
		reg->drv->setTransform (ETS_WORLD, m);
		reg->drv->setMaterial (mat);
		reg->drv->draw3DLine (node->getPosition (), m1->GetPosition ());
		reg->drv->draw3DLine (node->getPosition (), m2->GetPosition ());
	}
}

void MapJointBallSocket::Edit (MapMesh *m1, MapMesh *m2, vector3df anchor,
		bool constrain_x, bool constrain_y, bool constrain_z,
		short x_low, short x_high, short y_low, short y_high,
		short z_low, short z_high) {
	MapJointBallSocket::m1 = m1;
	MapJointBallSocket::m2 = m2;
	node->setPosition (anchor);
	OMFMesh m = m1->GetCopy ();
	strcpy (joint.mesh1_name, m.name);
	m = m2->GetCopy ();
	strcpy (joint.mesh2_name, m.name);
	joint.anchor.a = anchor.X;
	joint.anchor.b = anchor.Y;
	joint.anchor.c = anchor.Z;
	joint.constrain_x = constrain_x;
	joint.constrain_y = constrain_y;
	joint.constrain_z = constrain_z;
	joint.x_low = x_low;
	joint.x_high = x_high;
	joint.y_low = y_low;
	joint.y_high = y_high;
	joint.z_low = z_low;
	joint.z_high = z_high;
	if (!joint.constrain_x) {
		joint.x_low = x_low;
		joint.x_high = x_high;
	}
	else {
		joint.x_low = 0;
		joint.x_high = 0;
	}
	if (!joint.constrain_y) {
		joint.y_low = y_low;
		joint.y_high = y_high;
	}
	else {
		joint.y_low = 0;
		joint.y_high = 0;
	}
	if (!joint.constrain_z) {
		joint.z_low = z_low;
		joint.z_high = z_high;
	}
	else {
		joint.z_low = 0;
		joint.z_high = 0;
	}
}

OMFJointBallSocket MapJointBallSocket::GetCopy () const {
	return joint;
}

char MapJointBallSocket::GetType () const {
	return 4; // Ball-socket joint
}

void MapJointBallSocket::Select (bool state) {
	node->setMaterialFlag (EMF_LIGHTING, !state);
	selected = state;
}

