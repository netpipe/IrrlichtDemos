#pragma once
#include <irrlicht.h>
#define MAX_NEXT 4

using namespace irr;

class Waypoint
{
public:
	Waypoint();
	Waypoint(IrrlichtDevice* device, core::vector3df pos);
	~Waypoint(void);
	void AddNxt(Waypoint*);
	Waypoint* GetRandNxt();
	bool equals(Waypoint* o,Waypoint*other);

	core::vector3df position;
	core::array<Waypoint*> nxt;
	scene::IAnimatedMeshSceneNode* pNode;
	IrrlichtDevice* device;
	int count;
	Waypoint* t;
};
