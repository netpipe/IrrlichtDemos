#pragma once
#include <irrlicht.h>
#include "Waypoint.h"
#include "WPManager.h"

using namespace irr;

class Tile
{
public:
	Tile();
	Tile(IrrlichtDevice* device,core::vector3df pos,WPManager* wp,int dist,int vertOffset);
	~Tile(void);
	bool equals(Tile*);

	core::vector3df position;
	core::array<Waypoint*> waypoint;
	IrrlichtDevice* device;
	scene::IAnimatedMeshSceneNode* pNode;
	bool occupied;
};
