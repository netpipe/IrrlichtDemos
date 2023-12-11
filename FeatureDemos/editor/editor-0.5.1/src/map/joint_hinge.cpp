// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "joint_hinge.h"
#include "../misc/help.h"

using namespace irr;
using namespace core;
using namespace video;

MapJointHinge::MapJointHinge (Registry *reg, MapMesh *m1,
		MapMesh *m2, const vector3df anchor, const vector3df axis,
		const bool constrain, const short angle_low, const short angle_high) {
	MapJointHinge::reg = reg;
	MapJointHinge::m1 = m1;
	MapJointHinge::m2 = m2;
	selected = false;
	node = 0;
	memset (&joint, 0, sizeof (OMFJointHinge));
	// Make lines look good
	mat.MaterialType = EMT_SOLID;
	mat.Lighting = false;
	m.makeIdentity ();

	id = reg->GetFreeID ();
	stringc path = reg->settings->GetDataDir () + "models/joint_hinge.obj";
	node = reg->smgr->addAnimatedMeshSceneNode (reg->smgr->getMesh (
				path.c_str ()), 0, id, anchor);
	node->setMaterialFlag (EMF_WIREFRAME, true);
	stringc path2 = reg->settings->GetDataDir () + "textures/light.png";
	node->setMaterialTexture (0, reg->drv->getTexture (path2.c_str ()));
	Edit (m1, m2, anchor, axis, constrain, angle_low, angle_high);
}

void MapJointHinge::DrawAddons () {
	if (selected) {
		reg->drv->setTransform (ETS_WORLD, m);
		reg->drv->setMaterial (mat);
		reg->drv->draw3DLine (node->getPosition (), m1->GetPosition ());
		reg->drv->draw3DLine (node->getPosition (), m2->GetPosition ());
	}
}

void MapJointHinge::Edit (MapMesh *m1, MapMesh *m2, const vector3df anchor,
		const vector3df axis, const bool constrain, const short angle_low,
		const short angle_high) {
	MapJointHinge::m1 = m1;
	MapJointHinge::m2 = m2;
	node->setPosition (anchor);
	// TODO: make sure rotation works ok
	vector3df rot;
	
	//OLD CODE
	/*
	float fmp = sqrt (axis.X * axis.X + axis.Y * axis.Y + axis.Z * axis.Z);
	rot.X = acos (axis.X / fmp) * 180 / M_PI;
	rot.Y = acos (axis.Y / fmp) * 180 / M_PI; 
	rot.Z = acos (axis.Z / fmp) * 180 / M_PI; 
	*/
	/*
	rot.X = sqrt (axis.Z * axis.Z + axis.Y * axis.Y) / fmp;
	rot.Y = sqrt (axis.Z * axis.Z + axis.X * axis.X) / fmp;
	rot.Z = sqrt (axis.Y * axis.Y + axis.X * axis.X) / fmp;
	*/
	/*
	rot.Y = atan2 (axis.X, axis.Y);
	rot.Y *= (180 / M_PI);
	if (rot.Y < 0)
		rot.Y += 360;
	if (rot.Y > 360)
		rot.Y -= 360;
	float z1 = sqrt (axis.X * axis.X + axis.Z * axis.Z);
	rot.X = atan2 (z1, axis.Y);
	rot.X *= (180 / M_PI);
	if (rot.X < 360)
		rot.X += 360;
	if (rot.X > 360)
		rot.X -= 360;
	*/

	//Calculating angles
	if (axis.Y != 0) {
		rot.X = 57.295779513 * atan (abs(axis.Z) / abs(axis.Y));
	}
	else {
		if (axis.Z != 0) {
		rot.X = 90;
		}
		else {rot.X = 0;}
	}
	
	if (axis.Z != 0) {
		rot.Y = 57.295779513 * atan (abs(axis.X) / abs(axis.Z));
	}
	else {
		if (axis.X != 0) {
		    rot.Y = 90;
		    }
		else {
		    rot.Y = 0;
		    }
	}
	
	if (axis.X != 0) {
		rot.Z = 57.295779513 * atan (abs(axis.Y) / abs(axis.X));
	}
	else {
		if (axis.Y != 0) rot.Z = 90;
		else rot.Z = 0;
	}
	
	//Checking directions
	/*
	if (axis.Y < 0) {
		rot.X = rot.X + 90;
	}
	if (axis.Z < 0) {
		rot.X = rot.X * -1;
		rot.Y = rot.Y + 90;
	}
	if (axis.X < 0) {
		rot.Z = rot.Z + 90;
		rot.Y = rot.Y * -1;
	}
	if (axis.Y < 0) {
		rot.Z = rot.Z * -1;
	}
	*/
	
	PrintVector(rot);
	//kaisd try ended here

	node->setRotation (rot);
	// =================================
	OMFMesh m = m1->GetCopy ();
	strcpy (joint.mesh1_name, m.name);
	m = m2->GetCopy ();
	strcpy (joint.mesh2_name, m.name);
	joint.anchor.a = anchor.X;
	joint.anchor.b = anchor.Y;
	joint.anchor.c = anchor.Z;
	joint.axis.a = axis.X;
	joint.axis.b = axis.Y;
	joint.axis.c = axis.Z;
	joint.constrain = constrain;
	if (joint.constrain) {
		joint.angle_low = angle_low;
		joint.angle_high = angle_high;
	}
	else {
		joint.angle_low = 0;
		joint.angle_high = 0;
	}
}

OMFJointHinge MapJointHinge::GetCopy () const {
	return joint;
}

char MapJointHinge::GetType () const {
	return 5; // Hinge joint
}

void MapJointHinge::Select (bool state) {
	node->setMaterialFlag (EMF_LIGHTING, !state);
	selected = state;
}

void MapJointHinge::GetVectorFromAngles (const vector3df angles, vector3df S) {
	double R = sqrt(S.X*S.X + S.Y*S.Y + S.Z*S.Z);
	
	S.X = R * cos(angles.Y) * cos(angles.Z);
	S.Y = R *  sin(angles.X) * sin(angles.Z);
	S.Z = R * cos(angles.X) * (-sin(angles.Y));
}
