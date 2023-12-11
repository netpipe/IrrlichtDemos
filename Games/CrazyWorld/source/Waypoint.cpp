#include "Waypoint.h"
#include <iostream>

Waypoint::Waypoint()
{
}

Waypoint::Waypoint(IrrlichtDevice* device, core::vector3df pos)
{
	this->device = device;
	this->position = pos;
	/*pNode = device->getSceneManager()->addAnimatedMeshSceneNode(device->getSceneManager()->getMesh("../res/mod/locator.obj"));
	pNode->setMaterialFlag(video::EMF_LIGHTING, false);
	pNode->setScale(core::vector3df(1,1,1));
	pNode->setPosition(position);*/
	count=0;
}

Waypoint::~Waypoint(void)
{
}

void Waypoint::AddNxt(Waypoint* n)
{
	bool dup = false;
	for(int i=0; i<nxt.size();i++)
	{
		if(nxt[i]->equals(nxt[i],n))
		{
			dup = true;
		}
	}
	if(!dup)
	{
		if(nxt.size() < MAX_NEXT)
		{
			nxt.push_back(n);
		}else{
			std::cout << "AddNxt Failed: Nxt list has reached its max"<<std::endl;
		}
	}else{
		std::cout << "AddNxt Failed: The Nxt you are trying to insert existedin the Nxt list" <<std::endl;
	}
}

Waypoint* Waypoint::GetRandNxt()
{
	if(count < nxt.size())
	{
		t=this->nxt[count];
		count++;
		return t;
	}else{
		if(count>=nxt.size())
		{
			count = 0;
			t=this->nxt[count];
			count++;
			return t;
		}
	}
}

bool Waypoint::equals(Waypoint* o,Waypoint* other)
{
	return o->position.X==other->position.X && o->position.Y==other->position.Y && o->position.Z==other->position.Z;
}	