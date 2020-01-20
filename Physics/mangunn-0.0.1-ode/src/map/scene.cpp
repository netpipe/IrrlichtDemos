// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "scene.h"
#include "../misc/help.h"
using namespace irr;
using namespace core;
#include <iostream>
using namespace std;

MapScene::MapScene (Registry *rg) {
	reg = rg;
	player = 0;
	sky = new MapSky (rg);
	reg->world = dWorldCreate ();
	reg->space = dSimpleSpaceCreate (0);
	reg->jg = dJointGroupCreate (0);
}

MapScene::~MapScene () {
	if (player)
		delete player;
	dJointGroupDestroy (reg->jg);
	dSpaceDestroy (reg->space);
	dWorldDestroy (reg->world);
}

bool MapScene::AddJointBS (const char *name1, const char *name2,
		const vector3df anchor, const bool constrain_x,
		const bool constrain_y, const bool constrain_z,
		const short x_low, const short x_high, const short y_low,
		const short y_high, const short z_low, const short z_high) {
	MapMesh *m1 = 0;
	MapMesh *m2 = 0;
	unsigned short i = 0;
	if (meshes.size () < 2 || strlen (name1) < 1 || strlen (name2) < 1)
		return false;
	while ((!m1 && !m2) || i < meshes.size ()) {
		// Get first mesh
		if (!m1 && strcmp (meshes[i]->GetCopy ().name, name1) == 0)
			m1 = meshes[i];
		// Get second mesh
		if (!m2 && strcmp (meshes[i]->GetCopy ().name, name2) == 0)
			m2 = meshes[i];
		i++;
	}
	if (!m1 || !m2) {
		return false;
	}
	MapJointBS *jb = new MapJointBS (reg->world, m1, m2, anchor, constrain_x,
			constrain_y, constrain_z, x_low, x_high, y_low, y_high,
			z_low, z_high);
	jbs.push_back (jb);
	return true;
}

bool MapScene::AddJointBS (const OMFJointBallSocket b) {
	return AddJointBS (b.mesh1_name, b.mesh2_name,
			vector3df (b.anchor.a, b.anchor.b, b.anchor.c), b.constrain_x,
			b.constrain_y, b.constrain_z, b.x_low, b.x_high, b.y_low,
			b.y_high, b.z_low, b.z_high);
}

bool MapScene::AddJointHinge (const char *name1, const char *name2,
		const vector3df anchor, const vector3df axis, const bool constrain,
		const short angle_low, const short angle_high) {
	MapMesh *m1 = 0;
	MapMesh *m2 = 0;
	unsigned short i = 0;
	if (meshes.size () < 2 || strlen (name1) < 1 || strlen (name2) < 1)
		return false;
	while ((!m1 && !m2) || i < meshes.size ()) {
		// Get first mesh
		if (!m1 && strcmp (meshes[i]->GetCopy ().name, name1) == 0)
			m1 = meshes[i];
		// Get second mesh
		if (!m2 && strcmp (meshes[i]->GetCopy ().name, name2) == 0)
			m2 = meshes[i];
		i++;
	}
	if (!m1 || !m2) {
		return false;
	}
	MapJointHinge *jh = new MapJointHinge (reg->world, m1, m2, anchor, axis,
			constrain, angle_low, angle_high);
	jhs.push_back (jh);
	return true;

}

bool MapScene::AddJointHinge (const OMFJointHinge h) {
	return AddJointHinge (h.mesh1_name, h.mesh2_name,
			vector3df (h.anchor.a, h.anchor.b, h.anchor.c),
			vector3df (h.axis.a, h.axis.b, h.axis.c),
			h.constrain, h.angle_low, h.angle_high);
}

void MapScene::AddLight (OMFLight l) {
	MapLight *ll = new MapLight (reg, l);
	lights.push_back (ll);
}

void MapScene::AddMesh (OMFMesh m) {
	MapMesh *mm = new MapMesh (reg, reg->world, reg->space, m);
	mm->SetDisableParams (1, 1, 50);
	meshes.push_back (mm);
}

