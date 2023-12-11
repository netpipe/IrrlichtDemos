#include "Tile.h"
#include <iostream>

Tile::Tile()
{
}

Tile::Tile(IrrlichtDevice* device, core::vector3df pos,WPManager* wp,int dist,int vertOffset)
{
	this->device = device;
	this->occupied = false;
	this->position = pos;
	core::vector3df a1 = this->position;
	core::vector3df b1 = this->position;
	core::vector3df c1 = this->position;
	core::vector3df d1 = this->position;
	a1.X += dist;
	a1.Z += dist;

	b1.X += dist;
	b1.Z -= dist;

	c1.X -= dist;
	c1.Z += dist;

	d1.X -= dist;
	d1.Z -= dist;

	a1.Y = vertOffset;
	b1.Y = vertOffset;
	c1.Y = vertOffset;
	d1.Y = vertOffset;
	/*0=a
	1=b
	2=c
	3=d*/
	this->waypoint.push_back(new Waypoint(device,a1));
	this->waypoint.push_back(new Waypoint(device,b1));
	this->waypoint.push_back(new Waypoint(device,c1));
	this->waypoint.push_back(new Waypoint(device,d1));
	//Add into global waypoint array
	if(wp->GetSize() < 4)
	{
		this->waypoint[0]->AddNxt(this->waypoint[1]);
		this->waypoint[0]->AddNxt(this->waypoint[2]);
		this->waypoint[1]->AddNxt(this->waypoint[0]);
		this->waypoint[1]->AddNxt(this->waypoint[3]);
		this->waypoint[2]->AddNxt(this->waypoint[0]);
		this->waypoint[2]->AddNxt(this->waypoint[3]);
		this->waypoint[3]->AddNxt(this->waypoint[1]);
		this->waypoint[3]->AddNxt(this->waypoint[2]);
		for(int i=0;i<4;i++)
		{
			wp->InsertNode(this->waypoint[i]);
		}
	}else{
		for(int i=0;i<this->waypoint.size();i++)
		{
			for(int j=0;j<wp->GetSize();j++)
			{
				if(this->waypoint[i]->equals(this->waypoint[i],wp->GetNode(j)))
				{
					this->waypoint[i] = wp->GetNode(j);
				}
			}
		}
		this->waypoint[0]->AddNxt(this->waypoint[1]);
		this->waypoint[0]->AddNxt(this->waypoint[2]);
		this->waypoint[1]->AddNxt(this->waypoint[0]);
		this->waypoint[1]->AddNxt(this->waypoint[3]);
		this->waypoint[2]->AddNxt(this->waypoint[3]);
		this->waypoint[2]->AddNxt(this->waypoint[0]);
		this->waypoint[3]->AddNxt(this->waypoint[2]);
		this->waypoint[3]->AddNxt(this->waypoint[1]);
		for(int i=0;i<4;i++)
		{
			wp->InsertNode(this->waypoint[i]);
		}
	}

	pNode = device->getSceneManager()->addAnimatedMeshSceneNode(device->getSceneManager()->getMesh("../res/mod/ground.obj"));
	pNode->setMaterialTexture(0,device->getVideoDriver()->getTexture("../res/tex/g1.jpg"));
	pNode->setMaterialFlag(video::EMF_LIGHTING, false);
	pNode->setScale(core::vector3df(1.5,1.5,1.5));
	pNode->setPosition(position);
}

Tile::~Tile(void)
{
}

bool equals(Tile* o,Tile* other)
{
	return o->position.equals(other->position);
}