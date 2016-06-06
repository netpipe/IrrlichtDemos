#include "Building.h"
#include "Effects.h"
#include <iostream>

Building::Building(IrrlichtDevice* device, std::string name, std::string owner, Tile* tile, int multiplier)
{
	moda = "../res/mod/";
	texa = "../res/tex/";
	this->SetCurrHP(250);
	this->SetTotalHP(250);
	this->SetType("build");
	this->SetOwner(owner);
	lastFrame = device->getTimer()->getRealTime();
	rocps = 3000;
	timePassed = rocps; // Used as internal Cooldown Timer
	createFlag = false;
	this->multi = multiplier;

	this->device = device;
	this->driver = device->getVideoDriver();
	this->smgr = device->getSceneManager();
	eft = new Effects(device);
	this->parent = tile;
	this->positionVec = parent->position;
	this->directionVec = parent->waypoint[0]->position-this->positionVec;
	parent->occupied = true;
	next = new GameObject();
	this->setID(sin(rand()*123.456));
	this->SetDelFlag(false);
	this->moda.append(name);
	pNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh(this->moda.append(".obj").c_str()));
	this->texa.append(name);
	pNode->setMaterialTexture(0,driver->getTexture(this->texa.append(".jpg").c_str()));
	pNode->setMaterialFlag(video::EMF_LIGHTING, false);
	pNode->setScale(core::vector3df(.4,.4,.4));
	pNode->setPosition(positionVec);
	//pNode->setDebugDataVisible(scene::EDS_HALF_TRANSPARENCY);
}


Building::~Building(void)
{
}

void Building::TickAI()
{
	//std::cout << device->getTimer()->getRealTime()-lastFrame << std::endl;
	timePassed -= (device->getTimer()->getRealTime()-lastFrame)  * device->getTimer()->getSpeed();
	if(timePassed < 0)
	{
		timePassed = 0;
	}
	if(this->GetCurrHP()<=0){
		eft->particleSpray(device->getVideoDriver()->getTexture("../res/tex/firesmoke.tga"), positionVec,core::vector3df(0,1,0)*.05,50000,100,500,4); 
		this->SetDelFlag(true);
	}
	if(timePassed == 0)
	{
		this->createFlag = true;
	}
	
	lastFrame = device->getTimer()->getRealTime();
}

void Building::CheckCollide(GameObject * obj)
{
	if(obj->GetType().compare("proj") == 0)
	{
		if(this->pNode->getTransformedBoundingBox().intersectsWithBox(obj->pNode->getTransformedBoundingBox())==1)
		{
			if(this->GetOwner().compare(obj->GetOwner())!=0)
			{
				int amt = this->GetCurrHP() - ((Projectile*)obj)->dmg;
				if(amt < 0)
				{
					amt = 0;
				}
				this->SetCurrHP(amt);
				eft->particleSpray(device->getVideoDriver()->getTexture("../res/tex/particlewhite.tga"), obj->positionVec,(-obj->directionVec).normalize()*.01,500,100,500,4); 
				obj->SetDelFlag(true);
			}
		}
	}
}