void MapScene::AddPlaceTag (OMFPlaceTag p) {
	MapPlaceTag *pt = new MapPlaceTag (p);
	ptags.push_back (pt);
}

void MapScene::Clear () {
	for (unsigned short i = 0; i < jbs.size (); i++)
		delete jbs[i];
	jbs.clear ();
	for (unsigned short i = 0; i < jhs.size (); i++)
		delete jhs[i];
	jhs.clear ();
	for (unsigned short i = 0; i < meshes.size (); i++)
		delete meshes[i];
	meshes.clear ();
	for (unsigned short i = 0; i < lights.size (); i++)
		delete lights[i];
	lights.clear ();
	for (unsigned short i = 0; i < ptags.size (); i++)
		delete ptags[i];
	ptags.clear ();
	sky->Clear ();
}

vector3df MapScene::GetGravity () const {
	return gravity;
}

#ifdef USE_PLAYER_RAY
float MapScene::GetRayLength () const {
	return dist;
}
#endif

void MapScene::NearCallback (void *data, dGeomID o1, dGeomID o2) {
	MapScene *pthis = (MapScene*) data;
	if (pthis)
		pthis->NearCallback (o1, o2);
}

void MapScene::NearCallback (dGeomID o1, dGeomID o2) {
	dBodyID b1 = dGeomGetBody (o1);
	dBodyID b2 = dGeomGetBody (o2);
#ifdef USE_PLAYER_RAY
	if (dGeomGetClass (o1) == dRayClass) {
		dContactGeom cont;
		if (dCollide (player->GetRay (), o2, 1, &cont, sizeof (dContactGeom))) {
			dist = player->GetPosition ()[1] - 30 - cont.pos[1];
			player->UpdateRayLength (dist);
		}
		return;
	}
	if (dGeomGetClass (o2) == dRayClass) {
		dContactGeom cont;
		if (dCollide (player->GetRay (), o1, 1, &cont, sizeof (dContactGeom))) {
			dist = player->GetPosition ()[1] - 30 - cont.pos[1];
			player->UpdateRayLength (dist);
		}
		return;
	}
#endif
	// Do not check joined bodies
	/*
	if (b1 && b2 && dAreConnected (b1, b2))
		return;
	*/
	const int N = 4;
	dContact contact[N];
	int n = dCollide (o1, o2, N, &contact[0].geom, sizeof (dContact));
	if (n > 0)
		for (unsigned short i = 0; i < n; i++) {
			contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
				dContactSoftERP | dContactSoftCFM | dContactApprox1;
			contact[i].surface.mu = dInfinity;
			contact[i].surface.slip1 = 0.0;
			contact[i].surface.slip2 = 0.0;
			contact[i].surface.soft_erp = 0.001;
			contact[i].surface.soft_cfm = 0.001;
			dJointID c = dJointCreateContact (reg->world, reg->jg, &contact[i]);
			dJointAttach (c, b1, b2);
		}
}

bool MapScene::OnEvent (const irr::SEvent &e) {
	// Constant press
	if (e.EventType == EET_KEY_INPUT_EVENT && !reg->gui_shown && player) {
		if (e.KeyInput.Key == KEY_KEY_W) {
			player->MoveInDir (0, gravity);
			return true;
		}
		if (e.KeyInput.Key == KEY_KEY_S) {
			player->MoveInDir (1, gravity);
			return true;
		}
		if (e.KeyInput.Key == KEY_KEY_A) {
			player->MoveInDir (2, gravity);
			return true;
		}
		if (e.KeyInput.Key == KEY_KEY_D) {
			player->MoveInDir (3, gravity);
			return true;
		}
	}
	return false;
}

