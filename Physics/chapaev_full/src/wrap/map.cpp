// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "map.h"
#include "../help.h"
using namespace irr;
using namespace video;
using namespace core;

WrapMap::WrapMap (const dSpaceID space, IrrlichtDevice *dev,
		const vector3df pos, const char *map_path, const char *map_name) {
	WrapMap::pos = pos;
	path = map_path;
	name = map_name;

	CreateNode (dev);
	dTriMeshDataID data = GetGeomFromMesh (am->getMesh (0));
	// Create geom form mesh data
	map = dCreateTriMesh (space, data, 0, 0, 0);
	dGeomSetPosition (map, pos.X, pos.Y, pos.Z);
	// We don't create body for the map, because it must not be
	// affected by gravity
	dGeomSetBody (map, 0);
}

WrapMap::~WrapMap () {

}

void WrapMap::CreateNode (IrrlichtDevice *dev) {
	if (name.size () != 0) {
		// Load Quake3 map
		dev->getFileSystem ()->addZipFileArchive (path.c_str ());
		am = dev->getSceneManager ()->getMesh (name.c_str ());
	}
	else {
		// Load avarage model file
		am = dev->getSceneManager ()->getMesh (path.c_str ());
	}
	// Create oct tree scene node, because map is a very large model
	// Avarage scene node is not suitable here
	sn = dev->getSceneManager ()->addOctTreeSceneNode (am->getMesh (0));
	sn->setPosition (pos);
	sn->setMaterialFlag (EMF_LIGHTING, false);
	//sn->getMaterial (0).EmissiveColor.set (0, 0, 0, 0);
}

