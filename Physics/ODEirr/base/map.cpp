// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "map.h"

BaseMap::BaseMap () {

}

BaseMap::~BaseMap () {

}

void BaseMap::Create (dSpaceID space, float x, float y, float z,
		IrrlichtDevice *dev) {
	CreateNode (dev);
	dTriMeshDataID data = GetGeomFromMesh (map_mesh->getMesh (0));
	// Create geom from a mesh
	map = dCreateTriMesh (space, data, 0, 0, 0);
	dGeomSetPosition (map, -1300, -144, -1249);
	// We don't create body for map for it not to be affected by gravity
	dGeomSetBody (map, 0);
}

void BaseMap::CreateNode (IrrlichtDevice *dev) {
	// Load Quake3 map
	dev->getFileSystem ()->addZipFileArchive ("../../media/map-20kdm2.pk3");
	map_mesh = dev->getSceneManager ()->getMesh ("20kdm2.bsp");
	// Create oct tree scene node which is much more suitable for
	// large nodes with lots of triangles, than avarage one
	map_node = dev->getSceneManager ()->addOctTreeSceneNode (
			map_mesh->getMesh (0));
	map_node->setPosition (core::vector3df (-1300, -144, -1249));
}