void MapScene::Open (const char *fn) {
	Clear ();
	FILE *f;
	f = fopen (fn, "rb");
	if (f == NULL) {
		Report ("Couldn't open file", "MapScene::Open");
		return;
	}
	// Read version
	OMFVersion version;
	fread (&version, sizeof (OMFVersion), 1, f);
	if (version.major != VERSION_MAJOR ||
			version.minor != VERSION_MINOR) {
		Report ("This is not current OMF version.", "MapScene::Open");
		fclose (f);
		return;
	}
	// Read header
	fread (&header, sizeof (OMFHeader), 1, f);
	SetGravity (vector3df (header.gravity.a, header.gravity.b,
				header.gravity.c));
	PrintVector (GetGravity ());
	// Read sky[dome/box]
	if (header.skydome) {
		OMFSkyDome sd;
		fread (&sd, sizeof (OMFSkyDome), 1, f);
		sky->SetSkyDome (sd);
	}
	else {
		OMFSkyBox sb;
		fread (&sb, sizeof (OMFSkyBox), 1, f);
		sky->SetSkyBox (sb);
	}
	// Read meshes
	for (unsigned short i = 0; i < header.mesh_num; i++) {
		OMFMesh m;
		fread (&m, sizeof (OMFMesh), 1, f);
		AddMesh (m);
	}
	// Read lights
	for (unsigned short i = 0; i < header.light_num; i++) {
		OMFLight l;
		fread (&l, sizeof (OMFLight), 1, f);
		AddLight (l);
	}
	// Read zones
	for (unsigned short i = 0; i < header.zone_num; i++) {
		OMFZone z;
		fread (&z, sizeof (OMFZone), 1, f);
		//AddZone (z);
	}
	// Read place tags
	for (unsigned short i = 0; i < header.place_tag_num; i++) {
		OMFPlaceTag pt;
		fread (&pt, sizeof (OMFPlaceTag), 1, f);
		AddPlaceTag (pt);
	}
	// Add player
	player = new MapPlayer (reg, reg->world, reg->space);
	for (unsigned short i = 0; i < header.place_tag_num; i++) {
		if (strcmp (ptags[i]->GetCopy ().name, "start") == 0) {
			OMFPlaceTag pt = ptags[i]->GetCopy ();
			player->SetPosition (vector3df (pt.pos.a, pt.pos.b, pt.pos.c));
			break;
		}
	}
	//OMFPlaceTag pt = ptags[0]->GetCopy ();
	// Read ball-socket joints
	for (unsigned short i = 0; i < header.joint_ball_socket_num; i++) {
		OMFJointBallSocket bs;
		fread (&bs, sizeof (OMFJointBallSocket), 1, f);
		if (!AddJointBS (bs))
			Report ("Couldn't add ball-socket joint, smth has gone wrong",
					"MapScene::Open");
	}
	// Read hinge joints
	for (unsigned short i = 0; i < header.joint_hinge_num; i++) {
		OMFJointHinge h;
		fread (&h, sizeof (OMFJointHinge), 1, f);
		if (!AddJointHinge (h))
			Report ("Couldn't add hinge joint, smth has gone wrong",
					"MapScene::Open");
	}
	fclose (f);
}

void MapScene::RecreateNodes () {
	for (unsigned short i = 0; i < meshes.size (); i++)
		meshes[i]->CreateNode ();
	for (unsigned short i = 0; i < lights.size (); i++)
		lights[i]->CreateNode ();
	sky->CreateNode ();
}

void MapScene::SetGravity (vector3df gr) {
	gravity = gr;
	dWorldSetGravity (reg->world, gr.X, gr.Y, gr.Z);
}

void MapScene::Simulate (float dt, int iters) {
	dSpaceCollide (reg->space, this, NearCallback);
	dWorldSetQuickStepNumIterations (reg->world, iters);
	dWorldQuickStep (reg->world, dt);
	dJointGroupEmpty (reg->jg);
}

void MapScene::Update () {
	for (unsigned short i = 0; i < meshes.size (); i++)
		meshes[i]->Update ();
	if (player)
		player->Update ();
}

