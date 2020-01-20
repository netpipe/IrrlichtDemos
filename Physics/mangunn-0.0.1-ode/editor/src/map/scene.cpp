// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "scene.h"
#include "../misc/help.h"
using namespace irr;
using namespace core;
using namespace video;
using namespace std;

MapScene::MapScene (Registry *reg) {
	MapScene::reg = reg;
	sky = new MapSky (reg);
	Clean ();
}

MapScene::~MapScene () {
	Clear ();
	delete sky;
}

bool MapScene::AddJointBallSocket (const char *name1, const char *name2,
		const vector3df anchor, const bool constrain_x,
		const bool constrain_y, const bool constrain_z,
		const short x_low, const short x_high, const short y_low,
		const short y_high, const short z_low, const short z_high) {
	MapMesh *m1 = 0;
	MapMesh *m2 = 0;
	unsigned short i = 0;
	if (map_meshes.size () < 2 || strlen (name1) < 1 || strlen (name2) < 1)
		return false;
	while ((!m1 && !m2) || i < map_meshes.size ()) {
		// Get first mesh
		if (!m1 && strcmp (map_meshes[i]->GetCopy ().name, name1) == 0)
			m1 = map_meshes[i];
		// Get second mesh
		if (!m2 && strcmp (map_meshes[i]->GetCopy ().name, name2) == 0)
			m2 = map_meshes[i];
		i++;
	}
	if (!m1 || !m2) {
		return false;
	}
	MapJointBallSocket *jb = new MapJointBallSocket (reg, m1, m2, anchor,
			constrain_x, constrain_y, constrain_z, x_low, x_high, y_low,
			y_high, z_low, z_high);
	jbs.push_back (jb);
	return true;
}

