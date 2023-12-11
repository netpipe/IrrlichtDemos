#pragma once
#include <irrlicht.h>
#include "Waypoint.h"

using namespace irr;
#pragma comment(lib,"Irrlicht.lib")

class WPManager
{
public:
	WPManager(void);
	~WPManager(void);
	void InsertNode(Waypoint*);
	void RemoveNode(int index);
	Waypoint* GetNode(int index);
	int GetSize();
	
private:
	core::array<Waypoint*> data;
};
