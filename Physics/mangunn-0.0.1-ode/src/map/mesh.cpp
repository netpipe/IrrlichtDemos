// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "mesh.h"
#include "../misc/help.h"
using namespace irr;
using namespace scene;
using namespace core;
using namespace video;
#include <iostream>
using namespace std;

MapMesh::MapMesh (Registry *rg, const dWorldID wr, const dSpaceID sp,
		const OMFMesh m) {
	reg = rg;
	world = wr;
	space = sp;
	memset (&mesh, 0, sizeof (OMFMesh));
	body = 0;
	geom = 0;
	ds_cur = 0;
	ds_given = 0;
	dlvel = 0;
	davel = 0;
	sn = 0;
	am = 0;
	model_dir = reg->settings->GetDataDir () + "models/";

	Edit (m);
}

MapMesh::~MapMesh () {
	RemoveBody ();
	RemoveGeom ();
	RemoveNode ();
}

void MapMesh::CreateNode (const OMFMesh *m) {
	// We don't do RemoveNode here, because after device recreation
	// sn will be still pointing to some storage which it'll try to delete
	stringc model_path = model_dir;
	if (!m)
		model_path += mesh.model_file;
	else
		model_path += m->model_file;
	am = reg->smgr->getMesh (model_path.c_str ());
	if (!mesh.optimize)
		sn = reg->smgr->addAnimatedMeshSceneNode (am);
	else
		sn = reg->smgr->addOctTreeSceneNode (am->getMesh (0));
	PositionNode (mesh);
}

void MapMesh::Edit (const OMFMesh m) {
	/*
	// If we change optimize or / and model_file, we recreate
	// scene node
	if ((m.optimize != mesh.optimize) ||
			(strcmp (m.model_file, mesh.model_file) != 0)) {
		RemoveNode ();
		CreateNode (&m);
		PositionNode (m);
	}
	// If we change collidable or / and collision_model_file or / and
	// model_file when no collision_model_file, either create
	// or remove physics
	if ((m.collidable != mesh.collidable) ||
			(strcmp (m.collision_model_file, mesh.collision_model_file) != 0) ||
			(strcmp (m.model_file, mesh.model_file) != 0 &&
			 strlen (m.collision_model_file) < 5)) {
		// We need to remove physics in any case
		RemoveBody ();
		RemoveGeom ();
		// If collidable, construct geometry for it
		if (m.collidable) {
			stringc model_path;
			// If we have collision_model_file, use it
			if (strlen (m.collision_model_file) > 4)
				model_path = model_dir + m.collision_model_file;
			// Else	use model_file
			else
				model_path = model_dir + m.model_file;
			IAnimatedMesh *am = reg->smgr->getMesh (model_path.c_str ());
			dTriMeshDataID data = GetGeomFromMesh (am->getMesh (0));
			geom = dCreateTriMesh (space, data, 0, 0, 0);
			if (geom)
				cout <<  "collidable\n";
			// If movable, construct body for it
			if (m.mass >= 0) {
				body = dBodyCreate (world);
				dGeomSetBody (geom, body);
				cout << "linked geom & body\n";
			}
			else
				dGeomSetBody (geom, 0);
			PositionPhys (m);
		}
	}
	// If we change mass, we either apply it, or make mesh unmovable
	if ((m.mass != mesh.mass)) {
		if (m.mass < 0)
			RemoveBody ();
		else {
			SetMass (m.mass);
			cout << "set mass\n";
		}
	}
	// If we change position or / and rotation, position correctly
	if ((m.pos.a != mesh.pos.a) || (m.pos.a != mesh.pos.a) ||
			(m.pos.a != mesh.pos.a) || (m.rot.a != mesh.rot.a) ||
			(m.rot.a != mesh.rot.a) || (m.rot.a != mesh.rot.a)) {
		PositionNode (m);
		PositionPhys (m);
	}
	// TODO: Modify so that positioning doesn't take place after
	// node or / and physics restructure
	*/
	strcpy (mesh.name, m.name);
	strcpy (mesh.script_name, m.script_name);
	strcpy (mesh.model_file, m.model_file);
	strcpy (mesh.collision_model_file, m.collision_model_file);
	mesh.pos = m.pos;
	mesh.rot= m.rot;
	mesh.maggun_effect = m.maggun_effect;
	mesh.mass = m.mass;
	mesh.collidable = m.collidable;
	mesh.optimize = m.optimize;
	CreateNode ();
	if (mesh.collidable) {
		stringc model_path;
		if (strlen (m.collision_model_file) > 4)
			model_path = model_dir + m.collision_model_file;
		else
			model_path = model_dir + m.model_file;
		IAnimatedMesh *am = reg->smgr->getMesh (model_path.c_str ());
		dTriMeshDataID data = GetGeomFromMesh (am->getMesh (0));
		geom = dCreateTriMesh (space, data, 0, 0, 0);
	}
	if (mesh.mass > 0) {
		body = dBodyCreate (world);
		SetMass (mesh.mass);
		//cout << "linked geom & body\n";
	}
	dGeomSetBody (geom, body);
	PositionPhys (mesh);
	//UpdateObject (geom, sn);
}

OMFMesh MapMesh::GetCopy () const {
	return mesh;
}

void MapMesh::Update () {
	if (mesh.mass > 0) {
		DisableCheck ();
		UpdateObject (geom, sn);
	}
	/*
	reg->drv->setTransform (ETS_WORLD, sn->getAbsoluteTransformation ());
	reg->drv->draw3DBox (sn->getBoundingBox (), SColor (255, 255, 0, 0));
	*/
}

void MapMesh::PositionNode (const OMFMesh m) {
	sn->setPosition (vector3df (m.pos.a, m.pos.b, m.pos.c));
	sn->setRotation (vector3df (m.rot.a, m.rot.b, m.rot.c));
}

void MapMesh::PositionPhys (const OMFMesh m) {
	dGeomSetPosition (geom, m.pos.a, m.pos.b, m.pos.c);
	//cout << "pos: (" << m.pos.a << ", " << m.pos.b << ", " << m.pos.c << ")\n";
	dQuaternion q;
	EulerToQuaternion (vector3df (m.rot.a, m.rot.b, m.rot.c), q);
	dGeomSetQuaternion (geom, q);
}

void MapMesh::RemoveNode () {
	if (sn) {
		sn->remove ();
		sn = 0;
	}
}

void MapMesh::SetMass (const float m) {
	dMass mm;
	vector3df ext = sn->getBoundingBox ().getExtent ();
	//dMassSetTrimesh (&mm, 1, geom);
	//dMassSetBox (&mm, 1, 14, 14, 14);
	dMassSetBox (&mm, 1, ext.X, ext.Y, ext.Z);
	dMassAdjust (&mm, m);
	dBodySetMass (body, &mm);
}