bool MapScene::AddJointBallSocket (const OMFJointBallSocket b) {
	return AddJointBallSocket (b.mesh1_name, b.mesh2_name,
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
	if (map_meshes.size () < 2 || strlen (name1) < 1 || strlen (name2) < 1)
		return false;
	while ((!m1 && !m2) || i < map_meshes.size ()) {
		// Get first mesh
		if (!m1 && strcmp (map_meshes[i]->GetCopy ().name, name1) == 0)
			m1 = map_meshes[i];
		// Get second mesh
		if (!m2 && strcmp (map_meshes[i]->GetCopy ().name, name2) == 0)
			m2 = map_meshes[i];
		i++;
	}
	if (!m1 || !m2) {
		return false;
	}
	MapJointHinge *jh = new MapJointHinge (reg, m1, m2, anchor, axis,
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

void MapScene::AddLight (const char *name, const vector3df pos,
		const float rad, const SColorf col) {
	MapLight *map_light = new MapLight (reg, name, pos, rad, col);
	map_lights.push_back (map_light);
}

void MapScene::AddLight (const OMFLight l) {
	AddLight (l.name, vector3df (l.pos.a, l.pos.b, l.pos.c), l.radius,
			SColorf (l.color.a, l.color.b, l.color.c, l.color.d));
}

void MapScene::AddMesh (const char *name, const char *script_name,
		const char *model_file, const char *collision_model_file, 
		const vector3df pos, const vector3df rot, const char maggun_effect,
		const float mass, bool collidable, bool optimize) {
	MapMesh *map_mesh = new MapMesh (reg, name, script_name, model_file,
			collision_model_file, pos, rot, maggun_effect, mass,
			collidable, optimize);
	map_meshes.push_back (map_mesh);
}

void MapScene::AddMesh (const OMFMesh m) {
	AddMesh (m.name, m.script_name, m.model_file, m.collision_model_file,
			vector3df (m.pos.a, m.pos.b, m.pos.c),
			vector3df (m.rot.a, m.rot.b, m.rot.c), m.maggun_effect,
			m.mass, m.collidable, m.optimize);
}

void MapScene::AddPlaceTag (const char *name, const vector3df pos) {
	MapPlaceTag *pt = new MapPlaceTag (reg, name, pos);
	map_place_tags.push_back (pt);
}

void MapScene::AddPlaceTag (const OMFPlaceTag pt) {
	AddPlaceTag (pt.name, vector3df (pt.pos.a, pt.pos.b, pt.pos.c));
}

void MapScene::AddZone (const char *name, const char *model_file,
		const char *script_name, const vector3df pos, const vector3df rot) {
	MapZone *z = new MapZone (reg, name, model_file, script_name, pos, rot);
	map_zones.push_back (z);
}

void MapScene::AddZone (const OMFZone z) {
	AddZone (z.name, z.model_file, z.script_name, vector3df (z.pos.a,
				z.pos.b, z.pos.c), vector3df (z.rot.a, z.rot.b, z.rot.c));
}

void MapScene::Clean () {
	selected_item = 0;
	cur_file = "";
	name = "";
	script_name = "";
	gravity = vector3df (0, 0, 0);
}

void MapScene::Clear () {
	// Remove all map_meshes
	for (unsigned short i = 0; i < map_meshes.size (); i++)
		delete map_meshes[i];
	map_meshes.clear ();
	// Remove all map_lights
	for (unsigned short i = 0; i < map_lights.size (); i++)
		delete map_lights[i];
	map_lights.clear ();
	// Remove all map_zones
	for (unsigned short i = 0; i < map_zones.size (); i++)
		delete map_zones[i];
	map_zones.clear ();
	// Remove all map_place_tags
	for (unsigned short i = 0; i < map_place_tags.size (); i++)
		delete map_place_tags[i];
	map_place_tags.clear ();
	// Remove all jbs
	for (unsigned short i = 0; i < jbs.size (); i++)
		delete jbs[i];
	jbs.clear ();
	// Remove all jhs
	for (unsigned short i = 0; i < jhs.size (); i++)
		delete jhs[i];
	jhs.clear ();
	Clean ();
	sky->Clear ();
}

void MapScene::DeselectItem () {
	if (selected_item) {
		selected_item->Select (false);
		selected_item = 0;
	}
}

void MapScene::DrawAddons () {
	for (unsigned short i = 0; i < jbs.size (); i++)
		jbs[i]->DrawAddons ();
	for (unsigned short i = 0; i < jhs.size (); i++)
		jhs[i]->DrawAddons ();
}

bool MapScene::EditJointBallSocket (const unsigned int id, const char *name1,
		const char *name2, const vector3df anchor, const bool constrain_x,
		const bool constrain_y, const bool constrain_z,
		const short x_low, const short x_high, const short y_low,
		const short y_high, const short z_low, const short z_high) {
	MapMesh *m1 = 0;
	MapMesh *m2 = 0;
	unsigned short i = 0;
	while ((!m1 && !m2) || i < map_meshes.size ()) {
		// Get first mesh
		if (!m1 && strcmp (map_meshes[i]->GetCopy ().name, name1) == 0)
			m1 = map_meshes[i];
		// Get second mesh
		if (!m2 && strcmp (map_meshes[i]->GetCopy ().name, name2) == 0)
			m2 = map_meshes[i];
		i++;
	}
	if (!m1 || !m2) {
		return false;
	}
	for (unsigned short i = 0; i < jbs.size (); i++)
		if (id == jbs[i]->GetID ()) {
			jbs[i]->Edit (m1, m2, anchor, constrain_x, constrain_y,
					constrain_z, x_low, x_high, y_low, y_high, z_low,
					z_high);
			return true;
		}
	return false;
}

bool MapScene::EditJointHinge (const unsigned int id, const char *name1,
		const char *name2, const vector3df anchor, const vector3df axis,
		const bool constrain, const short angle_low, const short angle_high) {
	MapMesh *m1 = 0;
	MapMesh *m2 = 0;
	unsigned short i = 0;
	while ((!m1 && !m2) || i < map_meshes.size ()) {
		// Get first mesh
		if (!m1 && strcmp (map_meshes[i]->GetCopy ().name, name1) == 0)
			m1 = map_meshes[i];
		// Get second mesh
		if (!m2 && strcmp (map_meshes[i]->GetCopy ().name, name2) == 0)
			m2 = map_meshes[i];
		i++;
	}
	if (!m1 || !m2) {
		return false;
	}
	for (unsigned short i = 0; i < jhs.size (); i++)
		if (id == jhs[i]->GetID ()) {
			jhs[i]->Edit (m1, m2, anchor, axis, constrain, angle_low,
					angle_high);
			return true;
		}
	return false;
}

void MapScene::EditLight (const unsigned int id, const char *name,
		const vector3df pos, const float rad, const SColorf col) {
	for (unsigned short i = 0; i < map_lights.size (); i++)
		if (id == map_lights[i]->GetID ())
			map_lights[i]->Edit (name, pos, rad, col);
}

void MapScene::EditMesh (const unsigned int id, const char *name,
		const char *script_name, const char *model_file,
		const char *collision_model_file, const vector3df pos,
		const vector3df rot, const char maggun_effect, const float mass,
		bool collidable, bool optimize) {
	for (unsigned short i = 0; i < map_meshes.size (); i++)
		if (id == map_meshes[i]->GetID ())
			map_meshes[i]->Edit (name, script_name, model_file,
					collision_model_file,pos, rot, maggun_effect, mass,
					collidable, optimize);
}

void MapScene::EditPlaceTag (const unsigned int id, const char *name,
		const vector3df pos) {
	for (unsigned short i = 0; i < map_place_tags.size (); i++)
		if (id == map_place_tags[i]->GetID ())
			map_place_tags[i]->Edit (name, pos);
}

void MapScene::EditZone (const unsigned int id, const char *name,
		const char *model_file, const char *script_name,
		const vector3df pos, const vector3df rot) {
	for (unsigned short i = 0; i < map_zones.size (); i++)
		if (id == map_zones[i]->GetID ())
			map_zones[i]->Edit (name, model_file, script_name, pos, rot);

}

void MapScene::EditSelectedJointBallSocket (const char *name1,
		const char *name2, const vector3df anchor, const bool constrain_x,
		const bool constrain_y, const bool constrain_z,
		const short x_low, const short x_high, const short y_low,
		const short y_high, const short z_low, const short z_high) {
	if (selected_item) {
		selected_item->Select (true);
		if (!EditJointBallSocket (selected_item->GetID (), name1, name2,
				anchor, constrain_x, constrain_y, constrain_z, x_low,
				x_high, y_low, y_high, z_low, z_high))
			Report ("Couldn't find the two meshes with provided names",
					"MapScene::EditSelectedJointBallSocket");
	}
}

void MapScene::EditSelectedJointBallSocket (const OMFJointBallSocket b) {
	EditSelectedJointBallSocket (b.mesh1_name, b.mesh2_name, vector3df (
				b.anchor.a, b.anchor.b, b.anchor.c), b.constrain_x,
			b.constrain_y, b.constrain_z, b.x_low, b.x_high, b.y_low,
			b.y_high, b.z_low, b.z_high);
}

void MapScene::EditSelectedJointHinge (const char *name1, const char *name2,
		const vector3df anchor, const vector3df axis, const bool constrain,
		const short angle_low, const short angle_high) {
	if (selected_item) {
		selected_item->Select (true);
		if (!EditJointHinge (selected_item->GetID (), name1, name2,
				anchor, axis, constrain, angle_low, angle_high))
			Report ("Couldn't find the two meshes with provided names",
					"MapScene::EditSelectedJointHinge");
	}
}

void MapScene::EditSelectedJointHinge (const OMFJointHinge h) {
	EditSelectedJointHinge (h.mesh1_name, h.mesh2_name,
			vector3df (h.anchor.a, h.anchor.b, h.anchor.c),
			vector3df (h.axis.a, h.axis.b, h.axis.c),
			h.constrain, h.angle_low, h.angle_high);
}

void MapScene::EditSelectedLight (const char *name, const vector3df pos,
		const float rad, const SColorf col) {
	if (selected_item) {
		EditLight (selected_item->GetID (), name, pos, rad, col);
		selected_item->Select (true);
	}
}

void MapScene::EditSelectedLight (const OMFLight l) {
	EditSelectedLight (l.name, vector3df (l.pos.a, l.pos.b, l.pos.c),
			l.radius, SColorf (l.color.a, l.color.b, l.color.c, l.color.d));
}

void MapScene::EditSelectedMesh (const char *name, const char *script_name,
		const char *model_file, const char *collision_model_file, 
		const vector3df pos, const vector3df rot, const char maggun_effect,
		const float mass, bool collidable, bool optimize) {
	if (selected_item) {
		EditMesh (selected_item->GetID (), name, script_name, model_file,
				collision_model_file, pos, rot, maggun_effect, mass,
				collidable, optimize);
		selected_item->Select (true);
	}
}

void MapScene::EditSelectedMesh (const OMFMesh m) {
	EditSelectedMesh (m.name, m.script_name, m.model_file,
			m.collision_model_file, vector3df (m.pos.a, m.pos.b, m.pos.c),
			vector3df (m.rot.a, m.rot.b, m.rot.c), m.maggun_effect, m.mass,
			m.collidable, m.optimize);
}

void MapScene::EditSelectedPlaceTag (const char *name, const vector3df pos) {
	if (selected_item) {
		EditPlaceTag (selected_item->GetID (), name, pos);
		selected_item->Select (true);
	}
}

void MapScene::EditSelectedPlaceTag (const OMFPlaceTag pt) {
	EditSelectedPlaceTag (pt.name, vector3df (pt.pos.a, pt.pos.b, pt.pos.c));
}

void MapScene::EditSelectedZone (const char *name, const char *model_file,
		const char *script_name, const vector3df pos, const vector3df rot) {
	if (selected_item) {
		EditZone (selected_item->GetID (), name, model_file, script_name,
				pos, rot);
		selected_item->Select (true);
	}
}

void MapScene::EditSelectedZone (const OMFZone z) {
	EditSelectedZone (z.name, z.model_file, z.script_name, vector3df (
				z.pos.a, z.pos.b, z.pos.c), vector3df (z.rot.a, z.rot.b,
					z.rot.c));
}

stringc MapScene::GetCurFile () const {
	return cur_file;
}

vector3df MapScene::GetGravity () const {
	return gravity;
}

stringc MapScene::GetName () const {
	return name;
}

stringc MapScene::GetScriptName () const {
	return script_name;
}

OMFJointBallSocket MapScene::GetSelectedJointBallSocketCopy () const {
	if (selected_item) {
		unsigned int id = selected_item->GetID ();
		for (unsigned short i = 0; i < jbs.size (); i++)
			if (id == jbs[i]->GetID ())
				return jbs[i]->GetCopy ();
	}
	Report ("Can't get selected jbs copy",
			"MapScene::GetSelectedJointBallSocketCopy");
	OMFJointBallSocket jb;
	memset (&jb, 0, sizeof (OMFJointBallSocket));
	return jb;
}

OMFJointHinge MapScene::GetSelectedJointHingeCopy () const {
	if (selected_item) {
		unsigned int id = selected_item->GetID ();
		for (unsigned short i = 0; i < jhs.size (); i++)
			if (id == jhs[i]->GetID ())
				return jhs[i]->GetCopy ();
	}
	Report ("Can't get selected jhs copy",
			"MapScene::GetSelectedJointHingeCopy");
	OMFJointHinge jh;
	memset (&jh, 0, sizeof (OMFJointHinge));
	return jh;
}

OMFLight MapScene::GetSelectedLightCopy () const {
	if (selected_item) {
		unsigned int id = selected_item->GetID ();
		for (unsigned short i = 0; i < map_lights.size (); i++)
			if (id == map_lights[i]->GetID ())
				return map_lights[i]->GetCopy ();
	}
	Report ("Can't get selected light copy",
			"MapScene::GetSelectedLightCopy");
	OMFLight l;
	memset (&l, 0, sizeof (OMFLight));
	return l;
}

OMFMesh MapScene::GetSelectedMeshCopy () const {
	if (selected_item) {
		unsigned int id = selected_item->GetID ();
		for (unsigned short i = 0; i < map_meshes.size (); i++)
			if (id == map_meshes[i]->GetID ())
				return map_meshes[i]->GetCopy ();
	}
	Report ("Can't get selected mesh copy",
			"MapScene::GetSelectedMeshCopy");
	OMFMesh mesh;
	memset (&mesh, 0, sizeof (OMFMesh));
	return mesh;
}

OMFPlaceTag MapScene::GetSelectedPlaceTagCopy () const {
	if (selected_item) {
		unsigned int id = selected_item->GetID ();
		for (unsigned short i = 0; i < map_place_tags.size (); i++)
			if (id == map_place_tags[i]->GetID ())
				return map_place_tags[i]->GetCopy ();
	}
	Report ("Can't get selected place tag copy",
			"MapScene::GetSelectedPlaceTagCopy");
	OMFPlaceTag pt;
	memset (&pt, 0, sizeof (OMFPlaceTag));
	return pt;
}

OMFZone MapScene::GetSelectedZoneCopy () const {
	if (selected_item) {
		unsigned int id = selected_item->GetID ();
		for (unsigned short i = 0; i < map_zones.size (); i++)
			if (id == map_zones[i]->GetID ())
				return map_zones[i]->GetCopy ();
	}
	Report ("Can't get selected zone copy",
			"MapScene::GetSelectedZoneCopy");
	OMFZone z;
	memset (&z, 0, sizeof (OMFZone));
	return z;
}

char MapScene::GetSelectedType () const {
	if (selected_item)
		return selected_item->GetType ();
	else
		return -1;
}

MapSky* MapScene::GetMapSky () const {
	return sky;
}

bool MapScene::Import (FILE *f, const unsigned char major,
		const unsigned char minor) {
	// OMF 0.3
	if (major == 0 && minor == 3) {
		using namespace OMF_0_3;
		// Read header
		OMFHeader_0_3 header;
		fread (&header, sizeof (OMFHeader_0_3), 1, f);
		SetName (header.name);
		SetScriptName (header.script_name);
		SetGravity (vector3df (0, header.gravity, 0));
		// Read meshes
		for (unsigned short i = 0; i < header.mesh_num; i++) {
			OMFMesh_0_3 m;
			fread (&m, sizeof (OMFMesh_0_3), 1, f);
			AddMesh (m.name, "", m.model_file, "", vector3df (m.pos.a,
						m.pos.b, m.pos.c), vector3df (m.rot.a, m.rot.b,
							m.rot.c), m.maggun_effect, m.mass, true,
					m.optimize);
		}
		// Read lights
		for (unsigned short i = 0; i < header.light_num; i++) {
			OMFLight_0_3 l;
			fread (&l, sizeof (OMFLight_0_3), 1, f);
			AddLight (l.name, vector3df (l.pos.a, l.pos.b, l.pos.c), l.radius,
					SColorf (l.color.a, l.color.b, l.color.c, l.color.d));
		}
		// Read zones
		for (unsigned short i = 0; i < header.zone_num; i++) {
			OMFZone_0_3 z;
			fread (&z, sizeof (OMFZone_0_3), 1, f);
			AddZone (z.name, z.model_file, z.script_name, vector3df (z.pos.a,
						z.pos.b, z.pos.c), vector3df (z.rot.a, z.rot.b,
							z.rot.c));
		}
		// Read place tags
		for (unsigned short i = 0; i < header.place_tag_num; i++) {
			OMFPlaceTag_0_3 pt;
			fread (&pt, sizeof (OMFPlaceTag_0_3), 1, f);
			AddPlaceTag (pt.name, vector3df (pt.pos.a, pt.pos.b, pt.pos.c));
		}
		Report ("OMF 0.3 was imported successfully", "MapScene::Import");
		return true;
	}
	// OMF 0.4
	if (major == 0 && minor == 4) {
		using namespace OMF_0_4;
		// Read header
		OMFHeader_0_4 header;
		fread (&header, sizeof (OMFHeader_0_4), 1, f);
		SetName (header.name);
		SetScriptName (header.script_name);
		SetGravity (vector3df (0, header.gravity, 0));
		// Read sky[dome/box]
		if (header.skydome) {
			OMFSkyDome_0_4 sd;
			fread (&sd, sizeof (OMFSkyDome_0_4), 1, f);
			OMFSkyDome sd1;
			Report (sd.texture, "abasasba");
			strcpy (sd1.texture, sd.texture);
			sd1.horiz_res = sd.horiz_res;
			sd1.vert_res = sd.vert_res;
			sd1.tpercentage = sd.tpercentage;
			sd1.spercentage = sd.spercentage;
			Report ("a", "a");
			sky->SetSkyDome (sd1);
			Report ("a", "a");
		}
		else {
			OMFSkyBox_0_4 sb;
			fread (&sb, sizeof (OMFSkyBox_0_4), 1, f);
			OMFSkyBox sb1;
			strcpy (sb1.top, sb.top);
			strcpy (sb1.bottom, sb.bottom);
			strcpy (sb1.left, sb.left);
			strcpy (sb1.right, sb.right);
			strcpy (sb1.front, sb.front);
			strcpy (sb1.back, sb.back);
			Report ("a", "a");
			sky->SetSkyBox (sb1);
			Report ("a", "a");
		}
		// Read meshes
		for (unsigned short i = 0; i < header.mesh_num; i++) {
			OMFMesh_0_4 m;
			fread (&m, sizeof (OMFMesh_0_4), 1, f);
			AddMesh (m.name, "", m.model_file, "", vector3df (m.pos.a,
						m.pos.b, m.pos.c), vector3df (m.rot.a, m.rot.b,
							m.rot.c), m.maggun_effect, m.mass, m.collidable,
					m.optimize);
		}
		// Read lights
		for (unsigned short i = 0; i < header.light_num; i++) {
			OMFLight_0_4 l;
			fread (&l, sizeof (OMFLight_0_4), 1, f);
			AddLight (l.name, vector3df (l.pos.a, l.pos.b, l.pos.c), l.radius,
					SColorf (l.color.a, l.color.b, l.color.c, l.color.d));
		}
		// Read zones
		for (unsigned short i = 0; i < header.zone_num; i++) {
			OMFZone_0_4 z;
			fread (&z, sizeof (OMFZone_0_4), 1, f);
			AddZone (z.name, z.model_file, z.script_name, vector3df (z.pos.a,
						z.pos.b, z.pos.c), vector3df (z.rot.a, z.rot.b,
							z.rot.c));
		}
		// Read place tags
		for (unsigned short i = 0; i < header.place_tag_num; i++) {
			OMFPlaceTag_0_4 pt;
			fread (&pt, sizeof (OMFPlaceTag_0_4), 1, f);
			AddPlaceTag (pt.name, vector3df (pt.pos.a, pt.pos.b, pt.pos.c));
		}
		Report ("OMF 0.4 was imported successfully", "MapScene::Import");
		return true;
	}
	Report ("This OMF version is unsupported", "MapScene::Import");
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
	// If this is not current version, try to import it
	if (version.major != VERSION_MAJOR ||
			version.minor != VERSION_MINOR) {
		stringc s = "This is not current OMF version. Trying to import OMF ";
	   	s += version.major; 
		s += ".";
		s += version.minor;
		Report (s.c_str (), "MapScene::Open");
		if (Import (f, version.major, version.minor))
			cur_file = stringc (fn);
		fclose (f);
		return;
	}
	// Read header
	OMFHeader header;
	fread (&header, sizeof (OMFHeader), 1, f);
	SetName (header.name);
	SetScriptName (header.script_name);
	SetGravity (vector3df (header.gravity.a, header.gravity.b,
				header.gravity.c));
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
		AddZone (z);
	}
	// Read place tags
	for (unsigned short i = 0; i < header.place_tag_num; i++) {
		OMFPlaceTag pt;
		fread (&pt, sizeof (OMFPlaceTag), 1, f);
		AddPlaceTag (pt);
	}
	// Read ball-socket joints
	for (unsigned short i = 0; i < header.joint_ball_socket_num; i++) {
		OMFJointBallSocket bs;
		fread (&bs, sizeof (OMFJointBallSocket), 1, f);
		if (!AddJointBallSocket (bs))
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
	cur_file = stringc (fn);
}

void MapScene::RemoveItem (const unsigned int id) {
	// Search for id in map_meshes
	for (unsigned short i = 0; i < map_meshes.size (); i++)
		if (id == map_meshes[i]->GetID ()) {
			// First check if we need to delete any joint
			// Ball-socket joints
recheck_label:
			for (unsigned short j = 0; j < jbs.size (); j++)
				if (strcmp (map_meshes[i]->GetCopy ().name,
							jbs[j]->GetCopy ().mesh1_name) == 0 ||
						strcmp (map_meshes[i]->GetCopy ().name,
							jbs[j]->GetCopy ().mesh2_name) == 0) {
					delete jbs[j];
					jbs.erase (jbs.begin () + j);
					// Since vector element was removed,
					// we have wrong idices now. We need to reinit iterator
					goto recheck_label;
				}
			// Hinge joints
recheck_label2:
			for (unsigned short j = 0; j < jhs.size (); j++)
				if (strcmp (map_meshes[i]->GetCopy ().name,
							jhs[j]->GetCopy ().mesh1_name) == 0 ||
						strcmp (map_meshes[i]->GetCopy ().name,
							jhs[j]->GetCopy ().mesh2_name) == 0) {
					delete jhs[j];
					jhs.erase (jhs.begin () + j);
					goto recheck_label2;
				}
			// Remove item
			delete map_meshes[i];
			// Remove item from vector
			map_meshes.erase (map_meshes.begin () + i);
			return;
		}
	// Search for id in map_lights
	for (unsigned short i = 0; i < map_lights.size (); i++)
		if (id == map_lights[i]->GetID ()) {
			delete map_lights[i];
			map_lights.erase (map_lights.begin () + i);
			return;
		}
	// Search for id in map_zones
	for (unsigned short i = 0; i < map_zones.size (); i++)
		if (id == map_zones[i]->GetID ()) {
			delete map_zones[i];
			map_zones.erase (map_zones.begin () + i);
			return;
		}
	// Search for id in map_place_tags
	for (unsigned short i = 0; i < map_place_tags.size (); i++)
		if (id == map_place_tags[i]->GetID ()) {
			delete map_place_tags[i];
			map_place_tags.erase (map_place_tags.begin () + i);
			return;
		}
	// Search for id in jbs
	for (unsigned short i = 0; i < jbs.size (); i++)
		if (id == jbs[i]->GetID ()) {
			delete jbs[i];
			jbs.erase (jbs.begin () + i);
			return;
		}
	// Search for id in jhs
	for (unsigned short i = 0; i < jhs.size (); i++)
		if (id == jhs[i]->GetID ()) {
			delete jhs[i];
			jhs.erase (jhs.begin () + i);
			return;
		}
}

void MapScene::RemoveSelected () {
	if (selected_item) {
		RemoveItem (selected_item->GetID ());
		selected_item = 0;
	}
}

void MapScene::Save (const char *fn) {
	FILE *f = 0;
	f = fopen (fn, "wb");
	if (!f)
		return;
	// Save version
	OMFVersion version;
	version.major = VERSION_MAJOR;
	version.minor = VERSION_MINOR;
	fwrite (&version, sizeof (OMFVersion), 1, f);
	// Save header
	OMFHeader header;
	memset (&header, 0, sizeof (OMFHeader));
	strcpy (header.name, name.c_str ());
	strcpy (header.script_name, script_name.c_str ());
	header.gravity.a = gravity.X;
	header.gravity.b = gravity.Y;
	header.gravity.c = gravity.Z;
	header.skydome = sky->SkyDomeIsUsed ();
	header.mesh_num = map_meshes.size ();
	header.light_num = map_lights.size ();
	header.zone_num = map_zones.size ();
	header.place_tag_num = map_place_tags.size ();
	header.joint_ball_socket_num = jbs.size ();
	header.joint_hinge_num = jhs.size ();
	header.joint_slider_num = 0;
	header.joint_universal_num = 0;
	header.joint_hinge2_num = 0;
	header.joint_fixed_num = 0;
	fwrite (&header, sizeof (OMFHeader), 1, f);
	// Save sky[dome/box]
	if (header.skydome) {
		OMFSkyDome sd = sky->GetSkyDomeCopy ();
		fwrite (&sd, sizeof (OMFSkyDome), 1, f);
	}
	else {
		OMFSkyBox sb = sky->GetSkyBoxCopy ();
		fwrite (&sb, sizeof (OMFSkyBox), 1, f);
	}
	// Save meshes
	for (unsigned short i = 0; i < header.mesh_num; i++) {
		OMFMesh m = map_meshes[i]->GetCopy ();
		fwrite (&m, sizeof (OMFMesh), 1, f);
	}
	// Save lights
	for (unsigned short i = 0; i < header.light_num; i++) {
		OMFLight l = map_lights[i]->GetCopy ();
		fwrite (&l, sizeof (OMFLight), 1, f);
	}
	// Save zones
	for (unsigned short i = 0; i < header.zone_num; i++) {
		OMFZone z = map_zones[i]->GetCopy ();
		fwrite (&z, sizeof (OMFZone), 1, f);
	}
	// Save place tags
	for (unsigned short i = 0; i < header.place_tag_num; i++) {
		OMFPlaceTag pt = map_place_tags[i]->GetCopy ();
		fwrite (&pt, sizeof (OMFPlaceTag), 1, f);
	}
	// Save ball-socket joints
	for (unsigned short i = 0; i < header.joint_ball_socket_num; i++) {
		OMFJointBallSocket b = jbs[i]->GetCopy ();
		fwrite (&b, sizeof (OMFJointBallSocket), 1, f);
	}
	// Save hinge joints
	for (unsigned short i = 0; i < header.joint_hinge_num; i++) {
		OMFJointHinge h = jhs[i]->GetCopy ();
		fwrite (&h, sizeof (OMFJointHinge), 1, f);
	}
	fclose (f);
	cur_file = stringc (fn);
}

void MapScene::SelectItem (const unsigned int id, bool state) {
	if (state) {
		// If there's a selected item already, deselect it
		if (selected_item)
			selected_item->Select (false);
		// Search for id in map_meshes
		for (unsigned short i = 0; i < map_meshes.size (); i++)
			if (id == map_meshes[i]->GetID ()) {
				selected_item = (MapItem*)map_meshes[i];
				selected_item->Select (true);
			}
		// Search for id in map_lights
		for (unsigned short i = 0; i < map_lights.size (); i++)
			if (id == map_lights[i]->GetID ()) {
				selected_item = (MapItem*)map_lights[i];
				selected_item->Select (true);
			}
		// Search for id in map_zones
		for (unsigned short i = 0; i < map_zones.size (); i++)
			if (id == map_zones[i]->GetID ()) {
				selected_item = (MapItem*)map_zones[i];
				selected_item->Select (true);
			}
		// Search for id in map_place_tags
		for (unsigned short i = 0; i < map_place_tags.size (); i++)
			if (id == map_place_tags[i]->GetID ()) {
				selected_item = (MapItem*)map_place_tags[i];
				selected_item->Select (true);
			}
		// Search for id in jbs
		for (unsigned short i = 0; i < jbs.size (); i++)
			if (id == jbs[i]->GetID ()) {
				selected_item = (MapItem*)jbs[i];
				selected_item->Select (true);
			}
		// Search for id in jhs
		for (unsigned short i = 0; i < jhs.size (); i++)
			if (id == jhs[i]->GetID ()) {
				selected_item = (MapItem*)jhs[i];
				selected_item->Select (true);
			}
	}
	else
		DeselectItem ();
}

void MapScene::SetGravity (vector3df gr) {
	gravity = gr;
}

void MapScene::SetName (const char *name) {
	MapScene::name = name;
}

void MapScene::SetScriptName (const char *name) {
	script_name = name;
}

